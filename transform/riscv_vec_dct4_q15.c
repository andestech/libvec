/******************************************************************************
 * Copyright (C) 2010-2025 Arm Limited or its affiliates. All rights reserved.*
 * Copyright (C) 2018-2025 Andes Technology Corporation. All rights reserved. *
 *                                                                            *
 * SPDX-License-Identifier: Apache-2.0                                        *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the License); you may      *
 * not use this file except in compliance with the License.                   *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 * www.apache.org/licenses/LICENSE-2.0                                        *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT    *
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.           *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 ******************************************************************************/
#include "internal_vec_transform.h"
// #include "internal_nds_types.h"

#define TYPE_DCT4 1
#define TYPE_IDCT4 0

#define COS_BITS 14
#define COS_MUL (1<<COS_BITS)

static inline void vec_create_dct4_table1(q15_t * cos_table, q15_t * sin_table, uint32_t n, uint32_t table_size)
{
    const float32_t pi = NDSV_PI;

    cos_table[0] = 1.0f * COS_MUL;
    sin_table[0] = 0.0f * COS_MUL;
    for(int i = 1; i < table_size; i++)
    {
        cos_table[i] = cos(i / (float32_t)(n) * pi) * COS_MUL;
        sin_table[i] = sin(i / (float32_t)(n) * pi) * COS_MUL;
    }
}

static inline void vec_create_dct4_table2(q15_t * cos_table2, q15_t * sin_table2, uint32_t n, uint32_t table_size)
{
    const float32_t pi = NDSV_PI;
    
    for(int i = 0; i < table_size; i++)
    {
        cos_table2[i] = cos((2*i+1) / (float32_t)(4*n) * pi) * COS_MUL;
        sin_table2[i] = sin((2*i+1) / (float32_t)(4*n) * pi) * COS_MUL;
    }
}

riscv_vec_dct4_cfg_q15_t riscv_vec_dct4_init_q15(uint32_t n)
{
    if( (((n) & (n-1)) != 0) || (n<8) || (n>8192L) )
    {
        // only support the size with power of 2, and 8 ~ 8192
        return NULL;
    }
    int table1_size = n/4 + 1, table2_size = n/2;
    u32_t memneeded = sizeof(riscv_vec_dct4_state_q15_t)
                    + sizeof(q15_t) * table1_size        /* cos_table */
                    + sizeof(q15_t) * table1_size        /* sin_table */
                    + sizeof(q15_t) * table2_size        /* cos_table2 */
                    + sizeof(q15_t) * table2_size        /* sin_table2 */
                    + sizeof(riscv_vec_cpx_q15_t) * (((n>>1)+2)) /* rfft_out (2*(n_h>>1+1))*/
                    + NDSV_ALIGNMENT;              /* 64-bit alignment */

    riscv_vec_dct4_cfg_q15_t ret = NDSV_MALLOC(memneeded);
    if (ret == NULL)
    {
        return NULL;
    }

    uintptr_t addr = (uintptr_t)ret + sizeof(riscv_vec_dct4_state_q15_t);
    NDSV_ALIGN(addr, NDSV_ALIGNMENT);
    ret->n = n;
    ret->log2n = get_log2_32b(n);
    ret->cos_table = (q15_t*)addr;
    ret->sin_table = ret->cos_table + table1_size;
    ret->cos_table2 = ret->sin_table + table1_size;
    ret->sin_table2 = ret->cos_table2 + table2_size;
    ret->rfft_out = (riscv_vec_cpx_q15_t*)(ret->sin_table2 + table2_size);
    ret->nfactor = (1.0f / (n>>1)) * COS_MUL;
    ret->rfft_cfg = riscv_vec_rfft_init_q15(n>>1);

    vec_create_dct4_table1(ret->cos_table, ret->sin_table, n, table1_size);
    vec_create_dct4_table2(ret->cos_table2, ret->sin_table2, n, table2_size);

    return ret;
}

static inline void vec_four_point_rfft_q15(q15_t * dst, q15_t * src, int rfft_scale)
{
    q15_t a = src[0] + src[2];
    q15_t b = src[0] - src[2];
    q15_t c = src[1] + src[3];
    q15_t d = src[1] - src[3];

    if (rfft_scale == 1)
    {
        dst[0] = (a + c) >> 2;
        dst[1] = 0;
        dst[2] = (b) >> 2;
        dst[3] = (-d) >> 2;
        dst[4] = (a - c) >> 2;
        dst[5] = 0;
    }
    else
    {
        dst[0] = (a + c);
        dst[1] = 0;
        dst[2] = (b);
        dst[3] = (-d);
        dst[4] = (a - c);
        dst[5] = 0;
    }
}

static inline void vec_dc4_q15(q15_t *dst, q15_t *src, riscv_vec_dct4_cfg_q15_t cfg, uint32_t type)
{
    uint32_t n = cfg->n;
    uint32_t n_h = n >> 1;
    riscv_vec_cpx_q15_t * fft_out1 = cfg->rfft_out;
    riscv_vec_cpx_q15_t * fft_out2 = fft_out1 + ((n_h>>1)+1);
    q15_t *out1 = (q15_t *) &fft_out1[0];
    q15_t *out2 = (q15_t *) &fft_out2[0];
    int i, rfft_scale, post_bits;

    // pre-processing
    dst[0] = src[0];
    dst[n_h] = src[n - 1];

    for (i = 1; i + i < n_h; i++)
    {
        int ci = i*2;
        q15_t u, v, a, b, a2, b2;

        u = src[ci-1];
        v = src[ci];
        a = u + v;
        b2 = u - v;

        u = src[n-ci-1];
        v = src[n-ci];
        b = u + v;
        a2 = u - v;

        q15_t co, si;
        co = cfg->cos_table[i];
        si = cfg->sin_table[i];

        q15_t apb, amb;
        apb = a + b;
        amb = a - b;
        dst[i] = (((q31_t)co * amb + (q31_t)si * apb) >> (COS_BITS+1)); // +1 bit shift for 0.5f
        dst[n_h - i] = (((q31_t)co * apb - (q31_t)si * amb) >> (COS_BITS+1)); // +1 bit shift for 0.5f

        apb = a2 + b2;
        amb = a2 - b2;
        dst[n_h + i] = (((q31_t)co * amb + (q31_t)si * apb) >> (COS_BITS+1)); // +1 bit shift for 0.5f
        dst[n - i] = (((q31_t)co * apb - (q31_t)si * amb) >> (COS_BITS+1)); // +1 bit shift for 0.5f
    }
    
    if (type == TYPE_DCT4)
    {
        rfft_scale = 1;
        post_bits = COS_BITS + 1; // Because of the n/2 RFFT, an additional shift of 1 bit is required here.
    }
    else  // IDCT4
    {
        rfft_scale = 0;
        post_bits = COS_BITS - 1;
    }

    q15_t u, v;
    u = src[n_h - 1];
    v = src[n_h];
    dst[i] = (q31_t)(u + v) * (cfg->cos_table[i]) >> COS_BITS;
    dst[n - i] = (q31_t)(u - v) * (cfg->cos_table[i]) >> COS_BITS;
    
    // rfft
    if (n == 8)
    {
        vec_four_point_rfft_q15((q15_t*)fft_out1, dst, rfft_scale);
        vec_four_point_rfft_q15((q15_t*)fft_out2, dst+n_h, rfft_scale);
    }
    else
    {
        riscv_vec_rfft_q15(fft_out1, dst, cfg->rfft_cfg, rfft_scale);
        riscv_vec_rfft_q15(fft_out2, dst+n_h, cfg->rfft_cfg, rfft_scale);
    }

    // post-processing
    q15_t *co, *si;
    q15_t a, b;
    co = cfg->cos_table2; /* cos */
    si = cfg->sin_table2; /* sin */
    a = out1[0];
    b = out2[0];
    dst[0] = ((q31_t)(*co) * a + (q31_t)(*si) * b) >> post_bits;
    dst[n - 1] = ((q31_t)(*si) * a - (q31_t)(*co) * b) >> post_bits;
    co++;
    si++;
    for (i = 1; i + i < n_h; ++i)
    {
        int k, ci = i*2;
        q15_t u, v, u2, v2;
        u = out1[ci];
        v = out1[ci+1];
        u2 = out2[ci];
        v2 = out2[ci+1];
        k = (i + i) - 1;
        
        a = u - v;
        b = v2 - u2;
        dst[k] = ((q31_t)(*co) * a + (q31_t)(*si) * b) >> post_bits;
        dst[n - 1 - k] = ((q31_t)(*si) * a - (q31_t)(*co) * b) >> post_bits;
        
        k++;
        co++;
        si++;

        a = u + v;
        b = u2 + v2;
        dst[k] = ((q31_t)(*co) * a + (q31_t)(*si) * b) >> post_bits;
        dst[n - 1 - k] = ((q31_t)(*si) * a - (q31_t)(*co) * b) >> post_bits;

        co++;
        si++;
    }
    int k = (i + i) - 1;
    int ci = i*2;
    a = out1[ci];
    b = out2[ci];
    dst[k] = ((q31_t)(*co) * a - (q31_t)(*si) * b) >> post_bits;
    dst[n - 1 - k] = ((q31_t)(*si) * a + (q31_t)(*co) * b) >> post_bits;
}

void riscv_vec_dct4_q15(q15_t *dst, q15_t *src, riscv_vec_dct4_cfg_q15_t cfg)
{
    if (cfg == NULL)
    {
        return;
    }

    vec_dc4_q15(dst, src, cfg, TYPE_DCT4);
}

void riscv_vec_idct4_q15(q15_t *dst, q15_t *src, riscv_vec_dct4_cfg_q15_t cfg)
{
    if (cfg == NULL)
    {
        return;
    }
    
    vec_dc4_q15(dst, src, cfg, TYPE_IDCT4);
}

void riscv_vec_dct4_free_q15(riscv_vec_dct4_cfg_q15_t cfg)
{
    if (cfg->rfft_cfg != NULL)
    {
        NDSV_FREE(cfg->rfft_cfg);
    }

    NDSV_FREE(cfg);
}


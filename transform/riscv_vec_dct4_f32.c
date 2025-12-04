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


static inline void vec_create_dct4_table1(float32_t * cos_table, float32_t * sin_table, uint32_t n, uint32_t table_size)
{
    const float32_t pi = NDSV_PI;

    cos_table[0] = 1.0f;
    sin_table[0] = 0.0f;
    for(int i = 1; i < table_size; i++)
    {
        cos_table[i] = cos(i / (float32_t)(n) * pi);
        sin_table[i] = sin(i / (float32_t)(n) * pi);
    }
}

static inline void vec_create_dct4_table2(float32_t * cos_table2, float32_t * sin_table2, uint32_t n, uint32_t table_size)
{
    const float32_t pi = NDSV_PI;

    for(int i = 0; i < table_size; i++)
    {
        cos_table2[i] = cos((2 * i + 1) / (float32_t)(4 * n) * pi);
        sin_table2[i] = sin((2 * i + 1) / (float32_t)(4 * n) * pi);
    }
}

riscv_vec_dct4_cfg_f32_t riscv_vec_dct4_init_f32(uint32_t n)
{
    if( (((n) & (n - 1)) != 0) || (n < 8) || (n > 8192L) )
    {
        // only support the size with power of 2, and 8 ~ 8192
        return NULL;
    }
    int table1_size = n/4 + 1, table2_size = n/2;
    u32_t memneeded = sizeof(riscv_vec_dct4_state_f32_t)
                      + sizeof(float32_t) * table1_size        /* cos_table */
                      + sizeof(float32_t) * table1_size        /* sin_table */
                      + sizeof(float32_t) * table2_size        /* cos_table2 */
                      + sizeof(float32_t) * table2_size        /* sin_table2 */
                      + NDSV_ALIGNMENT;              /* 64-bit alignment */

    riscv_vec_dct4_cfg_f32_t ret = NDSV_MALLOC(memneeded);
    if (ret == NULL)
    {
        return NULL;
    }

    uintptr_t addr = (uintptr_t)ret + sizeof(riscv_vec_dct4_state_f32_t);
    NDSV_ALIGN(addr, NDSV_ALIGNMENT);
    ret->n = n;
    ret->cos_table = (float32_t*)addr;
    ret->sin_table = ret->cos_table + table1_size;
    ret->cos_table2 = ret->sin_table + table1_size;
    ret->sin_table2 = ret->cos_table2 + table2_size;
    ret->rfft_out = NDSV_MALLOC(sizeof(riscv_vec_cpx_f32_t) * ((n>>1) + 2)); // (2*(n_h>>1+1))
    ret->nfactor = 1.0f / (n >> 1);
    ret->rfft_cfg = riscv_vec_rfft_init_f32(n >> 1);

    vec_create_dct4_table1(ret->cos_table, ret->sin_table, n, table1_size);
    vec_create_dct4_table2(ret->cos_table2, ret->sin_table2, n, table2_size);

    return ret;
}

static inline void vec_four_point_rfft(float32_t * dst, float32_t * src)
{
    float32_t a = src[0] + src[2];
    float32_t b = src[0] - src[2];
    float32_t c = src[1] + src[3];
    float32_t d = src[1] - src[3];

    dst[0] = a + c;
    dst[1] = 0;
    dst[2] = b;
    dst[3] = -d;
    dst[4] = a - c;
    dst[5] = 0;
}

static inline void vec_dc4_f32(float32_t *dst, float32_t *src, riscv_vec_dct4_cfg_f32_t cfg, uint32_t type)
{

    uint32_t n = cfg->n;
    uint32_t n_h = n >> 1;
    riscv_vec_cpx_f32_t * fft_out1 = cfg->rfft_out;
    riscv_vec_cpx_f32_t * fft_out2 = fft_out1 + ((n_h >> 1) + 1);
    float32_t *out1 = (float32_t *) &fft_out1[0];
    float32_t *out2 = (float32_t *) &fft_out2[0];
    int i;
    float32_t nfactor;

    // pre-processing
    if (type == TYPE_DCT4)
    {
        dst[0] = src[0];
        dst[n_h] = src[n - 1];
        nfactor = 0.5f;
    }
    else // IDCT4
    {
        dst[0] = src[0] * cfg->nfactor;
        dst[n_h] = src[n - 1] * cfg->nfactor;
        nfactor = 0.5f * cfg->nfactor;
    }

    for (i = 1; i + i < n_h; i++)
    {
        int ci = i * 2;
        float32_t u, v, a, b, a2, b2;

        u = src[ci - 1];
        v = src[ci];
        a = u + v;
        b2 = u - v;

        u = src[n - ci - 1];
        v = src[n - ci];
        b = u + v;
        a2 = u - v;

        float32_t co, si;
        co = cfg->cos_table[i];
        si = cfg->sin_table[i];

        float32_t apb, amb;
        apb = a + b;
        amb = a - b;
        dst[i] = (co * amb + si * apb) * nfactor;
        dst[n_h - i] = (co * apb - si * amb) * nfactor;

        apb = a2 + b2;
        amb = a2 - b2;
        dst[n_h + i] = (co * amb + si * apb) * nfactor;
        dst[n - i] = (co * apb - si * amb) * nfactor;
    }

    float32_t u, v;
    if (type == TYPE_DCT4)
    {
        u = src[n_h - 1];
        v = src[n_h];
    }
    else  // IDCT4
    {
        u = src[n_h - 1] * cfg->nfactor;
        v = src[n_h] * cfg->nfactor;
    }

    dst[i] = (u + v) * (cfg->cos_table[i]);
    dst[n - i] = (u - v) * (cfg->cos_table[i]);

    // rfft
    if (n == 8)
    {
        vec_four_point_rfft((float32_t*)fft_out1, dst);
        vec_four_point_rfft((float32_t*)fft_out2, dst + n_h);
    }
    else
    {
        riscv_vec_rfft_f32(fft_out1, dst, cfg->rfft_cfg);
        riscv_vec_rfft_f32(fft_out2, dst + n_h, cfg->rfft_cfg);
    }

    // post-processing
    float32_t *co, *si;
    float32_t a, b;
    co = cfg->cos_table2; /* cos */
    si = cfg->sin_table2; /* sin */
    a = out1[0];
    b = out2[0];
    dst[0] = (*co) * a + (*si) * b;
    dst[n - 1] = (*si) * a - (*co) * b;
    co++;
    si++;
    for (i = 1; i + i < n_h; ++i)
    {
        int k, ci = i * 2;
        float32_t u, v, u2, v2;
        u = out1[ci];
        v = out1[ci + 1];
        u2 = out2[ci];
        v2 = out2[ci + 1];
        k = (i + i) - 1;

        a = u - v;
        b = v2 - u2;
        dst[k] = (*co) * a + (*si) * b;
        dst[n - 1 - k] = (*si) * a - (*co) * b;

        k++;
        co++;
        si++;

        a = u + v;
        b = u2 + v2;
        dst[k] = (*co) * a + (*si) * b;
        dst[n - 1 - k] = (*si) * a - (*co) * b;

        co++;
        si++;
    }
    int k = (i + i) - 1;
    int ci = i * 2;
    a = out1[ci];
    b = out2[ci];
    dst[k] = (*co) * a - (*si) * b;
    dst[n - 1 - k] = (*si) * a + (*co) * b;
}

void riscv_vec_dct4_f32(float32_t *dst, float32_t *src, riscv_vec_dct4_cfg_f32_t cfg)
{
    if (cfg == NULL)
    {
        return;
    }

    vec_dc4_f32(dst, src, cfg, TYPE_DCT4);
}

void riscv_vec_idct4_f32(float32_t *dst, float32_t *src, riscv_vec_dct4_cfg_f32_t cfg)
{
    if (cfg == NULL)
    {
        return;
    }

    vec_dc4_f32(dst, src, cfg, TYPE_IDCT4);
}

void riscv_vec_dct4_free_f32(riscv_vec_dct4_cfg_f32_t cfg)
{
    if (cfg->rfft_cfg != NULL)
    {
        NDSV_FREE(cfg->rfft_cfg);
    }

    NDSV_FREE(cfg);
}

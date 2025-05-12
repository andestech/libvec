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

#define COS_BITS 14
#define COS_MUL (1<<COS_BITS)
// #define RES_ROUND (1<<(COS_BITS-1))


static inline void vec_create_dct_table(q15_t *cos_table, q15_t *sin_table, uint32_t n)
{
    const float32_t pi = NDSV_PI;

    sin_table[0] = cos_table[0] = 1.0f * COS_MUL;
    for(int i = 1; i < n; i++)
    {
        sin_table[n-i] = cos_table[i] = cos(i / (float32_t)(2*n) * pi) * COS_MUL;
    }
}

riscv_vec_dct_cfg_q15_t riscv_vec_dct_init_q15(uint32_t n)
{
    if( (((n) & (n-1)) != 0) || (n<8) || (n>8192L) )
    {
        // only support the size with power of 2, and 8 ~ 8192
        return NULL;
    }

    u32_t memneeded = sizeof(riscv_vec_dct_state_q15_t)
                    + sizeof(q15_t) * n        /* cos_table */
                    + sizeof(q15_t) * n        /* sin_table */
                    + sizeof(riscv_vec_cpx_q15_t) * ((n>>1)+1) /* rfft_out */
                    + NDSV_ALIGNMENT;          /* 64-bit alignment */

    riscv_vec_dct_cfg_q15_t ret = NDSV_MALLOC(memneeded);
    if (ret == NULL)
    {
        return NULL;
    }

    uintptr_t addr = (uintptr_t)ret + sizeof(riscv_vec_dct_state_q15_t);
    NDSV_ALIGN(addr, NDSV_ALIGNMENT);
    ret->n = n;
    ret->log2n = get_log2_32b(n);
    ret->cos_table = (q15_t*)addr;
    ret->sin_table = ret->cos_table + n;
    ret->rfft_out = (riscv_vec_cpx_q15_t*)(ret->sin_table + n);
    ret->nfactor = (1.0f / n) * COS_MUL;
    ret->rfft_cfg = riscv_vec_rfft_init_q15(n);
    vec_create_dct_table(ret->cos_table, ret->sin_table, n);
    return ret;
}

void riscv_vec_dct_q15(q15_t *dst, q15_t *src, riscv_vec_dct_cfg_q15_t cfg)
{
    if (cfg == NULL)
    {
        return;
    }
    uint32_t n = cfg->n;
    riscv_vec_cpx_q15_t * fft_out = cfg->rfft_out;
    q15_t *out = (q15_t *) &fft_out[0];
    int i, j;

    // pre-processing
    for (i = 0, j = 0; i < n; i+=2, j++)
    {
        dst[j] = src[i];
        dst[n-j-1] = src[i+1];
    }

    // rfft
    riscv_vec_rfft_q15(fft_out, dst, cfg->rfft_cfg, 1);

    // post-processing
    dst[0] = out[0];
    for (i = 1; i < n - i; i++)
    {
        int ci = i * 2;
        q15_t re, im, co, si;
        re = out[ci];
        im = out[ci+1];
        co = cfg->cos_table[i];
        si = cfg->sin_table[i];

        dst[i]   = (q15_t)(((q31_t)co * re + (q31_t)si * im) >> COS_BITS);
        dst[n-i] = (q15_t)(((q31_t)si * re - (q31_t)co * im) >> COS_BITS);
    }
    dst[i] = (q15_t)(((q31_t)cfg->cos_table[i] * out[i*2] + (q31_t)cfg->sin_table[i] * out[i*2+1]) >> COS_BITS);
}

void riscv_vec_idct_q15(q15_t *dst, q15_t *src, riscv_vec_dct_cfg_q15_t cfg)
{
    if (cfg == NULL)
    {
        return;
    }
    uint32_t n = cfg->n;
    riscv_vec_cpx_q15_t * fft_out = cfg->rfft_out;
    q15_t *out = (q15_t *) &fft_out[0];
    int i;

    // pre-processing
    dst[0] = src[0];
    for (i = 1; i < n - i; i++)
    {
        q15_t a, b, apb, amb, co, si;
        a = src[i];
        b = src[n-i];
        apb = a + b;
        amb = a - b;
        co = cfg->cos_table[i];
        si = cfg->sin_table[i];

        dst[i]   = (q15_t)(((q31_t)co * amb + (q31_t)si * apb) >> COS_BITS);
        dst[n-i] = (q15_t)(((q31_t)co * apb - (q31_t)si * amb) >> COS_BITS);
    }
    // dst[i] = (q15_t)(((q31_t)2.0f * src[i] * cfg->cos_table[i]) >> COS_BITS);
    dst[i] = (q15_t)((((q31_t)src[i] * cfg->cos_table[i])) >> (COS_BITS-1));

    // rfft
    riscv_vec_rfft_q15(fft_out, dst, cfg->rfft_cfg, 0);

    // post-processing
    dst[0] = (q15_t)(((q31_t)out[0] * cfg->nfactor) >> (COS_BITS-(cfg->log2n)));
    for (i = 1; i < n - i; i++)
    {
        int ci = i * 2;
        q15_t re, im;
        re = out[ci];
        im = out[ci+1];
        dst[ci-1] = (q15_t)(((q31_t)(re - im) * cfg->nfactor) >> (COS_BITS-(cfg->log2n)));
        dst[ci] = (q15_t)(((q31_t)(re + im) * cfg->nfactor) >> (COS_BITS-(cfg->log2n)));
    }
    dst[i * 2 - 1] = (q15_t)(((q31_t)out[i * 2] * cfg->nfactor) >> (COS_BITS-(cfg->log2n)));
}

void riscv_vec_dct_free_q15(riscv_vec_dct_cfg_q15_t cfg)
{
    if (cfg->rfft_cfg != NULL)
    {
        NDSV_FREE(cfg->rfft_cfg);
    }

    NDSV_FREE(cfg);
}

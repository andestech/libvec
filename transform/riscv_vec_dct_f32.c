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


static inline void _create_dct_table(float32_t *cos_table, float32_t *sin_table, uint32_t n)
{
    const float32_t pi = NDSV_PI;

    sin_table[0] = cos_table[0] = 1.0f;
    for(int i = 1; i < n; i++)
    {
        sin_table[n - i] = cos_table[i] = cos(i / (float32_t)(2 * n) * pi);
    }
}

riscv_vec_dct_cfg_f32_t riscv_vec_dct_init_f32(uint32_t n)
{
    if( (((n) & (n - 1)) != 0) || (n < 8) || (n > 8192L) )
    {
        // only support the size with power of 2, and 8 ~ 8192
        return NULL;
    }

    u32_t memneeded = sizeof(riscv_vec_dct_state_f32_t)
                      + sizeof(float32_t) * n        /* cos_table */
                      + sizeof(float32_t) * n        /* sin_table */
                      + NDSV_ALIGNMENT;              /* 64-bit alignment */

    riscv_vec_dct_cfg_f32_t ret = NDSV_MALLOC(memneeded);
    if (ret == NULL)
    {
        return NULL;
    }

    uintptr_t addr = (uintptr_t)ret + sizeof(riscv_vec_dct_state_f32_t);
    NDSV_ALIGN(addr, NDSV_ALIGNMENT);
    ret->n = n;
    ret->cos_table = (float32_t*)addr;
    ret->sin_table = ret->cos_table + n;
    ret->rfft_out = NDSV_MALLOC(sizeof(riscv_vec_cpx_f32_t) * ((n>>1) + 1));
    ret->nfactor = 1.0f / n;
    ret->rfft_cfg = riscv_vec_rfft_init_f32(n);
    _create_dct_table(ret->cos_table, ret->sin_table, n);
    return ret;
}

void riscv_vec_dct_f32(float32_t *dst, float32_t *src, riscv_vec_dct_cfg_f32_t cfg)
{
    if (cfg == NULL)
    {
        return;
    }
    uint32_t n = cfg->n;
    riscv_vec_cpx_f32_t * fft_out = cfg->rfft_out;
    float32_t *out = (float32_t *) &fft_out[0];
    int i, j;

    // pre-processing
    for (i = 0, j = 0; i < n; i += 2, j++)
    {
        dst[j] = src[i];
        dst[n - j - 1] = src[i + 1];
    }

    // rfft
    riscv_vec_rfft_f32(fft_out, dst, cfg->rfft_cfg);

    // post-processing
    dst[0] = out[0];
    for (i = 1; i < n - i; i++)
    {
        int ci = i * 2;
        float32_t re, im, co, si;
        re = out[ci];
        im = out[ci + 1];
        co = cfg->cos_table[i];
        si = cfg->sin_table[i];

        dst[i] = co * re + si * im;
        dst[n - i] = si * re - co * im;
    }
    dst[i] = cfg->cos_table[i] * out[i * 2] + cfg->sin_table[i] * out[i * 2 + 1];
}

void riscv_vec_idct_f32(float32_t *dst, float32_t *src, riscv_vec_dct_cfg_f32_t cfg)
{
    if (cfg == NULL)
    {
        return;
    }
    uint32_t n = cfg->n;
    riscv_vec_cpx_f32_t * fft_out = cfg->rfft_out;
    float32_t *out = (float32_t *) &fft_out[0];
    int i;

    // pre-processing
    dst[0] = src[0];
    for (i = 1; i < n - i; i++)
    {
        float32_t a, b, apb, amb, co, si;
        a = src[i];
        b = src[n - i];
        apb = a + b;
        amb = a - b;
        co = cfg->cos_table[i];
        si = cfg->sin_table[i];

        dst[i] = co * amb + si * apb;
        dst[n - i] = co * apb - si * amb;
    }
    dst[i] = 2.0f * src[i] * cfg->cos_table[i];

    // rfft
    riscv_vec_rfft_f32(fft_out, dst, cfg->rfft_cfg);

    // post-processing
    dst[0] = out[0] * cfg->nfactor;
    for (i = 1; i < n - i; i++)
    {
        int ci = i * 2;
        float32_t re, im;
        re = out[ci];
        im = out[ci + 1];
        dst[ci - 1] = (re - im) * cfg->nfactor;
        dst[ci] = (re + im) * cfg->nfactor;
    }
    dst[i * 2 - 1] = out[i * 2] * cfg->nfactor;
}

void riscv_vec_dct_free_f32(riscv_vec_dct_cfg_f32_t cfg)
{
    if (cfg->rfft_cfg != NULL)
    {
        NDSV_FREE(cfg->rfft_cfg);
    }

    NDSV_FREE(cfg);
}

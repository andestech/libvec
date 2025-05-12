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
#include "riscv_vec_basic.h"
#include "riscv_vec_statistics.h"
#include "riscv_vec_complex.h"
#include "riscv_vec_utils.h"
#include "riscv_vec_matrix.h"

riscv_vec_mfcc_cfg_f16_t riscv_vec_mfcc_init_f16(uint32_t fft_len,
                                                 uint32_t n_mel_filters,
                                                 uint32_t n_dct_out,
                                                 float16_t *dct_coefs,
                                                 uint32_t *filter_pos,
                                                 uint32_t *filter_len,
                                                 float16_t *filter_coefs,
                                                 float16_t *window_coefs)
{
    if( (((fft_len) & (fft_len - 1)) != 0) || (fft_len < 8) || (fft_len > 8192L) )
    {
        // only support the size with power of 2, and 8 ~ 8192
        return NULL;
    }

    u32_t memneeded = sizeof(riscv_vec_mfcc_state_f16_t)
                      + NDSV_ALIGNMENT;              /* 64-bit alignment */

    riscv_vec_mfcc_cfg_f16_t ret = NDSV_MALLOC(memneeded);
    if (ret == NULL)
    {
        return NULL;
    }

    uintptr_t addr = (uintptr_t)ret + sizeof(riscv_vec_mfcc_state_f16_t);
    NDSV_ALIGN(addr, NDSV_ALIGNMENT);
    ret->fft_len = fft_len;
    ret->n_mel_filters = n_mel_filters;
    ret->n_dct_out = n_dct_out;
    ret->dct_coefs = dct_coefs;
    ret->filter_pos = filter_pos;
    ret->filter_len = filter_len;
    ret->filter_coefs = filter_coefs;
    ret->window_coefs = window_coefs;
    ret->rfft_cfg = riscv_vec_rfft_init_f16(fft_len);
    return ret;
}

void riscv_vec_mfcc_f16(float16_t * FUNC_RESTRICT dst, float16_t * FUNC_RESTRICT src, float16_t * FUNC_RESTRICT buf, riscv_vec_mfcc_cfg_f16_t cfg)
{
    if (cfg == NULL)
    {
        return;
    }

    float16_t maxValue, result;
    uint32_t i;
    float16_t *coefs = cfg->filter_coefs;

    /* Normalize */
    maxValue = riscv_vec_absmax_val_f16(src, cfg->fft_len);

    if (maxValue != 0.0f)
    {
        riscv_vec_mulc_f16(src, src, 1.0f / maxValue, cfg->fft_len);
    }
    /* Multiply by window */
    riscv_vec_mul_f16(src, src, cfg->window_coefs, cfg->fft_len);

    /* Compute spectrum magnitude */
    /* Default RFFT based implementation */
    riscv_vec_rfft_f16((riscv_vec_cpx_f16_t*)buf, src, cfg->rfft_cfg);

    riscv_vec_cmag_f16(src, buf, (cfg->fft_len>>1) + 1);

    if (maxValue != 0.0f)
    {
        riscv_vec_mulc_f16(src, src, maxValue, (cfg->fft_len>>1) + 1);
    }

    /* Apply MEL filters */
    for (i = 0; i < cfg->n_mel_filters; i++)
    {
        result = riscv_vec_dprod_f16(src + cfg->filter_pos[i], coefs, cfg->filter_len[i]);

        coefs += cfg->filter_len[i];
        buf[i] = result;
    }

    /* Compute the log */
    riscv_vec_addc_f16(buf, buf, 1.0e-4f, cfg->n_mel_filters);
    riscv_vec_log_f16(buf, buf, cfg->n_mel_filters);

    /* Multiply with the DCT matrix */
    riscv_vec_rmmat_mul_mxv_f16(cfg->dct_coefs, buf, dst, cfg->n_dct_out, cfg->n_mel_filters);
}

void riscv_vec_mfcc_free_f16(riscv_vec_mfcc_cfg_f16_t cfg)
{
    NDSV_FREE(cfg->rfft_cfg);
    NDSV_FREE(cfg);
}

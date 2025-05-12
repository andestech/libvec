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
#include "internal_vec_utils.h"
#include "riscv_vec_basic.h"
#include "riscv_vec_statistics.h"
#include "riscv_vec_complex.h"
#include "riscv_vec_utils.h"
#include "riscv_vec_matrix.h"

/* Constants for Q31 implementation */
#define LOG2TOLOG_Q31 0x02C5C860
#define MICRO_Q31 0x08637BD0
#define SHIFT_MELFILTER_SATURATION_Q31 10

riscv_vec_mfcc_cfg_q31_t riscv_vec_mfcc_init_q31(uint32_t fft_len,
                                                 uint32_t n_mel_filters,
                                                 uint32_t n_dct_out,
                                                 q31_t *dct_coefs,
                                                 uint32_t *filter_pos,
                                                 uint32_t *filter_len,
                                                 q31_t *filter_coefs,
                                                 q31_t *window_coefs)
{
    if( (((fft_len) & (fft_len - 1)) != 0) || (fft_len < 8) || (fft_len > 8192L) )
    {
        // only support the size with power of 2, and 8 ~ 8192
        return NULL;
    }

    u32_t memneeded = sizeof(riscv_vec_mfcc_state_q31_t)
                      + NDSV_ALIGNMENT;              /* 64-bit alignment */

    riscv_vec_mfcc_cfg_q31_t ret = NDSV_MALLOC(memneeded);
    if (ret == NULL)
    {
        return NULL;
    }

    uintptr_t addr = (uintptr_t)ret + sizeof(riscv_vec_mfcc_state_q31_t);
    NDSV_ALIGN(addr, NDSV_ALIGNMENT);
    ret->fft_len = fft_len;
    ret->log2_fft_len = get_log2_32b(fft_len);
    ret->n_mel_filters = n_mel_filters;
    ret->n_dct_out = n_dct_out;
    ret->dct_coefs = dct_coefs;
    ret->filter_pos = filter_pos;
    ret->filter_len = filter_len;
    ret->filter_coefs = filter_coefs;
    ret->window_coefs = window_coefs;
    ret->rfft_cfg = riscv_vec_rfft_init_q31(fft_len);
    return ret;
}

void riscv_vec_mfcc_q31(q31_t * FUNC_RESTRICT dst, q31_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT buf, riscv_vec_mfcc_cfg_q31_t cfg)
{
    if (cfg == NULL)
    {
        return;
    }

    q31_t *coefs = cfg->filter_coefs;
    q31_t maxValue, logExponent;
    uint32_t i, fftShift=0;
    q63_t result;

    /* Normalize */
    maxValue = riscv_vec_absmax_val_q31(src, cfg->fft_len);

    if ((maxValue != 0) && (maxValue != 0x7FFFFFFF))
    {
        q31_t quotient;
        int16_t shift;
        
        int status = vec_divide_q31(0x7FFFFFFF, maxValue, &quotient, &shift);
        if (status != 0)
        {
            return;
        }
        riscv_vec_mulc_q31(src, src, quotient, shift, cfg->fft_len);
    }

    /* Multiply by window */
    riscv_vec_mul_q31(src, src, cfg->window_coefs, cfg->fft_len);

    /* Compute spectrum magnitude */
    /* Default RFFT based implementation */
    riscv_vec_rfft_q31((riscv_vec_cpx_q31_t*)buf, src, cfg->rfft_cfg, 1);
    riscv_vec_shift_q31(buf, buf, 1, cfg->fft_len); // don't need fft_len+2 here because the last element(image) is 0 and cmag will do real * image. The last result of cmag is always 0 so the fft_len is enough.

    riscv_vec_cmag_q31(src, buf, (cfg->fft_len>>1) + 1);

    /* Apply MEL filters */
    for (i = 0; i < cfg->n_mel_filters; i++)
    {
        result = riscv_vec_dprod_q31(src + cfg->filter_pos[i], coefs, cfg->filter_len[i]);
        coefs += cfg->filter_len[i];

        // q16.48 - fftShift
        result += MICRO_Q31;
        result >>= (SHIFT_MELFILTER_SATURATION_Q31 + 18);

        // q16.29 - fftShift - satShift
        buf[i] = NDS_ISA_SATS(result, 31);
    }

    if ((maxValue != 0) && (maxValue != 0x7FFFFFFF))
    {
        riscv_vec_mulc_q31(buf, buf, maxValue, 0, cfg->n_mel_filters);
    }

    /* Compute the log */
    riscv_vec_log_q31(buf, buf, cfg->n_mel_filters);
    
    // q5.26
    fftShift = cfg->log2_fft_len - 1;
    logExponent = fftShift + 2 + SHIFT_MELFILTER_SATURATION_Q31;
    logExponent = logExponent * LOG2TOLOG_Q31;

    // q5.26
    riscv_vec_addc_q31(buf, buf, logExponent, cfg->n_mel_filters);
    riscv_vec_shift_q31(buf, buf, -3, cfg->n_mel_filters);
    
    // q8.23
    /* Multiply with the DCT matrix */
    riscv_vec_rmmat_mul_mxv_q31(cfg->dct_coefs, buf, dst, cfg->n_dct_out, cfg->n_mel_filters);
}

void riscv_vec_mfcc_free_q31(riscv_vec_mfcc_cfg_q31_t cfg)
{
    NDSV_FREE(cfg->rfft_cfg);
    NDSV_FREE(cfg);
}

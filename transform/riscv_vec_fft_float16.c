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
//#include "internal_nds_types.h"

#ifdef DEBUG_MODE
#define DBG_MSG         printf
#else
#define DBG_MSG(...)
#endif

#ifdef ENA_STOCKHAM_FFT
extern void riscv_vec_inverse_butterfly_f16 (riscv_vec_cpx_f16_t *out,
        riscv_vec_cpx_f16_t *in,
        q31_t *factors,
        riscv_vec_cpx_f16_t *twiddles,
        riscv_vec_cpx_f16_t *buffer);

/*
 * This function calculates the FFT for power-of-two input sizes using an ordered, mixed
 * radix-2/4/8 DIT algorithm.
 *
 * For the CFFT size <= 8 (RFFT size <= 16) the algorithm performs a radix-8 or radix-4 butterfly calculation.
 * For other case, the algorithm performs a radix-4 butterfly and then continually applies
 * radix-4 butterflies until the last stage of computation, the last stage will apply a
 * radix-4 or radix-2 butterflies deponds on the FFT size. The order in which results are
 * stored after each stage allows stages to load and store elements contiguously between
 * iterations, and for the final output order to be correct.
 */

static void riscv_vec_butterfly_f16 (riscv_vec_cpx_f16_t * FUNC_RESTRICT out,
                                     riscv_vec_cpx_f16_t * FUNC_RESTRICT in,
                                     q31_t * FUNC_RESTRICT factors,
                                     riscv_vec_cpx_f16_t * FUNC_RESTRICT twiddles,
                                     riscv_vec_cpx_f16_t * FUNC_RESTRICT buffer)
{
   // pure C version
    uint32_t input_m = factors[SIZE_FIRST_STAGE];
    uint32_t step = 1;
    uint32_t stage_num = factors[OUT_INDEX];
    uint32_t stage_idx = 0;
    float16_t tmp_r, tmp_i;
    uint32_t tmp_q = 0, tmp_p = 0 ;
    riscv_vec_cpx_f16_t *out_val = NULL ;
    riscv_vec_cpx_f16_t *in_val = NULL  ;
    riscv_vec_cpx_f16_t *out_next = NULL  ;
    riscv_vec_cpx_f16_t tmp_cal_1, tmp_cal_2, tmp_cal_wp;

    if(stage_num % 2 ==0)
    {
        out_val = buffer;
        out_next = out;
    }
    else
    {
        out_val = out;
        out_next = buffer;
    }
    in_val = in ;

    for(tmp_p =0; tmp_p < input_m ; tmp_p++)
    {
        tmp_cal_wp.r = twiddles[step*tmp_p].r;
        tmp_cal_wp.i = twiddles[step*tmp_p].i;
        for( tmp_q = 0 ; tmp_q < step; tmp_q++)
        {
            tmp_cal_1.r = in_val[tmp_q + step*(tmp_p + 0)].r;
            tmp_cal_2.r = in_val[tmp_q + step*(tmp_p + input_m)].r;
            tmp_cal_1.i = in_val[tmp_q + step*(tmp_p + 0)].i;
            tmp_cal_2.i = in_val[tmp_q + step*(tmp_p + input_m)].i;
            out_val[tmp_q + step*(2*tmp_p + 0)].r =  tmp_cal_1.r+tmp_cal_2.r;
            out_val[tmp_q + step*(2*tmp_p + 0)].i =  tmp_cal_1.i+tmp_cal_2.i;
            tmp_r = tmp_cal_1.r - tmp_cal_2.r;
            tmp_i = tmp_cal_1.i - tmp_cal_2.i;
            out_val[tmp_q + step*(2*tmp_p + 1)].r = (tmp_r* tmp_cal_wp.r) - (tmp_i* tmp_cal_wp.i);
            out_val[tmp_q + step*(2*tmp_p + 1)].i = (tmp_r* tmp_cal_wp.i) + (tmp_i* tmp_cal_wp.r);
        }
    }

    in_val = out_val;
    out_val = out_next;
    out_next = in_val;
    step = step*2;
    input_m = input_m /2 ;
    for(stage_idx = 1 ; stage_idx< stage_num-1; stage_idx++)
    {
        for(tmp_p =0; tmp_p < input_m ; tmp_p++)
        {
            tmp_cal_wp.r = twiddles[step*tmp_p].r;
            tmp_cal_wp.i = twiddles[step*tmp_p].i;
            for( tmp_q = 0 ; tmp_q < step; tmp_q++)
            {
                tmp_cal_1.r = in_val[tmp_q + step*(tmp_p + 0)].r;
                tmp_cal_2.r = in_val[tmp_q + step*(tmp_p + input_m)].r;
                tmp_cal_1.i = in_val[tmp_q + step*(tmp_p + 0)].i;
                tmp_cal_2.i = in_val[tmp_q + step*(tmp_p + input_m)].i;
                out_val[tmp_q + step*(2*tmp_p + 0)].r =  tmp_cal_1.r+tmp_cal_2.r;
                out_val[tmp_q + step*(2*tmp_p + 0)].i =  tmp_cal_1.i+tmp_cal_2.i;
                tmp_r = tmp_cal_1.r - tmp_cal_2.r;
                tmp_i = tmp_cal_1.i - tmp_cal_2.i;
                out_val[tmp_q + step*(2*tmp_p + 1)].r = (tmp_r* tmp_cal_wp.r) - (tmp_i* tmp_cal_wp.i);
                out_val[tmp_q + step*(2*tmp_p + 1)].i = (tmp_r* tmp_cal_wp.i) + (tmp_i* tmp_cal_wp.r);
            }
        }
        step = step*2 ;
        in_val = out_val;
        out_val = out_next;
        out_next = in_val;
        input_m = input_m /2 ;
    }

    // last radix-2
    for (tmp_q = 0; tmp_q < step; tmp_q++)
    {
        tmp_cal_1.r = in_val[tmp_q + step*0].r;
        tmp_cal_1.i = in_val[tmp_q + step*0].i;
        tmp_cal_2.r = in_val[tmp_q + step*1].r;
        tmp_cal_2.i = in_val[tmp_q + step*1].i;

        out_val[tmp_q + step*0].r = tmp_cal_1.r + tmp_cal_2.r;
        out_val[tmp_q + step*0].i = tmp_cal_1.i + tmp_cal_2.i;

        out_val[tmp_q + step*1].r = tmp_cal_1.r - tmp_cal_2.r;
        out_val[tmp_q + step*1].i = tmp_cal_1.i - tmp_cal_2.i;
    }

}

static void _vec_split_r2c_1d_f16 (riscv_vec_cpx_f16_t * FUNC_RESTRICT dst,
                                   const riscv_vec_cpx_f16_t * FUNC_RESTRICT src,
                                   riscv_vec_cpx_f16_t * FUNC_RESTRICT twiddles,
                                   q31_t ncfft)
{
    // pure C version last prcoess of RFFT
    riscv_vec_cpx_f16_t *out_val = dst;
    const riscv_vec_cpx_f16_t *in_val  = src;
    q31_t size = ncfft;
    riscv_vec_cpx_f16_t tmp_cal_1, tmp_cal_2, tmp_cal_wp;
    float16_t *wp_tbl_rfft_r = NULL, *wp_tbl_rfft_i = NULL;
    q31_t tmp_q = 0;
    float16_t div_2 = (float16_t)0.5f ;
    wp_tbl_rfft_r = &twiddles[0].r;
    wp_tbl_rfft_i = &twiddles[ncfft/2].r;

    out_val[0].r = in_val[0].r + in_val[0].i;
    out_val[size].r  = in_val[0].r - in_val[0].i;
    out_val[0].i = 0 ;
    out_val[size].i = 0 ;
    for( tmp_q = 1 ; tmp_q <= size/2; tmp_q++)
    {
        tmp_cal_wp.r = wp_tbl_rfft_r[tmp_q];    // tw_r;
        tmp_cal_wp.i = wp_tbl_rfft_i[tmp_q];    // tw_i;

        tmp_cal_1.r = in_val[tmp_q].r + in_val[size-tmp_q].r;   // xe_r
        tmp_cal_1.i = in_val[tmp_q].i - in_val[size-tmp_q].i;   // xe_i
        tmp_cal_2.r = in_val[size-tmp_q].r - in_val[tmp_q].r;   // xo_r
        tmp_cal_2.i = -(in_val[tmp_q].i + in_val[size-tmp_q].i);  // xo_i
        out_val[tmp_q].r = div_2*(tmp_cal_1.r - (tmp_cal_2.r*tmp_cal_wp.i + tmp_cal_2.i*tmp_cal_wp.r));  // (xe_r - xo*tw_i)/2
        out_val[tmp_q].i = div_2*(tmp_cal_1.i + (tmp_cal_2.r*tmp_cal_wp.r - tmp_cal_2.i*tmp_cal_wp.i));  // (xe_i + xo*tw_r)/2
        out_val[size-tmp_q].r = div_2*(tmp_cal_1.r + (tmp_cal_2.r*tmp_cal_wp.i + tmp_cal_2.i*tmp_cal_wp.r));
        out_val[size-tmp_q].i = div_2*((tmp_cal_2.r*tmp_cal_wp.r - tmp_cal_2.i*tmp_cal_wp.i) - tmp_cal_1.i);
    }
}

/**
 * cfft_init_f16
 */
/**
 * cfft_f16
 */
void riscv_vec_cfft_f16 (riscv_vec_cpx_f16_t * FUNC_RESTRICT out,
                         riscv_vec_cpx_f16_t * FUNC_RESTRICT in,
                         riscv_vec_cfft_cfg_f16_t cfg,
                         q31_t inverse_fft)
{
    if(cfg != NULL)
    {
        if (inverse_fft)
        {
            riscv_vec_inverse_butterfly_f16 (out, in, cfg->factors, cfg->twiddles, cfg->buffer);
        }
        else
        {
            riscv_vec_butterfly_f16 (out, in, cfg->factors, cfg->twiddles, cfg->buffer);
        }
    }
}

/**
 * @ingroup R2C_FFT_IFFT
 * @brief Creates a configuration structure for variants of @ref riscv_vec_rfft_f16 and @ref riscv_vec_rifft_f16.
 *
 * @param[in]   nfft             input length
 * @retval      st               pointer to an FFT configuration structure (allocated with `malloc`), or `NULL` to indicate an error
 *
 * Allocates and initialises an @ref riscv_vec_rfft_cfg_f16_t configuration structure for
 * the FP32 real-to-complex and complex-to-real FFT/IFFT. As part of this, it reserves a buffer used
 * internally by the FFT algorithm, factors the length of the FFT into simpler chunks, and generates a
 * "twiddle table" of coefficients used in the FFT "butterfly" calculations.
 */
/**
 * rfft_f16
 */
void riscv_vec_rfft_f16 (riscv_vec_cpx_f16_t * FUNC_RESTRICT out,
                         float16_t * FUNC_RESTRICT in,
                         riscv_vec_rfft_cfg_f16_t cfg)
{
    if(cfg != NULL)
    {
        riscv_vec_cpx_f16_t * tmpbuf = cfg->buffer;

        riscv_vec_butterfly_f16 (tmpbuf, (riscv_vec_cpx_f16_t*) in, cfg->factors, cfg->twiddles, out);
        _vec_split_r2c_1d_f16 (out, tmpbuf, cfg->super_twiddles, cfg->ncfft);

    }
}

#endif  //ENA_STOCKHAM_FFT

/******************************************************************************
 * Copyright (C) 2010-2023 Arm Limited or its affiliates. All rights reserved.*
 * Copyright (C) 2018-2023 Andes Technology Corporation. All rights reserved. *
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

/*
 * This function calculates the FFT for power-of-two input sizes using an ordered, mixed
 * radix-4/8 DIT algorithm.
 *
 * At each stage, 'fstride' holds the number of butterfly "sections" to be processed,
 * while 'mstride' holds the number of butterflies to be performed in each section. After
 * radix-4 butterflies, for example, we quarter the the 'fstride' (number of sections) and
 * quadruple the 'mstride' (size of each section) for the next stage. The exception to
 * this is the first stage, in which 'mstride' does not apply (as it is implicitly 1).
 *
 * The algorithm first performs either a radix-8 or radix-4 pass, depending on the size
 * of the input/output (as dictated by the 'factors' array), and then continually applies
 * radix-4 butterflies to completion. The order in which results are stored after each
 * stage allows stages to load and store elements contiguously between iterations, and for
 * the final output order to be correct.
 */

#include "internal_vec_transform.h"
#include "internal_nds_types.h"

#ifdef  ENA_STOCKHAM_FFT
//CIFFT
void riscv_vec_inverse_butterfly_f64 (riscv_vec_cpx_f64_t *out,
                                      riscv_vec_cpx_f64_t *in,
                                      q31_t *factors,
                                      riscv_vec_cpx_f64_t *twiddles,
                                      riscv_vec_cpx_f64_t *buffer)
{
    // pure C version
    uint32_t input_m = factors[SIZE_FIRST_STAGE];
    uint32_t step = 1;
    uint32_t stage_num = factors[OUT_INDEX];
    uint32_t stage_idx = 0;
    float64_t tmp_r, tmp_i;
    uint32_t tmp_q = 0, tmp_p = 0 ;
    riscv_vec_cpx_f64_t *out_val = NULL ;
    riscv_vec_cpx_f64_t *in_val = NULL  ;
    riscv_vec_cpx_f64_t *out_next = NULL  ;
    riscv_vec_cpx_f64_t tmp_cal_1, tmp_cal_2, tmp_cal_wp;
    float64_t inv_factor = 1.0f / ((float64_t)(2*input_m));  // 2*input_m = fft_size
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
    
    /*
        IFFT used the same twiddles table with FFT,
        that needs to add a "-" of image term of twiddle
        for the conjugate.
    */
    for(tmp_p =0; tmp_p < input_m ; tmp_p++)
    {
        tmp_cal_wp.r = twiddles[step*tmp_p].r;
        tmp_cal_wp.i = -twiddles[step*tmp_p].i;
        for( tmp_q = 0 ; tmp_q < step; tmp_q++)
        {
            tmp_cal_1.r = in_val[tmp_q + step*(tmp_p + 0)].r;
            tmp_cal_2.r = in_val[tmp_q + step*(tmp_p + input_m)].r;
            tmp_cal_1.i = in_val[tmp_q + step*(tmp_p + 0)].i;
            tmp_cal_2.i = in_val[tmp_q + step*(tmp_p + input_m)].i;
            out_val[tmp_q + step*(2*tmp_p + 0)].r =  inv_factor*(tmp_cal_1.r+tmp_cal_2.r);
            out_val[tmp_q + step*(2*tmp_p + 0)].i =  inv_factor*(tmp_cal_1.i+tmp_cal_2.i);
            tmp_r = tmp_cal_1.r - tmp_cal_2.r;
            tmp_i = tmp_cal_1.i - tmp_cal_2.i;
            out_val[tmp_q + step*(2*tmp_p + 1)].r = inv_factor*(tmp_r*tmp_cal_wp.r - tmp_i*tmp_cal_wp.i);
            out_val[tmp_q + step*(2*tmp_p + 1)].i = inv_factor*(tmp_r*tmp_cal_wp.i + tmp_i*tmp_cal_wp.r);
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
            tmp_cal_wp.i = -twiddles[step*tmp_p].i;
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
                out_val[tmp_q + step*(2*tmp_p + 1)].r = tmp_r*tmp_cal_wp.r - tmp_i*tmp_cal_wp.i;
                out_val[tmp_q + step*(2*tmp_p + 1)].i = tmp_r*tmp_cal_wp.i + tmp_i*tmp_cal_wp.r;
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

static void _vec_split_c2r_1d_f64 (riscv_vec_cpx_f64_t *dst,
                                   const riscv_vec_cpx_f64_t *src,
                                   riscv_vec_cpx_f64_t *twiddles,
                                   q31_t ncfft)
{
    riscv_vec_cpx_f64_t *out_val = dst;
    const riscv_vec_cpx_f64_t *in_val  = src;
    q31_t size = ncfft;
    riscv_vec_cpx_f64_t tmp_cal_1, tmp_cal_2, tmp_cal_wp;
    float64_t *wp_tbl_rifft_r = NULL, *wp_tbl_rifft_i = NULL;
    q31_t tmp_q = 0;
    wp_tbl_rifft_r = &twiddles[0].r;
    wp_tbl_rifft_i = &twiddles[ncfft/2].r;

    tmp_q = 0 ;
    tmp_cal_1.r = in_val[tmp_q].r + in_val[size- tmp_q].r;
    tmp_cal_1.i = in_val[tmp_q].r - in_val[size- tmp_q].r ;
    out_val[tmp_q].r = 0.5*(tmp_cal_1.r);
    out_val[tmp_q].i = 0.5*(tmp_cal_1.i);

    for (tmp_q = 1; tmp_q < size; tmp_q++)
    {
        tmp_cal_1.r = in_val[tmp_q].r + in_val[size- tmp_q].r;
        tmp_cal_1.i = in_val[tmp_q].i - in_val[size- tmp_q].i;
        tmp_cal_2.r = -(in_val[tmp_q].i + in_val[size- tmp_q].i);
        tmp_cal_2.i = in_val[tmp_q].r - in_val[size- tmp_q].r;
        tmp_cal_wp.r = wp_tbl_rifft_r[tmp_q];
        tmp_cal_wp.i = -wp_tbl_rifft_i[tmp_q];
        out_val[tmp_q].r = 0.5*(tmp_cal_1.r + (tmp_cal_2.r*tmp_cal_wp.r - tmp_cal_2.i*tmp_cal_wp.i));
        out_val[tmp_q].i = 0.5*(tmp_cal_1.i + (tmp_cal_2.r*tmp_cal_wp.i + tmp_cal_2.i*tmp_cal_wp.r));
    }

}

/**
 * rifft_f64
 */
void  riscv_vec_rifft_f64 (float64_t *out,
                          riscv_vec_cpx_f64_t *in,
                          riscv_vec_rfft_cfg_f64_t cfg)
{
    if(cfg != NULL)
    {
        riscv_vec_cpx_f64_t * tmpbuf1 = cfg->buffer;
        riscv_vec_cpx_f64_t * tmpbuf2 = cfg->buffer + cfg->ncfft;

        _vec_split_c2r_1d_f64 (tmpbuf1, in, cfg->super_twiddles, cfg->ncfft);
        riscv_vec_inverse_butterfly_f64 ( (riscv_vec_cpx_f64_t*) out, tmpbuf1, cfg->factors, cfg->twiddles, tmpbuf2);

    }
}

#endif  //ENA_STOCKHAM_FFT

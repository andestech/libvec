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
//#include "internal_nds_types.h"

#include "riscv_vec_matrix.h"


#ifdef  ENA_STOCKHAM_FFT
//CIFFT
void riscv_vec_inverse_butterfly_f32 (riscv_vec_cpx_f32_t * FUNC_RESTRICT out,
                                      riscv_vec_cpx_f32_t * FUNC_RESTRICT in,
                                      q31_t * FUNC_RESTRICT factors,
                                      riscv_vec_cpx_f32_t * FUNC_RESTRICT twiddles,
                                      riscv_vec_cpx_f32_t * FUNC_RESTRICT buffer)
{
    // pure C version
    uint32_t input_m = factors[SIZE_FIRST_STAGE];
    uint32_t step = 1;
    uint32_t stage_num = factors[OUT_INDEX];
    uint32_t stage_idx = 0;
    float tmp_r, tmp_i;
    uint32_t tmp_q = 0, tmp_p = 0 ;
    riscv_vec_cpx_f32_t *out_val = NULL ;
    riscv_vec_cpx_f32_t *in_val = NULL  ;
    riscv_vec_cpx_f32_t *out_next = NULL  ;
    riscv_vec_cpx_f32_t tmp_cal_1, tmp_cal_2, tmp_cal_wp;
    float32_t inv_factor = 1.0f / ((float32_t)(2 * input_m)); // 2*input_m = fft_size
    if(stage_num % 2 == 0)
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
    for(tmp_p = 0; tmp_p < input_m ; tmp_p++)
    {
        tmp_cal_wp.r = twiddles[step * tmp_p].r;
        tmp_cal_wp.i = -twiddles[step * tmp_p].i;
        for( tmp_q = 0 ; tmp_q < step; tmp_q++)
        {
            tmp_cal_1.r = in_val[tmp_q + step * (tmp_p + 0)].r;
            tmp_cal_2.r = in_val[tmp_q + step * (tmp_p + input_m)].r;
            tmp_cal_1.i = in_val[tmp_q + step * (tmp_p + 0)].i;
            tmp_cal_2.i = in_val[tmp_q + step * (tmp_p + input_m)].i;
            out_val[tmp_q + step * (2 * tmp_p + 0)].r =  inv_factor * (tmp_cal_1.r + tmp_cal_2.r);
            out_val[tmp_q + step * (2 * tmp_p + 0)].i =  inv_factor * (tmp_cal_1.i + tmp_cal_2.i);
            tmp_r = tmp_cal_1.r - tmp_cal_2.r;
            tmp_i = tmp_cal_1.i - tmp_cal_2.i;
            out_val[tmp_q + step * (2 * tmp_p + 1)].r = inv_factor * (tmp_r * tmp_cal_wp.r - tmp_i * tmp_cal_wp.i);
            out_val[tmp_q + step * (2 * tmp_p + 1)].i = inv_factor * (tmp_r * tmp_cal_wp.i + tmp_i * tmp_cal_wp.r);
        }
    }

    in_val = out_val;
    out_val = out_next;
    out_next = in_val;
    step = step * 2;
    input_m = input_m / 2 ;
    for(stage_idx = 1 ; stage_idx < stage_num - 1; stage_idx++)
    {
        for(tmp_p = 0; tmp_p < input_m ; tmp_p++)
        {
            tmp_cal_wp.r = twiddles[step * tmp_p].r;
            tmp_cal_wp.i = -twiddles[step * tmp_p].i;
            for( tmp_q = 0 ; tmp_q < step; tmp_q++)
            {
                tmp_cal_1.r = in_val[tmp_q + step * (tmp_p + 0)].r;
                tmp_cal_2.r = in_val[tmp_q + step * (tmp_p + input_m)].r;
                tmp_cal_1.i = in_val[tmp_q + step * (tmp_p + 0)].i;
                tmp_cal_2.i = in_val[tmp_q + step * (tmp_p + input_m)].i;
                out_val[tmp_q + step * (2 * tmp_p + 0)].r =  tmp_cal_1.r + tmp_cal_2.r;
                out_val[tmp_q + step * (2 * tmp_p + 0)].i =  tmp_cal_1.i + tmp_cal_2.i;
                tmp_r = tmp_cal_1.r - tmp_cal_2.r;
                tmp_i = tmp_cal_1.i - tmp_cal_2.i;
                out_val[tmp_q + step * (2 * tmp_p + 1)].r = tmp_r * tmp_cal_wp.r - tmp_i * tmp_cal_wp.i;
                out_val[tmp_q + step * (2 * tmp_p + 1)].i = tmp_r * tmp_cal_wp.i + tmp_i * tmp_cal_wp.r;
            }
        }
        step = step * 2 ;
        in_val = out_val;
        out_val = out_next;
        out_next = in_val;
        input_m = input_m / 2 ;
    }

    // last radix-2
    for (tmp_q = 0; tmp_q < step; tmp_q++)
    {
        tmp_cal_1.r = in_val[tmp_q + step * 0].r;
        tmp_cal_1.i = in_val[tmp_q + step * 0].i;
        tmp_cal_2.r = in_val[tmp_q + step * 1].r;
        tmp_cal_2.i = in_val[tmp_q + step * 1].i;

        out_val[tmp_q + step * 0].r = tmp_cal_1.r + tmp_cal_2.r;
        out_val[tmp_q + step * 0].i = tmp_cal_1.i + tmp_cal_2.i;

        out_val[tmp_q + step * 1].r = tmp_cal_1.r - tmp_cal_2.r;
        out_val[tmp_q + step * 1].i = tmp_cal_1.i - tmp_cal_2.i;
    }

}

static void _vec_split_c2r_1d_f32 (riscv_vec_cpx_f32_t * FUNC_RESTRICT dst,
                                   const riscv_vec_cpx_f32_t * FUNC_RESTRICT src,
                                   riscv_vec_cpx_f32_t * FUNC_RESTRICT twiddles,
                                   q31_t ncfft)
{
    riscv_vec_cpx_f32_t *out_val = dst;
    const riscv_vec_cpx_f32_t *in_val  = src;
    q31_t size = ncfft;
    riscv_vec_cpx_f32_t tmp_cal_1, tmp_cal_2, tmp_cal_wp;
    float32_t *wp_tbl_rifft_r = NULL, *wp_tbl_rifft_i = NULL;
    q31_t tmp_q = 0;
    wp_tbl_rifft_r = &twiddles[0].r;
    wp_tbl_rifft_i = &twiddles[ncfft / 2].r;

    tmp_q = 0 ;
    tmp_cal_1.r = in_val[tmp_q].r + in_val[size - tmp_q].r;
    tmp_cal_1.i = in_val[tmp_q].r - in_val[size - tmp_q].r ;
    out_val[tmp_q].r = 0.5 * (tmp_cal_1.r);
    out_val[tmp_q].i = 0.5 * (tmp_cal_1.i);

    for (tmp_q = 1; tmp_q < size; tmp_q++)
    {
        tmp_cal_1.r = in_val[tmp_q].r + in_val[size - tmp_q].r;
        tmp_cal_1.i = in_val[tmp_q].i - in_val[size - tmp_q].i;
        tmp_cal_2.r = -(in_val[tmp_q].i + in_val[size - tmp_q].i);
        tmp_cal_2.i = in_val[tmp_q].r - in_val[size - tmp_q].r;
        tmp_cal_wp.r = wp_tbl_rifft_r[tmp_q];
        tmp_cal_wp.i = -wp_tbl_rifft_i[tmp_q];
        out_val[tmp_q].r = 0.5 * (tmp_cal_1.r + (tmp_cal_2.r * tmp_cal_wp.r - tmp_cal_2.i * tmp_cal_wp.i));
        out_val[tmp_q].i = 0.5 * (tmp_cal_1.i + (tmp_cal_2.r * tmp_cal_wp.i + tmp_cal_2.i * tmp_cal_wp.r));
    }

}

/**
 * rifft_f32
 */
void riscv_vec_rifft_f32 (float32_t * FUNC_RESTRICT out,
                          riscv_vec_cpx_f32_t * FUNC_RESTRICT in,
                          riscv_vec_rfft_cfg_f32_t cfg)
{
    if( cfg != NULL)
    {
        riscv_vec_cpx_f32_t * tmpbuf1 = cfg->buffer;
        riscv_vec_cpx_f32_t * tmpbuf2 = cfg->buffer + cfg->ncfft;

        _vec_split_c2r_1d_f32 (tmpbuf1, in, cfg->super_twiddles, cfg->ncfft);
        riscv_vec_inverse_butterfly_f32 ( (riscv_vec_cpx_f32_t*) out, tmpbuf1, cfg->factors, cfg->twiddles, tmpbuf2);

    }
}

void riscv_vec_rifft2d_f32 (float32_t * FUNC_RESTRICT out,
                            riscv_vec_cpx_f32_t * FUNC_RESTRICT in,
                            riscv_vec_rfft2d_cfg_f32_t cfg)
{
    q31_t tmp_idx = 0, tmp_cp = 0;
    riscv_vec_cpx_f32_t *tmp_1d = (riscv_vec_cpx_f32_t*)&cfg->buffer_1d[0];
    riscv_vec_cpx_f32_t *tmp_buff = (riscv_vec_cpx_f32_t*)&cfg->buffer_2d[0];
    riscv_vec_cpx_f32_t *tmp_out = NULL;
    riscv_vec_cpx_f32_t *tmp_in = (riscv_vec_cpx_f32_t*)&cfg->tmp_1d_input[0];
    riscv_vec_cpx_f32_t *data_in = NULL;
    float32_t *data_out = NULL;
    riscv_vec_cpx_f32_t *cp_data_in = in;
    q31_t m = cfg->m;
    q31_t n = cfg->ncfft;
    if( cfg != NULL)
    {
        // 1st: process (n+1) times cifft for m-pts
        for(tmp_idx = 0; tmp_idx < n + 1 ; tmp_idx++)
        {
            tmp_out = (riscv_vec_cpx_f32_t*) &tmp_buff[tmp_idx * (m)];
            cp_data_in = &in[tmp_idx];
            data_in = tmp_in;
            for(tmp_cp = 0 ; tmp_cp < m ; tmp_cp++)
            {
                data_in[tmp_cp].r = cp_data_in[tmp_cp * (n + 1)].r;
                data_in[tmp_cp].i = cp_data_in[tmp_cp * (n + 1)].i;
            }
            riscv_vec_inverse_butterfly_f32 (tmp_out, data_in, cfg->factors_m, cfg->twiddles_m, tmp_1d);
        }

        // 2nd: process m-times rifft for n-pts
        for(tmp_idx = 0 ; tmp_idx < m ; tmp_idx++)
        {
            data_out = (float32_t*) &out[tmp_idx * (2 * n)];
            cp_data_in = &tmp_buff[tmp_idx];
            data_in = tmp_in;
            riscv_vec_cpx_f32_t * tmp_split = cfg-> tmp_split;
            for(tmp_cp = 0 ; tmp_cp < (n + 1) ; tmp_cp++)
            {
                data_in[tmp_cp].r = cp_data_in[tmp_cp * (m)].r;
                data_in[tmp_cp].i = cp_data_in[tmp_cp * (m)].i;
            }

            _vec_split_c2r_1d_f32 (tmp_split, data_in, cfg->super_twiddles, cfg->ncfft);
            riscv_vec_inverse_butterfly_f32 ((riscv_vec_cpx_f32_t*) data_out, tmp_split, cfg->factors_n, cfg->twiddles_n, tmp_1d);
        }

    }
}


#else

//CIFFT
void riscv_vec_inverse_butterfly_f32 (riscv_vec_cpx_f32_t * FUNC_RESTRICT out,
                                      riscv_vec_cpx_f32_t * FUNC_RESTRICT in,
                                      q31_t * FUNC_RESTRICT factors,
                                      riscv_vec_cpx_f32_t * FUNC_RESTRICT twiddles,
                                      riscv_vec_cpx_f32_t * FUNC_RESTRICT buffer)
{
    q31_t stage_count = factors[0];
    q31_t fstride = factors[1];
    q31_t mstride = factors[(stage_count << 1) - 1];
    q31_t first_radix = factors[stage_count << 1];
    float32_t one_by_nfft = (1.0f / (float32_t) (fstride * first_radix));
    q31_t step, f_count, m_count;
    riscv_vec_cpx_f32_t *src = in;
    riscv_vec_cpx_f32_t *dst = out;
    riscv_vec_cpx_f32_t *out_final = out;
    riscv_vec_cpx_f32_t *tw, *tmp;
    const float32_t TW_81 = 0.70710678;

    riscv_vec_cpx_f32_t scratch[16];
    riscv_vec_cpx_f32_t scratch_in[8];
    riscv_vec_cpx_f32_t scratch_out[8];
    riscv_vec_cpx_f32_t scratch_tw[6];

    // The first stage (using hardcoded twiddles)
    if (first_radix == 8) // nfft is of form 2^{odd}
    {
        for (f_count = 0; f_count < fstride; f_count++)
        {
            dst = &out[f_count * 8];

            // Prepare sums for the butterfly calculations
            scratch_in[0].r = src[0].r + src[fstride * 4].r;
            scratch_in[0].i = src[0].i + src[fstride * 4].i;
            scratch_in[1].r = src[0].r - src[fstride * 4].r;
            scratch_in[1].i = src[0].i - src[fstride * 4].i;
            scratch_in[2].r = src[fstride].r + src[fstride * 5].r;
            scratch_in[2].i = src[fstride].i + src[fstride * 5].i;
            scratch_in[3].r = src[fstride].r - src[fstride * 5].r;
            scratch_in[3].i = src[fstride].i - src[fstride * 5].i;
            scratch_in[4].r = src[fstride * 2].r + src[fstride * 6].r;
            scratch_in[4].i = src[fstride * 2].i + src[fstride * 6].i;
            scratch_in[5].r = src[fstride * 2].r - src[fstride * 6].r;
            scratch_in[5].i = src[fstride * 2].i - src[fstride * 6].i;
            scratch_in[6].r = src[fstride * 3].r + src[fstride * 7].r;
            scratch_in[6].i = src[fstride * 3].i + src[fstride * 7].i;
            scratch_in[7].r = src[fstride * 3].r - src[fstride * 7].r;
            scratch_in[7].i = src[fstride * 3].i - src[fstride * 7].i;

            // Multiply some of these by hardcoded radix-8 twiddles
            scratch[0] = scratch_in[0];
            scratch[1] = scratch_in[1];
            scratch[2] = scratch_in[2];
            scratch[3].r = (scratch_in[3].r - scratch_in[3].i) * TW_81;
            scratch[3].i = (scratch_in[3].i + scratch_in[3].r) * TW_81;
            scratch[4] = scratch_in[4];
            scratch[5].r = -scratch_in[5].i;
            scratch[5].i = scratch_in[5].r;
            scratch[6].r = scratch_in[6].r;
            scratch[6].i = scratch_in[6].i;
            scratch[7].r = (scratch_in[7].r + scratch_in[7].i) * TW_81;
            scratch[7].i = (scratch_in[7].i - scratch_in[7].r) * TW_81;

            // Combine the first set of pairs of these sums
            scratch[8].r = scratch[0].r  + scratch[4].r;
            scratch[8].i = scratch[0].i  + scratch[4].i;
            scratch[9].r = scratch[1].r  + scratch[5].r;
            scratch[9].i = scratch[1].i  + scratch[5].i;
            scratch[10].r = scratch[0].r - scratch[4].r;
            scratch[10].i = scratch[0].i - scratch[4].i;
            scratch[11].r = scratch[1].r - scratch[5].r;
            scratch[11].i = scratch[1].i - scratch[5].i;

            // Combine the second set of pairs of these sums
            scratch[12].r = scratch[2].r + scratch[6].r;
            scratch[12].i = scratch[2].i + scratch[6].i;
            scratch[13].r = scratch[3].r - scratch[7].r;
            scratch[13].i = scratch[3].i - scratch[7].i;
            scratch[14].r = scratch[2].r - scratch[6].r;
            scratch[14].i = scratch[2].i - scratch[6].i;
            scratch[15].r = scratch[3].r + scratch[7].r;
            scratch[15].i = scratch[3].i + scratch[7].i;

            // Combine these combined components (for the full radix-8 butterfly)
            scratch_out[0].r = scratch[8].r  + scratch[12].r;
            scratch_out[0].i = scratch[8].i  + scratch[12].i;
            scratch_out[1].r = scratch[9].r  + scratch[13].r;
            scratch_out[1].i = scratch[9].i  + scratch[13].i;
            scratch_out[2].r = scratch[10].r - scratch[14].i;
            scratch_out[2].i = scratch[10].i + scratch[14].r;
            scratch_out[3].r = scratch[11].r - scratch[15].i;
            scratch_out[3].i = scratch[11].i + scratch[15].r;
            scratch_out[4].r = scratch[8].r  - scratch[12].r;
            scratch_out[4].i = scratch[8].i  - scratch[12].i;
            scratch_out[5].r = scratch[9].r  - scratch[13].r;
            scratch_out[5].i = scratch[9].i  - scratch[13].i;
            scratch_out[6].r = scratch[10].r + scratch[14].i;
            scratch_out[6].i = scratch[10].i - scratch[14].r;
            scratch_out[7].r = scratch[11].r + scratch[15].i;
            scratch_out[7].i = scratch[11].i - scratch[15].r;

            // Store the results
            dst[0] = scratch_out[0];
            dst[1] = scratch_out[1];
            dst[2] = scratch_out[2];
            dst[3] = scratch_out[3];
            dst[4] = scratch_out[4];
            dst[5] = scratch_out[5];
            dst[6] = scratch_out[6];
            dst[7] = scratch_out[7];

            src++;
        } // f_count

        // Update variables for the next stages
        step = fstride << 1;
        stage_count--;
        fstride /= 4;

        if (stage_count == 0)
        {
            dst = out;
            for (f_count = 0; f_count < 8; f_count++)
            {
                dst[f_count].r *= one_by_nfft;
                dst[f_count].i *= one_by_nfft;
            }
        }
    }
    else if (first_radix == 4) // nfft is of form 2^{even} >= 4
    {
        for (f_count = fstride; f_count; f_count--)
        {
            // Load the four input values for a radix-4 butterfly
            scratch_in[0] = src[0];
            scratch_in[1] = src[fstride * 1];
            scratch_in[2] = src[fstride * 2];
            scratch_in[3] = src[fstride * 3];

            // Prepare the first set of sums for the butterfly calculations
            scratch[0].r = scratch_in[0].r + scratch_in[2].r;
            scratch[0].i = scratch_in[0].i + scratch_in[2].i;
            scratch[1].r = scratch_in[0].r - scratch_in[2].r;
            scratch[1].i = scratch_in[0].i - scratch_in[2].i;

            // Prepare the second set of sums for the butterfly calculations
            scratch[2].r = scratch_in[1].r + scratch_in[3].r;
            scratch[2].i = scratch_in[1].i + scratch_in[3].i;
            scratch[3].r = scratch_in[1].r - scratch_in[3].r;
            scratch[3].i = scratch_in[1].i - scratch_in[3].i;

            // Combine these sums (for the full radix-4 butterfly)
            scratch_out[0].r = scratch[0].r + scratch[2].r;
            scratch_out[0].i = scratch[0].i + scratch[2].i;
            scratch_out[1].r = scratch[1].r - scratch[3].i;
            scratch_out[1].i = scratch[1].i + scratch[3].r;
            scratch_out[2].r = scratch[0].r - scratch[2].r;
            scratch_out[2].i = scratch[0].i - scratch[2].i;
            scratch_out[3].r = scratch[1].r + scratch[3].i;
            scratch_out[3].i = scratch[1].i - scratch[3].r;

            // Store the results
            *dst++ = scratch_out[0];
            *dst++ = scratch_out[1];
            *dst++ = scratch_out[2];
            *dst++ = scratch_out[3];

            src++;
        } // f_count

        // Update variables for the next stages
        step = fstride;
        stage_count--;
        fstride /= 4;

        if (stage_count == 0)
        {
            dst = out;
            for (f_count = 0; f_count < 4; f_count++)
            {
                dst[f_count].r *= one_by_nfft;
                dst[f_count].i *= one_by_nfft;
            }
        }
    }
    else if (first_radix == 2) // nfft = 2
    {
        dst[0].r = (src[0].r + src[1].r) * one_by_nfft;
        dst[0].i = (src[0].i + src[1].i) * one_by_nfft;
        dst[1].r = (src[0].r - src[1].r) * one_by_nfft;
        dst[1].i = (src[0].i - src[1].i) * one_by_nfft;
        return;
    }
    else // nfft = 1
    {
        dst[0] = src[0];
        return;
    }

    // The next stage should read the output of the first stage as input
    in = out;
    out = buffer;

    // Middle stages (after the first, excluding the last)
    for (; stage_count > 1; stage_count--)
    {
        src = in;
        for (f_count = 0; f_count < fstride; f_count++)
        {
            dst = &out[f_count * (mstride * 4)];
            tw = twiddles;
            for (m_count = mstride; m_count ; m_count --)
            {
                // Load the three twiddles and four input values for a radix-4 butterfly
                scratch_tw[0] = tw[0];
                scratch_tw[1] = tw[mstride * 1];
                scratch_tw[2] = tw[mstride * 2];
                scratch_in[0] = src[0];
                scratch_in[1] = src[step * 1];
                scratch_in[2] = src[step * 2];
                scratch_in[3] = src[step * 3];

                // Multiply input elements by their associated twiddles
                scratch[0] = scratch_in[0];
                scratch[1].r = scratch_in[1].r * scratch_tw[0].r + scratch_in[1].i * scratch_tw[0].i;
                scratch[1].i = scratch_in[1].i * scratch_tw[0].r - scratch_in[1].r * scratch_tw[0].i;
                scratch[2].r = scratch_in[2].r * scratch_tw[1].r + scratch_in[2].i * scratch_tw[1].i;
                scratch[2].i = scratch_in[2].i * scratch_tw[1].r - scratch_in[2].r * scratch_tw[1].i;
                scratch[3].r = scratch_in[3].r * scratch_tw[2].r + scratch_in[3].i * scratch_tw[2].i;
                scratch[3].i = scratch_in[3].i * scratch_tw[2].r - scratch_in[3].r * scratch_tw[2].i;

                // Prepare the first set of sums for the butterfly calculations
                scratch[4].r = scratch[0].r + scratch[2].r;
                scratch[4].i = scratch[0].i + scratch[2].i;
                scratch[5].r = scratch[0].r - scratch[2].r;
                scratch[5].i = scratch[0].i - scratch[2].i;

                // Prepare the second set of sums for the butterfly calculations
                scratch[6].r = scratch[1].r + scratch[3].r;
                scratch[6].i = scratch[1].i + scratch[3].i;
                scratch[7].r = scratch[1].r - scratch[3].r;
                scratch[7].i = scratch[1].i - scratch[3].i;

                // Combine these sums (for the full radix-4 butterfly)
                scratch_out[0].r = scratch[4].r + scratch[6].r;
                scratch_out[0].i = scratch[4].i + scratch[6].i;
                scratch_out[1].r = scratch[5].r - scratch[7].i;
                scratch_out[1].i = scratch[5].i + scratch[7].r;
                scratch_out[2].r = scratch[4].r - scratch[6].r;
                scratch_out[2].i = scratch[4].i - scratch[6].i;
                scratch_out[3].r = scratch[5].r + scratch[7].i;
                scratch_out[3].i = scratch[5].i - scratch[7].r;

                // Store the results
                dst[0] = scratch_out[0];
                dst[mstride * 1] = scratch_out[1];
                dst[mstride * 2] = scratch_out[2];
                dst[mstride * 3] = scratch_out[3];

                tw++;
                src++;
                dst++;
            } // m_count
        } // f_count

        // Update variables for the next stages
        twiddles += mstride * 3;
        mstride *= 4;
        fstride /= 4;

        // Swap the input and output buffers for the next stage
        tmp = in;
        in = out;
        out = tmp;
    } // stage_count

    // The last stage
    if (stage_count)
    {
        src = in;

        // Always write to the final output buffer (if necessary, we can calculate this
        // in-place as the final stage reads and writes at the same offsets)
        dst = out_final;

        for (f_count = 0; f_count < fstride; f_count++)
        {
            tw = twiddles;
            for (m_count = mstride; m_count; m_count--)
            {
                // Load the three twiddles and four input values for a radix-4 butterfly
                scratch_tw[0] = tw[0];
                scratch_tw[1] = tw[mstride * 1];
                scratch_tw[2] = tw[mstride * 2];
                scratch_in[0] = src[0];
                scratch_in[1] = src[step * 1];
                scratch_in[2] = src[step * 2];
                scratch_in[3] = src[step * 3];

                // Multiply input elements by their associated twiddles
                scratch[0] = scratch_in[0];
                scratch[1].r = scratch_in[1].r * scratch_tw[0].r + scratch_in[1].i * scratch_tw[0].i;
                scratch[1].i = scratch_in[1].i * scratch_tw[0].r - scratch_in[1].r * scratch_tw[0].i;
                scratch[2].r = scratch_in[2].r * scratch_tw[1].r + scratch_in[2].i * scratch_tw[1].i;
                scratch[2].i = scratch_in[2].i * scratch_tw[1].r - scratch_in[2].r * scratch_tw[1].i;
                scratch[3].r = scratch_in[3].r * scratch_tw[2].r + scratch_in[3].i * scratch_tw[2].i;
                scratch[3].i = scratch_in[3].i * scratch_tw[2].r - scratch_in[3].r * scratch_tw[2].i;

                // Prepare the first set of sums for the butterfly calculations
                scratch[4].r = scratch[0].r + scratch[2].r;
                scratch[4].i = scratch[0].i + scratch[2].i;
                scratch[5].r = scratch[0].r - scratch[2].r;
                scratch[5].i = scratch[0].i - scratch[2].i;

                // Prepare the second set of sums for the butterfly calculations
                scratch[6].r = scratch[1].r + scratch[3].r;
                scratch[6].i = scratch[1].i + scratch[3].i;
                scratch[7].r = scratch[1].r - scratch[3].r;
                scratch[7].i = scratch[1].i - scratch[3].i;

                // Combine these sums (for the full radix-4 butterfly) and multiply by
                // (1 / nfft).
                scratch_out[0].r = (scratch[4].r + scratch[6].r) * one_by_nfft;
                scratch_out[0].i = (scratch[4].i + scratch[6].i) * one_by_nfft;
                scratch_out[1].r = (scratch[5].r - scratch[7].i) * one_by_nfft;
                scratch_out[1].i = (scratch[5].i + scratch[7].r) * one_by_nfft;
                scratch_out[2].r = (scratch[4].r - scratch[6].r) * one_by_nfft;
                scratch_out[2].i = (scratch[4].i - scratch[6].i) * one_by_nfft;
                scratch_out[3].r = (scratch[5].r + scratch[7].i) * one_by_nfft;
                scratch_out[3].i = (scratch[5].i - scratch[7].r) * one_by_nfft;

                // Store the results
                dst[0] = scratch_out[0];
                dst[step * 1] = scratch_out[1];
                dst[step * 2] = scratch_out[2];
                dst[step * 3] = scratch_out[3];

                tw++;
                src++;
                dst++;
            } // m_count
        } // f_count
    } // last stage
}

static void _vec_split_c2r_1d_f32 (riscv_vec_cpx_f32_t * FUNC_RESTRICT dst,
                                   const riscv_vec_cpx_f32_t * FUNC_RESTRICT src,
                                   riscv_vec_cpx_f32_t * FUNC_RESTRICT twiddles,
                                   q31_t ncfft)
{

    q31_t k;
    riscv_vec_cpx_f32_t fk, fnkc, fek, fok, tmp;


    dst[0].r = (src[0].r + src[ncfft].r) * 0.5f;
    dst[0].i = (src[0].r - src[ncfft].r) * 0.5f;

    for (k = 1; k <= ncfft / 2; k++)
    {
        fk = src[k];
        fnkc.r = src[ncfft - k].r;
        fnkc.i = -src[ncfft - k].i;

        fek.r = fk.r + fnkc.r;
        fek.i = fk.i + fnkc.i;

        tmp.r = fk.r - fnkc.r;
        tmp.i = fk.i - fnkc.i;

        fok.r = tmp.r * twiddles[k - 1].r + tmp.i * twiddles[k - 1].i;
        fok.i = tmp.i * twiddles[k - 1].r - tmp.r * twiddles[k - 1].i;

        dst[k].r = (fek.r + fok.r) * 0.5f;
        dst[k].i = (fek.i + fok.i) * 0.5f;

        dst[ncfft - k].r = (fek.r - fok.r) * 0.5f;
        dst[ncfft - k].i = (fok.i - fek.i) * 0.5f;
    }
}

/**
 * rifft_f32
 */
void  riscv_vec_rifft_f32 (float32_t * FUNC_RESTRICT out,
                           riscv_vec_cpx_f32_t * FUNC_RESTRICT in,
                           riscv_vec_rfft_cfg_f32_t cfg)
{
    if( cfg != NULL)
    {
        riscv_vec_cpx_f32_t * tmpbuf1 = cfg->buffer;
        riscv_vec_cpx_f32_t * tmpbuf2 = cfg->buffer + cfg->ncfft;

        _vec_split_c2r_1d_f32 (tmpbuf1, in, cfg->super_twiddles, cfg->ncfft);
        riscv_vec_inverse_butterfly_f32 ( (riscv_vec_cpx_f32_t*) out, tmpbuf1, cfg->factors, cfg->twiddles, tmpbuf2);

    }
}

#endif  //ENA_STOCKHAM_FFT

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

#ifdef  ENA_STOCKHAM_FFT
void riscv_vec_inverse_butterfly_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT Fout,
                                      riscv_vec_cpx_q31_t * FUNC_RESTRICT Fin,
                                      q31_t * FUNC_RESTRICT factors,
                                      riscv_vec_cpx_q31_t * FUNC_RESTRICT twiddles,
                                      riscv_vec_cpx_q31_t * FUNC_RESTRICT buffer,
                                      q31_t scaled_flag)
{
    // pure C version
    q31_t input_m = factors[SIZE_FIRST_STAGE];
    q31_t step = 1;
    q31_t stage_num = factors[OUT_INDEX];
    q31_t stage_idx = 0;
    q31_t tmp_r, tmp_i;
    q31_t tmp_q = 0, tmp_p = 0 ;
    uint32_t q_shift_m = 0;
    //q31_t debug_size = input_m*2 ;
    uint8_t rfft_en = (uint8_t) factors[RFFT_EN];
    riscv_vec_cpx_q31_t *out_val = NULL ;
    riscv_vec_cpx_q31_t *in_val = NULL  ;
    riscv_vec_cpx_q31_t *out_next = NULL  ;
    riscv_vec_cpx_q31_t tmp_cal_1, tmp_cal_2, tmp_cal_wp;
    // scale will shift the fixed point result.
    // aligned the result with NE-10, do not scaled the ifft output data.
    /*
      float32_t inv_factor = 1.0f / ((float32_t)(2*input_m));  // 2*input_m = fft_size
      q31_t inv_factor = 0x7fffffff;   // 1 in Q31 format.
      inv_factor = (inv_factor) / ((2*input_m));    // factor in output format.
    */
    if(scaled_flag)
    {
        if(rfft_en)
        {
            q_shift_m = 1;
        }
        else
        {
            q_shift_m = 2;
        }

    }
    else
    {
        q_shift_m = 0;
    }

    if(stage_num % 2 == 0)
    {
        out_val = buffer;
        out_next = Fout;
    }
    else
    {
        out_val = Fout;
        out_next = buffer;
    }
    in_val = Fin ;
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
            tmp_cal_1.r = in_val[tmp_q + step * (tmp_p + 0)].r >> q_shift_m;
            tmp_cal_2.r = in_val[tmp_q + step * (tmp_p + input_m)].r >> q_shift_m;
            tmp_cal_1.i = in_val[tmp_q + step * (tmp_p + 0)].i >> q_shift_m;
            tmp_cal_2.i = in_val[tmp_q + step * (tmp_p + input_m)].i >> q_shift_m;
            out_val[tmp_q + step * (2 * tmp_p + 0)].r = (tmp_cal_1.r + tmp_cal_2.r);
            out_val[tmp_q + step * (2 * tmp_p + 0)].i = (tmp_cal_1.i + tmp_cal_2.i);
            tmp_r = tmp_cal_1.r - tmp_cal_2.r;
            tmp_i = tmp_cal_1.i - tmp_cal_2.i;
            out_val[tmp_q + step * (2 * tmp_p + 1)].r = (q31_t)((((NDSV_F2I32_SAMPPROD)tmp_r * tmp_cal_wp.r) - ((NDSV_F2I32_SAMPPROD)tmp_i * tmp_cal_wp.i)) >> (31));
            out_val[tmp_q + step * (2 * tmp_p + 1)].i = (q31_t)((((NDSV_F2I32_SAMPPROD)tmp_r * tmp_cal_wp.i) + ((NDSV_F2I32_SAMPPROD)tmp_i * tmp_cal_wp.r)) >> (31));
        }
    }

    in_val = out_val;
    out_val = out_next;
    out_next = in_val;
    step = step * 2;
    input_m = input_m / 2 ;
    if(scaled_flag)
    {
        q_shift_m = 1 ;
    }
    else
    {
        q_shift_m = 0 ;
    }

    for(stage_idx = 1 ; stage_idx < stage_num - 1; stage_idx++)
    {
        for(tmp_p = 0; tmp_p < input_m ; tmp_p++)
        {
            tmp_cal_wp.r = twiddles[step * tmp_p].r;
            tmp_cal_wp.i = -twiddles[step * tmp_p].i;
            for( tmp_q = 0 ; tmp_q < step; tmp_q++)
            {
                tmp_cal_1.r = (in_val[tmp_q + step * (tmp_p + 0)].r) >> q_shift_m;
                tmp_cal_2.r = (in_val[tmp_q + step * (tmp_p + input_m)].r) >> q_shift_m;
                tmp_cal_1.i = (in_val[tmp_q + step * (tmp_p + 0)].i) >> q_shift_m;
                tmp_cal_2.i = (in_val[tmp_q + step * (tmp_p + input_m)].i) >> q_shift_m;
                out_val[tmp_q + step * (2 * tmp_p + 0)].r =  (tmp_cal_1.r + tmp_cal_2.r);
                out_val[tmp_q + step * (2 * tmp_p + 0)].i =  (tmp_cal_1.i + tmp_cal_2.i);
                tmp_r = tmp_cal_1.r - tmp_cal_2.r;
                tmp_i = tmp_cal_1.i - tmp_cal_2.i;
                out_val[tmp_q + step * (2 * tmp_p + 1)].r = (q31_t)( ( ((NDSV_F2I32_SAMPPROD)tmp_r * tmp_cal_wp.r) - ((NDSV_F2I32_SAMPPROD)tmp_i * tmp_cal_wp.i) ) >> (31) );
                out_val[tmp_q + step * (2 * tmp_p + 1)].i = (q31_t)( ( ((NDSV_F2I32_SAMPPROD)tmp_r * tmp_cal_wp.i) + ((NDSV_F2I32_SAMPPROD)tmp_i * tmp_cal_wp.r) ) >> (31) );
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

        out_val[tmp_q + step * 0].r = (tmp_cal_1.r + tmp_cal_2.r) ;
        out_val[tmp_q + step * 0].i = (tmp_cal_1.i + tmp_cal_2.i) ;

        out_val[tmp_q + step * 1].r = (tmp_cal_1.r - tmp_cal_2.r) ;
        out_val[tmp_q + step * 1].i = (tmp_cal_1.i - tmp_cal_2.i) ;
    }
}

void riscv_vec_rifft_split_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT dst,
                                const riscv_vec_cpx_q31_t * FUNC_RESTRICT src,
                                riscv_vec_cpx_q31_t * FUNC_RESTRICT twiddles,
                                q31_t ncfft,
                                q31_t scaled_flag)
{
    // in this part, output shift will be 1 (div by 2)
    riscv_vec_cpx_q31_t *out_val = dst;
    const riscv_vec_cpx_q31_t *in_val  = src;
    q31_t size = ncfft;
    riscv_vec_cpx_q31_t tmp_cal_1, tmp_cal_2, tmp_cal_wp;
    q31_t *wp_tbl_rifft_r = NULL, *wp_tbl_rifft_i = NULL;
    NDSV_F2I32_SAMPPROD tmp_1 = 0, tmp_2 = 0, tmp_3 = 0, tmp_4 = 0;
    q31_t tmp_q = 0, m = 0 ;
    if(scaled_flag)
    {
        m = 2;
    }
    wp_tbl_rifft_r = &twiddles[0].r;
    wp_tbl_rifft_i = &twiddles[ncfft / 2].r;

    out_val[0].r = (((in_val[0].r >> m) + (in_val[ncfft].r >> m)))  ;
    out_val[0].i = (((in_val[0].r >> m) - (in_val[ncfft].r >> m)))  ;

    for( tmp_q = 1 ; tmp_q <= size / 2; tmp_q++)
    {
        tmp_cal_wp.r = wp_tbl_rifft_r[tmp_q];    // tw_r;
        tmp_cal_wp.i = -wp_tbl_rifft_i[tmp_q];    // tw_i;
        tmp_cal_1.r = (in_val[tmp_q].r >> m) + (in_val[size - tmp_q].r >> m); // xe_r
        tmp_cal_1.i = (in_val[tmp_q].i >> m) - (in_val[size - tmp_q].i >> m); // xe_i
        tmp_cal_2.r = (in_val[tmp_q].r >> m) - (in_val[size - tmp_q].r >> m); // xo_r
        tmp_cal_2.i = ((in_val[tmp_q].i >> m) + (in_val[size - tmp_q].i >> m)); // xo_i

        tmp_1 = ((NDSV_F2I32_SAMPPROD)tmp_cal_2.r * (NDSV_F2I32_SAMPPROD)tmp_cal_wp.i) >> (31);
        tmp_2 = ((NDSV_F2I32_SAMPPROD)tmp_cal_2.i * (NDSV_F2I32_SAMPPROD)tmp_cal_wp.r) >> (31);
        tmp_3 = ((NDSV_F2I32_SAMPPROD)tmp_cal_2.r * (NDSV_F2I32_SAMPPROD)tmp_cal_wp.r) >> (31);
        tmp_4 = ((NDSV_F2I32_SAMPPROD)tmp_cal_2.i * (NDSV_F2I32_SAMPPROD)tmp_cal_wp.i) >> (31);
        out_val[tmp_q].r = (tmp_cal_1.r - ( (q31_t)(tmp_1) + (q31_t)(tmp_2)))   ;
        out_val[tmp_q].i = (tmp_cal_1.i + ( (q31_t)(tmp_3) - (q31_t)(tmp_4)))   ;
        out_val[size - tmp_q].r = (tmp_cal_1.r + ((q31_t)(tmp_1) + (q31_t) (tmp_2)))  ;
        out_val[size - tmp_q].i = (((q31_t)(tmp_3) - (q31_t)(tmp_4)) - tmp_cal_1.i)     ;
    }

}
#endif

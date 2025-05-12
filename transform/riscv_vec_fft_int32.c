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
static void riscv_vec_bfly_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT Fout,
                                riscv_vec_cpx_q31_t * FUNC_RESTRICT Fin,
                                q31_t * FUNC_RESTRICT factors,
                                riscv_vec_cpx_q31_t * FUNC_RESTRICT twiddles,
                                riscv_vec_cpx_q31_t * FUNC_RESTRICT buffer,
                                q31_t scaled_flag)
{
    // pure C version
    uint32_t input_m = factors[SIZE_FIRST_STAGE];
    uint32_t step = 1;
    uint32_t stage_num = factors[OUT_INDEX];
    uint32_t stage_idx = 0;
    q31_t tmp_r, tmp_i;
    uint32_t tmp_q = 0, tmp_p = 0 ;
    riscv_vec_cpx_q31_t *out_val = NULL ;
    riscv_vec_cpx_q31_t *in_val = NULL  ;
    riscv_vec_cpx_q31_t *out_next = NULL  ;
    riscv_vec_cpx_q31_t tmp_cal_1, tmp_cal_2, tmp_cal_wp;
    q31_t q_shift_m  = 0;
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

    if(scaled_flag)
    {
        q_shift_m = 2;
    }
    else
    {
        q_shift_m = 0;
    }
    for(tmp_p = 0; tmp_p < input_m ; tmp_p++)
    {
        tmp_cal_wp.r = twiddles[step * tmp_p].r;
        tmp_cal_wp.i = twiddles[step * tmp_p].i;
        for( tmp_q = 0 ; tmp_q < step; tmp_q++)
        {
            tmp_cal_1.r = (in_val[tmp_q + step * (tmp_p + 0)].r) >> q_shift_m;
            tmp_cal_2.r = (in_val[tmp_q + step * (tmp_p + input_m)].r) >> q_shift_m;
            tmp_cal_1.i = (in_val[tmp_q + step * (tmp_p + 0)].i) >> q_shift_m;
            tmp_cal_2.i = (in_val[tmp_q + step * (tmp_p + input_m)].i) >> q_shift_m;

            out_val[tmp_q + step * (2 * tmp_p + 0)].r =  tmp_cal_1.r + tmp_cal_2.r;
            out_val[tmp_q + step * (2 * tmp_p + 0)].i =  tmp_cal_1.i + tmp_cal_2.i;
            tmp_r = tmp_cal_1.r - tmp_cal_2.r;
            tmp_i = tmp_cal_1.i - tmp_cal_2.i;

            out_val[tmp_q + step * (2 * tmp_p + 1)].r = (q31_t)( ( (NDSV_F2I32_SAMPPROD)tmp_r * tmp_cal_wp.r - (NDSV_F2I32_SAMPPROD)tmp_i * tmp_cal_wp.i) >> (31) );
            out_val[tmp_q + step * (2 * tmp_p + 1)].i = (q31_t)( ( (NDSV_F2I32_SAMPPROD)tmp_r * tmp_cal_wp.i + (NDSV_F2I32_SAMPPROD)tmp_i * tmp_cal_wp.r) >> (31) );

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
            tmp_cal_wp.i = twiddles[step * tmp_p].i;
            for( tmp_q = 0 ; tmp_q < step; tmp_q++)
            {
                tmp_cal_1.r = (in_val[tmp_q + step * (tmp_p + 0)].r) >> q_shift_m;
                tmp_cal_2.r = (in_val[tmp_q + step * (tmp_p + input_m)].r) >> q_shift_m;
                tmp_cal_1.i = (in_val[tmp_q + step * (tmp_p + 0)].i) >> q_shift_m;
                tmp_cal_2.i = (in_val[tmp_q + step * (tmp_p + input_m)].i) >> q_shift_m;

                out_val[tmp_q + step * (2 * tmp_p + 0)].r =  tmp_cal_1.r + tmp_cal_2.r;
                out_val[tmp_q + step * (2 * tmp_p + 0)].i =  tmp_cal_1.i + tmp_cal_2.i;
                tmp_r = tmp_cal_1.r - tmp_cal_2.r;
                tmp_i = tmp_cal_1.i - tmp_cal_2.i;
                out_val[tmp_q + step * (2 * tmp_p + 1)].r = ((q31_t)( ( (NDSV_F2I32_SAMPPROD)tmp_r * tmp_cal_wp.r - (NDSV_F2I32_SAMPPROD)tmp_i * tmp_cal_wp.i) >> (31) ));
                out_val[tmp_q + step * (2 * tmp_p + 1)].i = ((q31_t)( ( (NDSV_F2I32_SAMPPROD)tmp_r * tmp_cal_wp.i + (NDSV_F2I32_SAMPPROD)tmp_i * tmp_cal_wp.r) >> (31) ));
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

static void riscv_vec_ibfly_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT Fout,
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

static void riscv_vec_rfft_split_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT dst,
                                      const riscv_vec_cpx_q31_t * FUNC_RESTRICT src,
                                      riscv_vec_cpx_q31_t * FUNC_RESTRICT twiddles,
                                      q31_t ncfft,
                                      q31_t scaled_flag)
{
    // pure C version last prcoess of RFFT
    riscv_vec_cpx_q31_t *out_val = dst;
    const riscv_vec_cpx_q31_t *in_val  = src;
    q31_t size = ncfft;
    riscv_vec_cpx_q31_t tmp_cal_1, tmp_cal_2, tmp_cal_wp;
    NDSV_F2I32_SAMPPROD tmp_1 = 0, tmp_2 = 0, tmp_3 = 0, tmp_4 = 0;
    q31_t *wp_tbl_rfft_r = NULL, *wp_tbl_rfft_i = NULL;
    q31_t tmp_q = 0;
    uint32_t m = 0;
    if(scaled_flag)
    {
        m = 1;
    }
    wp_tbl_rfft_r = &twiddles[0].r;
    wp_tbl_rfft_i = &twiddles[ncfft / 2].r;

    out_val[0].r = (in_val[0].r >> m) + (in_val[0].i >> m);
    out_val[size].r  = (in_val[0].r >> m) - (in_val[0].i >> m);
    out_val[0].i = 0 ;
    out_val[size].i = 0 ;
    for( tmp_q = 1 ; tmp_q <= size / 2; tmp_q++)
    {
        tmp_cal_wp.r = wp_tbl_rfft_r[tmp_q];    // tw_r;
        tmp_cal_wp.i = wp_tbl_rfft_i[tmp_q];    // tw_i;
        tmp_cal_1.r = (in_val[tmp_q].r >> m) + (in_val[size - tmp_q].r >> m); // xe_r
        tmp_cal_1.i = (in_val[tmp_q].i >> m) - (in_val[size - tmp_q].i >> m); // xe_i
        tmp_cal_2.r = (in_val[size - tmp_q].r >> m) - (in_val[tmp_q].r >> m); // xo_r
        tmp_cal_2.i = -((in_val[tmp_q].i >> m) + (in_val[size - tmp_q].i >> m)); // xo_i

        tmp_1 = ((NDSV_F2I32_SAMPPROD)tmp_cal_2.r * (NDSV_F2I32_SAMPPROD)tmp_cal_wp.i) >> (31);
        tmp_2 = ((NDSV_F2I32_SAMPPROD)tmp_cal_2.i * (NDSV_F2I32_SAMPPROD)tmp_cal_wp.r) >> (31);
        tmp_3 = ((NDSV_F2I32_SAMPPROD)tmp_cal_2.r * (NDSV_F2I32_SAMPPROD)tmp_cal_wp.r) >> (31);
        tmp_4 = ((NDSV_F2I32_SAMPPROD)tmp_cal_2.i * (NDSV_F2I32_SAMPPROD)tmp_cal_wp.i) >> (31);
        out_val[tmp_q].r = (tmp_cal_1.r - ( (q31_t)(tmp_1) + (q31_t)(tmp_2))) >> 1;
        out_val[tmp_q].i = (tmp_cal_1.i + ( (q31_t)(tmp_3) - (q31_t)(tmp_4))) >> 1;
        out_val[size - tmp_q].r = (tmp_cal_1.r + ((q31_t)(tmp_1) + (q31_t) (tmp_2))) >> 1;
        out_val[size - tmp_q].i = (((q31_t)(tmp_3) - (q31_t)(tmp_4)) - tmp_cal_1.i) >> 1;
    }
}

static void riscv_vec_rifft_split_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT dst,
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

void  riscv_vec_cfft_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT out,
                          riscv_vec_cpx_q31_t * FUNC_RESTRICT in,
                          riscv_vec_cfft_cfg_q31_t cfg,
                          q31_t inverse_fft,
                          q31_t scaled_flag)
{
    if( cfg != NULL)
    {
        if (inverse_fft)
        {
            riscv_vec_ibfly_q31 (out, in, cfg->factors, cfg->twiddles, cfg->buffer, scaled_flag);
        }
        else
        {
            riscv_vec_bfly_q31 (out, in, cfg->factors, cfg->twiddles, cfg->buffer, scaled_flag);
        }

    }
}
void  riscv_vec_rfft_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT out,
                          q31_t * FUNC_RESTRICT in,
                          riscv_vec_rfft_cfg_q31_t cfg,
                          q31_t scaled_flag)
{
    if( cfg != NULL)
    {
        riscv_vec_cpx_q31_t * tmpbuf = cfg->buffer;

        riscv_vec_bfly_q31 (tmpbuf, (riscv_vec_cpx_q31_t*) in, cfg->factors, cfg->twiddles, out, scaled_flag);
        riscv_vec_rfft_split_q31 (out, tmpbuf, cfg->super_twiddles, cfg->ncfft, scaled_flag);
    }
}

void  riscv_vec_rifft_q31 (q31_t * FUNC_RESTRICT out,
                           riscv_vec_cpx_q31_t * FUNC_RESTRICT in,
                           riscv_vec_rfft_cfg_q31_t cfg,
                           q31_t scaled_flag)
{
    if( cfg != NULL)
    {
        riscv_vec_cpx_q31_t * tmpbuf1 = cfg->buffer;
        riscv_vec_cpx_q31_t * tmpbuf2 = cfg->buffer + cfg->ncfft;

        riscv_vec_rifft_split_q31 (tmpbuf1, in, cfg->super_twiddles, cfg->ncfft, scaled_flag);
        riscv_vec_ibfly_q31 ( (riscv_vec_cpx_q31_t*) out, tmpbuf1, cfg->factors, cfg->twiddles, tmpbuf2, scaled_flag);

    }
}

#else  //ENA_STOCKHAM_FFT

static void riscv_vec_bfly_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT Fout,
                                riscv_vec_cpx_q31_t * FUNC_RESTRICT Fin,
                                q31_t * FUNC_RESTRICT factors,
                                riscv_vec_cpx_q31_t * FUNC_RESTRICT twiddles,
                                riscv_vec_cpx_q31_t * FUNC_RESTRICT buffer,
                                q31_t scaled_flag)
{
    q31_t fstride, mstride, N;
    q31_t f_count, m_count;
    q31_t stage_count;

    riscv_vec_cpx_q31_t   scratch_in_radix8[8];
    riscv_vec_cpx_q31_t   scratch_in[8];
    riscv_vec_cpx_q31_t   scratch_out[8];
    riscv_vec_cpx_q31_t   scratch[16];
    riscv_vec_cpx_q31_t   scratch_tw[6];

    riscv_vec_cpx_q31_t   *Fin1, *Fin2, *Fout1, *Fout2;
    riscv_vec_cpx_q31_t   *Fout_ls = Fout;
    riscv_vec_cpx_q31_t   *Ftmp;
    riscv_vec_cpx_q31_t   *tw, *tw1, *tw2;
    const q31_t TW_81 = 1518500249;
    const q31_t TW_81N = -1518500249;

    // init fstride, mstride, N
    stage_count = factors[0];
    fstride = factors[1];
    mstride = factors[ (stage_count << 1) - 1 ];
    N = factors[ stage_count << 1 ]; // radix
    tw = twiddles;

    // the first stage
    Fin1 = Fin;
    Fout1 = Fout;
    if (N == 8)   // length of FFT is 2^n (n is odd)
    {
        DBG_MSG("Radix-8\n");

        // radix 8
        N = fstride << 1; // 1/4 of length of FFT
        tw = twiddles;

        Fin1 = Fin;
        for (f_count = 0; f_count < fstride; f_count ++)
        {
            Fout1 = & Fout[ f_count * 8 ];
            // load
            scratch_in_radix8[0] = Fin1[0];
            scratch_in_radix8[1] = Fin1[fstride];
            scratch_in_radix8[2] = Fin1[fstride * 2];
            scratch_in_radix8[3] = Fin1[fstride * 3];
            scratch_in_radix8[4] = Fin1[fstride * 4];
            scratch_in_radix8[5] = Fin1[fstride * 5];
            scratch_in_radix8[6] = Fin1[fstride * 6];
            scratch_in_radix8[7] = Fin1[fstride * 7];
            if (scaled_flag == 1)
            {
                NDSV_F2I32_FIXDIV (scratch_in_radix8[0], 8);
                NDSV_F2I32_FIXDIV (scratch_in_radix8[1], 8);
                NDSV_F2I32_FIXDIV (scratch_in_radix8[2], 8);
                NDSV_F2I32_FIXDIV (scratch_in_radix8[3], 8);
                NDSV_F2I32_FIXDIV (scratch_in_radix8[4], 8);
                NDSV_F2I32_FIXDIV (scratch_in_radix8[5], 8);
                NDSV_F2I32_FIXDIV (scratch_in_radix8[6], 8);
                NDSV_F2I32_FIXDIV (scratch_in_radix8[7], 8);
            }
            scratch_in[0].r = scratch_in_radix8[0].r + scratch_in_radix8[4].r;
            scratch_in[0].i = scratch_in_radix8[0].i + scratch_in_radix8[4].i;
            scratch_in[1].r = scratch_in_radix8[0].r - scratch_in_radix8[4].r;
            scratch_in[1].i = scratch_in_radix8[0].i - scratch_in_radix8[4].i;
            scratch_in[2].r = scratch_in_radix8[1].r + scratch_in_radix8[5].r;
            scratch_in[2].i = scratch_in_radix8[1].i + scratch_in_radix8[5].i;
            scratch_in[3].r = scratch_in_radix8[1].r - scratch_in_radix8[5].r;
            scratch_in[3].i = scratch_in_radix8[1].i - scratch_in_radix8[5].i;
            scratch_in[4].r = scratch_in_radix8[2].r + scratch_in_radix8[6].r;
            scratch_in[4].i = scratch_in_radix8[2].i + scratch_in_radix8[6].i;
            scratch_in[5].r = scratch_in_radix8[2].r - scratch_in_radix8[6].r;
            scratch_in[5].i = scratch_in_radix8[2].i - scratch_in_radix8[6].i;
            scratch_in[6].r = scratch_in_radix8[3].r + scratch_in_radix8[7].r;
            scratch_in[6].i = scratch_in_radix8[3].i + scratch_in_radix8[7].i;
            scratch_in[7].r = scratch_in_radix8[3].r - scratch_in_radix8[7].r;
            scratch_in[7].i = scratch_in_radix8[3].i - scratch_in_radix8[7].i;

            // radix 4 butterfly without twiddles
            scratch[0] = scratch_in[0];
            scratch[1] = scratch_in[1];

            scratch[2] = scratch_in[2];
            scratch[3].r = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) (scratch_in[3].r + scratch_in[3].i) * TW_81) >> 31);
            scratch[3].i = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) (scratch_in[3].i - scratch_in[3].r) * TW_81) >> 31);

            scratch[4] = scratch_in[4];
            scratch[5].r = scratch_in[5].i;
            scratch[5].i = -scratch_in[5].r;

            scratch[6].r = scratch_in[6].r;
            scratch[6].i = scratch_in[6].i;
            scratch[7].r = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) (scratch_in[7].r - scratch_in[7].i) * TW_81N) >> 31);
            scratch[7].i = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) (scratch_in[7].i + scratch_in[7].r) * TW_81N) >> 31);

            // radix 2 butterfly
            scratch[8].r = scratch[0].r + scratch[4].r;
            scratch[8].i = scratch[0].i + scratch[4].i;
            scratch[9].r = scratch[1].r + scratch[5].r;
            scratch[9].i = scratch[1].i + scratch[5].i;

            scratch[10].r = scratch[0].r - scratch[4].r;
            scratch[10].i = scratch[0].i - scratch[4].i;
            scratch[11].r = scratch[1].r - scratch[5].r;
            scratch[11].i = scratch[1].i - scratch[5].i;

            // radix 2 butterfly
            scratch[12].r = scratch[2].r + scratch[6].r;
            scratch[12].i = scratch[2].i + scratch[6].i;
            scratch[13].r = scratch[3].r + scratch[7].r;
            scratch[13].i = scratch[3].i + scratch[7].i;

            scratch[14].r = scratch[2].r - scratch[6].r;
            scratch[14].i = scratch[2].i - scratch[6].i;
            scratch[15].r = scratch[3].r - scratch[7].r;
            scratch[15].i = scratch[3].i - scratch[7].i;

            // third result
            scratch_out[4].r = scratch[8].r - scratch[12].r;
            scratch_out[4].i = scratch[8].i - scratch[12].i;
            scratch_out[5].r = scratch[9].r - scratch[13].r;
            scratch_out[5].i = scratch[9].i - scratch[13].i;

            // first result
            scratch_out[0].r = scratch[8].r + scratch[12].r;
            scratch_out[0].i = scratch[8].i + scratch[12].i;
            scratch_out[1].r = scratch[9].r + scratch[13].r;
            scratch_out[1].i = scratch[9].i + scratch[13].i;

            // second result
            scratch_out[2].r = scratch[10].r + scratch[14].i;
            scratch_out[2].i = scratch[10].i - scratch[14].r;
            scratch_out[3].r = scratch[11].r + scratch[15].i;
            scratch_out[3].i = scratch[11].i - scratch[15].r;

            // forth result
            scratch_out[6].r = scratch[10].r - scratch[14].i;
            scratch_out[6].i = scratch[10].i + scratch[14].r;
            scratch_out[7].r = scratch[11].r - scratch[15].i;
            scratch_out[7].i = scratch[11].i + scratch[15].r;

            Fout1[0] = scratch_out[0];
            Fout1[1] = scratch_out[1];
            Fout1[2] = scratch_out[2];
            Fout1[3] = scratch_out[3];
            Fout1[4] = scratch_out[4];
            Fout1[5] = scratch_out[5];
            Fout1[6] = scratch_out[6];
            Fout1[7] = scratch_out[7];

            Fin1 += 1;
        } // f_count
        fstride >>= 2;
        stage_count--;

        // swap
        Ftmp = buffer;
        buffer = Fout;
        Fout = Ftmp;
    }
    else if (N == 4)   // length of FFT is 2^n (n is even) >= 4
    {
        DBG_MSG("Radix-4\n");

        //fstride is nfft>>2
        for (f_count = fstride; f_count ; f_count --)
        {
            // load
            scratch_in[0] = *Fin1;
            Fin2 = Fin1 + fstride;
            scratch_in[1] = *Fin2;
            Fin2 = Fin2 + fstride;
            scratch_in[2] = *Fin2;
            Fin2 = Fin2 + fstride;
            scratch_in[3] = *Fin2;

            // radix 4 butterfly without twiddles
            if (scaled_flag == 1)
            {
                NDSV_F2I32_FIXDIV (scratch_in[0], 4);
                NDSV_F2I32_FIXDIV (scratch_in[1], 4);
                NDSV_F2I32_FIXDIV (scratch_in[2], 4);
                NDSV_F2I32_FIXDIV (scratch_in[3], 4);
            }

            // radix 2 butterfly
            scratch[0].r = scratch_in[0].r + scratch_in[2].r;
            scratch[0].i = scratch_in[0].i + scratch_in[2].i;

            scratch[1].r = scratch_in[0].r - scratch_in[2].r;
            scratch[1].i = scratch_in[0].i - scratch_in[2].i;

            // radix 2 butterfly
            scratch[2].r = scratch_in[1].r + scratch_in[3].r;
            scratch[2].i = scratch_in[1].i + scratch_in[3].i;

            scratch[3].r = scratch_in[1].r - scratch_in[3].r;
            scratch[3].i = scratch_in[1].i - scratch_in[3].i;

            // third result
            scratch_out[2].r = scratch[0].r - scratch[2].r;
            scratch_out[2].i = scratch[0].i - scratch[2].i;

            // first result
            scratch_out[0].r = scratch[0].r + scratch[2].r;
            scratch_out[0].i = scratch[0].i + scratch[2].i;

            // second result
            scratch_out[1].r = scratch[1].r + scratch[3].i;
            scratch_out[1].i = scratch[1].i - scratch[3].r;

            // forth result
            scratch_out[3].r = scratch[1].r - scratch[3].i;
            scratch_out[3].i = scratch[1].i + scratch[3].r;

            // store
            * Fout1 ++ = scratch_out[0];
            * Fout1 ++ = scratch_out[1];
            * Fout1 ++ = scratch_out[2];
            * Fout1 ++ = scratch_out[3];

            Fin1++;
        } // f_count

        N = fstride; // 1/4 of length of FFT

        // swap
        Ftmp = buffer;
        buffer = Fout;
        Fout = Ftmp;

        // update address for other stages
        stage_count--;
        fstride >>= 2;
        // end of first stage
    }
    else if (N == 2) // Length of FFT is 2
    {
        DBG_MSG("Radix-2\n");

        scratch_in[0] = Fin1[0];
        scratch_in[1] = Fin1[1];

        if (scaled_flag == 1)
        {
            NDSV_F2I32_FIXDIV (scratch_in[0], 2);
            NDSV_F2I32_FIXDIV (scratch_in[1], 2);
        }

        Fout1[0].r = scratch_in[0].r + scratch_in[1].r;
        Fout1[0].i = scratch_in[0].i + scratch_in[1].i;
        Fout1[1].r = scratch_in[0].r - scratch_in[1].r;
        Fout1[1].i = scratch_in[0].i - scratch_in[1].i;
        return;
    }
    else // Length of FFT is 1
    {
        Fout1[0] = Fin1[0];
        return;
    }

    // others but the last one
    for (; stage_count > 1 ; stage_count--)
    {
        Fin1 = buffer;
        for (f_count = 0; f_count < fstride; f_count ++)
        {
            Fout1 = & Fout[ f_count * mstride << 2 ];
            tw1 = tw;
            for (m_count = mstride; m_count ; m_count --)
            {
                // load
                scratch_tw[0] = *tw1;
                tw2 = tw1 + mstride;
                scratch_tw[1] = *tw2;
                tw2 += mstride;
                scratch_tw[2] = *tw2;
                scratch_in[0] = * Fin1;
                Fin2 = Fin1 + N;
                scratch_in[1] = * Fin2;
                Fin2 += N;
                scratch_in[2] = * Fin2;
                Fin2 += N;
                scratch_in[3] = * Fin2;
                if (scaled_flag == 1)
                {
                    NDSV_F2I32_FIXDIV (scratch_in[0], 4);
                    NDSV_F2I32_FIXDIV (scratch_in[1], 4);
                    NDSV_F2I32_FIXDIV (scratch_in[2], 4);
                    NDSV_F2I32_FIXDIV (scratch_in[3], 4);
                }

                // radix 4 butterfly with twiddles

                scratch[0] = scratch_in[0];
                scratch[1].r = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[1].r * scratch_tw[0].r
                                           - (NDSV_F2I32_SAMPPROD) scratch_in[1].i * scratch_tw[0].i) >> 31);
                scratch[1].i = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[1].i * scratch_tw[0].r
                                           + (NDSV_F2I32_SAMPPROD) scratch_in[1].r * scratch_tw[0].i) >> 31);

                scratch[2].r = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[2].r * scratch_tw[1].r
                                           - (NDSV_F2I32_SAMPPROD) scratch_in[2].i * scratch_tw[1].i) >> 31);
                scratch[2].i = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[2].i * scratch_tw[1].r
                                           + (NDSV_F2I32_SAMPPROD) scratch_in[2].r * scratch_tw[1].i) >> 31);

                scratch[3].r = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[3].r * scratch_tw[2].r
                                           - (NDSV_F2I32_SAMPPROD) scratch_in[3].i * scratch_tw[2].i) >> 31);
                scratch[3].i = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[3].i * scratch_tw[2].r
                                           + (NDSV_F2I32_SAMPPROD) scratch_in[3].r * scratch_tw[2].i) >> 31);

                // radix 2 butterfly
                scratch[4].r = scratch[0].r + scratch[2].r;
                scratch[4].i = scratch[0].i + scratch[2].i;

                scratch[5].r = scratch[0].r - scratch[2].r;
                scratch[5].i = scratch[0].i - scratch[2].i;

                // radix 2 butterfly
                scratch[6].r = scratch[1].r + scratch[3].r;
                scratch[6].i = scratch[1].i + scratch[3].i;

                scratch[7].r = scratch[1].r - scratch[3].r;
                scratch[7].i = scratch[1].i - scratch[3].i;

                // third result
                scratch_out[2].r = scratch[4].r - scratch[6].r;
                scratch_out[2].i = scratch[4].i - scratch[6].i;

                // first result
                scratch_out[0].r = scratch[4].r + scratch[6].r;
                scratch_out[0].i = scratch[4].i + scratch[6].i;

                // second result
                scratch_out[1].r = scratch[5].r + scratch[7].i;
                scratch_out[1].i = scratch[5].i - scratch[7].r;

                // forth result
                scratch_out[3].r = scratch[5].r - scratch[7].i;
                scratch_out[3].i = scratch[5].i + scratch[7].r;

                // store
                *Fout1 = scratch_out[0];
                Fout2 = Fout1 + mstride;
                *Fout2 = scratch_out[1];
                Fout2 += mstride;
                *Fout2 = scratch_out[2];
                Fout2 += mstride;
                *Fout2 = scratch_out[3];

                tw1++;
                Fin1 ++;
                Fout1 ++;
            } // m_count
        } // f_count
        tw += mstride * 3;
        mstride <<= 2;
        // swap
        Ftmp = buffer;
        buffer = Fout;
        Fout = Ftmp;
        fstride >>= 2;
    } // stage_count

    // the last one
    if (stage_count)
    {
        Fin1 = buffer;
        // if stage count is even, output to the input array
        Fout1 = Fout_ls;

        for (f_count = 0; f_count < fstride; f_count ++)
        {
            tw1 = tw;
            for (m_count = mstride; m_count ; m_count --)
            {
                // load
                scratch_tw[0] = *tw1;
                tw2 = tw1 + mstride;
                scratch_tw[1] = *tw2;
                tw2 += mstride;
                scratch_tw[2] = *tw2;
                scratch_in[0] = * Fin1;
                Fin2 = Fin1 + N;
                scratch_in[1] = * Fin2;
                Fin2 += N;
                scratch_in[2] = * Fin2;
                Fin2 += N;
                scratch_in[3] = * Fin2;
                if (scaled_flag == 1)
                {
                    NDSV_F2I32_FIXDIV (scratch_in[0], 4);
                    NDSV_F2I32_FIXDIV (scratch_in[1], 4);
                    NDSV_F2I32_FIXDIV (scratch_in[2], 4);
                    NDSV_F2I32_FIXDIV (scratch_in[3], 4);
                }

                // radix 4 butterfly with twiddles

                scratch[0] = scratch_in[0];
                scratch[1].r = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[1].r * scratch_tw[0].r
                                           - (NDSV_F2I32_SAMPPROD) scratch_in[1].i * scratch_tw[0].i) >> 31);
                scratch[1].i = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[1].i * scratch_tw[0].r
                                           + (NDSV_F2I32_SAMPPROD) scratch_in[1].r * scratch_tw[0].i) >> 31);

                scratch[2].r = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[2].r * scratch_tw[1].r
                                           - (NDSV_F2I32_SAMPPROD) scratch_in[2].i * scratch_tw[1].i) >> 31);
                scratch[2].i = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[2].i * scratch_tw[1].r
                                           + (NDSV_F2I32_SAMPPROD) scratch_in[2].r * scratch_tw[1].i) >> 31);

                scratch[3].r = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[3].r * scratch_tw[2].r
                                           - (NDSV_F2I32_SAMPPROD) scratch_in[3].i * scratch_tw[2].i) >> 31);
                scratch[3].i = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[3].i * scratch_tw[2].r
                                           + (NDSV_F2I32_SAMPPROD) scratch_in[3].r * scratch_tw[2].i) >> 31);

                // radix 2 butterfly
                scratch[4].r = scratch[0].r + scratch[2].r;
                scratch[4].i = scratch[0].i + scratch[2].i;

                scratch[5].r = scratch[0].r - scratch[2].r;
                scratch[5].i = scratch[0].i - scratch[2].i;

                // radix 2 butterfly
                scratch[6].r = scratch[1].r + scratch[3].r;
                scratch[6].i = scratch[1].i + scratch[3].i;

                scratch[7].r = scratch[1].r - scratch[3].r;
                scratch[7].i = scratch[1].i - scratch[3].i;

                // third result
                scratch_out[2].r = scratch[4].r - scratch[6].r;
                scratch_out[2].i = scratch[4].i - scratch[6].i;

                // first result
                scratch_out[0].r = scratch[4].r + scratch[6].r;
                scratch_out[0].i = scratch[4].i + scratch[6].i;

                // second result
                scratch_out[1].r = scratch[5].r + scratch[7].i;
                scratch_out[1].i = scratch[5].i - scratch[7].r;

                // forth result
                scratch_out[3].r = scratch[5].r - scratch[7].i;
                scratch_out[3].i = scratch[5].i + scratch[7].r;

                // store
                *Fout1 = scratch_out[0];
                Fout2 = Fout1 + N;
                *Fout2 = scratch_out[1];
                Fout2 += N;
                *Fout2 = scratch_out[2];
                Fout2 += N;
                *Fout2 = scratch_out[3];

                tw1 ++;
                Fin1 ++;
                Fout1 ++;
            } // m_count
        } // f_count
    } // last stage
}

static void riscv_vec_ibfly_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT Fout,
                                 riscv_vec_cpx_q31_t * FUNC_RESTRICT Fin,
                                 q31_t * FUNC_RESTRICT factors,
                                 riscv_vec_cpx_q31_t * FUNC_RESTRICT twiddles,
                                 riscv_vec_cpx_q31_t * FUNC_RESTRICT buffer,
                                 q31_t scaled_flag)
{
    q31_t fstride, mstride, N;
    q31_t f_count, m_count;
    q31_t stage_count;

    riscv_vec_cpx_q31_t   scratch_in_radix8[8];
    riscv_vec_cpx_q31_t   scratch_in[8];
    riscv_vec_cpx_q31_t   scratch_out[8];
    riscv_vec_cpx_q31_t   scratch[16];
    riscv_vec_cpx_q31_t   scratch_tw[6];

    riscv_vec_cpx_q31_t   *Fin1, *Fin2, *Fout1, *Fout2;
    riscv_vec_cpx_q31_t   *Fout_ls = Fout;
    riscv_vec_cpx_q31_t   *Ftmp;
    riscv_vec_cpx_q31_t   *tw, *tw1, *tw2;
    const q31_t TW_81 = 1518500249;
    const q31_t TW_81N = -1518500249;

    // init fstride, mstride, N
    stage_count = factors[0];
    fstride = factors[1];
    mstride = factors[ (stage_count << 1) - 1 ];
    N = factors[ stage_count << 1 ]; // radix
    tw = twiddles;

    // the first stage
    Fin1 = Fin;
    Fout1 = Fout;
    if (N == 8)   // length of FFT is 2^n (n is odd)
    {
        // radix 8
        N = fstride << 1; // 1/4 of length of FFT
        tw = twiddles;

        Fin1 = Fin;
        for (f_count = 0; f_count < fstride; f_count ++)
        {
            Fout1 = & Fout[ f_count * 8 ];
            // load
            scratch_in_radix8[0] = Fin1[0];
            scratch_in_radix8[1] = Fin1[fstride * 1];
            scratch_in_radix8[2] = Fin1[fstride * 2];
            scratch_in_radix8[3] = Fin1[fstride * 3];
            scratch_in_radix8[4] = Fin1[fstride * 4];
            scratch_in_radix8[5] = Fin1[fstride * 5];
            scratch_in_radix8[6] = Fin1[fstride * 6];
            scratch_in_radix8[7] = Fin1[fstride * 7];
            if (scaled_flag == 1)
            {
                NDSV_F2I32_FIXDIV (scratch_in_radix8[0], 8);
                NDSV_F2I32_FIXDIV (scratch_in_radix8[1], 8);
                NDSV_F2I32_FIXDIV (scratch_in_radix8[2], 8);
                NDSV_F2I32_FIXDIV (scratch_in_radix8[3], 8);
                NDSV_F2I32_FIXDIV (scratch_in_radix8[4], 8);
                NDSV_F2I32_FIXDIV (scratch_in_radix8[5], 8);
                NDSV_F2I32_FIXDIV (scratch_in_radix8[6], 8);
                NDSV_F2I32_FIXDIV (scratch_in_radix8[7], 8);
            }
            scratch_in[0].r = scratch_in_radix8[0].r + scratch_in_radix8[4].r;
            scratch_in[0].i = scratch_in_radix8[0].i + scratch_in_radix8[4].i;
            scratch_in[1].r = scratch_in_radix8[0].r - scratch_in_radix8[4].r;
            scratch_in[1].i = scratch_in_radix8[0].i - scratch_in_radix8[4].i;
            scratch_in[2].r = scratch_in_radix8[1].r + scratch_in_radix8[5].r;
            scratch_in[2].i = scratch_in_radix8[1].i + scratch_in_radix8[5].i;
            scratch_in[3].r = scratch_in_radix8[1].r - scratch_in_radix8[5].r;
            scratch_in[3].i = scratch_in_radix8[1].i - scratch_in_radix8[5].i;
            scratch_in[4].r = scratch_in_radix8[2].r + scratch_in_radix8[6].r;
            scratch_in[4].i = scratch_in_radix8[2].i + scratch_in_radix8[6].i;
            scratch_in[5].r = scratch_in_radix8[2].r - scratch_in_radix8[6].r;
            scratch_in[5].i = scratch_in_radix8[2].i - scratch_in_radix8[6].i;
            scratch_in[6].r = scratch_in_radix8[3].r + scratch_in_radix8[7].r;
            scratch_in[6].i = scratch_in_radix8[3].i + scratch_in_radix8[7].i;
            scratch_in[7].r = scratch_in_radix8[3].r - scratch_in_radix8[7].r;
            scratch_in[7].i = scratch_in_radix8[3].i - scratch_in_radix8[7].i;
            // radix 4 butterfly with twiddles

            scratch[0] = scratch_in[0];
            scratch[1] = scratch_in[1];

            scratch[2] = scratch_in[2];
            scratch[3].r = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) (scratch_in[3].r - scratch_in[3].i) * TW_81) >> 31);
            scratch[3].i = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) (scratch_in[3].i + scratch_in[3].r) * TW_81) >> 31);

            scratch[4] = scratch_in[4];
            scratch[5].r = -scratch_in[5].i;
            scratch[5].i = scratch_in[5].r;

            scratch[6].r = scratch_in[6].r;
            scratch[6].i = scratch_in[6].i;
            scratch[7].r = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) (scratch_in[7].r + scratch_in[7].i) * TW_81N) >> 31);
            scratch[7].i = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) (scratch_in[7].i - scratch_in[7].r) * TW_81N) >> 31);

            // radix 2 butterfly
            scratch[8].r = scratch[0].r + scratch[4].r;
            scratch[8].i = scratch[0].i + scratch[4].i;
            scratch[9].r = scratch[1].r + scratch[5].r;
            scratch[9].i = scratch[1].i + scratch[5].i;

            scratch[10].r = scratch[0].r - scratch[4].r;
            scratch[10].i = scratch[0].i - scratch[4].i;
            scratch[11].r = scratch[1].r - scratch[5].r;
            scratch[11].i = scratch[1].i - scratch[5].i;

            // radix 2 butterfly
            scratch[12].r = scratch[2].r + scratch[6].r;
            scratch[12].i = scratch[2].i + scratch[6].i;
            scratch[13].r = scratch[3].r + scratch[7].r;
            scratch[13].i = scratch[3].i + scratch[7].i;

            scratch[14].r = scratch[2].r - scratch[6].r;
            scratch[14].i = scratch[2].i - scratch[6].i;
            scratch[15].r = scratch[3].r - scratch[7].r;
            scratch[15].i = scratch[3].i - scratch[7].i;

            // third result
            scratch_out[4].r = scratch[8].r - scratch[12].r;
            scratch_out[4].i = scratch[8].i - scratch[12].i;
            scratch_out[5].r = scratch[9].r - scratch[13].r;
            scratch_out[5].i = scratch[9].i - scratch[13].i;

            // first result
            scratch_out[0].r = scratch[8].r + scratch[12].r;
            scratch_out[0].i = scratch[8].i + scratch[12].i;
            scratch_out[1].r = scratch[9].r + scratch[13].r;
            scratch_out[1].i = scratch[9].i + scratch[13].i;

            // second result
            scratch_out[2].r = scratch[10].r - scratch[14].i;
            scratch_out[2].i = scratch[10].i + scratch[14].r;
            scratch_out[3].r = scratch[11].r - scratch[15].i;
            scratch_out[3].i = scratch[11].i + scratch[15].r;

            // forth result
            scratch_out[6].r = scratch[10].r + scratch[14].i;
            scratch_out[6].i = scratch[10].i - scratch[14].r;
            scratch_out[7].r = scratch[11].r + scratch[15].i;
            scratch_out[7].i = scratch[11].i - scratch[15].r;

            // store
            Fout1[0] = scratch_out[0];
            Fout1[1] = scratch_out[1];
            Fout1[2] = scratch_out[2];
            Fout1[3] = scratch_out[3];
            Fout1[4] = scratch_out[4];
            Fout1[5] = scratch_out[5];
            Fout1[6] = scratch_out[6];
            Fout1[7] = scratch_out[7];

            Fin1 += 1;
        } // f_count
        fstride >>= 2;
        stage_count--;

        // swap
        Ftmp = buffer;
        buffer = Fout;
        Fout = Ftmp;
    }
    else if (N == 4)   // length of FFT is 2^n (n is even) >= 4
    {
        //fstride is nfft>>2
        for (f_count = fstride; f_count ; f_count --)
        {
            // load
            scratch_in[0] = *Fin1;
            Fin2 = Fin1 + fstride;
            scratch_in[1] = *Fin2;
            Fin2 = Fin2 + fstride;
            scratch_in[2] = *Fin2;
            Fin2 = Fin2 + fstride;
            scratch_in[3] = *Fin2;

            // radix 4 butterfly without twiddles
            if (scaled_flag == 1)
            {
                NDSV_F2I32_FIXDIV (scratch_in[0], 4);
                NDSV_F2I32_FIXDIV (scratch_in[1], 4);
                NDSV_F2I32_FIXDIV (scratch_in[2], 4);
                NDSV_F2I32_FIXDIV (scratch_in[3], 4);
            }

            // radix 2 butterfly
            scratch[0].r = scratch_in[0].r + scratch_in[2].r;
            scratch[0].i = scratch_in[0].i + scratch_in[2].i;

            scratch[1].r = scratch_in[0].r - scratch_in[2].r;
            scratch[1].i = scratch_in[0].i - scratch_in[2].i;

            // radix 2 butterfly
            scratch[2].r = scratch_in[1].r + scratch_in[3].r;
            scratch[2].i = scratch_in[1].i + scratch_in[3].i;

            scratch[3].r = scratch_in[1].r - scratch_in[3].r;
            scratch[3].i = scratch_in[1].i - scratch_in[3].i;

            // third result
            scratch_out[2].r = scratch[0].r - scratch[2].r;
            scratch_out[2].i = scratch[0].i - scratch[2].i;

            // first result
            scratch_out[0].r = scratch[0].r + scratch[2].r;
            scratch_out[0].i = scratch[0].i + scratch[2].i;

            // second result
            scratch_out[1].r = scratch[1].r - scratch[3].i;
            scratch_out[1].i = scratch[1].i + scratch[3].r;

            // forth result
            scratch_out[3].r = scratch[1].r + scratch[3].i;
            scratch_out[3].i = scratch[1].i - scratch[3].r;

            // store
            * Fout1 ++ = scratch_out[0];
            * Fout1 ++ = scratch_out[1];
            * Fout1 ++ = scratch_out[2];
            * Fout1 ++ = scratch_out[3];

            Fin1++;
        } // f_count

        N = fstride; // 1/4 of length of FFT
        // update address for other stages
        stage_count--;
        fstride >>= 2;

        // swap
        Ftmp = buffer;
        buffer = Fout;
        Fout = Ftmp;

        // end of first stage
    }
    else if (N == 2) // Length of FFT is 2
    {
        scratch_in[0] = Fin1[0];
        scratch_in[1] = Fin1[1];

        if (scaled_flag == 1)
        {
            NDSV_F2I32_FIXDIV (scratch_in[0], 2);
            NDSV_F2I32_FIXDIV (scratch_in[1], 2);
        }

        Fout1[0].r = scratch_in[0].r + scratch_in[1].r;
        Fout1[0].i = scratch_in[0].i + scratch_in[1].i;
        Fout1[1].r = scratch_in[0].r - scratch_in[1].r;
        Fout1[1].i = scratch_in[0].i - scratch_in[1].i;
        return;
    }
    else // Length of FFT is 1
    {
        Fout1[0] = Fin1[0];
        return;
    }


    // others but the last one
    for (; stage_count > 1 ; stage_count--)
    {
        Fin1 = buffer;
        for (f_count = 0; f_count < fstride; f_count ++)
        {
            Fout1 = & Fout[ f_count * mstride << 2 ];
            tw1 = tw;
            for (m_count = mstride; m_count ; m_count --)
            {
                // load
                scratch_tw[0] = *tw1;
                tw2 = tw1 + mstride;
                scratch_tw[1] = *tw2;
                tw2 += mstride;
                scratch_tw[2] = *tw2;
                scratch_in[0] = * Fin1;
                Fin2 = Fin1 + N;
                scratch_in[1] = * Fin2;
                Fin2 += N;
                scratch_in[2] = * Fin2;
                Fin2 += N;
                scratch_in[3] = * Fin2;
                if (scaled_flag == 1)
                {
                    NDSV_F2I32_FIXDIV (scratch_in[0], 4);
                    NDSV_F2I32_FIXDIV (scratch_in[1], 4);
                    NDSV_F2I32_FIXDIV (scratch_in[2], 4);
                    NDSV_F2I32_FIXDIV (scratch_in[3], 4);
                }

                // radix 4 butterfly with twiddles

                scratch[0] = scratch_in[0];
                scratch[1].r = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[1].r * scratch_tw[0].r
                                           + (NDSV_F2I32_SAMPPROD) scratch_in[1].i * scratch_tw[0].i) >> 31);
                scratch[1].i = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[1].i * scratch_tw[0].r
                                           - (NDSV_F2I32_SAMPPROD) scratch_in[1].r * scratch_tw[0].i) >> 31);

                scratch[2].r = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[2].r * scratch_tw[1].r
                                           + (NDSV_F2I32_SAMPPROD) scratch_in[2].i * scratch_tw[1].i) >> 31);
                scratch[2].i = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[2].i * scratch_tw[1].r
                                           - (NDSV_F2I32_SAMPPROD) scratch_in[2].r * scratch_tw[1].i) >> 31);

                scratch[3].r = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[3].r * scratch_tw[2].r
                                           + (NDSV_F2I32_SAMPPROD) scratch_in[3].i * scratch_tw[2].i) >> 31);
                scratch[3].i = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[3].i * scratch_tw[2].r
                                           - (NDSV_F2I32_SAMPPROD) scratch_in[3].r * scratch_tw[2].i) >> 31);

                // radix 2 butterfly
                scratch[4].r = scratch[0].r + scratch[2].r;
                scratch[4].i = scratch[0].i + scratch[2].i;

                scratch[5].r = scratch[0].r - scratch[2].r;
                scratch[5].i = scratch[0].i - scratch[2].i;

                // radix 2 butterfly
                scratch[6].r = scratch[1].r + scratch[3].r;
                scratch[6].i = scratch[1].i + scratch[3].i;

                scratch[7].r = scratch[1].r - scratch[3].r;
                scratch[7].i = scratch[1].i - scratch[3].i;

                // third result
                scratch_out[2].r = scratch[4].r - scratch[6].r;
                scratch_out[2].i = scratch[4].i - scratch[6].i;

                // first result
                scratch_out[0].r = scratch[4].r + scratch[6].r;
                scratch_out[0].i = scratch[4].i + scratch[6].i;

                // second result
                scratch_out[1].r = scratch[5].r - scratch[7].i;
                scratch_out[1].i = scratch[5].i + scratch[7].r;

                // forth result
                scratch_out[3].r = scratch[5].r + scratch[7].i;
                scratch_out[3].i = scratch[5].i - scratch[7].r;

                // store
                *Fout1 = scratch_out[0];
                Fout2 = Fout1 + mstride;
                *Fout2 = scratch_out[1];
                Fout2 += mstride;
                *Fout2 = scratch_out[2];
                Fout2 += mstride;
                *Fout2 = scratch_out[3];

                tw1++;
                Fin1 ++;
                Fout1 ++;
            } // m_count
        } // f_count
        tw += mstride * 3;
        mstride <<= 2;
        // swap
        Ftmp = buffer;
        buffer = Fout;
        Fout = Ftmp;
        fstride >>= 2;
    } // stage_count

    // the last one
    if (stage_count)
    {
        Fin1 = buffer;
        // if stage count is even, output to the input array
        Fout1 = Fout_ls;

        for (f_count = 0; f_count < fstride; f_count ++)
        {
            tw1 = tw;
            for (m_count = mstride; m_count ; m_count --)
            {
                // load
                scratch_tw[0] = *tw1;
                tw2 = tw1 + mstride;
                scratch_tw[1] = *tw2;
                tw2 += mstride;
                scratch_tw[2] = *tw2;
                scratch_in[0] = * Fin1;
                Fin2 = Fin1 + N;
                scratch_in[1] = * Fin2;
                Fin2 += N;
                scratch_in[2] = * Fin2;
                Fin2 += N;
                scratch_in[3] = * Fin2;
                if (scaled_flag == 1)
                {
                    NDSV_F2I32_FIXDIV (scratch_in[0], 4);
                    NDSV_F2I32_FIXDIV (scratch_in[1], 4);
                    NDSV_F2I32_FIXDIV (scratch_in[2], 4);
                    NDSV_F2I32_FIXDIV (scratch_in[3], 4);
                }

                // radix 4 butterfly with twiddles

                scratch[0] = scratch_in[0];
                scratch[1].r = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[1].r * scratch_tw[0].r
                                           + (NDSV_F2I32_SAMPPROD) scratch_in[1].i * scratch_tw[0].i) >> 31);
                scratch[1].i = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[1].i * scratch_tw[0].r
                                           - (NDSV_F2I32_SAMPPROD) scratch_in[1].r * scratch_tw[0].i) >> 31);

                scratch[2].r = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[2].r * scratch_tw[1].r
                                           + (NDSV_F2I32_SAMPPROD) scratch_in[2].i * scratch_tw[1].i) >> 31);
                scratch[2].i = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[2].i * scratch_tw[1].r
                                           - (NDSV_F2I32_SAMPPROD) scratch_in[2].r * scratch_tw[1].i) >> 31);

                scratch[3].r = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[3].r * scratch_tw[2].r
                                           + (NDSV_F2I32_SAMPPROD) scratch_in[3].i * scratch_tw[2].i) >> 31);
                scratch[3].i = (q31_t) ( ( (NDSV_F2I32_SAMPPROD) scratch_in[3].i * scratch_tw[2].r
                                           - (NDSV_F2I32_SAMPPROD) scratch_in[3].r * scratch_tw[2].i) >> 31);

                // radix 2 butterfly
                scratch[4].r = scratch[0].r + scratch[2].r;
                scratch[4].i = scratch[0].i + scratch[2].i;

                scratch[5].r = scratch[0].r - scratch[2].r;
                scratch[5].i = scratch[0].i - scratch[2].i;

                // radix 2 butterfly
                scratch[6].r = scratch[1].r + scratch[3].r;
                scratch[6].i = scratch[1].i + scratch[3].i;

                scratch[7].r = scratch[1].r - scratch[3].r;
                scratch[7].i = scratch[1].i - scratch[3].i;

                // third result
                scratch_out[2].r = scratch[4].r - scratch[6].r;
                scratch_out[2].i = scratch[4].i - scratch[6].i;

                // first result
                scratch_out[0].r = scratch[4].r + scratch[6].r;
                scratch_out[0].i = scratch[4].i + scratch[6].i;

                // second result
                scratch_out[1].r = scratch[5].r - scratch[7].i;
                scratch_out[1].i = scratch[5].i + scratch[7].r;

                // forth result
                scratch_out[3].r = scratch[5].r + scratch[7].i;
                scratch_out[3].i = scratch[5].i - scratch[7].r;

                // store
                *Fout1 = scratch_out[0];
                Fout2 = Fout1 + N;
                *Fout2 = scratch_out[1];
                Fout2 += N;
                *Fout2 = scratch_out[2];
                Fout2 += N;
                *Fout2 = scratch_out[3];

                tw1 ++;
                Fin1 ++;
                Fout1 ++;
            } // m_count
        } // f_count
    } // last stage
}

static void riscv_vec_rfft_split_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT dst,
                                      const riscv_vec_cpx_q31_t * FUNC_RESTRICT src,
                                      riscv_vec_cpx_q31_t * FUNC_RESTRICT twiddles,
                                      q31_t ncfft,
                                      q31_t scaled_flag)
{
    q31_t k;
    riscv_vec_cpx_q31_t fpnk, fpk, f1k, f2k, tw, tdc;

    tdc.r = src[0].r;
    tdc.i = src[0].i;

    if (scaled_flag)
        NDSV_F2I32_FIXDIV (tdc, 2);

    dst[0].r = tdc.r + tdc.i;
    dst[ncfft].r = tdc.r - tdc.i;
    dst[ncfft].i = dst[0].i = 0;

    for (k = 1; k <= ncfft / 2 ; ++k)
    {
        fpk    = src[k];
        fpnk.r =   src[ncfft - k].r;
        fpnk.i = - src[ncfft - k].i;
        if (scaled_flag)
        {
            NDSV_F2I32_FIXDIV (fpk, 2);
            NDSV_F2I32_FIXDIV (fpnk, 2);
        }

        f1k.r = fpk.r + fpnk.r;
        f1k.i = fpk.i + fpnk.i;

        f2k.r = fpk.r - fpnk.r;
        f2k.i = fpk.i - fpnk.i;

        tw.r = ( ( (q31_t) ( ( (NDSV_F2I32_SAMPPROD) f2k.r * (twiddles[k - 1]).r) >> 32)) - ( (q31_t) ( ( (NDSV_F2I32_SAMPPROD) f2k.i * (twiddles[k - 1]).i) >> 32))) << 1;
        tw.i = ( ( (q31_t) ( ( (NDSV_F2I32_SAMPPROD) f2k.r * (twiddles[k - 1]).i) >> 32)) + ( (q31_t) ( ( (NDSV_F2I32_SAMPPROD) f2k.i * (twiddles[k - 1]).r) >> 32))) << 1;

        dst[k].r = (f1k.r + tw.r) >> 1;
        dst[k].i = (f1k.i + tw.i) >> 1;
        dst[ncfft - k].r = (f1k.r - tw.r) >> 1;
        dst[ncfft - k].i = (tw.i - f1k.i) >> 1;
    }
}

static void riscv_vec_rifft_split_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT dst,
                                       const riscv_vec_cpx_q31_t * FUNC_RESTRICT src,
                                       riscv_vec_cpx_q31_t * FUNC_RESTRICT twiddles,
                                       q31_t ncfft,
                                       q31_t scaled_flag)
{

    q31_t k;
    riscv_vec_cpx_q31_t fk, fnkc, fek, fok, tmp;


    if (scaled_flag)
    {
        dst[0].r = (src[0].r / 2) + (src[ncfft].r / 2);
        dst[0].i = (src[0].r / 2) - (src[ncfft].r / 2);
    }
    else
    {
        dst[0].r = src[0].r + src[ncfft].r;
        dst[0].i = src[0].r - src[ncfft].r;
    }
    for (k = 1; k <= ncfft / 2; k++)
    {
        fk = src[k];
        fnkc.r = src[ncfft - k].r;
        fnkc.i = -src[ncfft - k].i;
        if (scaled_flag)
        {
            NDSV_F2I32_FIXDIV (fk, 2);
            NDSV_F2I32_FIXDIV (fnkc, 2);
        }

        fek.r = fk.r + fnkc.r;
        fek.i = fk.i + fnkc.i;

        tmp.r = fk.r - fnkc.r;
        tmp.i = fk.i - fnkc.i;

        fok.r = ( ( (q31_t) ( ( (NDSV_F2I32_SAMPPROD) tmp.r * (twiddles[k - 1]).r) >> 32)) + ( (q31_t) ( ( (NDSV_F2I32_SAMPPROD) tmp.i * (twiddles[k - 1]).i) >> 32))) << 1;
        fok.i = ( ( (q31_t) ( ( (NDSV_F2I32_SAMPPROD) tmp.i * (twiddles[k - 1]).r) >> 32)) - ( (q31_t) ( ( (NDSV_F2I32_SAMPPROD) tmp.r * (twiddles[k - 1]).i) >> 32))) << 1;

        dst[k].r = fek.r + fok.r;
        dst[k].i = fek.i + fok.i;

        dst[ncfft - k].r = fek.r - fok.r;
        dst[ncfft - k].i = fok.i - fek.i;
    }
}

riscv_vec_cfft_cfg_q31_t riscv_vec_cfft_init_q31 (q31_t nfft)
{
    riscv_vec_cfft_cfg_q31_t st = NULL;
    u32_t memneeded = sizeof (riscv_vec_cfft_state_q31_t)
                      + sizeof (q31_t) * (NDSV_FACTOR_MAX * 2) /* factors */
                      + sizeof (riscv_vec_cpx_q31_t) * nfft         /* twiddles */
                      + sizeof (riscv_vec_cpx_q31_t) * nfft           /* buffer */
                      + NDSV_ALIGNMENT;             /* 64-bit alignment */

    st = (riscv_vec_cfft_cfg_q31_t) NDSV_MALLOC (memneeded);
    if (st)
    {
        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_cfft_state_q31_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors = (q31_t*) address;
        st->twiddles = (riscv_vec_cpx_q31_t*) (st->factors + (NDSV_FACTOR_MAX * 2));
        st->buffer = st->twiddles + nfft;
        st->nfft = nfft;

        riscv_vec_create_factor (nfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        riscv_vec_create_twiddle_q31 (st->twiddles, st->factors, nfft);
    }

    return st;
}

void riscv_vec_cfft_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT out,
                         riscv_vec_cpx_q31_t * FUNC_RESTRICT in,
                         riscv_vec_cfft_cfg_q31_t cfg,
                         q31_t inverse_fft,
                         q31_t scaled_flag)
{
    if( cfg != NULL)
    {
        if (inverse_fft)
        {
            riscv_vec_ibfly_q31 (out, in, cfg->factors, cfg->twiddles, cfg->buffer, scaled_flag);
        }
        else
        {
            riscv_vec_bfly_q31 (out, in, cfg->factors, cfg->twiddles, cfg->buffer, scaled_flag);
        }

    }
}

riscv_vec_rfft_cfg_q31_t riscv_vec_rfft_init_q31 (q31_t nfft)
{
    riscv_vec_rfft_cfg_q31_t st = NULL;
    q31_t ncfft = nfft >> 1;

    u32_t memneeded = sizeof (riscv_vec_rfft_state_q31_t)
                      + sizeof (q31_t) * (NDSV_FACTOR_MAX * 2)    /* factors */
                      + sizeof (riscv_vec_cpx_q31_t) * ncfft           /* twiddles */
                      + sizeof (riscv_vec_cpx_q31_t) * ncfft / 2 /* super twiddles */
                      + sizeof (riscv_vec_cpx_q31_t) * nfft              /* buffer */
                      + NDSV_ALIGNMENT;                /* 64-bit alignment */

    st = (riscv_vec_rfft_cfg_q31_t) NDSV_MALLOC (memneeded);

    if (st)
    {
        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_rfft_state_q31_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors = (q31_t*) address;
        st->twiddles = (riscv_vec_cpx_q31_t*) (st->factors + (NDSV_FACTOR_MAX * 2));
        st->super_twiddles = st->twiddles + ncfft;
        st->buffer = st->super_twiddles + (ncfft / 2);
        st->ncfft = ncfft;

        riscv_vec_create_factor (ncfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        q31_t j, k;
        q31_t *factors = st->factors;
        riscv_vec_cpx_q31_t *twiddles = st->twiddles;
        q31_t stage_count = factors[0];
        q31_t fstride = factors[1];
        q31_t mstride;
        q31_t cur_radix;
        float32_t phase;
        const float32_t pi = NDSV_PI;

        // Don't generate any twiddles for the first stage
        stage_count --;

        // Generate twiddles for the other stages
        for (; stage_count > 0; stage_count --)
        {
            cur_radix = factors[2 * stage_count];
            fstride /= cur_radix;
            mstride = factors[2 * stage_count + 1];
            for (j = 0; j < mstride; j++)
            {
                for (k = 1; k < cur_radix; k++) // phase = 1 when k = 0
                {
                    phase = -2 * pi * fstride * k * j / ncfft;
                    twiddles[mstride * (k - 1) + j].r = (q31_t) floor (0.5f + NDSV_F2I32_MAX * cos (phase));
                    twiddles[mstride * (k - 1) + j].i = (q31_t) floor (0.5f + NDSV_F2I32_MAX * sin (phase));
                }
            }
            twiddles += mstride * (cur_radix - 1);
        }

        twiddles = st->super_twiddles;
        for (j = 0; j < ncfft / 2; j++)
        {
            phase = -pi * ( (float32_t) (j + 1) / ncfft + 0.5f);
            twiddles->r = (q31_t) floor (0.5f + NDSV_F2I32_MAX * cos (phase));
            twiddles->i = (q31_t) floor (0.5f + NDSV_F2I32_MAX * sin (phase));
            twiddles++;
        }
    }

    return st;
}

void riscv_vec_rfft_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT out,
                         q31_t * FUNC_RESTRICT in,
                         riscv_vec_rfft_cfg_q31_t cfg,
                         q31_t scaled_flag)
{
    if( cfg != NULL)
    {
        riscv_vec_cpx_q31_t * tmpbuf = cfg->buffer;

        riscv_vec_bfly_q31 (tmpbuf, (riscv_vec_cpx_q31_t*) in, cfg->factors, cfg->twiddles, out, scaled_flag);
        riscv_vec_rfft_split_q31 (out, tmpbuf, cfg->super_twiddles, cfg->ncfft, scaled_flag);

    }
}

void  riscv_vec_rifft_q31 (q31_t * FUNC_RESTRICT out,
                           riscv_vec_cpx_q31_t * FUNC_RESTRICT in,
                           riscv_vec_rfft_cfg_q31_t cfg,
                           q31_t scaled_flag)
{
    if( cfg != NULL)
    {
        riscv_vec_cpx_q31_t * tmpbuf1 = cfg->buffer;
        riscv_vec_cpx_q31_t * tmpbuf2 = cfg->buffer + cfg->ncfft;

        riscv_vec_rifft_split_q31 (tmpbuf1, in, cfg->super_twiddles, cfg->ncfft, scaled_flag);
        riscv_vec_ibfly_q31 ( (riscv_vec_cpx_q31_t*) out, tmpbuf1, cfg->factors, cfg->twiddles, tmpbuf2, scaled_flag);

    }
}
#endif

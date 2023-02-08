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

#include "internal_vec_transform.h"
#include "internal_nds_types.h"

#ifdef DEBUG_MODE
#define DBG_MSG         printf
#else
#define DBG_MSG(...)
#endif

#ifdef ENA_STOCKHAM_FFT
extern void riscv_vec_inverse_butterfly_f32 (riscv_vec_cpx_f32_t *out,
        riscv_vec_cpx_f32_t *in,
        q31_t *factors,
        riscv_vec_cpx_f32_t *twiddles,
        riscv_vec_cpx_f32_t *buffer);

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

static void riscv_vec_butterfly_f32 (riscv_vec_cpx_f32_t *out,
                                     riscv_vec_cpx_f32_t *in,
                                     q31_t *factors,
                                     riscv_vec_cpx_f32_t *twiddles,
                                     riscv_vec_cpx_f32_t *buffer)
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
            out_val[tmp_q + step*(2*tmp_p + 1)].r = tmp_r*tmp_cal_wp.r - tmp_i*tmp_cal_wp.i;
            out_val[tmp_q + step*(2*tmp_p + 1)].i = tmp_r*tmp_cal_wp.i + tmp_i*tmp_cal_wp.r;
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

static void _vec_split_r2c_1d_f32 (riscv_vec_cpx_f32_t *dst,
                                   const riscv_vec_cpx_f32_t *src,
                                   riscv_vec_cpx_f32_t *twiddles,
                                   q31_t ncfft)
{
    // pure C version last prcoess of RFFT
    riscv_vec_cpx_f32_t *out_val = dst;
    const riscv_vec_cpx_f32_t *in_val  = src;
    q31_t size = ncfft;
    riscv_vec_cpx_f32_t tmp_cal_1, tmp_cal_2, tmp_cal_wp;
    float32_t *wp_tbl_rfft_r = NULL, *wp_tbl_rfft_i = NULL;
    q31_t tmp_q = 0;
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
        out_val[tmp_q].r = 0.5*(tmp_cal_1.r - (tmp_cal_2.r*tmp_cal_wp.i + tmp_cal_2.i*tmp_cal_wp.r));  // (xe_r - xo*tw_i)/2
        out_val[tmp_q].i = 0.5*(tmp_cal_1.i + (tmp_cal_2.r*tmp_cal_wp.r - tmp_cal_2.i*tmp_cal_wp.i));  // (xe_i + xo*tw_r)/2
        out_val[size-tmp_q].r = 0.5*(tmp_cal_1.r + (tmp_cal_2.r*tmp_cal_wp.i + tmp_cal_2.i*tmp_cal_wp.r));
        out_val[size-tmp_q].i = 0.5*((tmp_cal_2.r*tmp_cal_wp.r - tmp_cal_2.i*tmp_cal_wp.i) - tmp_cal_1.i);
    }
}

/**
 * cfft_init_f32
 */
riscv_vec_cfft_cfg_f32_t riscv_vec_cfft_init_f32 (q31_t nfft)
{
    riscv_vec_cfft_cfg_f32_t st = NULL;
    if( (((nfft) & (nfft-1)) != 0) || (nfft<8) || (nfft>16384L) )
    {
        // only support the size of power of 2 and 8 ~ 16384
        return NULL;
    }
    int32_t twdl_size = nfft/2;

    u32_t memneeded = sizeof (riscv_vec_cfft_state_f32_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)       /* factors */
                      + sizeof (riscv_vec_cpx_f32_t) * twdl_size     /* twiddles */
                      + sizeof (riscv_vec_cpx_f32_t) * nfft          /* buffer */
                      + NDSV_ALIGNMENT;             /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif

    st = (riscv_vec_cfft_cfg_f32_t) NDSV_MALLOC (memneeded);

    if(st != NULL)
    {
        // Only backward FFT is scaled by default.
        st->is_forward_scaled = 0;
        st->is_backward_scaled = 1;

        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_cfft_state_f32_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors = (q31_t*) address;
        st->twiddles = (riscv_vec_cpx_f32_t*) (st->factors + (NDSV_STOCKHAM_FACTOR_MAX));
        st->buffer = st->twiddles + twdl_size;
        st->nfft = nfft;

        riscv_vec_create_factor(nfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        st->factors[RFFT_EN] = 0 ; // 1:rfft / 0: cfft
        riscv_vec_create_twiddle_f32 (st->twiddles, st->factors, nfft);
    }
    return st;
}

/**
 * cfft_f32
 */
void riscv_vec_cfft_f32 (riscv_vec_cpx_f32_t *out,
                         riscv_vec_cpx_f32_t *in,
                         riscv_vec_cfft_cfg_f32_t cfg,
                         q31_t inverse_fft)
{
    if(cfg != NULL)
    {
        if (inverse_fft)
        {
            riscv_vec_inverse_butterfly_f32 (out, in, cfg->factors, cfg->twiddles, cfg->buffer);
        }
        else
        {
            riscv_vec_butterfly_f32 (out, in, cfg->factors, cfg->twiddles, cfg->buffer);
        }
    
    }
}

/**
 * @ingroup R2C_FFT_IFFT
 * @brief Creates a configuration structure for variants of @ref riscv_vec_rfft_f32 and @ref riscv_vec_rifft_f32.
 *
 * @param[in]   nfft             input length
 * @retval      st               pointer to an FFT configuration structure (allocated with `malloc`), or `NULL` to indicate an error
 *
 * Allocates and initialises an @ref riscv_vec_rfft_cfg_f32_t configuration structure for
 * the FP32 real-to-complex and complex-to-real FFT/IFFT. As part of this, it reserves a buffer used
 * internally by the FFT algorithm, factors the length of the FFT into simpler chunks, and generates a
 * "twiddle table" of coefficients used in the FFT "butterfly" calculations.
 */
riscv_vec_rfft_cfg_f32_t riscv_vec_rfft_init_f32 (q31_t nfft)
{
    riscv_vec_rfft_cfg_f32_t st = NULL;
    riscv_vec_cpx_f32_t* twiddles = NULL;
    float32_t *wp_tbl_rfft_r = NULL, *wp_tbl_rfft_i = NULL;
    int32_t ncfft = nfft/2;
    if( (((nfft) & (nfft-1)) != 0) || (nfft<8) || (nfft>16384L) )
    {
        // only support the size of power of 2 and 8 ~ 16384
        return NULL;
    }
    int32_t twdl_size = ncfft/2;
    float factor = 2*NDSV_PI/nfft;
    int32_t j = 0 ;

    u32_t memneeded = sizeof (riscv_vec_rfft_state_f32_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)        /* factors */
                      + sizeof (riscv_vec_cpx_f32_t) * (twdl_size)    /* twiddle */
                      + sizeof (riscv_vec_cpx_f32_t) * (ncfft)        /* super twiddles */
                      + sizeof (riscv_vec_cpx_f32_t) * (nfft)         /* buffer */
                      + NDSV_ALIGNMENT;                    /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif

    st = (riscv_vec_rfft_cfg_f32_t) NDSV_MALLOC (memneeded);

    if (st != NULL)
    {
        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_rfft_state_f32_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors = (q31_t*) address;
        st->twiddles = (riscv_vec_cpx_f32_t*) (st->factors + (NDSV_STOCKHAM_FACTOR_MAX));
        st->super_twiddles = st->twiddles + twdl_size;
        st->buffer = st->super_twiddles + (ncfft);
        st->ncfft = ncfft;
        riscv_vec_create_factor (ncfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        st->factors[RFFT_EN] = 1 ; // 1:rfft / 0: cfft
        riscv_vec_create_twiddle_f32 (st->twiddles, st->factors, ncfft);
        twiddles = st->super_twiddles;
        wp_tbl_rfft_r = &twiddles[0].r;
        wp_tbl_rfft_i = &twiddles[ncfft/2].r;
        for (j = 0; j < ncfft ; j++)
        {
            //phase = j* factor;
            wp_tbl_rfft_r[j] =  cos (j*factor);
            wp_tbl_rfft_i[j] =  -sin (j*factor);
            
        }
    }
    return st;
}

/**
 * rfft_f32
 */
void riscv_vec_rfft_f32 (riscv_vec_cpx_f32_t *out,
                         float32_t *in,
                         riscv_vec_rfft_cfg_f32_t cfg)
{
    if(cfg != NULL)
    {
        riscv_vec_cpx_f32_t * tmpbuf = cfg->buffer;

        riscv_vec_butterfly_f32 (tmpbuf, (riscv_vec_cpx_f32_t*) in, cfg->factors, cfg->twiddles, out);
        _vec_split_r2c_1d_f32 (out, tmpbuf, cfg->super_twiddles, cfg->ncfft);
        
    }
}

#else  //ENA_STOCKHAM_FFT
extern void riscv_vec_inverse_butterfly_f32 (riscv_vec_cpx_f32_t *out,
        riscv_vec_cpx_f32_t *in,
        q31_t *factors,
        riscv_vec_cpx_f32_t *twiddles,
        riscv_vec_cpx_f32_t *buffer);

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

static void riscv_vec_butterfly_f32 (riscv_vec_cpx_f32_t *out,
                                     riscv_vec_cpx_f32_t *in,
                                     q31_t *factors,
                                     riscv_vec_cpx_f32_t *twiddles,
                                     riscv_vec_cpx_f32_t *buffer)
{
    q31_t stage_count = factors[0];
    q31_t fstride = factors[1];
    q31_t mstride = factors[(stage_count << 1) - 1];
    q31_t first_radix = factors[stage_count << 1];
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

    DBG_MSG("First stage:\n");


    // The first stage (using hardcoded twiddles)
    if (first_radix == 8) // For our factoring this means nfft is of form 2^{odd}
    {

        DBG_MSG("Radix-8\n");

        for (f_count = 0; f_count < fstride; f_count++)
        {
            DBG_MSG("f_count = %d:\n", f_count);

            dst = &out[f_count * 8];

            // X[0] +/- X[4N/8]
            scratch_in[0].r = src[0].r + src[fstride * 4].r;
            scratch_in[0].i = src[0].i + src[fstride * 4].i;
            scratch_in[1].r = src[0].r - src[fstride * 4].r;
            scratch_in[1].i = src[0].i - src[fstride * 4].i;

            // X[N/8] +/- X[5N/8]
            scratch_in[2].r = src[fstride].r + src[fstride * 5].r;
            scratch_in[2].i = src[fstride].i + src[fstride * 5].i;
            scratch_in[3].r = src[fstride].r - src[fstride * 5].r;
            scratch_in[3].i = src[fstride].i - src[fstride * 5].i;

            // X[2N/8] +/- X[6N/8]
            scratch_in[4].r = src[fstride * 2].r + src[fstride * 6].r;
            scratch_in[4].i = src[fstride * 2].i + src[fstride * 6].i;
            scratch_in[5].r = src[fstride * 2].r - src[fstride * 6].r;
            scratch_in[5].i = src[fstride * 2].i - src[fstride * 6].i;

            // X[3N/8] +/- X[7N/8]
            scratch_in[6].r = src[fstride * 3].r + src[fstride * 7].r;
            scratch_in[6].i = src[fstride * 3].i + src[fstride * 7].i;
            scratch_in[7].r = src[fstride * 3].r - src[fstride * 7].r;
            scratch_in[7].i = src[fstride * 3].i - src[fstride * 7].i;


            scratch[0] = scratch_in[0]; // X[0] + X[4N/8]
            scratch[1] = scratch_in[1]; // X[0] - X[4N/8]
            scratch[2] = scratch_in[2]; // X[N/8] + X[5N/8]
            scratch[4] = scratch_in[4]; // X[2N/8] + X[6N/8]
            scratch[6] = scratch_in[6]; // X[3N/8] + X[7N/8]

            // (X[2N/8] - X[6N/8]) * -i
            scratch[5].r = scratch_in[5].i;
            scratch[5].i = -scratch_in[5].r;

            // (X[N/8] - X[5N/8]) * (TW_81 - TW_81i)
            scratch[3].r = (scratch_in[3].r + scratch_in[3].i) * TW_81;
            scratch[3].i = (scratch_in[3].i - scratch_in[3].r) * TW_81;

            // (X[3N/8] - X[7N/8]) * (TW_81 + TW_81i)
            scratch[7].r = (scratch_in[7].r - scratch_in[7].i) * TW_81;
            scratch[7].i = (scratch_in[7].i + scratch_in[7].r) * TW_81;

            // Combine the (X[0] +/- X[4N/8]) and (X[2N/8] +/- X[6N/8]) components
            scratch[8].r  = scratch[0].r + scratch[4].r;
            scratch[8].i  = scratch[0].i + scratch[4].i;
            scratch[9].r  = scratch[1].r + scratch[5].r;
            scratch[9].i  = scratch[1].i + scratch[5].i;
            scratch[10].r = scratch[0].r - scratch[4].r;
            scratch[10].i = scratch[0].i - scratch[4].i;
            scratch[11].r = scratch[1].r - scratch[5].r;
            scratch[11].i = scratch[1].i - scratch[5].i;

            // Combine the (X[N/8] +/- X[5N/8]) and (X[3N/8] +/- X[7N/8]) components
            scratch[12].r = scratch[2].r + scratch[6].r;
            scratch[12].i = scratch[2].i + scratch[6].i;
            scratch[13].r = scratch[3].r - scratch[7].r;
            scratch[13].i = scratch[3].i - scratch[7].i;
            scratch[14].r = scratch[2].r - scratch[6].r;
            scratch[14].i = scratch[2].i - scratch[6].i;
            scratch[15].r = scratch[3].r + scratch[7].r;
            scratch[15].i = scratch[3].i + scratch[7].i;

            // Combine the two combined components (for the full radix-8 butterfly)
            scratch_out[0].r = scratch[8].r  + scratch[12].r;
            scratch_out[0].i = scratch[8].i  + scratch[12].i;
            scratch_out[1].r = scratch[9].r  + scratch[13].r;
            scratch_out[1].i = scratch[9].i  + scratch[13].i;
            scratch_out[2].r = scratch[10].r + scratch[14].i;
            scratch_out[2].i = scratch[10].i - scratch[14].r;
            scratch_out[3].r = scratch[11].r + scratch[15].i;
            scratch_out[3].i = scratch[11].i - scratch[15].r;
            scratch_out[4].r = scratch[8].r  - scratch[12].r;
            scratch_out[4].i = scratch[8].i  - scratch[12].i;
            scratch_out[5].r = scratch[9].r  - scratch[13].r;
            scratch_out[5].i = scratch[9].i  - scratch[13].i;
            scratch_out[6].r = scratch[10].r - scratch[14].i;
            scratch_out[6].i = scratch[10].i + scratch[14].r;
            scratch_out[7].r = scratch[11].r - scratch[15].i;
            scratch_out[7].i = scratch[11].i + scratch[15].r;

            // Store the results
            dst[0] = scratch_out[0];
            dst[1] = scratch_out[1];
            dst[2] = scratch_out[2];
            dst[3] = scratch_out[3];
            dst[4] = scratch_out[4];
            dst[5] = scratch_out[5];
            dst[6] = scratch_out[6];
            dst[7] = scratch_out[7];

#ifdef DEBUG_MODE
            for (int i = 0; i < 8; i++)
            {
                DBG_MSG("[%d] %14.7f(0x%08X) + %14.7fi(0x%08X)\n", i, dst[i].r, *(unsigned int*)&dst[i].r, dst[i].i, *(unsigned int*)&dst[i].i);
            }
#endif

            src++;
        } // f_count

        // Update variables for the next stages
        step = fstride << 1; // For C2C, 1/4 of input size (fstride is nfft/8)
        stage_count--;
        fstride /= 4;

    }
    else if (first_radix == 4) // For our factoring this means nfft is of form 2^{even} >= 4
    {
        for (f_count = fstride; f_count; f_count--)
        {
            // Load the four input values for a radix-4 butterfly
            scratch_in[0] = src[0];           // X[0]
            scratch_in[1] = src[fstride * 1]; // X[N/4]
            scratch_in[2] = src[fstride * 2]; // X[2N/4]
            scratch_in[3] = src[fstride * 3]; // X[3N/4]

            // X[0] +/- X[2N/4]
            scratch[0].r = scratch_in[0].r + scratch_in[2].r;
            scratch[0].i = scratch_in[0].i + scratch_in[2].i;
            scratch[1].r = scratch_in[0].r - scratch_in[2].r;
            scratch[1].i = scratch_in[0].i - scratch_in[2].i;

            // X[N/4] +/- X[3N/4]
            scratch[2].r = scratch_in[1].r + scratch_in[3].r;
            scratch[2].i = scratch_in[1].i + scratch_in[3].i;
            scratch[3].r = scratch_in[1].r - scratch_in[3].r;
            scratch[3].i = scratch_in[1].i - scratch_in[3].i;

            // Combine the (X[0] +/- X[2N/4]) and (X[N/4] +/- X[3N/4]) components (for
            // the full radix-4 butterfly)
            scratch_out[0].r = scratch[0].r + scratch[2].r;
            scratch_out[0].i = scratch[0].i + scratch[2].i;
            scratch_out[1].r = scratch[1].r + scratch[3].i; // scratch[1] - i*scratch[3]
            scratch_out[1].i = scratch[1].i - scratch[3].r;
            scratch_out[2].r = scratch[0].r - scratch[2].r;
            scratch_out[2].i = scratch[0].i - scratch[2].i;
            scratch_out[3].r = scratch[1].r - scratch[3].i; // scratch[1] + i*scratch[3]
            scratch_out[3].i = scratch[1].i + scratch[3].r;

            // Store the results
            *dst++ = scratch_out[0];
            *dst++ = scratch_out[1];
            *dst++ = scratch_out[2];
            *dst++ = scratch_out[3];

            src++;
        } // f_count

        // Update variables for the next stages
        step = fstride; // For C2C, 1/4 of input size (fstride is nfft/4)
        stage_count--;
        fstride /= 4;
    }
    else if (first_radix == 2) // nfft = 2
    {
        dst[0].r = src[0].r + src[1].r;
        dst[0].i = src[0].i + src[1].i;
        dst[1].r = src[0].r - src[1].r;
        dst[1].i = src[0].i - src[1].i;
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
            tw = twiddles; // Reset the twiddle pointer for the next section
            for (m_count = mstride; m_count; m_count--)
            {
                // Load the three twiddles and four input values for a radix-4 butterfly
                scratch_tw[0] = tw[0];           // w^{k}
                scratch_tw[1] = tw[mstride * 1]; // w^{2k}
                scratch_tw[2] = tw[mstride * 2]; // w^{3k}
                scratch_in[0] = src[0];
                scratch_in[1] = src[step * 1];
                scratch_in[2] = src[step * 2];
                scratch_in[3] = src[step * 3];

                // Multiply input elements by their associated twiddles
                scratch[0] = scratch_in[0];
                scratch[1].r = scratch_in[1].r * scratch_tw[0].r - scratch_in[1].i * scratch_tw[0].i;
                scratch[1].i = scratch_in[1].i * scratch_tw[0].r + scratch_in[1].r * scratch_tw[0].i;
                scratch[2].r = scratch_in[2].r * scratch_tw[1].r - scratch_in[2].i * scratch_tw[1].i;
                scratch[2].i = scratch_in[2].i * scratch_tw[1].r + scratch_in[2].r * scratch_tw[1].i;
                scratch[3].r = scratch_in[3].r * scratch_tw[2].r - scratch_in[3].i * scratch_tw[2].i;
                scratch[3].i = scratch_in[3].i * scratch_tw[2].r + scratch_in[3].r * scratch_tw[2].i;

                // X[0] +/- X[2N/4]
                scratch[4].r = scratch[0].r + scratch[2].r;
                scratch[4].i = scratch[0].i + scratch[2].i;
                scratch[5].r = scratch[0].r - scratch[2].r;
                scratch[5].i = scratch[0].i - scratch[2].i;

                // X[N/4] +/- X[3N/4]
                scratch[6].r = scratch[1].r + scratch[3].r;
                scratch[6].i = scratch[1].i + scratch[3].i;
                scratch[7].r = scratch[1].r - scratch[3].r;
                scratch[7].i = scratch[1].i - scratch[3].i;

                // Combine the (X[0] +/- X[2N/4]) and (X[N/4] +/- X[3N/4]) components (for
                // the full radix-4 butterfly)
                scratch_out[0].r = scratch[4].r + scratch[6].r;
                scratch_out[0].i = scratch[4].i + scratch[6].i;
                scratch_out[1].r = scratch[5].r + scratch[7].i;
                scratch_out[1].i = scratch[5].i - scratch[7].r;
                scratch_out[2].r = scratch[4].r - scratch[6].r;
                scratch_out[2].i = scratch[4].i - scratch[6].i;
                scratch_out[3].r = scratch[5].r - scratch[7].i;
                scratch_out[3].i = scratch[5].i + scratch[7].r;

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

        for (f_count = fstride; f_count; f_count--) // Note: for C2C, fstride = 1
        {
            tw = twiddles; // Reset the twiddle pointer for the next section
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
                scratch[1].r = scratch_in[1].r * scratch_tw[0].r - scratch_in[1].i * scratch_tw[0].i;
                scratch[1].i = scratch_in[1].i * scratch_tw[0].r + scratch_in[1].r * scratch_tw[0].i;
                scratch[2].r = scratch_in[2].r * scratch_tw[1].r - scratch_in[2].i * scratch_tw[1].i;
                scratch[2].i = scratch_in[2].i * scratch_tw[1].r + scratch_in[2].r * scratch_tw[1].i;
                scratch[3].r = scratch_in[3].r * scratch_tw[2].r - scratch_in[3].i * scratch_tw[2].i;
                scratch[3].i = scratch_in[3].i * scratch_tw[2].r + scratch_in[3].r * scratch_tw[2].i;

                // X[0] +/- X[2N/4]
                scratch[4].r = scratch[0].r + scratch[2].r;
                scratch[4].i = scratch[0].i + scratch[2].i;
                scratch[5].r = scratch[0].r - scratch[2].r;
                scratch[5].i = scratch[0].i - scratch[2].i;

                // X[N/4] +/- X[3N/4]
                scratch[6].r = scratch[1].r + scratch[3].r;
                scratch[6].i = scratch[1].i + scratch[3].i;
                scratch[7].r = scratch[1].r - scratch[3].r;
                scratch[7].i = scratch[1].i - scratch[3].i;

                // Combine the (X[0] +/- X[2N/4]) and (X[N/4] +/- X[3N/4]) components (for
                // the full radix-4 butterfly)
                scratch_out[0].r = scratch[4].r + scratch[6].r;
                scratch_out[0].i = scratch[4].i + scratch[6].i;
                scratch_out[1].r = scratch[5].r + scratch[7].i;
                scratch_out[1].i = scratch[5].i - scratch[7].r;
                scratch_out[2].r = scratch[4].r - scratch[6].r;
                scratch_out[2].i = scratch[4].i - scratch[6].i;
                scratch_out[3].r = scratch[5].r - scratch[7].i;
                scratch_out[3].i = scratch[5].i + scratch[7].r;

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

static void _vec_split_r2c_1d_f32 (riscv_vec_cpx_f32_t *dst,
                                   const riscv_vec_cpx_f32_t *src,
                                   riscv_vec_cpx_f32_t *twiddles,
                                   q31_t ncfft)
{
    q31_t k;
    riscv_vec_cpx_f32_t fpnk, fpk, f1k, f2k, tw, tdc;

    tdc.r = src[0].r;
    tdc.i = src[0].i;

    dst[0].r = tdc.r + tdc.i;
    dst[ncfft].r = tdc.r - tdc.i;
    dst[ncfft].i = dst[0].i = 0;

    for (k = 1; k <= ncfft / 2 ; ++k)
    {
        fpk    = src[k];
        fpnk.r =   src[ncfft - k].r;
        fpnk.i = - src[ncfft - k].i;

        f1k.r = fpk.r + fpnk.r;
        f1k.i = fpk.i + fpnk.i;

        f2k.r = fpk.r - fpnk.r;
        f2k.i = fpk.i - fpnk.i;

        tw.r = f2k.r * (twiddles[k - 1]).r - f2k.i * (twiddles[k - 1]).i;
        tw.i = f2k.r * (twiddles[k - 1]).i + f2k.i * (twiddles[k - 1]).r;

        dst[k].r = (f1k.r + tw.r) * 0.5f;
        dst[k].i = (f1k.i + tw.i) * 0.5f;
        dst[ncfft - k].r = (f1k.r - tw.r) * 0.5f;
        dst[ncfft - k].i = (tw.i - f1k.i) * 0.5f;
    }
}

/**
 * cfft_init_f32
 */
riscv_vec_cfft_cfg_f32_t riscv_vec_cfft_init_f32 (q31_t nfft)
{
    riscv_vec_cfft_cfg_f32_t st = NULL;
    u32_t memneeded = sizeof (riscv_vec_cfft_state_f32_t)
                      + sizeof (q31_t) * (NDSV_FACTOR_MAX * 2) /* factors */
                      + sizeof (riscv_vec_cpx_f32_t) * nfft       /* twiddles */
                      + sizeof (riscv_vec_cpx_f32_t) * nfft         /* buffer */
                      + NDSV_ALIGNMENT;             /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif
    if( (((nfft) & (nfft-1)) != 0) || (nfft<8) || (nfft>16384L) )
    {
        // only suppor the size with power of 2 and 8 ~ 16384
        return NULL;
    }

    st = (riscv_vec_cfft_cfg_f32_t) NDSV_MALLOC (memneeded);

    if (st == NULL)
    {
        return st;
    }

    // Only backward FFT is scaled by default.
    st->is_forward_scaled = 0;
    st->is_backward_scaled = 1;

    uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_cfft_state_f32_t);
    NDSV_ALIGN (address, NDSV_ALIGNMENT);
    st->factors = (q31_t*) address;
    st->twiddles = (riscv_vec_cpx_f32_t*) (st->factors + (NDSV_FACTOR_MAX * 2));
    st->buffer = st->twiddles + nfft;
    st->nfft = nfft;

    riscv_vec_create_factor (nfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
    riscv_vec_create_twiddle_f32 (st->twiddles, st->factors, nfft);

    return st;
}

/**
 * cfft_f32
 */
void riscv_vec_cfft_f32 (riscv_vec_cpx_f32_t *out,
                         riscv_vec_cpx_f32_t *in,
                         riscv_vec_cfft_cfg_f32_t cfg,
                         q31_t inverse_fft)
{
    if(cfg != NULL)
    {
        if (inverse_fft)
        {
            riscv_vec_inverse_butterfly_f32 (out, in, cfg->factors, cfg->twiddles, cfg->buffer);
        }
        else
        {
            riscv_vec_butterfly_f32 (out, in, cfg->factors, cfg->twiddles, cfg->buffer);
        }
    }
}

/**
 * @ingroup R2C_FFT_IFFT
 * @brief Creates a configuration structure for variants of @ref riscv_vec_rfft_f32 and @ref riscv_vec_rifft_f32.
 *
 * @param[in]   nfft             input length
 * @retval      st               pointer to an FFT configuration structure (allocated with `malloc`), or `NULL` to indicate an error
 *
 * Allocates and initialises an @ref riscv_vec_rfft_cfg_f32_t configuration structure for
 * the FP32 real-to-complex and complex-to-real FFT/IFFT. As part of this, it reserves a buffer used
 * internally by the FFT algorithm, factors the length of the FFT into simpler chunks, and generates a
 * "twiddle table" of coefficients used in the FFT "butterfly" calculations.
 */
riscv_vec_rfft_cfg_f32_t riscv_vec_rfft_init_f32 (q31_t nfft)
{
    riscv_vec_rfft_cfg_f32_t st = NULL;
    q31_t ncfft = nfft >> 1;

    if( (((nfft) & (nfft-1)) != 0) || (nfft<8) || (nfft>16384L) )
    {
        // only suppor the size with power of 2 and 8 ~ 16384
        return NULL;
    }

    u32_t memneeded = sizeof (riscv_vec_rfft_state_f32_t)
                      + sizeof (q31_t) * (NDSV_FACTOR_MAX * 2)        /* factors */
                      + sizeof (riscv_vec_cpx_f32_t) * ncfft              /* twiddle */
                      + sizeof (riscv_vec_cpx_f32_t) * (ncfft / 2) /* super twiddles */
                      + sizeof (riscv_vec_cpx_f32_t) * nfft                /* buffer */
                      + NDSV_ALIGNMENT;                    /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif

    st = (riscv_vec_rfft_cfg_f32_t) NDSV_MALLOC (memneeded);

    if (st)
    {
        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_rfft_state_f32_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors = (q31_t*) address;
        st->twiddles = (riscv_vec_cpx_f32_t*) (st->factors + (NDSV_FACTOR_MAX * 2));
        st->super_twiddles = st->twiddles + ncfft;
        st->buffer = st->super_twiddles + (ncfft / 2);
        st->ncfft = ncfft;

        riscv_vec_create_factor (ncfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        q31_t j, k;
        q31_t *factors = st->factors;
        riscv_vec_cpx_f32_t *twiddles = st->twiddles;
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
                    twiddles[mstride * (k - 1) + j].r = (float32_t) cos (phase);
                    twiddles[mstride * (k - 1) + j].i = (float32_t) sin (phase);
                }
            }
            twiddles += mstride * (cur_radix - 1);
        }

        twiddles = st->super_twiddles;
        for (j = 0; j < ncfft / 2; j++)
        {
            phase = -pi * ( (float32_t) (j + 1) / ncfft + 0.5f);
            twiddles->r = (float32_t) cos (phase);
            twiddles->i = (float32_t) sin (phase);
            twiddles++;
        }
    }

    return st;
}

/**
 * rfft_f32
 */
void riscv_vec_rfft_f32 (riscv_vec_cpx_f32_t *out,
                         float32_t *in,
                         riscv_vec_rfft_cfg_f32_t cfg)
{
    if(cfg != NULL)
    {
        riscv_vec_cpx_f32_t * tmpbuf = cfg->buffer;

        riscv_vec_butterfly_f32 (tmpbuf, (riscv_vec_cpx_f32_t*) in, cfg->factors, cfg->twiddles, out);
        _vec_split_r2c_1d_f32 (out, tmpbuf, cfg->super_twiddles, cfg->ncfft);
    }
}

#endif  //ENA_STOCKHAM_FFT

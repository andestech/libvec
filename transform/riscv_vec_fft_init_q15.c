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

#ifdef ENA_MEASURE_HEAP
#include <stdio.h>
#endif

#ifdef ENA_STOCKHAM_FFT
/*
 * This function outputs a factor buffer ('facbuf') that decomposes an FFT of input size
 * n into a number of radix-r butterfly calculations (for r in some set of radix values).
 *
 * Factor buffer layout:
 *     index 0: buffer init info
 *     index 1: stride for the first stage (also for the info of fft_size)
 *     index 2: RFFT flag ( 1: RFFT / 0: CFFT)
 *
 * for C version, stockham FFT using radix-2 only.
 * for vector version, stockham fft using radix-4 + radix-2.
 *    example:
 *        fft size 1024 (2^10) -> radix 4 for all stage
 *        ffr size 2048 (2^11) -> radix 2 for the last stage and radix-4 for the others.
 *
 * Only the leading 42 int32 is used to store factors.
 * The left can be used as algorithm flags, or status flags.
 * Even the leading bits of stage number can be reused.
 */
// this is a temp function for radix8 create factor, now only for float type support the
// algorithm
static q31_t riscv_vec_create_factor_q15 (q31_t n,
                                          q31_t * facbuf,
                                          q31_t vec_factor_flags)
{
#ifdef ENA_STOCKHAM_FFT
    q31_t p = 2;
    q31_t stage_num = 0;
    q31_t stride_max = n;
    do
    {
        n /= p;
        stage_num++;
    }
    while (n > 1);

    facbuf[OUT_INDEX] = stage_num;
    facbuf[SIZE_FIRST_STAGE] = stride_max / p;
    // facbuf[2 * i] = alg_flag;
#endif  //ENA_STOCKHAM_FFT
    return NDSV_OK;
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
 * cfft_q15 cifft_q15 initial and free function
 */
riscv_vec_cfft_cfg_q15_t riscv_vec_cfft_init_q15 (q31_t nfft)
{
    riscv_vec_cfft_cfg_q15_t st = NULL;
    q31_t tmp_i = 0 ;
    const float64_t pi = NDSV_PI;
    float phase = 2 * pi / nfft;
    q15_t tmp_cmp = -32767;
    riscv_vec_cpx_q15_t *twiddles = NULL;
    if( (((nfft) & (nfft - 1)) != 0) || (nfft < 8) || (nfft > 16384L) )
    {
        // only support the size with power of 2 and 8 ~ 16384
        return NULL;
    }
    int32_t twdl_size = nfft / 2;

    u32_t memneeded = sizeof (riscv_vec_cfft_state_q15_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)       /* factors */
                      + sizeof (riscv_vec_cpx_q15_t) * (twdl_size)     /* twiddles */
                      + sizeof (riscv_vec_cpx_q15_t) * (nfft)          /* buffer */
                      + NDSV_ALIGNMENT;             /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif

    st = (riscv_vec_cfft_cfg_q15_t) NDSV_MALLOC (memneeded);

    if(st != NULL)
    {
        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_cfft_state_q15_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors = (q31_t*) address;
        st->twiddles = (riscv_vec_cpx_q15_t*) (st->factors + (NDSV_STOCKHAM_FACTOR_MAX));
        st->buffer = st->twiddles + twdl_size;
        st->nfft = nfft;

        riscv_vec_create_factor_q15(nfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        st->factors[RFFT_EN] = 0 ; // 1:rfft / 0: cfft

        q31_t cal_fft = nfft / 2;
        twiddles  = st->twiddles;
        for(tmp_i = 0 ; tmp_i < cal_fft ; tmp_i++)
        {
            twiddles[tmp_i].r = ((q15_t)floor(0.5f + NDSV_F2I16_MAX * cos((tmp_i) * phase)));
            twiddles[tmp_i].i = ((q15_t)floor(-0.5f - NDSV_F2I16_MAX * sin((tmp_i) * phase)));
            /*
            if(twiddles[tmp_i].i <= tmp_cmp)
            {
                twiddles[tmp_i].i = tmp_cmp;
            }
            */
            twiddles[tmp_i].i = ((twiddles[tmp_i].i) > (tmp_cmp)) ? (twiddles[tmp_i].i) : (tmp_cmp);
        }
    } //if(st)
    return st;
}

void riscv_vec_cfft_free_q15 (riscv_vec_cfft_cfg_q15_t cfg)
{
    //free (cfg);
    NDSV_FREE(cfg);
}

/**
 * rfft_q15 rifft_q15 initial and free function
 */
riscv_vec_rfft_cfg_q15_t riscv_vec_rfft_init_q15 (q31_t nfft)
{
    riscv_vec_rfft_cfg_q15_t st = NULL;
    riscv_vec_cpx_q15_t* twiddles = NULL;
    q15_t *wp_tbl_rfft_r = NULL, *wp_tbl_rfft_i = NULL;
    q15_t tmp_cmp = -32767;
    int32_t ncfft = nfft / 2;
    float phase = 2 * NDSV_PI / ncfft;
    if( (((nfft) & (nfft - 1)) != 0) || (nfft < 8) || (nfft > 16384L) )
    {
        // only support the size with power of 2 and 8 ~ 16384
        return NULL;
    }
    int32_t twdl_size = ncfft / 2;
    int32_t j = 0 ;

    u32_t memneeded = sizeof (riscv_vec_rfft_state_q15_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)        /* factors */
                      + sizeof (riscv_vec_cpx_q15_t) * (twdl_size)    /* twiddle */
                      + sizeof (riscv_vec_cpx_q15_t) * (ncfft)        /* super twiddles */
                      + sizeof (riscv_vec_cpx_q15_t) * (nfft)         /* buffer */
                      + NDSV_ALIGNMENT;                    /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif

    st = (riscv_vec_rfft_cfg_q15_t) NDSV_MALLOC (memneeded);

    if (st != NULL)
    {
        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_rfft_state_q15_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors = (q31_t*) address;
        st->twiddles = (riscv_vec_cpx_q15_t*) (st->factors + (NDSV_STOCKHAM_FACTOR_MAX));
        st->super_twiddles = st->twiddles + twdl_size;
        st->buffer = st->super_twiddles + (ncfft);
        st->ncfft = ncfft;
        riscv_vec_create_factor_q15 (ncfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        st->factors[RFFT_EN] = 1 ; // 1:rfft / 0: cfft
        //riscv_vec_create_twiddle_q15 (st->twiddles, st->factors, ncfft);
        // Pure C version using RADIX-2 only
        q31_t cal_fft = ncfft / 2;
        riscv_vec_cpx_q15_t *wp_tbl_cmplx = st->twiddles;
        for(j = 0 ; j < cal_fft ; j++)
        {
            wp_tbl_cmplx[j].r = ((q15_t)floor(0.5f + NDSV_F2I16_MAX * cos((j) * phase)));
            wp_tbl_cmplx[j].i = ((q15_t)floor(-0.5f - NDSV_F2I16_MAX * sin((j) * phase)));
            /*
            if(wp_tbl_cmplx[j].i <= tmp_cmp)
            {
                wp_tbl_cmplx[j].i = tmp_cmp;
            }
            */
            wp_tbl_cmplx[j].i = ((wp_tbl_cmplx[j].i) > (tmp_cmp)) ? (wp_tbl_cmplx[j].i) : (tmp_cmp);
        }
        phase = 2 * NDSV_PI / nfft;
        twiddles = st->super_twiddles;
        wp_tbl_rfft_r = &twiddles[0].r;
        wp_tbl_rfft_i = &twiddles[ncfft / 2].r;
        //m = st->factors[3] + 1;

        for (j = 0; j < ncfft ; j++)
        {
            //phase = j* factor;
            wp_tbl_rfft_r[j] = ((q15_t)floor(0.5f + NDSV_F2I16_MAX * cos((j) * phase)));
            wp_tbl_rfft_i[j] = ((q15_t)floor(-0.5f - NDSV_F2I16_MAX * sin((j) * phase)));
            /*
            if(wp_tbl_rfft_i[j] < tmp_cmp)
            {
                wp_tbl_rfft_i[j] = tmp_cmp;
            }
            */
            wp_tbl_rfft_i[j] = ((wp_tbl_rfft_i[j]) > (tmp_cmp)) ? (wp_tbl_rfft_i[j]) : (tmp_cmp);
        }
    } // if(st)
    return st;
}

void riscv_vec_rfft_free_q15 (riscv_vec_rfft_cfg_q15_t cfg)
{
    //free (cfg);
    NDSV_FREE(cfg);
}

#endif  //ENA_SOTCLHAM_FFT

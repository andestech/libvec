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
static q31_t riscv_vec_create_factor_int_q31(q31_t n,
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

 //#ifndef NDS
// Twiddles matrix [mstride][radix-1]
// First column (k == 0)is ignored because phase == 1, and
// twiddle = (1.0, 0.0).
static void riscv_vec_create_twiddle_segment_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT twiddles,
        const q31_t mstride,
        const q31_t fstride,
        const q31_t radix,
        const q31_t nfft)
{
    q31_t tmp_i = 0;
    const float64_t pi = NDSV_PI;
    float phase = 2 * pi / nfft;
    // Pure C version using RADIX-2 only
    q31_t cal_fft = nfft / 2;
    riscv_vec_cpx_q31_t *wp_tbl_cmplx = twiddles;
    for(tmp_i = 0 ; tmp_i < cal_fft ; tmp_i++)
    {
        wp_tbl_cmplx[tmp_i].r = ((q31_t)floor(0.5f + NDSV_F2I32_MAX * cos((tmp_i) * phase)));
        wp_tbl_cmplx[tmp_i].i = ((q31_t)floor(-0.5f - NDSV_F2I32_MAX * sin((tmp_i) * phase)));
        /*
        if(wp_tbl_cmplx[tmp_i].i < -2147483646)
        {
                   // this part is used for prevent the case
                   // -(0x80000000) = 0x8000000 (desired: 0x7fffffff)
                     wp_tbl_cmplx[tmp_i].i = -2147483647;
               }
               */
        wp_tbl_cmplx[tmp_i].i = ((wp_tbl_cmplx[tmp_i].i) > (-2147483647L)) ? (wp_tbl_cmplx[tmp_i].i) : (-2147483647L);
    }

}

static riscv_vec_cpx_q31_t* riscv_vec_create_twiddle_q31 (
    riscv_vec_cpx_q31_t * FUNC_RESTRICT twiddles,
    const q31_t * FUNC_RESTRICT factors,
    const q31_t nfft )
{
    q31_t mstride = 1 ;
    //q31_t cur_radix = 1 ;
    q31_t fstride = 1 ;
    q31_t rfft_en = factors[RFFT_EN];
    // keep the same interface, using dummy parameter for twiddle generator, replace
    // the input "radix" to "rfft_en" that will be used to control the format of twiddle.
    riscv_vec_create_twiddle_segment_q31(twiddles, mstride, fstride, rfft_en, nfft);

    return twiddles;
}
/**
 * cfft_q31 cifft_q31 initial and free function
 */
riscv_vec_cfft_cfg_q31_t riscv_vec_cfft_init_q31 (q31_t nfft)
{
    riscv_vec_cfft_cfg_q31_t st = NULL;
    if( (((nfft) & (nfft - 1)) != 0) || (nfft < 8) || (nfft > 16384L) )
    {
        // only support size with power of 2 and 8 ~ 16384
        return NULL;
    }
    int32_t twdl_size = nfft / 2;


    u32_t memneeded = sizeof (riscv_vec_cfft_state_q31_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)       /* factors */
                      + sizeof (riscv_vec_cpx_q31_t) * (twdl_size)     /* twiddles */
                      + sizeof (riscv_vec_cpx_q31_t) * (nfft)          /* buffer */
                      + NDSV_ALIGNMENT;             /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif

    st = (riscv_vec_cfft_cfg_q31_t) NDSV_MALLOC (memneeded);

    if(st != NULL)
    {
        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_cfft_state_q31_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors = (q31_t*) address;
        st->twiddles = (riscv_vec_cpx_q31_t*) (st->factors + (NDSV_STOCKHAM_FACTOR_MAX));
        st->buffer = st->twiddles + twdl_size;
        st->nfft = nfft;

        riscv_vec_create_factor_int_q31(nfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        st->factors[RFFT_EN] = 0 ; // 1:rfft / 0: cfft
        riscv_vec_create_twiddle_q31 (st->twiddles, st->factors, nfft);
    }
    return st;
}

void riscv_vec_cfft_free_q31 (riscv_vec_cfft_cfg_q31_t cfg)
{
    //free (cfg);
    NDSV_FREE(cfg);
}

/**
 * rfft_q31 rifft_q31 initial and free function
 */
riscv_vec_rfft_cfg_q31_t riscv_vec_rfft_init_q31 (q31_t nfft)
{

    riscv_vec_rfft_cfg_q31_t st = NULL;
    riscv_vec_cpx_q31_t* twiddles = NULL;
    q31_t *wp_tbl_rfft_r = NULL, *wp_tbl_rfft_i = NULL;
    //q31_t m = 0, tmp_size = nfft;
    int32_t ncfft = nfft / 2;
    if( (((nfft) & (nfft - 1)) != 0) || (nfft < 8) || (nfft > 16384L) )
    {
        // only support size with power of 2 and 8 ~ 16384
        return NULL;
    }

    int32_t twdl_size = ncfft / 2;
    float factor = 2 * NDSV_PI / nfft;
    int32_t j = 0 ;

    u32_t memneeded = sizeof (riscv_vec_rfft_state_q31_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)        /* factors */
                      + sizeof (riscv_vec_cpx_q31_t) * (twdl_size)    /* twiddle */
                      + sizeof (riscv_vec_cpx_q31_t) * (ncfft)        /* super twiddles */
                      + sizeof (riscv_vec_cpx_q31_t) * (nfft)         /* buffer */
                      + NDSV_ALIGNMENT;                    /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif

    st = (riscv_vec_rfft_cfg_q31_t) NDSV_MALLOC (memneeded);

    if (st != NULL)
    {
        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_rfft_state_q31_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors = (q31_t*) address;
        st->twiddles = (riscv_vec_cpx_q31_t*) (st->factors + (NDSV_STOCKHAM_FACTOR_MAX));
        st->super_twiddles = st->twiddles + twdl_size;
        st->buffer = st->super_twiddles + (ncfft);
        st->ncfft = ncfft;
        riscv_vec_create_factor_int_q31 (ncfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        st->factors[RFFT_EN] = 1 ; // 1:rfft / 0: cfft
        riscv_vec_create_twiddle_q31 (st->twiddles, st->factors, ncfft);
        twiddles = st->super_twiddles;
        wp_tbl_rfft_r = &twiddles[0].r;
        wp_tbl_rfft_i = &twiddles[ncfft / 2].r;
        //m = st->factors[3] + 1;

        for (j = 0; j < ncfft ; j++)
        {
            //phase = j* factor;
            wp_tbl_rfft_r[j] = ((q31_t)floor(0.5f + NDSV_F2I32_MAX * cos((j) * factor)));
            wp_tbl_rfft_i[j] = ((q31_t)floor(-0.5f - NDSV_F2I32_MAX * sin((j) * factor)));
            /*
            if(wp_tbl_rfft_i[j] <= -2147483647)
            {
                wp_tbl_rfft_i[j] = -2147483647;
            }
            */
            wp_tbl_rfft_i[j] = ((wp_tbl_rfft_i[j]) > (-2147483647L)) ? (wp_tbl_rfft_i[j]) : (-2147483647L);
        }
    }
    return st;

}

void riscv_vec_rfft_free_q31 (riscv_vec_rfft_cfg_q31_t cfg)
{
    //free (cfg);
    NDSV_FREE(cfg);
}

#endif  //ENA_SOTCLHAM_FFT

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

extern q31_t riscv_vec_create_factor (
    q31_t n,
    q31_t * FUNC_RESTRICT facbuf,
    q31_t vec_factor_flags);

typedef void (*segment_generator_f32)(riscv_vec_cpx_f32_t*,
                                      const q31_t,
                                      const q31_t,
                                      const q31_t,
                                      const q31_t);

// Inline the code the coverage will fail, this code does not count in benchmarks, so doesn't matter
__attribute__ ((noinline)) riscv_vec_cpx_f32_t* riscv_vec_create_twiddle_instance_f32 (
    segment_generator_f32 generator,
    riscv_vec_cpx_f32_t * FUNC_RESTRICT twiddles,
    const q31_t * FUNC_RESTRICT factors,
    const q31_t nfft)
{
    q31_t mstride = 1 ;
    q31_t cur_radix = 1 ;
    q31_t fstride = 1 ;
    // keep the same interface, using dummy parameter for twiddle generator.
    generator (twiddles, mstride, fstride, cur_radix, nfft);

    return twiddles;
}

// Twiddles matrix [radix-1][mstride]
// First column (k == 0) is ignored because phase == 1, and
// twiddle = (1.0, 0.0).
static void riscv_vec_create_twiddle_segment_f32 (riscv_vec_cpx_f32_t * FUNC_RESTRICT twiddles,
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
    riscv_vec_cpx_f32_t *wp_tbl_cmplx = twiddles;
    for(tmp_i = 0 ; tmp_i < cal_fft ; tmp_i++)
    {
        wp_tbl_cmplx[tmp_i].r = cosf((tmp_i) * phase);
        wp_tbl_cmplx[tmp_i].i = -sinf((tmp_i) * phase);
    }
}

static riscv_vec_cpx_f32_t* riscv_vec_create_twiddle_f32 (riscv_vec_cpx_f32_t * FUNC_RESTRICT twiddles,
        const q31_t * FUNC_RESTRICT factors,
        const q31_t nfft )
{
    segment_generator_f32 generator = riscv_vec_create_twiddle_segment_f32;
    twiddles = riscv_vec_create_twiddle_instance_f32(generator,
               twiddles, factors, nfft);
    return twiddles;
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

/**
 * cfft_f32 cifft_f32 initial and free function
 */
riscv_vec_cfft_cfg_f32_t riscv_vec_cfft_init_f32 (q31_t nfft)
{
    riscv_vec_cfft_cfg_f32_t st = NULL;
    if( (((nfft) & (nfft - 1)) != 0) || (nfft < 8) || (nfft > 16384L) )
    {
        // only support the size of power of 2 and 8 ~ 16384
        return NULL;
    }
    int32_t twdl_size = nfft / 2;

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

void riscv_vec_cfft_free_f32 (riscv_vec_cfft_cfg_f32_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}

/**
 * cfft2d_f32 cifft2d_f32 initial and free function
 */
riscv_vec_cfft2d_cfg_f32_t riscv_vec_cfft2d_init_f32 (riscv_vec_fft2d_size dim)
{
    q31_t m = dim.m;
    q31_t n = dim.n;

    riscv_vec_cfft2d_cfg_f32_t st = NULL;
    q31_t max_1d = 0;

    if( (((m) & (m - 1)) != 0) || (m < 8) || (m > 16384L) )
    {
        // only support the size of power of 2 and 8 ~ 16384
        return NULL;
    }

    if( (((n) & (n - 1)) != 0) || (n < 8) || (n > 16384L) )
    {
        // only suppor the size with power of 2 and 8 ~ 16384
        return NULL;
    }
    int32_t twdl_size_m = m / 2;
    int32_t twdl_size_n = n / 2;
    max_1d = (m >= n ? m : n);
    u32_t memneeded = sizeof (riscv_vec_cfft2d_state_f32_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)       /* factors_m */
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)       /* factors_n */
                      + sizeof (riscv_vec_cpx_f32_t) * twdl_size_m        /* twiddles_m */
                      + sizeof (riscv_vec_cpx_f32_t) * twdl_size_n        /* twiddles_n */
                      + sizeof (riscv_vec_cpx_f32_t) * (max_1d)            /* buffer */
                      + sizeof (riscv_vec_cpx_f32_t) * (m * n)            /* for transpose using */
                      + NDSV_ALIGNMENT;             /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif

    st = (riscv_vec_cfft2d_cfg_f32_t) NDSV_MALLOC (memneeded);

    if(st != NULL)
    {
        // Only backward FFT is scaled by default.
        st->is_forward_scaled = 0;
        st->is_backward_scaled = 1;

        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_cfft2d_state_f32_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors_m = (q31_t*) address;
        st->factors_n = (q31_t*) (st->factors_m + NDSV_STOCKHAM_FACTOR_MAX);
        st->twiddles_m = (riscv_vec_cpx_f32_t*) (st->factors_n + NDSV_STOCKHAM_FACTOR_MAX);
        st->twiddles_n = (riscv_vec_cpx_f32_t*) (st->twiddles_m + twdl_size_m );
        st->buffer_1d = (riscv_vec_cpx_f32_t*)  (st->twiddles_n +  twdl_size_n);
        st->buffer_2d = (riscv_vec_cpx_f32_t*)  (st->buffer_1d +  max_1d);
        st->m = m;
        st->n = n;
        riscv_vec_create_factor(m, st->factors_m, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        riscv_vec_create_factor(n, st->factors_n, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        st->factors_m[RFFT_EN] = 0 ; // 1:rfft / 0: cfft
        st->factors_n[RFFT_EN] = 0 ; // 1:rfft / 0: cfft
        st->factors_m[IS_RADIX_8] = 0;
        st->factors_n[IS_RADIX_8] = 0;
        riscv_vec_create_twiddle_f32 (st->twiddles_m, st->factors_m, m);
        riscv_vec_create_twiddle_f32 (st->twiddles_n, st->factors_n, n);
    }
    return st;
}

void riscv_vec_cfft2d_free_f32 (riscv_vec_cfft2d_cfg_f32_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}

/**
 * rfft_f32 rifft_f32 initial and free function
 */
riscv_vec_rfft_cfg_f32_t riscv_vec_rfft_init_f32 (q31_t nfft)
{
    riscv_vec_rfft_cfg_f32_t st = NULL;
    riscv_vec_cpx_f32_t* twiddles = NULL;
    float32_t *wp_tbl_rfft_r = NULL, *wp_tbl_rfft_i = NULL;
    int32_t ncfft = nfft / 2;
    if( (((nfft) & (nfft - 1)) != 0) || (nfft < 8) || (nfft > 16384L) )
    {
        // only support the size of power of 2 and 8 ~ 16384
        return NULL;
    }
    int32_t twdl_size = ncfft / 2;
    float factor = 2 * NDSV_PI / nfft;
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
        wp_tbl_rfft_i = &twiddles[ncfft / 2].r;
        for (j = 0; j < ncfft ; j++)
        {
            //phase = j* factor;
            wp_tbl_rfft_r[j] =  cosf (j * factor);
            wp_tbl_rfft_i[j] =  -sinf (j * factor);

        }
    }
    return st;
}

void riscv_vec_rfft_free_f32 (riscv_vec_rfft_cfg_f32_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}

/**
 * rfft2d_f32 rifft2d_f32 initial and free function
 */
riscv_vec_rfft2d_cfg_f32_t riscv_vec_rfft2d_init_f32 (riscv_vec_fft2d_size dim)
{
    /*
     * m * n input => take m times, n-point rfft first (due to the row-major memory layout)
     * and then take (n/2 + 1) times m-point cfft.
    */
    q31_t m = dim.m;
    q31_t n = dim.n;
    int32_t ncfft = n / 2;
    riscv_vec_rfft2d_cfg_f32_t st = NULL;
    q31_t max_1d = 0;

    if( (((m) & (m - 1)) != 0) || (m < 8) || (m > 16384L) )
    {
        // only support the size of power of 2 and 8 ~ 16384
        return NULL;
    }
    if( (((n) & (n - 1)) != 0) || (n < 8) || (n > 16384L) )
    {
        // only suppor the size with power of 2 and 8 ~ 16384
        return NULL;
    }
    int32_t twdl_size_m = m / 2;
    int32_t twdl_size_n = ncfft / 2;
    max_1d = (m >= (ncfft + 1) ? m : (ncfft + 1));

    u32_t memneeded = sizeof (riscv_vec_rfft2d_state_f32_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)     /* factors_m */
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)     /* factors_n */
                      + sizeof (riscv_vec_cpx_f32_t) * (twdl_size_m)    /* twiddles_m */
                      + sizeof (riscv_vec_cpx_f32_t) * (twdl_size_n)    /* twiddles_n */
                      + sizeof (riscv_vec_cpx_f32_t) * (ncfft)          /* super twiddles */
                      + sizeof (riscv_vec_cpx_f32_t) * (max_1d)         /* buffer */
                      + sizeof (riscv_vec_cpx_f32_t) * (max_1d)              /* for 1d-rifft tmp input using */
                      + sizeof (riscv_vec_cpx_f32_t) * (ncfft)              /* for 1d-rifft tmp using */
                      + sizeof (riscv_vec_cpx_f32_t) * (m * (ncfft + 1)) /* for transpose/tmp output using */
                      + NDSV_ALIGNMENT;                    /* 64-bit alignment */

    st = (riscv_vec_rfft2d_cfg_f32_t) NDSV_MALLOC (memneeded);

    if( st != NULL)
    {
        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_rfft2d_state_f32_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors_m = (q31_t*) address;
        st->factors_n = (q31_t*) (st->factors_m + NDSV_STOCKHAM_FACTOR_MAX);
        st->twiddles_m = (riscv_vec_cpx_f32_t*) (st->factors_n + NDSV_STOCKHAM_FACTOR_MAX);
        st->twiddles_n = (riscv_vec_cpx_f32_t*) (st->twiddles_m + twdl_size_m );
        st->super_twiddles = (riscv_vec_cpx_f32_t*)  (st->twiddles_n +  twdl_size_n);
        st->buffer_1d = (riscv_vec_cpx_f32_t*)  (st->super_twiddles +  ncfft);
        st->tmp_1d_input = (riscv_vec_cpx_f32_t*)  (st->buffer_1d +  max_1d);
        st->tmp_split = (riscv_vec_cpx_f32_t*)  (st->tmp_1d_input +  max_1d);
        st->buffer_2d = (riscv_vec_cpx_f32_t*)  (st->tmp_split +  ncfft);
        st->m = m;
        st->ncfft = ncfft; // ncfft = n / 2;
        riscv_vec_create_factor(m, st->factors_m, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        riscv_vec_create_factor(ncfft, st->factors_n, NDSV_FACTOR_EIGHT_FIRST_STAGE);

        st->factors_m[RFFT_EN] = 0 ; // 1:rfft / 0: cfft
        st->factors_n[RFFT_EN] = 1 ; // 1:rfft / 0: cfft
        st->factors_m[IS_RADIX_8] = 0;
        st->factors_n[IS_RADIX_8] = 0;
        riscv_vec_create_twiddle_f32 (st->twiddles_m, st->factors_m, m);
        riscv_vec_create_twiddle_f32 (st->twiddles_n, st->factors_n, ncfft);

        riscv_vec_cpx_f32_t* twiddles = NULL;
        float32_t *wp_tbl_rfft_r = NULL, *wp_tbl_rfft_i = NULL;
        float factor = 2 * NDSV_PI / n;
        int32_t j = 0 ;

        twiddles = st->super_twiddles;
        wp_tbl_rfft_r = &twiddles[0].r;
        wp_tbl_rfft_i = &twiddles[ncfft / 2].r;
        for (j = 0; j < ncfft ; j++)
        {
            //phase = j* factor;
            wp_tbl_rfft_r[j] =  cos (j * factor);
            wp_tbl_rfft_i[j] =  -sin (j * factor);
        }
    }
    return st;

}

void riscv_vec_rfft2d_free_f32 (riscv_vec_rfft2d_cfg_f32_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}

#endif  //ENA_SOTCLHAM_FFT

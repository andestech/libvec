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
#define NDS_SUPPORT
q31_t riscv_vec_create_factor (q31_t n,
                               q31_t * FUNC_RESTRICT facbuf,
                               q31_t vec_factor_flags)
{
#ifdef NDS_SUPPORT
    q31_t p = 2;
    q31_t stage_num = 0;
    q31_t stride_max = n;
    if( n > 1)
    {
        do
        {
            n /= p;
            stage_num++;
        }
        while (n > 1);
    }
    facbuf[OUT_INDEX] = stage_num;
    facbuf[SIZE_FIRST_STAGE] = stride_max / p;
#else
#endif
    // facbuf[2 * i] = alg_flag;
    return NDSV_OK;
}

// this is the create factor function for bf16 pure c
q31_t riscv_vec_create_factor_bf16_purec(q31_t n, q31_t * FUNC_RESTRICT facbuf, q31_t vec_factor_flags)
{
    q31_t p = 2;
    q31_t stage_num = 0;
    q31_t stride_max = n;
    if( n > 1)
    {
        do
        {
            n /= p;
            stage_num++;
        }
        while (n > 1);
    }
    facbuf[OUT_INDEX] = stage_num;
    facbuf[SIZE_FIRST_STAGE] = stride_max / p;
    // facbuf[2 * i] = alg_flag;
    return NDSV_OK;
}

// this is a temp function for radix8 create factor, now only for float type support the
// algorithm
#ifdef STOCKHAM_FFT_MAIN_RADIX8
#endif //STOCKHAM_FFT_MAIN_RADIX8


q31_t riscv_vec_create_factor_f64 (q31_t n,
                                   q31_t * FUNC_RESTRICT facbuf,
                                   q31_t vec_factor_flags)
{
#ifdef NDS_SUPPORT
    q31_t p = 2;
    q31_t stage_num = 0;
    q31_t stride_max = n;
    if( n > 1)
    {
        do
        {
            n /= p;
            stage_num++;
        }
        while (n > 1);
    }
    facbuf[OUT_INDEX] = stage_num;
    facbuf[SIZE_FIRST_STAGE] = stride_max / p;
#else
#endif
    // facbuf[2 * i] = alg_flag;
    return NDSV_OK;
}





q31_t riscv_vec_create_factor_int (q31_t n,
                                   q31_t * facbuf,
                                   q31_t vec_factor_flags)
{
#ifdef ENA_STOCKHAM_FFT
#ifdef NDS_SUPPORT
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
#endif
    // facbuf[2 * i] = alg_flag;
#endif  //ENA_STOCKHAM_FFT
    return NDSV_OK;
}


q31_t riscv_vec_create_factor_int_q31(q31_t n,
                                      q31_t * facbuf,
                                      q31_t vec_factor_flags)
{
#ifdef ENA_STOCKHAM_FFT
#ifdef NDS_SUPPORT
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
#endif
    // facbuf[2 * i] = alg_flag;
#endif  //ENA_STOCKHAM_FFT
    return NDSV_OK;
}

// Twiddles matrix [radix-1][mstride]
// First column (k == 0) is ignored because phase == 1, and
// twiddle = (1.0, 0.0).
void riscv_vec_create_twiddle_segment_f32 (riscv_vec_cpx_f32_t * FUNC_RESTRICT twiddles,
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

void riscv_vec_create_twiddle_segment_f64 (riscv_vec_cpx_f64_t * FUNC_RESTRICT twiddles,
        const q31_t mstride,
        const q31_t fstride,
        const q31_t radix,
        const q31_t nfft)
{
    q31_t tmp_i = 0;
    const float64_t pi = (float64_t)(3.1415926535897932384626433832795);
    double phase = 2 * pi / (double)nfft;
    // Pure C version using RADIX-2 only
    q31_t cal_fft = nfft / 2;
    riscv_vec_cpx_f64_t *wp_tbl_cmplx = twiddles;
    for(tmp_i = 0 ; tmp_i < cal_fft ; tmp_i++)
    {
        wp_tbl_cmplx[tmp_i].r = cos((tmp_i) * phase);
        wp_tbl_cmplx[tmp_i].i = -sin((tmp_i) * phase);
    }
}


#if defined (__riscv_zfh)
void riscv_vec_create_twiddle_segment_f16 (riscv_vec_cpx_f16_t * FUNC_RESTRICT twiddles,
        const q31_t mstride,
        const q31_t fstride,
        const q31_t radix,
        const q31_t nfft)
{
    q31_t tmp_i = 0;
    const float64_t pi = NDSV_PI;
    //float phase = 2*pi/nfft;
    double phase = 2 * pi / nfft;
    // Pure C version using RADIX-2 only
    q31_t cal_fft = nfft / 2;
    riscv_vec_cpx_f16_t *wp_tbl_cmplx = twiddles;
    for(tmp_i = 0 ; tmp_i < cal_fft ; tmp_i++)
    {
        wp_tbl_cmplx[tmp_i].r = (float16_t) cos((tmp_i) * phase);
        wp_tbl_cmplx[tmp_i].i = (float16_t) - sin((tmp_i) * phase);
    }
}
#endif

#if defined (__riscv_zfbfmin)
void riscv_vec_create_twiddle_segment_bf16 (riscv_vec_cpx_bf16_t * FUNC_RESTRICT twiddles,
        const q31_t mstride,
        const q31_t fstride,
        const q31_t radix,
        const q31_t nfft)
{
    q31_t tmp_i = 0;
    const float64_t pi = NDSV_PI;
    //float phase = 2*pi/nfft;
    double phase = 2 * pi / nfft;
    // Pure C version using RADIX-2 only
    q31_t cal_fft = nfft / 2;
    riscv_vec_cpx_bf16_t *wp_tbl_cmplx = twiddles;
    for(tmp_i = 0 ; tmp_i < cal_fft ; tmp_i++)
    {
        wp_tbl_cmplx[tmp_i].r = (bf16_t) cos((tmp_i) * phase);
        wp_tbl_cmplx[tmp_i].i = (bf16_t) - sin((tmp_i) * phase);
    }
}
#endif //defined (__riscv_zfbfmin)



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

riscv_vec_cpx_q31_t* riscv_vec_create_twiddle_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT twiddles,
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
//#endif

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

typedef void (*segment_generator_f64)(riscv_vec_cpx_f64_t*,
                                      const q31_t,
                                      const q31_t,
                                      const q31_t,
                                      const q31_t);

// Inline the code the coverage will fail, this code does not count in benchmarks, so doesn't matter
__attribute__ ((noinline)) riscv_vec_cpx_f64_t* riscv_vec_create_twiddle_instance_f64 (
    segment_generator_f64 generator,
    riscv_vec_cpx_f64_t * FUNC_RESTRICT twiddles,
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

#if defined (__riscv_zfh)
typedef void (*segment_generator_f16)(riscv_vec_cpx_f16_t*,
                                      const q31_t,
                                      const q31_t,
                                      const q31_t,
                                      const q31_t);

// Inline the code the coverage will fail, this code does not count in benchmarks, so doesn't matter
__attribute__ ((noinline)) riscv_vec_cpx_f16_t* riscv_vec_create_twiddle_instance_f16 (
    segment_generator_f16 generator,
    riscv_vec_cpx_f16_t * FUNC_RESTRICT twiddles,
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
#endif  //defined (__riscv_zfh)

#if defined (__riscv_zfbfmin)
typedef void (*segment_generator_bf16)(riscv_vec_cpx_bf16_t*,
                                       const q31_t,
                                       const q31_t,
                                       const q31_t,
                                       const q31_t);

// Inline the code the coverage will fail, this code does not count in benchmarks, so doesn't matter
__attribute__ ((noinline)) riscv_vec_cpx_bf16_t* riscv_vec_create_twiddle_instance_bf16 (
    segment_generator_bf16 generator,
    riscv_vec_cpx_bf16_t * FUNC_RESTRICT twiddles,
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
#endif  //defined (__riscv_zfbfmin)

riscv_vec_cpx_f32_t* riscv_vec_create_twiddle_f32 (riscv_vec_cpx_f32_t * FUNC_RESTRICT twiddles,
        const q31_t * FUNC_RESTRICT factors,
        const q31_t nfft )
{
    segment_generator_f32 generator = riscv_vec_create_twiddle_segment_f32;
    twiddles = riscv_vec_create_twiddle_instance_f32(generator,
               twiddles, factors, nfft);
    return twiddles;
}

riscv_vec_cpx_f64_t* riscv_vec_create_twiddle_f64 (riscv_vec_cpx_f64_t * FUNC_RESTRICT twiddles,
        const q31_t * FUNC_RESTRICT factors,
        const q31_t nfft )
{
    segment_generator_f64 generator = riscv_vec_create_twiddle_segment_f64;
    twiddles = riscv_vec_create_twiddle_instance_f64(generator,
               twiddles, factors, nfft);
    return twiddles;
}

#if defined (__riscv_zfh)
riscv_vec_cpx_f16_t* riscv_vec_create_twiddle_f16 (riscv_vec_cpx_f16_t * FUNC_RESTRICT twiddles,
        const q31_t * FUNC_RESTRICT factors,
        const q31_t nfft )
{
    segment_generator_f16 generator = riscv_vec_create_twiddle_segment_f16;
    twiddles = riscv_vec_create_twiddle_instance_f16(generator,
               twiddles, factors, nfft);
    return twiddles;
}
#endif  //defined (__riscv_zfh)

#if defined (__riscv_zfbfmin)
riscv_vec_cpx_bf16_t* riscv_vec_create_twiddle_bf16 (riscv_vec_cpx_bf16_t * FUNC_RESTRICT twiddles,
        const q31_t * FUNC_RESTRICT factors,
        const q31_t nfft )
{
    segment_generator_bf16 generator = riscv_vec_create_twiddle_segment_bf16;
    twiddles = riscv_vec_create_twiddle_instance_bf16(generator,
               twiddles, factors, nfft);
    return twiddles;
}
#endif  //defined (__riscv_zfbfmin)

/**
 * @addtogroup C2C_FFT_IFFT
 * @{
 */

/**
 * @brief Destroys the configuration structure allocated by variants of @ref cfft_f32 (frees memory, etc.)
 *
 * @param[in]   cfg     pointer to the FFT configuration structure
 */
void riscv_vec_cfft_free_f32 (riscv_vec_cfft_cfg_f32_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}

void riscv_vec_cfft_free_f64 (riscv_vec_cfft_cfg_f64_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}

#if defined (__riscv_zfh)
void riscv_vec_cfft_free_f16 (riscv_vec_cfft_cfg_f16_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}
#endif

#if defined (__riscv_zfbfmin)
void riscv_vec_cfft_free_bf16 (riscv_vec_cfft_cfg_bf16_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}
#endif

void riscv_vec_cfft2d_free_f32 (riscv_vec_cfft2d_cfg_f32_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}

void riscv_vec_cfft2d_free_f64 (riscv_vec_cfft2d_cfg_f64_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}

#if defined (__riscv_zfh)
void riscv_vec_cfft2d_free_f16 (riscv_vec_cfft2d_cfg_f16_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}
#endif

#if defined (__riscv_zfbfmin)
void riscv_vec_cfft2d_free_bf16 (riscv_vec_cfft2d_cfg_bf16_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}
#endif

/**
 * @brief Destroys the configuration structure allocated by variants of @ref cfft_q31 (frees memory, etc.)
 *
 * @param[in]   cfg     pointer to the FFT configuration structure
 */
void riscv_vec_cfft_free_q31 (riscv_vec_cfft_cfg_q31_t cfg)
{
    //free (cfg);
    NDSV_FREE(cfg);
}

/**
 * @brief Destroys the configuration structure allocated by @ref cfft_q15 (frees memory, etc.)
 *
 * @param[in]   cfg     pointer to the FFT configuration structure
 */
void riscv_vec_cfft_free_q15 (riscv_vec_cfft_cfg_q15_t cfg)
{
    //free (cfg);
    NDSV_FREE(cfg);
}

/** @} */ // C2C_FFT_IFFT

/**
 * @addtogroup R2C_FFT_IFFT
 * @{
 */

/**
 * @brief Destroys the configuration structure allocated by @ref rfft_f32 (frees memory, etc.)
 *
 * @param[in]   cfg     pointer to the FFT configuration structure
 */
void riscv_vec_rfft_free_f32 (riscv_vec_rfft_cfg_f32_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}

void riscv_vec_rfft_free_f64 (riscv_vec_rfft_cfg_f64_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}

#if defined (__riscv_zfh)
void riscv_vec_rfft_free_f16 (riscv_vec_rfft_cfg_f16_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}
#endif

#if defined (__riscv_zfbfmin)
void riscv_vec_rfft_free_bf16 (riscv_vec_rfft_cfg_bf16_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}
#endif

// #ifndef NDS
/**
 * @brief Destroys the configuration structure allocated by @ref rfft_q31 (frees memory, etc.)
 *
 * @param[in]   cfg     pointer to the FFT configuration structure
 */
void riscv_vec_rfft_free_q31 (riscv_vec_rfft_cfg_q31_t cfg)
{
    //free (cfg);
    NDSV_FREE(cfg);
}

/**
 * @brief Destroys the configuration structure allocated by @ref rfft_q15 (frees memory, etc.)
 *
 * @param[in]   cfg     pointer to the FFT configuration structure
 */
void riscv_vec_rfft_free_q15 (riscv_vec_rfft_cfg_q15_t cfg)
{
    //free (cfg);
    NDSV_FREE(cfg);
}

void riscv_vec_rfft2d_free_f32 (riscv_vec_rfft2d_cfg_f32_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}

void riscv_vec_rfft2d_free_f64 (riscv_vec_rfft2d_cfg_f64_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}

#if defined (__riscv_zfh)
void riscv_vec_rfft2d_free_f16 (riscv_vec_rfft2d_cfg_f16_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}
#endif

#if defined (__riscv_zfbfmin)
void riscv_vec_rfft2d_free_bf16 (riscv_vec_rfft2d_cfg_bf16_t cfg)
{
    //free(cfg);
    NDSV_FREE(cfg);
}
#endif

// fft init function
#if defined (__riscv_zfh)
riscv_vec_cfft_cfg_f16_t riscv_vec_cfft_init_f16 (q31_t nfft)
{
    riscv_vec_cfft_cfg_f16_t st = NULL;
    if( (((nfft) & (nfft - 1)) != 0) || (nfft < 8) || (nfft > 16384L) )
    {
        // only support the size of power of 2 and 8 ~ 16384
        return NULL;
    }
    int32_t twdl_size = nfft / 2;

    u32_t memneeded = sizeof (riscv_vec_cfft_state_f16_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)       /* factors */
                      + sizeof (riscv_vec_cpx_f16_t) * twdl_size     /* twiddles */
                      + sizeof (riscv_vec_cpx_f16_t) * nfft          /* buffer */
                      + NDSV_ALIGNMENT;             /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif

    st = (riscv_vec_cfft_cfg_f16_t) NDSV_MALLOC (memneeded);

    if(st != NULL)
    {
        // Only backward FFT is scaled by default.
        st->is_forward_scaled = 0;
        st->is_backward_scaled = 1;

        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_cfft_state_f16_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors = (q31_t*) address;
        st->twiddles = (riscv_vec_cpx_f16_t*) (st->factors + (NDSV_STOCKHAM_FACTOR_MAX));
        st->buffer = st->twiddles + twdl_size;
        st->nfft = nfft;
        riscv_vec_create_factor(nfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        st->factors[RFFT_EN] = 0 ; // 1:rfft / 0: cfft
        riscv_vec_create_twiddle_f16 (st->twiddles, st->factors, nfft);
    }
    return st;
}

riscv_vec_rfft_cfg_f16_t riscv_vec_rfft_init_f16 (q31_t nfft)
{
    riscv_vec_rfft_cfg_f16_t st = NULL;
    riscv_vec_cpx_f16_t* twiddles = NULL;
    float16_t *wp_tbl_rfft_r = NULL, *wp_tbl_rfft_i = NULL;
    int32_t ncfft = nfft / 2;
    if( (((nfft) & (nfft - 1)) != 0) || (nfft < 8) || (nfft > 16384L) )
    {
        // only support the size of power of 2 and 8 ~ 16384
        return NULL;
    }
    int32_t twdl_size = ncfft / 2;
    float factor = 2 * NDSV_PI / nfft;
    int32_t j = 0 ;

    u32_t memneeded = sizeof (riscv_vec_rfft_state_f16_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)        /* factors */
                      + sizeof (riscv_vec_cpx_f16_t) * (twdl_size)    /* twiddle */
                      + sizeof (riscv_vec_cpx_f16_t) * (ncfft)        /* super twiddles */
                      + sizeof (riscv_vec_cpx_f16_t) * (nfft)         /* buffer */
                      + NDSV_ALIGNMENT;                    /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif

    st = (riscv_vec_rfft_cfg_f16_t) NDSV_MALLOC (memneeded);

    if (st != NULL)
    {
        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_rfft_state_f16_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors = (q31_t*) address;
        st->twiddles = (riscv_vec_cpx_f16_t*) (st->factors + (NDSV_STOCKHAM_FACTOR_MAX));
        st->super_twiddles = st->twiddles + twdl_size;
        st->buffer = st->super_twiddles + (ncfft);
        st->ncfft = ncfft;
        riscv_vec_create_factor (ncfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        st->factors[RFFT_EN] = 1 ; // 1:rfft / 0: cfft
        riscv_vec_create_twiddle_f16 (st->twiddles, st->factors, ncfft);
        twiddles = st->super_twiddles;
        wp_tbl_rfft_r = &twiddles[0].r;
        wp_tbl_rfft_i = &twiddles[ncfft / 2].r;
        for (j = 0; j < ncfft ; j++)
        {
            //phase = j* factor;
            wp_tbl_rfft_r[j] = (float16_t) cosf (j * factor);
            wp_tbl_rfft_i[j] = (float16_t) - sinf (j * factor);

        }
    }
    return st;
}
#endif // defined (__riscv_zfh)

#if defined (__riscv_zfbfmin)
riscv_vec_cfft_cfg_bf16_t riscv_vec_cfft_init_bf16 (q31_t nfft)
{
    riscv_vec_cfft_cfg_bf16_t st = NULL;
    if( (((nfft) & (nfft - 1)) != 0) || (nfft < 8) || (nfft > 16384L) )
    {
        // only support the size of power of 2 and 8 ~ 16384
        return NULL;
    }
    int32_t twdl_size = nfft / 2;

    u32_t memneeded = sizeof (riscv_vec_cfft_state_bf16_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)       /* factors */
                      + sizeof (riscv_vec_cpx_bf16_t) * twdl_size     /* twiddles */
                      + sizeof (riscv_vec_cpx_bf16_t) * nfft          /* buffer */
                      + NDSV_ALIGNMENT;             /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif

    st = (riscv_vec_cfft_cfg_bf16_t) NDSV_MALLOC (memneeded);

    if(st != NULL)
    {
        // Only backward FFT is scaled by default.
        st->is_forward_scaled = 0;
        st->is_backward_scaled = 1;

        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_cfft_state_bf16_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors = (q31_t*) address;
        st->twiddles = (riscv_vec_cpx_bf16_t*) (st->factors + (NDSV_STOCKHAM_FACTOR_MAX));
        st->buffer = st->twiddles + twdl_size;
        st->nfft = nfft;
        riscv_vec_create_factor_bf16_purec(nfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        st->factors[RFFT_EN] = 0 ; // 1:rfft / 0: cfft
        riscv_vec_create_twiddle_bf16 (st->twiddles, st->factors, nfft);
    }
    return st;
}

riscv_vec_rfft_cfg_bf16_t riscv_vec_rfft_init_bf16 (q31_t nfft)
{
    riscv_vec_rfft_cfg_bf16_t st = NULL;
    riscv_vec_cpx_bf16_t* twiddles = NULL;
    bf16_t *wp_tbl_rfft_r = NULL, *wp_tbl_rfft_i = NULL;
    int32_t ncfft = nfft / 2;
    if( (((nfft) & (nfft - 1)) != 0) || (nfft < 8) || (nfft > 16384L) )
    {
        // only support the size of power of 2 and 8 ~ 16384
        return NULL;
    }
    int32_t twdl_size = ncfft / 2;
    float factor = 2 * NDSV_PI / nfft;
    int32_t j = 0 ;

    u32_t memneeded = sizeof (riscv_vec_rfft_state_bf16_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)        /* factors */
                      + sizeof (riscv_vec_cpx_bf16_t) * (twdl_size)    /* twiddle */
                      + sizeof (riscv_vec_cpx_bf16_t) * (ncfft)        /* super twiddles */
                      + sizeof (riscv_vec_cpx_bf16_t) * (nfft)         /* buffer */
                      + NDSV_ALIGNMENT;                    /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif

    st = (riscv_vec_rfft_cfg_bf16_t) NDSV_MALLOC (memneeded);

    if (st != NULL)
    {
        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_rfft_state_bf16_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors = (q31_t*) address;
        st->twiddles = (riscv_vec_cpx_bf16_t*) (st->factors + (NDSV_STOCKHAM_FACTOR_MAX));
        st->super_twiddles = st->twiddles + twdl_size;
        st->buffer = st->super_twiddles + (ncfft);
        st->ncfft = ncfft;
        riscv_vec_create_factor_bf16_purec(ncfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        st->factors[RFFT_EN] = 1 ; // 1:rfft / 0: cfft
        riscv_vec_create_twiddle_bf16 (st->twiddles, st->factors, ncfft);
        twiddles = st->super_twiddles;
        wp_tbl_rfft_r = &twiddles[0].r;
        wp_tbl_rfft_i = &twiddles[ncfft / 2].r;
        for (j = 0; j < ncfft ; j++)
        {
            //phase = j* factor;
            wp_tbl_rfft_r[j] = (bf16_t) cos (j * factor);
            wp_tbl_rfft_i[j] = (bf16_t) - sin (j * factor);

        }
    }
    return st;
}

#endif //defined(__riscv_zfbfmin)


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

riscv_vec_cfft_cfg_f64_t riscv_vec_cfft_init_f64 (q31_t nfft)
{
    riscv_vec_cfft_cfg_f64_t st = NULL;
    if( (((nfft) & (nfft - 1)) != 0) || (nfft < 8) || (nfft > 16384L) )
    {
        // only support the size of power of 2 and 8 ~ 16384
        return NULL;
    }
    int32_t twdl_size = nfft / 2;

    u32_t memneeded = sizeof (riscv_vec_cfft_state_f64_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)       /* factors */
                      + sizeof (riscv_vec_cpx_f64_t) * twdl_size     /* twiddles */
                      + sizeof (riscv_vec_cpx_f64_t) * nfft          /* buffer */
                      + NDSV_ALIGNMENT;             /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif

    st = (riscv_vec_cfft_cfg_f64_t) NDSV_MALLOC (memneeded);
    if(st != NULL)
    {
        // Only backward FFT is scaled by default.
        st->is_forward_scaled = 0;
        st->is_backward_scaled = 1;

        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_cfft_state_f64_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors = (q31_t*) address;
        st->twiddles = (riscv_vec_cpx_f64_t*) (st->factors + (NDSV_STOCKHAM_FACTOR_MAX));
        st->buffer = st->twiddles + twdl_size;
        st->nfft = nfft;
        riscv_vec_create_factor_f64(nfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        st->factors[RFFT_EN] = 0 ; // 1:rfft / 0: cfft
        riscv_vec_create_twiddle_f64 (st->twiddles, st->factors, nfft);
    }
    return st;
}

riscv_vec_rfft_cfg_f64_t riscv_vec_rfft_init_f64 (q31_t nfft)
{
    riscv_vec_rfft_cfg_f64_t st = NULL;
    riscv_vec_cpx_f64_t* twiddles = NULL;
    float64_t *wp_tbl_rfft_r = NULL, *wp_tbl_rfft_i = NULL;
    int32_t ncfft = nfft / 2;
    if( (((nfft) & (nfft - 1)) != 0) || (nfft < 8) || (nfft > 16384L) )
    {
        // only support the size of power of 2 and 8 ~ 16384
        return NULL;
    }
    int32_t twdl_size = ncfft / 2;
    double CONST_PI =  (double)(3.1415926535897932384626433832795);
    double factor = 2 * CONST_PI / nfft;
    int32_t j = 0 ;

    u32_t memneeded = sizeof (riscv_vec_rfft_state_f64_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)        /* factors */
                      + sizeof (riscv_vec_cpx_f64_t) * (twdl_size)    /* twiddle */
                      + sizeof (riscv_vec_cpx_f64_t) * (ncfft)        /* super twiddles */
                      + sizeof (riscv_vec_cpx_f64_t) * (nfft)         /* buffer */
                      + NDSV_ALIGNMENT;                    /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif

    st = (riscv_vec_rfft_cfg_f64_t) NDSV_MALLOC (memneeded);

    if (st != NULL)
    {
        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_rfft_state_f64_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors = (q31_t*) address;
        st->twiddles = (riscv_vec_cpx_f64_t*) (st->factors + (NDSV_STOCKHAM_FACTOR_MAX));
        st->super_twiddles = st->twiddles + twdl_size;
        st->buffer = st->super_twiddles + (ncfft);
        st->ncfft = ncfft;
        riscv_vec_create_factor_f64 (ncfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        st->factors[RFFT_EN] = 1 ; // 1:rfft / 0: cfft
        riscv_vec_create_twiddle_f64 (st->twiddles, st->factors, ncfft);
        twiddles = st->super_twiddles;
        wp_tbl_rfft_r = &twiddles[0].r;
        wp_tbl_rfft_i = &twiddles[ncfft / 2].r;
        for (j = 0; j < ncfft ; j++)
        {
            //phase = j* factor;
            wp_tbl_rfft_r[j] =  cos (j * factor);
            wp_tbl_rfft_i[j] =  sin (-j * factor);

        }
    }
    return st;
}

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

        riscv_vec_create_factor_int(nfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
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
        riscv_vec_create_factor_int (ncfft, st->factors, NDSV_FACTOR_EIGHT_FIRST_STAGE);
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

riscv_vec_cfft2d_cfg_f32_t riscv_vec_cfft2d_init_f32 (riscv_vec_fft2d_size fft2_size)
{
    q31_t m = fft2_size.m;
    q31_t n = fft2_size.n;

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

riscv_vec_cfft2d_cfg_f64_t riscv_vec_cfft2d_init_f64 (riscv_vec_fft2d_size fft2_size)
{
    q31_t m = fft2_size.m;
    q31_t n = fft2_size.n;

    riscv_vec_cfft2d_cfg_f64_t st = NULL;
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
    u32_t memneeded = sizeof (riscv_vec_cfft2d_state_f64_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)       /* factors_m */
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)       /* factors_n */
                      + sizeof (riscv_vec_cpx_f64_t) * twdl_size_m        /* twiddles_m */
                      + sizeof (riscv_vec_cpx_f64_t) * twdl_size_n        /* twiddles_n */
                      + sizeof (riscv_vec_cpx_f64_t) * (max_1d)            /* buffer */
                      + sizeof (riscv_vec_cpx_f64_t) * (m * n)            /* for transpose using */
                      + NDSV_ALIGNMENT;             /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif

    st = (riscv_vec_cfft2d_cfg_f64_t) NDSV_MALLOC (memneeded);

    if(st != NULL)
    {
        // Only backward FFT is scaled by default.
        st->is_forward_scaled = 0;
        st->is_backward_scaled = 1;

        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_cfft2d_state_f64_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors_m = (q31_t*) address;
        st->factors_n = (q31_t*) (st->factors_m + NDSV_STOCKHAM_FACTOR_MAX);
        st->twiddles_m = (riscv_vec_cpx_f64_t*) (st->factors_n + NDSV_STOCKHAM_FACTOR_MAX);
        st->twiddles_n = (riscv_vec_cpx_f64_t*) (st->twiddles_m + twdl_size_m );
        st->buffer_1d = (riscv_vec_cpx_f64_t*)  (st->twiddles_n +  twdl_size_n);
        st->buffer_2d = (riscv_vec_cpx_f64_t*)  (st->buffer_1d +  max_1d);
        st->m = m;
        st->n = n;
        riscv_vec_create_factor_f64(m, st->factors_m, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        riscv_vec_create_factor_f64(n, st->factors_n, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        st->factors_m[RFFT_EN] = 0 ; // 1:rfft / 0: cfft
        st->factors_n[RFFT_EN] = 0 ; // 1:rfft / 0: cfft
        st->factors_m[IS_RADIX_8] = 0;
        st->factors_n[IS_RADIX_8] = 0;
        riscv_vec_create_twiddle_f64 (st->twiddles_m, st->factors_m, m);
        riscv_vec_create_twiddle_f64 (st->twiddles_n, st->factors_n, n);
    }
    return st;
}

#if defined (__riscv_zfh)
riscv_vec_cfft2d_cfg_f16_t riscv_vec_cfft2d_init_f16 (riscv_vec_fft2d_size fft2_size)
{
    q31_t m = fft2_size.m;
    q31_t n = fft2_size.n;

    riscv_vec_cfft2d_cfg_f16_t st = NULL;
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
    u32_t memneeded = sizeof (riscv_vec_cfft2d_state_f16_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)       /* factors_m */
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)       /* factors_n */
                      + sizeof (riscv_vec_cpx_f16_t) * twdl_size_m        /* twiddles_m */
                      + sizeof (riscv_vec_cpx_f16_t) * twdl_size_n        /* twiddles_n */
                      + sizeof (riscv_vec_cpx_f16_t) * (max_1d)            /* buffer */
                      + sizeof (riscv_vec_cpx_f16_t) * (m * n)            /* for transpose using */
                      + NDSV_ALIGNMENT;             /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif

    st = (riscv_vec_cfft2d_cfg_f16_t) NDSV_MALLOC (memneeded);

    if(st != NULL)
    {
        // Only backward FFT is scaled by default.
        st->is_forward_scaled = 0;
        st->is_backward_scaled = 1;

        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_cfft2d_state_f16_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors_m = (q31_t*) address;
        st->factors_n = (q31_t*) (st->factors_m + NDSV_STOCKHAM_FACTOR_MAX);
        st->twiddles_m = (riscv_vec_cpx_f16_t*) (st->factors_n + NDSV_STOCKHAM_FACTOR_MAX);
        st->twiddles_n = (riscv_vec_cpx_f16_t*) (st->twiddles_m + twdl_size_m );
        st->buffer_1d = (riscv_vec_cpx_f16_t*)  (st->twiddles_n +  twdl_size_n);
        st->buffer_2d = (riscv_vec_cpx_f16_t*)  (st->buffer_1d +  max_1d);
        st->m = m;
        st->n = n;
        riscv_vec_create_factor(m, st->factors_m, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        riscv_vec_create_factor(n, st->factors_n, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        st->factors_m[RFFT_EN] = 0 ; // 1:rfft / 0: cfft
        st->factors_n[RFFT_EN] = 0 ; // 1:rfft / 0: cfft
        st->factors_m[IS_RADIX_8] = 0;
        st->factors_n[IS_RADIX_8] = 0;
        riscv_vec_create_twiddle_f16 (st->twiddles_m, st->factors_m, m);
        riscv_vec_create_twiddle_f16 (st->twiddles_n, st->factors_n, n);
    }
    return st;
}
#endif

#if defined (__riscv_zfbfmin)
riscv_vec_cfft2d_cfg_bf16_t riscv_vec_cfft2d_init_bf16 (riscv_vec_fft2d_size fft2_size)
{
    q31_t m = fft2_size.m;
    q31_t n = fft2_size.n;

    riscv_vec_cfft2d_cfg_bf16_t st = NULL;
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
    u32_t memneeded = sizeof (riscv_vec_cfft2d_state_bf16_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)       /* factors_m */
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)       /* factors_n */
                      + sizeof (riscv_vec_cpx_bf16_t) * twdl_size_m        /* twiddles_m */
                      + sizeof (riscv_vec_cpx_bf16_t) * twdl_size_n        /* twiddles_n */
                      + sizeof (riscv_vec_cpx_bf16_t) * (max_1d)            /* buffer */
                      + sizeof (riscv_vec_cpx_bf16_t) * (m * n)            /* for transpose using */
                      + NDSV_ALIGNMENT;             /* 64-bit alignment */

#ifdef ENA_MEASURE_HEAP
    printf("Max heap usage: %d bytes\n", memneeded);
#endif

    st = (riscv_vec_cfft2d_cfg_bf16_t) NDSV_MALLOC (memneeded);

    if(st != NULL)
    {
        // Only backward FFT is scaled by default.
        st->is_forward_scaled = 0;
        st->is_backward_scaled = 1;

        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_cfft2d_state_bf16_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors_m = (q31_t*) address;
        st->factors_n = (q31_t*) (st->factors_m + NDSV_STOCKHAM_FACTOR_MAX);
        st->twiddles_m = (riscv_vec_cpx_bf16_t*) (st->factors_n + NDSV_STOCKHAM_FACTOR_MAX);
        st->twiddles_n = (riscv_vec_cpx_bf16_t*) (st->twiddles_m + twdl_size_m );
        st->buffer_1d = (riscv_vec_cpx_bf16_t*)  (st->twiddles_n +  twdl_size_n);
        st->buffer_2d = (riscv_vec_cpx_bf16_t*)  (st->buffer_1d +  max_1d);
        st->m = m;
        st->n = n;
        riscv_vec_create_factor_bf16_purec(m, st->factors_m, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        riscv_vec_create_factor_bf16_purec(n, st->factors_n, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        st->factors_m[RFFT_EN] = 0 ; // 1:rfft / 0: cfft
        st->factors_n[RFFT_EN] = 0 ; // 1:rfft / 0: cfft
        st->factors_m[IS_RADIX_8] = 0;
        st->factors_n[IS_RADIX_8] = 0;
        riscv_vec_create_twiddle_bf16 (st->twiddles_m, st->factors_m, m);
        riscv_vec_create_twiddle_bf16 (st->twiddles_n, st->factors_n, n);
    }
    return st;
}
#endif //__riscv_zfbfmin


riscv_vec_rfft2d_cfg_f32_t riscv_vec_rfft2d_init_f32 (riscv_vec_fft2d_size fft2_size)
{
    /*
     * m * n input => take m times, n-point rfft first (due to the row-major memory layout)
     * and then take (n/2 + 1) times m-point cfft.
    */
    q31_t m = fft2_size.m;
    q31_t n = fft2_size.n;
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

riscv_vec_rfft2d_cfg_f64_t riscv_vec_rfft2d_init_f64 (riscv_vec_fft2d_size fft2_size)
{
    /*
     * m * n input => take m times, n-point rfft first (due to the row-major memory layout)
     * and then take (n/2 + 1) times m-point cfft.
    */
    q31_t m = fft2_size.m;
    q31_t n = fft2_size.n;
    int32_t ncfft = n / 2;
    riscv_vec_rfft2d_cfg_f64_t st = NULL;
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

    u32_t memneeded = sizeof (riscv_vec_rfft2d_state_f64_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)     /* factors_m */
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)     /* factors_n */
                      + sizeof (riscv_vec_cpx_f64_t) * (twdl_size_m)    /* twiddles_m */
                      + sizeof (riscv_vec_cpx_f64_t) * (twdl_size_n)    /* twiddles_n */
                      + sizeof (riscv_vec_cpx_f64_t) * (ncfft)          /* super twiddles */
                      + sizeof (riscv_vec_cpx_f64_t) * (max_1d)         /* buffer */
                      + sizeof (riscv_vec_cpx_f64_t) * (max_1d)              /* for 1d-rifft tmp input using */
                      + sizeof (riscv_vec_cpx_f64_t) * (ncfft)              /* for 1d-rifft tmp using */
                      + sizeof (riscv_vec_cpx_f64_t) * (m * (ncfft + 1)) /* for transpose/tmp output using */
                      + NDSV_ALIGNMENT;                    /* 64-bit alignment */

    st = (riscv_vec_rfft2d_cfg_f64_t) NDSV_MALLOC (memneeded);

    if( st != NULL)
    {
        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_rfft2d_state_f64_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors_m = (q31_t*) address;
        st->factors_n = (q31_t*) (st->factors_m + NDSV_STOCKHAM_FACTOR_MAX);
        st->twiddles_m = (riscv_vec_cpx_f64_t*) (st->factors_n + NDSV_STOCKHAM_FACTOR_MAX);
        st->twiddles_n = (riscv_vec_cpx_f64_t*) (st->twiddles_m + twdl_size_m );
        st->super_twiddles = (riscv_vec_cpx_f64_t*)  (st->twiddles_n +  twdl_size_n);
        st->buffer_1d = (riscv_vec_cpx_f64_t*)  (st->super_twiddles +  ncfft);
        st->tmp_1d_input = (riscv_vec_cpx_f64_t*)  (st->buffer_1d +  max_1d);
        st->tmp_split = (riscv_vec_cpx_f64_t*)  (st->tmp_1d_input +  max_1d);
        st->buffer_2d = (riscv_vec_cpx_f64_t*)  (st->tmp_split +  ncfft);
        st->m = m;
        st->ncfft = ncfft; // ncfft = n / 2;
        riscv_vec_create_factor_f64(m, st->factors_m, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        riscv_vec_create_factor_f64(ncfft, st->factors_n, NDSV_FACTOR_EIGHT_FIRST_STAGE);

        st->factors_m[RFFT_EN] = 0 ; // 1:rfft / 0: cfft
        st->factors_n[RFFT_EN] = 1 ; // 1:rfft / 0: cfft
        st->factors_m[IS_RADIX_8] = 0;
        st->factors_n[IS_RADIX_8] = 0;
        riscv_vec_create_twiddle_f64 (st->twiddles_m, st->factors_m, m);
        riscv_vec_create_twiddle_f64 (st->twiddles_n, st->factors_n, ncfft);

        riscv_vec_cpx_f64_t* twiddles = NULL;
        float64_t *wp_tbl_rfft_r = NULL, *wp_tbl_rfft_i = NULL;

        const float64_t DP_PI = 3.1415926535897932384626433832795;

        float64_t factor = 2 * DP_PI / n;
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

#if defined (__riscv_zfh)
riscv_vec_rfft2d_cfg_f16_t riscv_vec_rfft2d_init_f16 (riscv_vec_fft2d_size fft2_size)
{
    /*
     * m * n input => take m times, n-point rfft first (due to the row-major memory layout)
     * and then take (n/2 + 1) times m-point cfft.
    */
    q31_t m = fft2_size.m;
    q31_t n = fft2_size.n;
    int32_t ncfft = n / 2;
    riscv_vec_rfft2d_cfg_f16_t st = NULL;
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

    u32_t memneeded = sizeof (riscv_vec_rfft2d_state_f16_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)     /* factors_m */
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)     /* factors_n */
                      + sizeof (riscv_vec_cpx_f16_t) * (twdl_size_m)    /* twiddles_m */
                      + sizeof (riscv_vec_cpx_f16_t) * (twdl_size_n)    /* twiddles_n */
                      + sizeof (riscv_vec_cpx_f16_t) * (ncfft)          /* super twiddles */
                      + sizeof (riscv_vec_cpx_f16_t) * (max_1d)         /* buffer */
                      + sizeof (riscv_vec_cpx_f16_t) * (max_1d)              /* for 1d-rifft tmp input using */
                      + sizeof (riscv_vec_cpx_f16_t) * (ncfft)              /* for 1d-rifft tmp using */
                      + sizeof (riscv_vec_cpx_f16_t) * (m * (ncfft + 1)) /* for transpose/tmp output using */
                      + NDSV_ALIGNMENT;                    /* 64-bit alignment */

    st = (riscv_vec_rfft2d_cfg_f16_t) NDSV_MALLOC (memneeded);

    if( st != NULL)
    {
        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_rfft2d_state_f16_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors_m = (q31_t*) address;
        st->factors_n = (q31_t*) (st->factors_m + NDSV_STOCKHAM_FACTOR_MAX);
        st->twiddles_m = (riscv_vec_cpx_f16_t*) (st->factors_n + NDSV_STOCKHAM_FACTOR_MAX);
        st->twiddles_n = (riscv_vec_cpx_f16_t*) (st->twiddles_m + twdl_size_m );
        st->super_twiddles = (riscv_vec_cpx_f16_t*)  (st->twiddles_n +  twdl_size_n);
        st->buffer_1d = (riscv_vec_cpx_f16_t*)  (st->super_twiddles +  ncfft);
        st->tmp_1d_input = (riscv_vec_cpx_f16_t*)  (st->buffer_1d +  max_1d);
        st->tmp_split = (riscv_vec_cpx_f16_t*)  (st->tmp_1d_input +  max_1d);
        st->buffer_2d = (riscv_vec_cpx_f16_t*)  (st->tmp_split +  ncfft);
        st->m = m;
        st->ncfft = ncfft; // ncfft = n / 2;
        riscv_vec_create_factor(m, st->factors_m, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        riscv_vec_create_factor(ncfft, st->factors_n, NDSV_FACTOR_EIGHT_FIRST_STAGE);

        st->factors_m[RFFT_EN] = 0 ; // 1:rfft / 0: cfft
        st->factors_n[RFFT_EN] = 1 ; // 1:rfft / 0: cfft
        st->factors_m[IS_RADIX_8] = 0;
        st->factors_n[IS_RADIX_8] = 0;
        riscv_vec_create_twiddle_f16 (st->twiddles_m, st->factors_m, m);
        riscv_vec_create_twiddle_f16 (st->twiddles_n, st->factors_n, ncfft);

        riscv_vec_cpx_f16_t* twiddles = NULL;
        float16_t *wp_tbl_rfft_r = NULL, *wp_tbl_rfft_i = NULL;
        float factor = 2 * NDSV_PI / n;
        int32_t j = 0 ;

        twiddles = st->super_twiddles;
        wp_tbl_rfft_r = &twiddles[0].r;
        wp_tbl_rfft_i = &twiddles[ncfft / 2].r;
        for (j = 0; j < ncfft ; j++)
        {
            //phase = j* factor;
            wp_tbl_rfft_r[j] = (float16_t) cos (j * factor);
            wp_tbl_rfft_i[j] = (float16_t) - sin (j * factor);
        }
    }
    return st;

}

#endif

#if defined (__riscv_zfbfmin)
riscv_vec_rfft2d_cfg_bf16_t riscv_vec_rfft2d_init_bf16 (riscv_vec_fft2d_size fft2_size)
{
    /*
     * m * n input => take m times, n-point rfft first (due to the row-major memory layout)
     * and then take (n/2 + 1) times m-point cfft.
    */
    q31_t m = fft2_size.m;
    q31_t n = fft2_size.n;
    int32_t ncfft = n / 2;
    riscv_vec_rfft2d_cfg_bf16_t st = NULL;
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

    u32_t memneeded = sizeof (riscv_vec_rfft2d_state_bf16_t)
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)     /* factors_m */
                      + sizeof (q31_t) * (NDSV_STOCKHAM_FACTOR_MAX)     /* factors_n */
                      + sizeof (riscv_vec_cpx_bf16_t) * (twdl_size_m)    /* twiddles_m */
                      + sizeof (riscv_vec_cpx_bf16_t) * (twdl_size_n)    /* twiddles_n */
                      + sizeof (riscv_vec_cpx_bf16_t) * (ncfft)          /* super twiddles */
                      + sizeof (riscv_vec_cpx_bf16_t) * (max_1d)         /* buffer */
                      + sizeof (riscv_vec_cpx_bf16_t) * (max_1d)              /* for 1d-rifft tmp input using */
                      + sizeof (riscv_vec_cpx_bf16_t) * (ncfft)              /* for 1d-rifft tmp using */
                      + sizeof (riscv_vec_cpx_bf16_t) * (m * (ncfft + 1)) /* for transpose/tmp output using */
                      + NDSV_ALIGNMENT;                    /* 64-bit alignment */

    st = (riscv_vec_rfft2d_cfg_bf16_t) NDSV_MALLOC (memneeded);

    if( st != NULL)
    {
        uintptr_t address = (uintptr_t) st + sizeof (riscv_vec_rfft2d_state_bf16_t);
        NDSV_ALIGN (address, NDSV_ALIGNMENT);
        st->factors_m = (q31_t*) address;
        st->factors_n = (q31_t*) (st->factors_m + NDSV_STOCKHAM_FACTOR_MAX);
        st->twiddles_m = (riscv_vec_cpx_bf16_t*) (st->factors_n + NDSV_STOCKHAM_FACTOR_MAX);
        st->twiddles_n = (riscv_vec_cpx_bf16_t*) (st->twiddles_m + twdl_size_m );
        st->super_twiddles = (riscv_vec_cpx_bf16_t*)  (st->twiddles_n +  twdl_size_n);
        st->buffer_1d = (riscv_vec_cpx_bf16_t*)  (st->super_twiddles +  ncfft);
        st->tmp_1d_input = (riscv_vec_cpx_bf16_t*)  (st->buffer_1d +  max_1d);
        st->tmp_split = (riscv_vec_cpx_bf16_t*)  (st->tmp_1d_input +  max_1d);
        st->buffer_2d = (riscv_vec_cpx_bf16_t*)  (st->tmp_split +  ncfft);
        st->m = m;
        st->ncfft = ncfft; // ncfft = n / 2;
        riscv_vec_create_factor_bf16_purec(m, st->factors_m, NDSV_FACTOR_EIGHT_FIRST_STAGE);
        riscv_vec_create_factor_bf16_purec(ncfft, st->factors_n, NDSV_FACTOR_EIGHT_FIRST_STAGE);

        st->factors_m[RFFT_EN] = 0 ; // 1:rfft / 0: cfft
        st->factors_n[RFFT_EN] = 1 ; // 1:rfft / 0: cfft
        st->factors_m[IS_RADIX_8] = 0;
        st->factors_n[IS_RADIX_8] = 0;
        riscv_vec_create_twiddle_bf16 (st->twiddles_m, st->factors_m, m);
        riscv_vec_create_twiddle_bf16 (st->twiddles_n, st->factors_n, ncfft);

        riscv_vec_cpx_bf16_t* twiddles = NULL;
        bf16_t *wp_tbl_rfft_r = NULL, *wp_tbl_rfft_i = NULL;
        float factor = 2 * NDSV_PI / n;
        int32_t j = 0 ;

        twiddles = st->super_twiddles;
        wp_tbl_rfft_r = &twiddles[0].r;
        wp_tbl_rfft_i = &twiddles[ncfft / 2].r;
        for (j = 0; j < ncfft ; j++)
        {
            //phase = j* factor;
            wp_tbl_rfft_r[j] = (bf16_t) cos (j * factor);
            wp_tbl_rfft_i[j] = (bf16_t) - sin (j * factor);
        }
    }
    return st;

}

#endif //__riscv_zfbfmin



#else  //ENA_STOCKHAM_FFT
/*
 * This function outputs a factor buffer ('facbuf') that decomposes an FFT of input size
 * n into a number of radix-r butterfly calculations (for r in some set of radix values).
 *
 * Factor buffer layout:
 *     index 0: stage count
 *     index 1: stride for the first stage
 *     index 2 to (2*stage_count + 1): pairs of factors (number of sections, section size)
 *     index (2*stage_count + 2): an flag specifying which algorithm to use
 *
 * e.g. 1024 samples might result in the following five stage radix-4 factors buffer:
 *          [5, 256, 4, 256, 4, 64, 4, 16, 4, 4, 4, 1]
 *          i.e. 1024 = 4x256, each of which is 4x64, each of which is 4x16, each of which
 *               is 4x4, each of which is 4x1. There are 5 stages, and the stride for the
 *               first stage is 256 (1024 / 4, for radix-4).
 *
 * Only the leading 42 int32 is used to store factors.
 * The left can be used as algorithm flags, or status flags.
 * Even the leading bits of stage number can be reused.
 */
#define NDS_SUPPORT
q31_t riscv_vec_create_factor (q31_t n,
                               q31_t * FUNC_RESTRICT facbuf,
                               q31_t vec_factor_flags)
{
#ifdef NDS_SUPPORT
    q31_t p;
    q31_t i = 1;
    q31_t stage_num = 0;
    q31_t stride_max = n;

    do
    {
        if ((vec_factor_flags & NDSV_FACTOR_EIGHT_FIRST_STAGE)
                && ((n == 8) || (n == 40) || (n == 24)))
        {
            p = 8;
        }
        else if ((vec_factor_flags & NDSV_FACTOR_EIGHT) && ((n % 8) == 0))
        {
            p = 8;
        }
        else if ((n % 4) == 0)
        {
            p = 4;
        }
        else // stop factoring
        {
            p = n;
        }

        n /= p;
        facbuf[2 * i] = p;
        facbuf[2 * i + 1] = n;
        i++;
        stage_num++;
    }
    while (n > 1);
    facbuf[0] = stage_num;
    facbuf[1] = stride_max / p;
#else
    // This is a workaround. We need to "return" some flags.
    // Otherwise, we need to modify signature of riscv_vec_create_factor.
    assert (NDSV_FACTOR_MAX >= 32);

    if ((facbuf == NULL)
            || (n <= 0))
    {
        return NDSV_FAIL;
    }

    q31_t p;
    q31_t i = 1;
    q31_t stage_num = 0;
    q31_t stride_max = n;

    // Factor out powers of 4, 2, 5, and 3. Additionally, factor out powers
    // of 8 if the right factor flags are passed. If none of these factors
    // can be applied at any stage, the remaining size is used as a factor.
    do
    {
        // If NDSV_FACTOR_EIGHT_FIRST_STAGE is enabled, we can generate
        // a first stage of radix-8 (e.g. by combining one radix-4 and
        // one radix-2 stage into a single radix-8 stage).
        if ((vec_factor_flags & NDSV_FACTOR_EIGHT_FIRST_STAGE)
                && ((n == 8) || (n == 40) || (n == 24)))
        {
            switch (n)
            {
            case 8:
                p = 8;
                break;
            case 24:
                p = 3;
                break;
            default: // n == 40
                p = 5;
                break;
            }
        }
        else if ((vec_factor_flags & NDSV_FACTOR_EIGHT) && ((n % 8) == 0))
        {
            p = 8;
        }
        else if ((n % 4) == 0)
        {
            p = 4;
        }
        else if ((n % 2) == 0)
        {
            p = 2;
        }
        else if ((n % 5) == 0)
        {
            p = 5;
        }
        else if ((n % 3) == 0)
        {
            p = 3;
        }
        else // stop factoring
        {
            p = n;
        }

        n /= p;
        facbuf[2 * i] = p;
        facbuf[2 * i + 1] = n;
        i++;
        stage_num++;
    }
    while (n > 1);
    facbuf[0] = stage_num;
    facbuf[1] = stride_max / p;

    if (stage_num > 21)
    {
        // Since nfft is q31_t, stage_num can never be greater than 21,
        // because 3^21 > 2^32
        return NDSV_FAIL;
    }
#endif
    // facbuf[2 * i] = alg_flag;
    return NDSV_OK;
}
#ifndef ENA_STOCKHAM_FFT
// Twiddles matrix [radix-1][mstride]
// First column (k == 0) is ignored because phase == 1, and
// twiddle = (1.0, 0.0).
void riscv_vec_create_twiddle_segment_f32 (riscv_vec_cpx_f32_t * FUNC_RESTRICT twiddles,
        const q31_t mstride,
        const q31_t fstride,
        const q31_t radix,
        const q31_t nfft)
{
    q31_t j, k;
    float32_t phase;
    const float64_t pi = NDSV_PI;

    for (j = 0; j < mstride; j++)
    {
        for (k = 1; k < radix; k++) // phase = 1 when k = 0
        {
            phase = -2 * pi * fstride * k * j / nfft;
            twiddles[mstride * (k - 1) + j].r = (float32_t) cos (phase);
            twiddles[mstride * (k - 1) + j].i = (float32_t) sin (phase);

            //printf("[%d, %d, %d, %d] twiddle = %14.7f + %14.7fi\n", mstride, radix, j, k, twiddles[mstride * (k - 1) + j].r, twiddles[mstride * (k - 1) + j].i);
        } // radix
    } // mstride
}
#endif

typedef void (*segment_generator_f32)(riscv_vec_cpx_f32_t*,
                                      const q31_t,
                                      const q31_t,
                                      const q31_t,
                                      const q31_t);

riscv_vec_cpx_f32_t* riscv_vec_create_twiddle_instance_f32 (
    segment_generator_f32 generator,
    riscv_vec_cpx_f32_t * FUNC_RESTRICT twiddles,
    const q31_t * FUNC_RESTRICT factors,
    const q31_t nfft)
{
    q31_t stage_count = factors[0];
    q31_t fstride = factors[1];
    q31_t mstride;
    q31_t cur_radix; // current radix

    // for first stage
    cur_radix = factors[2 * stage_count];
#ifndef NDS_SUPPORT
    if (cur_radix % 2) // current radix is not 4 or 2
    {
        twiddles[0].r = 1.0;
        twiddles[0].i = 0.0;
        twiddles += 1;
        generator (twiddles, 1, fstride, cur_radix, nfft);
        twiddles += cur_radix - 1;
    }
#endif
    stage_count --;

    // for other stage
    for (; stage_count > 0; stage_count --)
    {
        cur_radix = factors[2 * stage_count];
        fstride /= cur_radix;
        mstride = factors[2 * stage_count + 1];
        generator (twiddles, mstride, fstride, cur_radix, nfft);
        twiddles += mstride * (cur_radix - 1);
    } // stage_count

    return twiddles;
}

riscv_vec_cpx_f32_t* riscv_vec_create_twiddle_f32 (riscv_vec_cpx_f32_t * twiddles,
        const q31_t * factors,
        const q31_t nfft )
{
    segment_generator_f32 generator = riscv_vec_create_twiddle_segment_f32;
    twiddles = riscv_vec_create_twiddle_instance_f32(generator,
               twiddles, factors, nfft);
    return twiddles;
}

/**
 * @addtogroup C2C_FFT_IFFT
 * @{
 */

/**
 * @brief Destroys the configuration structure allocated by variants of @ref cfft_f32 (frees memory, etc.)
 *
 * @param[in]   cfg     pointer to the FFT configuration structure
 */
void riscv_vec_cfft_free_f32 (riscv_vec_cfft_cfg_f32_t cfg)
{
    free(cfg);
}

/**
 * @brief Destroys the configuration structure allocated by variants of @ref cfft_q31 (frees memory, etc.)
 *
 * @param[in]   cfg     pointer to the FFT configuration structure
 */
void riscv_vec_cfft_free_q31 (riscv_vec_cfft_cfg_q31_t cfg)
{
    free (cfg);
}

/**
 * @brief Destroys the configuration structure allocated by @ref cfft_q15 (frees memory, etc.)
 *
 * @param[in]   cfg     pointer to the FFT configuration structure
 */
void riscv_vec_cfft_free_q15 (riscv_vec_cfft_cfg_q15_t cfg)
{
    free (cfg);
}

/** @} */ // C2C_FFT_IFFT

/**
 * @addtogroup R2C_FFT_IFFT
 * @{
 */

/**
 * @brief Destroys the configuration structure allocated by @ref rfft_f32 (frees memory, etc.)
 *
 * @param[in]   cfg     pointer to the FFT configuration structure
 */
void riscv_vec_rfft_free_f32 (riscv_vec_rfft_cfg_f32_t cfg)
{
    free(cfg);
}

// #ifndef NDS
/**
 * @brief Destroys the configuration structure allocated by @ref rfft_q31 (frees memory, etc.)
 *
 * @param[in]   cfg     pointer to the FFT configuration structure
 */
void riscv_vec_rfft_free_q31 (riscv_vec_rfft_cfg_q31_t cfg)
{
    free (cfg);
}

/**
 * @brief Destroys the configuration structure allocated by @ref rfft_q15 (frees memory, etc.)
 *
 * @param[in]   cfg     pointer to the FFT configuration structure
 */
void riscv_vec_rfft_free_q15 (riscv_vec_rfft_cfg_q15_t cfg)
{
    free (cfg);
}
// #endif
#endif  //ENA_SOTCLHAM_FFT
/** @} */ // R2C_FFT_IFFT

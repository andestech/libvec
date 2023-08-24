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
    //float phase = 2*pi/nfft;
    double phase = 2 * pi / nfft;
    // Pure C version using RADIX-2 only
    q31_t cal_fft = nfft / 2;
    riscv_vec_cpx_f32_t *wp_tbl_cmplx = twiddles;
    for(tmp_i = 0 ; tmp_i < cal_fft ; tmp_i++)
    {
        wp_tbl_cmplx[tmp_i].r = cos((tmp_i) * phase);
        wp_tbl_cmplx[tmp_i].i = -sin((tmp_i) * phase);
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
    //float phase = 2*pi/nfft;
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

void riscv_vec_cfft_free_f64 (riscv_vec_cfft_cfg_f64_t cfg)
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

void riscv_vec_rfft_free_f64 (riscv_vec_rfft_cfg_f64_t cfg)
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

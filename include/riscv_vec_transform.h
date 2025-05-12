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
#ifndef __RISCV_VEC_TRANSFORM_H__
#define __RISCV_VEC_TRANSFORM_H__


/** @file*/

#ifdef __cplusplus
extern "C" {
#endif

#include "riscv_dsp_math_types.h"
#include "riscv_vec_types.h"
/**
 * @defgroup groupTrans Transform Functions
 *
 * @brief Transform Functions
 */

/**
 * @addtogroup groupTrans
 * @{
 */

/////////////////////////////////////////////////////////
// definitions for fft
/////////////////////////////////////////////////////////

/**
 * @defgroup Trans_struct Transform Function Structures
 */
/**
 * @addtogroup Trans_struct
 * @{
 */

/**
 * @brief Structure for the single-precision floating-point complex vector.
 */
typedef struct
{
    float32_t r;  /**< Real part of a single-precision floating-point complex number */
    float32_t i;  /**< Imaginary part of a single-precision floating-point complex number */
} riscv_vec_cpx_f32_t;

/**
 * @brief Structure for the double-precision floating-point complex vector.
 */
typedef struct
{
    float64_t r;  /**< Real part of a floating-point complex number */
    float64_t i;  /**< Imaginary part of a floating-point complex number */
} riscv_vec_cpx_f64_t;

#if defined (__riscv_zfh)
/**
 * @brief Structure for the half-precision floating-point complex vector.
 */
typedef struct
{
    float16_t r;  /**< Real part of a floating-point complex number */
    float16_t i;  /**< Imaginary part of a floating-point complex number */
}riscv_vec_cpx_f16_t;
#endif

/**
 * @brief Structure for the single-precision floating-point CFFT state
 */
typedef struct
{
    q31_t nfft;                           /**< input elements */
    q31_t *factors;                       /**< pointer of the factors vector */
    riscv_vec_cpx_f32_t *twiddles;        /**< pointer of the twiddles vector, which has a
                                               data type of riscv_vec_cpx_f32_t */
    riscv_vec_cpx_f32_t *buffer;          /**< pointer of the buffer vector, which has a data
                                               type of riscv_vec_cpx_f32_t */
    riscv_vec_cpx_f32_t *last_twiddles;   /**< pointer of the last twiddles vector, which has
                                               a data type of riscv_vec_cpx_f32_t */
    q31_t is_forward_scaled;              /**< Scale value of CFFT */
    q31_t is_backward_scaled;             /**< Scale value of CIFFT */
} riscv_vec_cfft_state_f32_t;

/**
 * @brief Structure for the double-precision floating-point CFFT state
 */
typedef struct
{
    q31_t nfft;                           /**< input elements */
    q31_t *factors;                       /**< pointer of the factors vector */
    riscv_vec_cpx_f64_t *twiddles;        /**< pointer of the twiddles vector, which has a
                                               data type of riscv_vec_cpx_f64_t */
    riscv_vec_cpx_f64_t *buffer;          /**< pointer of the buffer vector, which has a data
                                               type of riscv_vec_cpx_f64_t */
    riscv_vec_cpx_f64_t *last_twiddles;   /**< pointer of the last twiddles vector, which has
                                               a data type of riscv_vec_cpx_f64_t */
    q31_t is_forward_scaled;              /**< Scale value of CFFT */
    q31_t is_backward_scaled;             /**< Scale value of CIFFT */
} riscv_vec_cfft_state_f64_t;


#if defined (__riscv_zfh)
/**
 * @brief Structure for the half-precision floating-point CFFT state
 */
typedef struct
{
    q31_t nfft;                           /**< input elements */
    q31_t *factors;                       /**< pointer of the factors vector */
    riscv_vec_cpx_f16_t *twiddles;        /**< pointer of the twiddles vector, which has a
                                               data type of riscv_vec_cpx_f16_t */
    riscv_vec_cpx_f16_t *buffer;          /**< pointer of the buffer vector, which has a data
                                               type of riscv_vec_cpx_f16_t */
    riscv_vec_cpx_f16_t *last_twiddles;   /**< pointer of the last twiddles vector, which has
                                               a data type of riscv_vec_cpx_f16_t */
    q31_t is_forward_scaled;              /**< Scale value of CFFT */
    q31_t is_backward_scaled;             /**< Scale value of CIFFT */
} riscv_vec_cfft_state_f16_t;
#endif

/**
 * @brief Structure pointer for riscv_vec_cfft_state_f32_t
 */
typedef riscv_vec_cfft_state_f32_t* riscv_vec_cfft_cfg_f32_t;

/**
 * @brief Structure pointer for riscv_vec_cfft_state_f64_t
 */
typedef riscv_vec_cfft_state_f64_t* riscv_vec_cfft_cfg_f64_t;

#if defined (__riscv_zfh)
/**
 * @brief Structure pointer for riscv_vec_cfft_state_f16_t
 */
typedef riscv_vec_cfft_state_f16_t* riscv_vec_cfft_cfg_f16_t;
#endif

/**
 * @brief Structure for the single-precision floating-point RFFT state
 */
typedef struct
{
    riscv_vec_cpx_f32_t *buffer;          /**< pointer of the buffer vector, which has a data
                                               type of riscv_vec_cpx_f32_t */
    q31_t ncfft;                          /**< input elements */
    q31_t *factors;                       /**< pointer of the factors vector */
    riscv_vec_cpx_f32_t *twiddles;        /**< pointer of the twiddles vector, which has a data
                                               type of riscv_vec_cpx_f32_t */
    riscv_vec_cpx_f32_t *super_twiddles;  /**< pointer of the super twiddles vector, which has a
                                               data type of riscv_vec_cpx_f32_t */
} riscv_vec_rfft_state_f32_t;

/**
 * @brief Structure for the double-precision floating-point RFFT state
 */
typedef struct
{
    riscv_vec_cpx_f64_t *buffer;          /**< pointer of the buffer vector, which has a data
                                               type of riscv_vec_cpx_f32_t */
    q31_t ncfft;                          /**< input elements */
    q31_t *factors;                       /**< pointer of the factors vector */
    riscv_vec_cpx_f64_t *twiddles;        /**< pointer of the twiddles vector, which has a data
                                               type of riscv_vec_cpx_f32_t */
    riscv_vec_cpx_f64_t *super_twiddles;  /**< pointer of the super twiddles vector, which has a
                                               data type of riscv_vec_cpx_f32_t */
} riscv_vec_rfft_state_f64_t;

#if defined (__riscv_zfh)
/**
 * @brief Structure for the half-precision floating-point RFFT state
 */
typedef struct
{
    riscv_vec_cpx_f16_t *buffer;
    q31_t ncfft;
    q31_t *factors;
    riscv_vec_cpx_f16_t *twiddles;
    riscv_vec_cpx_f16_t *super_twiddles;
} riscv_vec_rfft_state_f16_t;
#endif

/**
 * @brief Structure pointer for riscv_vec_rfft_state_f32_t
 */
typedef riscv_vec_rfft_state_f32_t* riscv_vec_rfft_cfg_f32_t;

/**
 * @brief Structure pointer for riscv_vec_rfft_state_f64_t
 */
typedef riscv_vec_rfft_state_f64_t* riscv_vec_rfft_cfg_f64_t;

#if defined (__riscv_zfh)
/**
 * @brief Structure pointer for riscv_vec_rfft_state_f16_t
 */
typedef riscv_vec_rfft_state_f16_t* riscv_vec_rfft_cfg_f16_t;
#endif

/**
 * @brief Structure for the Q15 complex vector.
 */
typedef struct
{
    q15_t r;  /**< real part of a Q15 complex number */
    q15_t i;  /**< imaginary part of a Q15 complex number */
} riscv_vec_cpx_q15_t;

/**
 * @brief Structure for the Q15 CFFT state
 */
typedef struct
{
    q31_t nfft;                       /**< input elements */
    q31_t *factors;                   /**< pointer of the factors vector */
    riscv_vec_cpx_q15_t *twiddles;    /**< pointer of the twiddles vector, which has a data
                                           type of riscv_vec_cpx_q15_t */
    riscv_vec_cpx_q15_t *buffer;      /**< pointer of the buffer vector, which has a data type
                                           of riscv_vec_cpx_q15_t */
} riscv_vec_cfft_state_q15_t;

/**
 * @brief Structure pointer for riscv_vec_cfft_state_q15_t
 */
typedef riscv_vec_cfft_state_q15_t* riscv_vec_cfft_cfg_q15_t;

/**
 * @brief Structure for the Q15 RFFT state
 */
typedef struct
{
    q31_t nfft;                             /**< elements of RFFT */
    q31_t ncfft;                            /**< elements of CFFT */
    q31_t *factors;                         /**< pointer of the factors vector */
    riscv_vec_cpx_q15_t *twiddles;          /**< pointer of the twiddles vector, which has
                                                 a data type of riscv_vec_cpx_q15_t */
    riscv_vec_cpx_q15_t *super_twiddles;    /**< pointer of the super twiddles vector, which has
                                                 a data type of riscv_vec_cpx_q15_t */
    riscv_vec_cpx_q15_t *buffer;            /**< pointer of the buffer vector, which has a data type
                                                 of riscv_vec_cpx_q15_t */
} riscv_vec_rfft_state_q15_t;

/**
 * @brief Structure pointer for riscv_vec_rfft_state_q15_t
 */
typedef riscv_vec_rfft_state_q15_t* riscv_vec_rfft_cfg_q15_t;

/**
 * @brief Structure for the Q31 complex vector.
 */
typedef struct
{
    q31_t r;    /**< real part of a Q31 complex number */
    q31_t i;    /**< imaginary part of a Q31 complex number */
} riscv_vec_cpx_q31_t;

/**
 * @brief Structure for the Q31 CFFT state
 */
typedef struct
{
    q31_t nfft;                         /**< input elements */
    q31_t *factors;                     /**< pointer of the factors vector */
    riscv_vec_cpx_q31_t *twiddles;      /**< pointer of the twiddles vector, which has a data
                                             type of riscv_vec_cpx_q31_t */
    riscv_vec_cpx_q31_t *buffer;        /**< pointer of the buffer vector, which has a data type
                                             of riscv_vec_cpx_q31_t */
    riscv_vec_cpx_q31_t *last_twiddles; /**< pointer of the last twiddles vector, which has a data
                                             type of riscv_vec_cpx_q31_t */
} riscv_vec_cfft_state_q31_t;

/**
 * @brief Structure pointer for riscv_vec_cfft_state_q31_t
 */
typedef riscv_vec_cfft_state_q31_t* riscv_vec_cfft_cfg_q31_t;

/**
 * @brief Structure for the Q31 RFFT state
 */
typedef struct
{
    q31_t nfft;                              /**< elements of RFFT */
    q31_t ncfft;                             /**< elements of CFFT */
    q31_t *factors;                          /**< pointer of the factors vector */
    riscv_vec_cpx_q31_t *twiddles;           /**< pointer of the twiddles vector, which has a data
                                                  type of riscv_vec_cpx_q31_t */
    riscv_vec_cpx_q31_t *super_twiddles;     /**< pointer of the super twiddles vector, which has a
                                                  data type of riscv_vec_cpx_q31_t */
    riscv_vec_cpx_q31_t *buffer;             /**< pointer of the buffer vector, which has a data type
                                                  of riscv_vec_cpx_q31_t */
} riscv_vec_rfft_state_q31_t;

/**
 * @brief Structure pointer for riscv_vec_rfft_state_q31_t
 */
typedef riscv_vec_rfft_state_q31_t* riscv_vec_rfft_cfg_q31_t;

/**
 * @brief The structure of F32 DCT states
 */
typedef struct
{
    uint32_t n;                         /**< Input elements */
    float32_t *cos_table;               /**< Points to the cosine table */
    float32_t *sin_table;               /**< Points to the sine table */
    float32_t nfactor;                  /**< Factor of n DCT */
    riscv_vec_cpx_f32_t *rfft_out;      /**< Points to the RFFT output, which has a data type of riscv_vec_cpx_f32_t */
    riscv_vec_rfft_cfg_f32_t rfft_cfg;  /**< Points to the RFFT structure configuration */
} riscv_vec_dct_state_f32_t;

/**
 * @brief Structure pointer for riscv_vec_dct_state_f32_t
 */
typedef riscv_vec_dct_state_f32_t* riscv_vec_dct_cfg_f32_t;

/**
 * @brief Structure for the single-precision floating-point DCT4 state
 */
typedef struct
{
    uint32_t n;                        /**< Input elements */
    float32_t *cos_table;              /**< Points to the cosine table */
    float32_t *sin_table;              /**< Points to the sine table */
    float32_t *cos_table2;             /**< Points to the cosine table2 */
    float32_t *sin_table2;             /**< Points to the sine table2 */
    float32_t nfactor;                 /**< Factor of n DCT */
    riscv_vec_cpx_f32_t *rfft_out;     /**< Points to the RFFT output, which has a data type of riscv_vec_cpx_f32_t */
    riscv_vec_rfft_cfg_f32_t rfft_cfg; /**< Points to the RFFT structure configuration */
} riscv_vec_dct4_state_f32_t;

/**
 * @brief Structure pointer for riscv_vec_dct4_state_f32_t
 */
typedef riscv_vec_dct4_state_f32_t* riscv_vec_dct4_cfg_f32_t;

/**
 * @brief Structure for the single-precision Q15 DCT state
 */
typedef struct
{
    uint32_t n;                        /**< Input elements */
    uint32_t log2n;                    /**< log 2 of n */
    q15_t *cos_table;                  /**< Points to the cosine table */
    q15_t *sin_table;                  /**< Points to the sine table */
    q15_t nfactor;                     /**< Factor of n DCT */
    riscv_vec_cpx_q15_t *rfft_out;     /**< Points to the RFFT output, which has a data type of riscv_vec_cpx_q15_t */
    riscv_vec_rfft_cfg_q15_t rfft_cfg; /**< Points to the RFFT structure configuration */
} riscv_vec_dct_state_q15_t;

/**
 * @brief Structure pointer for riscv_vec_dct_state_q15_t
 */
typedef riscv_vec_dct_state_q15_t* riscv_vec_dct_cfg_q15_t;

/**
 * @brief Structure for the single-precision Q15 DCT4 state
 */
typedef struct
{
    uint32_t n;                         /**< Input elements */
    uint32_t log2n;                     /**< log 2 of n */
    q15_t *cos_table;                   /**< Points to the cosine table */
    q15_t *sin_table;                   /**< Points to the sine table */
    q15_t *cos_table2;                  /**< Points to the cosine table2 */
    q15_t *sin_table2;                  /**< Points to the sine table2 */
    q15_t nfactor;                      /**< Factor of n DCT */
    riscv_vec_cpx_q15_t *rfft_out;      /**< Points to the RFFT output, which has a data type of riscv_vec_cpx_q15_t */
    riscv_vec_rfft_cfg_q15_t rfft_cfg;  /**< Points to the RFFT structure configuration */
} riscv_vec_dct4_state_q15_t;

/**
 * @brief Structure pointer for riscv_vec_dct4_state_q15_t
 */
typedef riscv_vec_dct4_state_q15_t* riscv_vec_dct4_cfg_q15_t;

/**
 * @brief Structure for the Q31 DCT state
 */
typedef struct
{
    uint32_t n;                        /**< Input elements */
    uint32_t log2n;                    /**< log 2 of n */
    q31_t *cos_table;                  /**< Points to the cosine table */
    q31_t *sin_table;                  /**< Points to the sine table */
    q31_t nfactor;                     /**< Factor of n DCT */
    riscv_vec_cpx_q31_t *rfft_out;     /**< Points to the RFFT output, which has a data type of riscv_vec_cpx_q31_t */
    riscv_vec_rfft_cfg_q31_t rfft_cfg; /**< Points to the RFFT structure configuration */
} riscv_vec_dct_state_q31_t;

/**
 * @brief Structure pointer for riscv_vec_dct_state_q31_t
 */
typedef riscv_vec_dct_state_q31_t* riscv_vec_dct_cfg_q31_t;

/**
 * @brief Structure for the Q31 DCT4 state
 */
typedef struct
{
    uint32_t n;                         /**< Input elements */
    uint32_t log2n;                     /**< log 2 of n */
    q31_t *cos_table;                   /**< Points to the cosine table */
    q31_t *sin_table;                   /**< Points to the sine table */
    q31_t *cos_table2;                  /**< Points to the cosine table2 */
    q31_t *sin_table2;                  /**< Points to the sine table2 */
    q31_t nfactor;                      /**< Factor of n DCT */
    riscv_vec_cpx_q31_t *rfft_out;      /**< Points to the RFFT output, which has a data type of riscv_vec_cpx_q31_t */
    riscv_vec_rfft_cfg_q31_t rfft_cfg;  /**< Points to the RFFT structure configuration */
} riscv_vec_dct4_state_q31_t;

/**
 * @brief Structure pointer for riscv_vec_dct4_state_q31_t
 */
typedef riscv_vec_dct4_state_q31_t* riscv_vec_dct4_cfg_q31_t;

/**
 * @brief Structure for the Q31 DCT2D_8x8 state
 */
typedef struct
{
    u8_t *idct_limit;     /**< Points to the IDCT limit table */
    q31_t *buffer;        /**< Points to temporary buffer */
} riscv_vec_dct2d_8x8_state_q31_t;

/**
 * @brief Structure pointer for riscv_vec_dct2d_8x8_state_q31_t
 */
typedef riscv_vec_dct2d_8x8_state_q31_t* riscv_vec_dct2d_8x8_cfg_q31_t;

/**
 * @brief Structure for the Q15 DCT2D_8x8 state
 */
typedef struct
{
    q15_t *cos_table;        /**< Points to the cosine table */
    q15_t *idct_cos_table;   /**< Points to the IDCT cosine table */
    u8_t *idct_limit;        /**< Points to the IDCT limit table */
    q31_t *buffer;           /**< Points to the temporary buffer */
} riscv_vec_dct2d_8x8_state_q15_t;

/**
 * @brief Structure pointer for riscv_vec_dct2d_8x8_state_q15_t
 */
typedef riscv_vec_dct2d_8x8_state_q15_t* riscv_vec_dct2d_8x8_cfg_q15_t;

/** @} Trans_struct */

/**
 * @defgroup TransCfft Complex FFT Functions
 *
 * @brief Complex FFT Functions
 *
 * Complex FFT functions include fast Fourier transform (CFFT) and inverse fast
 * Fourier transform (CIFFT) functions, which implement the famous Cooley-Tukey
 * algorithm to transform signals from time domain to frequency domain. The
 * complex data in the input vector are arranged as [real, imaginary, real,
 * imaginary..., real, imaginary].
 *
 * Andes vector library supports distinct CFFT and CIFFT functions for the following data types: single-, double- and half-precision floating-point, Q31 and Q15. These functions are introduced in the subsections below. Note that overflow may occur in Q31 and Q15 CFFT and CIFFT functions. If you want to call any of these functions in which the input parameter scaled_flag is set to 0, make sure to do an arithmetic right shift beforehand so that the overflow can be avoided. For details about the input parameter scaled_flag for Q31 and Q15 CFFT /CIFFT functions, please refer to Section 2.5.2.9 to 2.5.2.10.
 *
 * To ensure correct results for the CFFT function being used, you must initialize the structure with a corresponding initialization function (e.g., riscv_vec_cfft_init_f32) first and release the allocated memory for the structure with an associated free function (e.g., riscv_vec_cfft_free_f32) after the CFFT function is executed.
 *
 * The CFFT initialization function accepts a parameter nfft that defines the number of input samples and uses it to allocate memory internally. The needed memory size is determined according to the calculations listed in the following table. If there is no enough memory space for the allocation, the initialization function will return NULL to indicate the error.
 *
 * @image html cfft_mem.png ""
 *
 */

/**
 * @addtogroup TransCfft
 * @{
 */

/**
 * @brief Function to initialize single-precision floating-point CFFT
 *
 * @param[in] nfft number of input samples
 * @return Structure pointer of the single-precision floating-point CFFT configuration if a temporary buffer for the structure is allocated sucessfullysuccessfully, or NULL if the buffer allocation fails.
 *
 * @b Note:
 *
 * This function will allocate a temporary buffer to store the twiddle table, calculation results and some control flags. If the buffer allocation is not successful, the function will return NULL. The required buffer size varies depending on the number of input samples. For detailed memory usage of the buffer, please refer to Section 2.5.2.
 */
extern riscv_vec_cfft_cfg_f32_t riscv_vec_cfft_init_f32(q31_t nfft);

/**
 * @brief Function to initialize double-precision floating-point CFFT
 *
 * @param[in] nfft number of input samples
 * @return Structure pointer of the double-precision floating-point CFFT configuration if a temporary buffer for the structure is allocated successfully, or NULL if the buffer allocation fails.
 *
 * @b Note:
 *
 * This function will allocate a temporary buffer to store the twiddle table, calculation results and some control flags. If the buffer allocation is not successful, the function will return NULL. The required buffer size varies depending on the number of input samples. For detailed memory usage of the buffer, please refer to Section 2.5.2.
 */
extern riscv_vec_cfft_cfg_f64_t riscv_vec_cfft_init_f64(q31_t nfft);

#if defined (__riscv_zfh)
/**
 * @brief Function to initialize half-precision floating-point CFFT
 *
 * @param[in] nfft number of input samples
 * @return Structure pointer of the half-precision floating-point CFFT configuration if a temporary buffer for the structure is allocated successfully, or NULL if the buffer allocation fails.
 *
 * @b Note:
 *
 * 1.	This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”, please refer to Section 1.1.
 *
 * 2.	This function will allocate a temporary buffer to store the twiddle table, calculation results and some control flags. If the buffer allocation is not successful, the function will return NULL. The required buffer size varies depending on the number of input samples. For detailed memory usage of the buffer, please refer to Section 2.5.2.
 */
extern riscv_vec_cfft_cfg_f16_t riscv_vec_cfft_init_f16(q31_t nfft);
#endif

/**
 * @brief Function to initialize Q31 CFFT
 *
 * @param[in] nfft number of input samples
 * @return Structure pointer of the Q31 CFFT configuration if a temporary buffer for the structure is allocated successfully, or NULL if the buffer allocation fails.
 *
 * @b Note:
 *
 * This function will allocate a temporary buffer to store the twiddle table, calculation results and some control flags. If the buffer allocation is not successful, the function will return NULL. The required buffer size varies depending on the number of input samples. For detailed memory usage of the buffer, please refer to Section 2.5.2.
 */
extern riscv_vec_cfft_cfg_q31_t riscv_vec_cfft_init_q31(q31_t nfft);

/**
 * @brief Function to initialize Q15 CFFT
 *
 * @param[in] nfft number of input samples
 * @return Structure pointer of the Q15 CFFT configuration if a temporary buffer for the structure is allocated successfully, or NULL if the buffer allocation fails.
 *
 * @b Note:
 *
 * This function will allocate a temporary buffer to store the twiddle table, calculation results and some control flags. If the buffer allocation is not successful, the function will return NULL. The required buffer size varies depending on the number of input samples. For detailed memory usage of the buffer, please refer to Section 2.5.2.
 */
extern riscv_vec_cfft_cfg_q15_t riscv_vec_cfft_init_q15(q31_t nfft);

/**
 * @brief Single-precision Floating-point CFFT function
 * @param[out] out pointer of the output complex vector
 * @param[in] in pointer of the input complex vector
 * @param[in] cfg structure of the single-precision floating-point CFFT configuration
 * @param[in] inverse_fft an FFT or IFFT (0: FFT, 1: IFFT)
 *
 * @b Note:
 *
 * To ensure correct results, you must initialize the structure with the
 * function riscv_vec_cfft_init_f32 before using the function riscv_vec_cfft_f32
 * and free the structure with another function riscv_vec_cfft_free_f32 in the end.
 * For detailed steps, please refer to the example below.
 *
 * @b Example
 *     <pre>
 * Given 128 samples, the example of floating-point CFFT and CIFFT is as follows:
 *
 *      \#define FFT_LOGN		7
 *      \#define SAMPLES   (1 << FFT_LOGN)
 *
 *      riscv_vec_cpx_f32_t src[SAMPLES] = {…};
 *      riscv_vec_cpx_f32_t dst[SAMPLES];
 *      riscv_vec_cfft_cfg_f32_t cfg;
 *
 *      cfg = riscv_vec_cfft_init_f32(SAMPLES);
 *      if(cfg != NULL)
 *      {
 *          riscv_vec_cfft_f32(dst, src, cfg, 0);   // FOR CFFT
 *          riscv_vec_cfft_f32(dst, src, cfg, 1);   // FOR CIFFT
 *          riscv_vec_cfft_free_f32(cfg);
 *          return RISCV_VEC_OK;
 *      }
 *      else
 *      {
 *          return RISCV_VEC_FAIL;
 *      }
 *
 * This example also serves as a reference for examples of F16, F64, Q31 and Q15 CFFT and CIFFT functions.
 *     </pre>
 */
extern void  riscv_vec_cfft_f32(riscv_vec_cpx_f32_t *out,
                            riscv_vec_cpx_f32_t *in,
                            riscv_vec_cfft_cfg_f32_t cfg,
                            q31_t inverse_fft);

/**
 * @brief Double-precision Floating-point CFFT function
 * @param[out] out pointer of the output complex vector
 * @param[in] in pointer of the input complex vector
 * @param[in] cfg structure of the double-precision floating-point CFFT configuration
 * @param[in] inverse_fft an FFT or IFFT (0: FFT, 1: IFFT)
 *
 * @b Note:
 *
 * To ensure correct results, you must initialize the structure with the
 * function riscv_vec_cfft_init_f64 before using the function riscv_vec_cfft_f64
 * and free the structure with another function riscv_vec_cfft_free_f64 in the end.
 */
extern void  riscv_vec_cfft_f64(riscv_vec_cpx_f64_t *out,
                            riscv_vec_cpx_f64_t *in,
                            riscv_vec_cfft_cfg_f64_t cfg,
                            q31_t inverse_fft);

#if defined (__riscv_zfh)
/**
 * @brief Half-precision Floating-point CFFT function
 * @param[out] out pointer of the output complex vector
 * @param[in] in pointer of the input complex vector
 * @param[in] cfg structure of the half-precision floating-point CFFT configuration
 * @param[in] inverse_fft an FFT or IFFT (0: FFT, 1: IFFT)
 *
 * @b Note:
 *
 * 1. This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”.
 *
 * 2. To ensure correct results, you must initialize the structure with the
 *    function riscv_vec_cfft_init_f16 before using the function riscv_vec_cfft_f16
 *    and free the structure with another function riscv_vec_cfft_free_f16 in the end.
 */
extern void  riscv_vec_cfft_f16(riscv_vec_cpx_f16_t *out,
                            riscv_vec_cpx_f16_t *in,
                            riscv_vec_cfft_cfg_f16_t cfg,
                            q31_t inverse_fft);
#endif

/**
 * @brief Q31 CFFT function
 *
 * @param[out] out pointer of the output complex vector
 * @param[in] in pointer of the input complex vector
 * @param[in] cfg structure of the Q31 CFFT configuration
 * @param[in] inverse_fft an FFT or IFFT (0: FFT, 1: IFFT)
 * @param[in] scaled_flag enable/disable scaling down (1: enable scaling-down, 0:
 *                        disable scaling-down. Please refer to Note 2 below.)
 *
 * @b Note:
 *
 * 1. To ensure correct results, you must initialize the structure with the
 *    function riscv_vec_cfft_init_q31 before using the function riscv_vec_cfft_q31
 *    and free the structure with another function riscv_vec_cfft_free_q31 in the
 *    end. For detailed steps, please refer to the example in floating-point cfft.
 *
 * 2. When the input parameter scaled_flag is set to 1, the algorithm will perform
 *    scale-down calculation for each sample loop and produce a scaled-down result
 *    that won’t cause overflow. In contrast, when scaled_flag is set to 0, the algorithm
 *    will not perform scale-down calculation. To ensure that the input data has no risk of
 *    overflow in such case, you will need to do an arithmetic right shift before calling
 *    the function.
 */
extern void  riscv_vec_cfft_q31(riscv_vec_cpx_q31_t *out,
                            riscv_vec_cpx_q31_t *in,
                            riscv_vec_cfft_cfg_q31_t cfg,
                            q31_t inverse_fft,
                            q31_t scaled_flag);

/**
 * @brief Q15 CFFT function
 *
 * @param[out] out pointer of the output complex vector
 * @param[in] in pointer of the input complex vector
 * @param[in] cfg structure of the Q15 CFFT
 * @param[in] inverse_fft an FFT or IFFT (0: FFT, 1: IFFT)
 * @param[in] scaled_flag fractional scaling value
 *                        disable scaling-down. Please refer to Note 2 below.)
 *
 * @b Note:
 *
 * 1. To ensure correct results, you must initialize the structure with the
 *    function riscv_vec_cfft_init_q15 before using the function riscv_vec_cfft_q15
 *    and free the structure with another function riscv_vec_cfft_free_q15 in the
 *    end. For detailed steps, please refer to the example in floating-point cfft.
 *
 * 2. When the input parameter scaled_flag is set to 1, the algorithm will perform
 *    scale-down calculation for each sample loop and produce a scaled-down result
 *    that won’t cause overflow. In contrast, when scaled_flag is set to 0, the algorithm
 *    will not perform scale-down calculation. To ensure that the input data has no risk of
 *    overflow in such case, you will need to do an arithmetic right shift before calling
 *    the function.
 */
extern void  riscv_vec_cfft_q15(riscv_vec_cpx_q15_t *out,
                            riscv_vec_cpx_q15_t *in,
                            riscv_vec_cfft_cfg_q15_t cfg,
                            q31_t inverse_fft,
                            q31_t scaled_flag);

/**
 * @brief Function to free single-precision floating-point CFFT structure
 * @param[in] cfg structure of the single-precision floating-point CFFT configuration
 */
extern void riscv_vec_cfft_free_f32(riscv_vec_cfft_cfg_f32_t cfg);

/**
 * @brief Function to free double-precision floating-point CFFT structure
 * @param[in] cfg structure of the double-precision floating-point CFFT configuration
 */
extern void riscv_vec_cfft_free_f64(riscv_vec_cfft_cfg_f64_t cfg);

#if defined (__riscv_zfh)
/**
 * @brief Function to free half-precision floating-point CFFT structure
 * @param[in] cfg structure of the half-precision floating-point CFFT configuration
 */
extern void riscv_vec_cfft_free_f16(riscv_vec_cfft_cfg_f16_t cfg);
#endif

/**
 * @brief Function to free Q31 CFFT structure
 *
 * @param[in] cfg structure of the Q31 CFFT configuration
 */
extern void riscv_vec_cfft_free_q31(riscv_vec_cfft_cfg_q31_t cfg);

/**
 * @brief Function to free Q15 CFFT structure
 *
 * @param[in] cfg structure of the Q15 CFFT configuration
 */
extern void riscv_vec_cfft_free_q15(riscv_vec_cfft_cfg_q15_t cfg);

/** @} TransCfft */

/**
 * @defgroup TransRfft Real FFT Functions
 *
 * @brief RFFT Functions
 *
 * Real FFT functions include real fast Fourier transform (RFFT) and inverse fast
 * Fourier transform (RIFFT) functions, which transform signals composed of real
 * values from time domain to frequency domain.
 *
 * The RFFT algorithm treats the N real data in the input as N/2 complex and
 * performs a CFFT. After the CFFT is computed, the output layout of the RFFT
 * is thus arranged as [r[0], i[0], r[1], i[1], ..., r[N/2 - 1], i[N/2 - 1],
 * r[N/2]. i[N/2]] where r is real, i is imaginary and N is the size of the input data.
 *
 * As to the RIFFT, its input data has the same format as the output of the RFFT and
 * its output data after computation is converted from the frequency spectrum to the
 * time domain.
 *
 * Andes vector library supports distinct RFFT and RIFFT functions for the following data types: single-, double- and half-precision floating-point, Q31 and Q15. These functions are introduced in the subsections below. Note that overflow may occur in Q31 and Q15 RFFT and RIFFT functions. If you want to call any of these functions in which the input parameter scaled_flag is set to 0, make sure to do an arithmetic right shift beforehand so that the overflow can be avoided. For details about the input parameter scaled_flag for Q31 and Q15 RFFT/RIFFT functions.
 *
 * To ensure correct results for the RFFT or RIFFT function being used, you must initialize the structure with a corresponding RFFT initialization function (e.g., riscv_vec_rfft_init_f32) first and release the allocated memory for the structure with an associated free function (e.g., riscv_vec_rfft_free_f32) after the RFFT/RIFFT function is executed.
 *
 * The RFFT initialization function accepts a parameter nfft that defines the number of input samples and uses it to allocate memory internally. The needed memory size is determined according to the calculations listed in the following table. If there is no enough memory space for the allocation, the initialization function will return NULL to indicate the error.
 *
 * @image html rfft_mem.png ""
 */

 /**
  * @addtogroup TransRfft
  * @{
  */

/**
 * @brief Function to initialize single-precision floating-point RFFT
 *
 * @param[in] nfft number of input samples
 * @return Structure pointer of the single-precision floating-point RFFT configuration if a temporary buffer for the structure is allocated successfully, or NULL if the buffer allocation fails.
 *
 * @b Note:
 *
 * This function will allocate a temporary buffer to store the twiddle table, calculation results and some control flags. If the buffer allocation is not successful, the function will return NULL. The required buffer size varies depending on the number of input samples. For detailed memory usage of the buffer.
 */
extern riscv_vec_rfft_cfg_f32_t riscv_vec_rfft_init_f32(q31_t nfft);

/**
 * @brief Function to initialize double-precision floating-point RFFT
 *
 * @param[in] nfft number of input samples
 * @return Structure pointer of the double-precision floating-point RFFT configuration if a temporary buffer for the structure is allocated successfully, or NULL if the buffer allocation fails.
 *
 * @b Note:
 *
 * This function will allocate a temporary buffer to store the twiddle table, calculation results and some control flags. If the buffer allocation is not successful, the function will return NULL. The required buffer size varies depending on the number of input samples. For detailed memory usage of the buffer.
 */
extern riscv_vec_rfft_cfg_f64_t riscv_vec_rfft_init_f64(q31_t nfft);

#if defined (__riscv_zfh)
/**
 * @brief Function to initialize half-precision floating-point RFFT
 *
 * @param[in] nfft number of input samples
 * @return Structure pointer of the half-precision floating-point RFFT configuration if a temporary buffer for the structure is allocated successfully, or NULL if the buffer allocation fails.
 *
 * @b Note:
 *
 * 1.	This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”.
 *
 * 2.	This function will allocate a temporary buffer to store the twiddle table, calculation results and some control flags. If the buffer allocation is not successful, the function will return NULL. The required buffer size varies depending on the number of input samples. For detailed memory usage of the buffer.
 */
extern riscv_vec_rfft_cfg_f16_t riscv_vec_rfft_init_f16(q31_t nfft);
#endif

/**
 * @brief Function to initialize Q31 RFFT
 *
 * @param[in] nfft number of input samples
 * @return Structure pointer of the Q31 RFFT configuration if a temporary buffer for the structure is allocated successfully, or NULL if the buffer allocation fails.
 *
 * @b Note:
 *
 * This function will allocate a temporary buffer to store the twiddle table, calculation results and some control flags. If the buffer allocation is not successful, the function will return NULL. The required buffer size varies depending on the number of input samples. For detailed memory usage of the buffer.
 */
extern riscv_vec_rfft_cfg_q31_t riscv_vec_rfft_init_q31(q31_t nfft);

/**
 * @brief Function to initialize Q15 RFFT
 *
 * @param[in] nfft number of input sample
 * @return Structure pointer of the Q15 RFFT configuration if a temporary buffer for the structure is allocated successfully, or NULL if the buffer allocation fails.
 *
 * @b Note:
 *
 * This function will allocate a temporary buffer to store the twiddle table, calculation results and some control flags. If the buffer allocation is not successful, the function will return NULL. The required buffer size varies depending on the number of input samples. For detailed memory usage of the buffer.
 */
extern riscv_vec_rfft_cfg_q15_t riscv_vec_rfft_init_q15(q31_t nfft);

/**
 * @brief Single-precision floating-point RFFT function
 * @param[out] out pointer of the output complex vector
 * @param[in] in pointer of the input vector
 * @param[in] cfg structure of the single-precision floating-point RFFT
 *
 * @b Note:
 *
 * To ensure correct results, you must initialize the structure with the
 * function riscv_vec_rfft_init_f32 before using the function riscv_vec_rfft_f32
 * and free the structure with another function riscv_vec_rfft_free_f32 in the end.
 * For detailed steps, please refer to the example below.
 *
 * @b Example
 *     <pre>
 * Given 128 samples, the example of floating-point RFFT is as follows:
 *
 *      \#define FFT_LOGN		7
 *      \#define SAMPLES   (1 << FFT_LOGN)
 *
 *      float32_t src[SAMPLES] = {…};
 *      riscv_vec_cpx_f32_t dst[(SAMPLES / 2) + 1] = {0};
 *      riscv_vec_cfft_cfg_f32_t cfg;
 *
 *      cfg = riscv_vec_rfft_init_f32(SAMPLES);
 *      if(cfg != NULL)
 *      {
 *          riscv_vec_rfft_f32(dst, src, cfg);
 *          riscv_vec_rfft_free_f32(cfg);
 *          return RISCV_VEC_OK;
 *      }
 *      else
 *      {
 *          return RISCV_VEC_FAIL;
 *      }
 *
 * This example also serves as a reference for examples of F16, F64, Q31 and Q15 RFFT functions.
 *     </pre>
 */
extern void  riscv_vec_rfft_f32(riscv_vec_cpx_f32_t *out,
                            float32_t *in,
                            riscv_vec_rfft_cfg_f32_t cfg);

/**
 * @brief Double-precision floating-point RFFT function
 * @param[out] out pointer of the output complex vector
 * @param[in] in pointer of the input vector
 * @param[in] cfg structure of the Double-precision floating-point RFFT
 *
 * @b Note:
 *
 * To ensure correct results, you must initialize the structure with the function riscv_vec_rfft_init_f64 before using the function riscv_vec_rfft_f64 and free the structure with another function riscv_vec_rfft_free_f64 in the end.
 */
extern void  riscv_vec_rfft_f64(riscv_vec_cpx_f64_t *out,
                            float64_t *in,
                            riscv_vec_rfft_cfg_f64_t cfg);

#if defined (__riscv_zfh)
/**
 * @brief Half-precision floating-point RFFT function
 * @param[out] out pointer of the output complex vector
 * @param[in] in pointer of the input vector
 * @param[in] cfg structure of the Half-precision floating-point RFFT
 *
 * @b Note:
 *
 * 1.	This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”.
 *
 * 2.	To ensure correct results, you must initialize the structure with the function riscv_vec_rfft_init_f16 before using the function riscv_vec_rfft_f16 and free the structure with another function riscv_vec_rfft_free_f16 in the end.
 */
extern void  riscv_vec_rfft_f16(riscv_vec_cpx_f16_t *out,
                            float16_t *in,
                            riscv_vec_rfft_cfg_f16_t cfg);
#endif


/**
 * @brief Q31 RFFT function
 *
 * @param[out] out pointer of the output complex vector
 * @param[in] in pointer of the input vector
 * @param[in] cfg structure of the Q31 RFFT
 * @param[in] scaled_flag fractional scaling value
 *                        disable scaling-down. Please refer to Note 2 below.)
 *
 * @b Note:
 *
 * 1. To ensure correct results, you must initialize the structure with the
 *    function riscv_vec_rfft_init_q31 before using the function riscv_vec_rfft_q31
 *    and free the structure with another function riscv_vec_rfft_free_q31 in the
 *    end. For detailed steps, please refer to the example in floating-point rfft.
 *
 * 2. When the input parameter scaled_flag is set to 1, the algorithm will perform
 *    scale-down calculation for each sample loop and produce a scaled-down result
 *    that won’t cause overflow. In contrast, when scaled_flag is set to 0, the algorithm
 *    will not perform scale-down calculation. To ensure that the input data has no risk of
 *    overflow in such case, you will need to do an arithmetic right shift before calling
 *    the function.
 */
extern void  riscv_vec_rfft_q31(riscv_vec_cpx_q31_t *out,
                            q31_t *in,
                            riscv_vec_rfft_cfg_q31_t cfg,
                            q31_t scaled_flag);

/**
 * @brief Q15 RFFT function
 *
 * @param[out] out pointer of the output complex vector
 * @param[in] in pointer of the input vector
 * @param[in] cfg structure of the Q15 RFFT
 * @param[in] scaled_flag fractional scaling value
 *                        disable scaling-down. Please refer to Note 2 below.)
 *
 * @b Note:
 *
 * 1. To ensure correct results, you must initialize the structure with the
 *    function riscv_vec_rfft_init_q15 before using the function riscv_vec_rfft_q15
 *    and free the structure with another function riscv_vec_rfft_free_q15 in the
 *    end. For detailed steps, please refer to the example in floating-point rfft.
 *
 * 2. When the input parameter scaled_flag is set to 1, the algorithm will perform
 *    scale-down calculation for each sample loop and produce a scaled-down result
 *    that won’t cause overflow. In contrast, when scaled_flag is set to 0, the algorithm
 *    will not perform scale-down calculation. To ensure that the input data has no risk of
 *    overflow in such case, you will need to do an arithmetic right shift before calling
 *    the function.
 */
extern void  riscv_vec_rfft_q15(riscv_vec_cpx_q15_t *out,
                            q15_t *in,
                            riscv_vec_rfft_cfg_q15_t cfg,
                            q31_t scaled_flag);

/**
 * @brief Single-precision floating-point RIFFT function
 *
 * @param[out] out pointer of the output vector
 * @param[in] in pointer of the input complex vector
 * @param[in] cfg structure of the single-precision floating-point RFFT
 *
 * @b Note:
 *
 * To ensure correct results, you must initialize the structure with the
 * function riscv_vec_rfft_init_f32 before using the function riscv_vec_rifft_f32
 * and free the structure with another function riscv_vec_rfft_free_f32 in the end.
 * For detailed steps, please refer to the example below.
 *
 * @b Example
 *     <pre>
 * Given 128 samples, the example of floating-point RIFFT is as follows:
 *
 *      \#define FFT_LOGN		7
 *      \#define SAMPLES   (1 << FFT_LOGN)
 *
 *      riscv_vec_cpx_f32_t src[(SAMPLES / 2) + 1] = {…};
 *      float32_t dst[SAMPLES] = {0};
 *      riscv_vec_cfft_cfg_f32_t cfg;
 *
 *      cfg = riscv_vec_rfft_init_f32(SAMPLES);
 *      if(cfg != NULL)
 *      {
 *          riscv_vec_rifft_f32(dst, src, cfg);
 *          riscv_vec_rfft_free_f32(cfg);
 *          return RISCV_VEC_OK;
 *      }
 *      else
 *      {
 *          return RISCV_VEC_FAIL;
 *      }
 *
 * This example also serves as a reference for examples of F16, F64, Q31 and Q15 RIFFT functions.
 *     </pre>
 */
extern void  riscv_vec_rifft_f32(float32_t *out,
                            riscv_vec_cpx_f32_t *in,
                            riscv_vec_rfft_cfg_f32_t cfg);

/**
 * @brief Double-precision floating-point RIFFT function
 *
 * @param[out] out pointer of the output vector
 * @param[in] in pointer of the input complex vector
 * @param[in] cfg structure of the double-precision floating-point RFFT
 *
 * @b Note:
 *
 * To ensure correct results, you must initialize the structure with the function riscv_vec_rfft_init_f64 before using the function riscv_vec_rifft_f64 and free the structure with another function riscv_vec_rfft_free_f64 in the end.
 */
extern void  riscv_vec_rifft_f64(float64_t *out,
                            riscv_vec_cpx_f64_t *in,
                            riscv_vec_rfft_cfg_f64_t cfg);

#if defined (__riscv_zfh)
/**
 * @brief Half-precision floating-point RIFFT function
 *
 * @param[out] out pointer of the output vector
 * @param[in] in pointer of the input complex vector
 * @param[in] cfg structure of the half-precision floating-point RFFT
 *
 * @b Note:
 *
 * 1. This function is only supported by the toolchain with the HW FPU support. Also, you need to apply option “-mzfh” both in compiling flags and linking flags for applications that use this function. For details about the linking option“-mzfh”.
 *
 * 2. To ensure correct results, you must initialize the structure with the function riscv_vec_rfft_init_f16 before using the function riscv_vec_rifft_f16 and free the structure with another function riscv_vec_rfft_free_f16 in the end.
 */
extern void  riscv_vec_rifft_f16(float16_t *out,
                            riscv_vec_cpx_f16_t *in,
                            riscv_vec_rfft_cfg_f16_t cfg);
#endif

/**
 * @brief Q31 RIFFT function
 *
 * @param[out] out pointer of the output vector
 * @param[in] in pointer of the input complex vector
 * @param[in] cfg structure of the Q31 RFFT
 * @param[in] scaled_flag fractional scaling value
 *                        disable scaling-down. Please refer to Note 2 below.)
 *
 * @b Note:
 *
 * 1. To ensure correct results, you must initialize the structure with the
 *    function riscv_vec_rfft_init_q31 before using the function riscv_vec_rifft_q31
 *    and free the structure with another function riscv_vec_rfft_free_q31 in the
 *    end. For detailed steps, please refer to the example in floating-point rifft.
 *
 * 2. When the input parameter scaled_flag is set to 1, the algorithm will perform
 *    scale-down calculation for each sample loop and produce a scaled-down result
 *    that won’t cause overflow. In contrast, when scaled_flag is set to 0, the algorithm
 *    will not perform scale-down calculation. To ensure that the input data has no risk of
 *    overflow in such case, you will need to do an arithmetic right shift before calling
 *    the function.
 */
extern void  riscv_vec_rifft_q31(q31_t *out,
                            riscv_vec_cpx_q31_t *in,
                            riscv_vec_rfft_cfg_q31_t cfg,
                            q31_t scaled_flag);

/**
 * @brief Q15 RIFFT function
 *
 * @param[out] out pointer of the output vector
 * @param[in] in pointer of the input complex vector
 * @param[in] cfg structure of the Q15 RFFT
 * @param[in] scaled_flag fractional scaling value
 *                        disable scaling-down. Please refer to Note 2 below.)
 *
 * @b Note:
 *
 * 1. To ensure correct results, you must initialize the structure with the
 *    function riscv_vec_rfft_init_q15 before using the function riscv_vec_rifft_q15
 *    and free the structure with another function riscv_vec_rfft_free_q15 in the
 *    end. For detailed steps, please refer to the example in floating-point rifft.
 *
 * 2. When the input parameter scaled_flag is set to 1, the algorithm will perform
 *    scale-down calculation for each sample loop and produce a scaled-down result
 *    that won’t cause overflow. In contrast, when scaled_flag is set to 0, the algorithm
 *    will not perform scale-down calculation. To ensure that the input data has no risk of
 *    overflow in such case, you will need to do an arithmetic right shift before calling
 *    the function.
 */
extern void  riscv_vec_rifft_q15(q15_t *out,
                            riscv_vec_cpx_q15_t *in,
                            riscv_vec_rfft_cfg_q15_t cfg,
                            q31_t scaled_flag);
/**
 * @brief Function to free single-precision floating-point RFFT structure
 * @param[in] cfg structure of the single-precision floating-point RFFT configuration
 */
extern void riscv_vec_rfft_free_f32(riscv_vec_rfft_cfg_f32_t cfg);

/**
 * @brief Function to free double-precision floating-point RFFT structure
 * @param[in] cfg structure of the double-precision floating-point RFFT configuration
 */
extern void riscv_vec_rfft_free_f64(riscv_vec_rfft_cfg_f64_t cfg);

#if defined (__riscv_zfh)
/**
 * @brief Function to free half-precision floating-point RFFT structure
 * @param[in] cfg structure of the half-precision floating-point RFFT configuration
 */
extern void riscv_vec_rfft_free_f16(riscv_vec_rfft_cfg_f16_t cfg);
#endif

/**
 * @brief Function to free Q31 RFFT structure
 *
 * @param[in] cfg structure of the Q31 RFFT configuration
 */
extern void riscv_vec_rfft_free_q31(riscv_vec_rfft_cfg_q31_t cfg);

/**
 * @brief Function to free Q15 RFFT structure
 *
 * @param[in] cfg structure of the Q15 RFFT configuration
 */
extern void riscv_vec_rfft_free_q15(riscv_vec_rfft_cfg_q15_t cfg);

/** @} TransRfft */

/**
 * @defgroup TransDct2 DCT Type II Functions
 * @brief DCT Type II Functions
 *
 * Discrete cosine transform (DCT) type II functions implement DCT with the following equation:
 * @image html dct2_1.gif ""
 * and IDCT (DCT Type III, also called as the inverse of DCT II), with:
 * @image html dct2_2.gif ""
 * where c[0] = 1 / 2.0 and c[n] = 1 for n != 0.
 *
 * Andes vector library supports distinct DCT and IDCT functions for the following data types: F32, Q31, and Q15. These functions are described in the subsections below. For Q31 and Q15 DCT and IDCT functions, you may apply an arithmetic right shift before calling them.
 *
 * To ensure correct results when using the DCT or ICDT function, you must initialize the structure with the corresponding DCT initialization function (e.g., riscv_vec_dct_init_f32). After executing the DCT or IDCT function, release the allocated memory using the associated free function (e.g., riscv_vec_dct_free_f32).
 *
 * The DCT initialization function accepts a parameter n that defines the number of input samples and uses it to allocate memory internally. The required memory size is determined according to the calculations listed in the following table. If there is not enough memory available for the allocation, the initialization function returns NULL to indicate an error.
 * @image html dct2_3.gif ""
 */

/**
 * @addtogroup TransDct2
 * @{
 */

/**
 * @brief Function to initialize single-precision floating-point DCT
 *
 * @param[in] n number of input samples
 * @return Structure pointer of the single-precision floating-point DCT configuration if a temporary buffer for the structure is allocated sucessfully or NULL if the buffer allocation fails.
 */
extern riscv_vec_dct_cfg_f32_t riscv_vec_dct_init_f32(uint32_t n);

/**
 * @brief F32 DCT function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input complex vector
 * @param[in] cfg structure of the F32 DCT configuration
 */
extern void riscv_vec_dct_f32(float32_t *dst, float32_t *src, riscv_vec_dct_cfg_f32_t cfg);

/**
 * @brief F32 IDCT function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input complex vector
 * @param[in] cfg structure of the F32 DCT configuration
 */
extern void riscv_vec_idct_f32(float32_t *dst, float32_t *src, riscv_vec_dct_cfg_f32_t cfg);

/**
 * @brief Function to free F32 DCT structure
 *
 * @param[in] cfg structure of the F32 DCT configuration
 */
extern void riscv_vec_dct_free_f32(riscv_vec_dct_cfg_f32_t cfg);

/**
 * @brief Function to initialize Q15 DCT
 *
 * @param[in] n number of input samples
 * @return Structure pointer of the single-precision Q7 DCT configuration if a temporary buffer for the structure is allocated sucessfully or NULL if the buffer allocation fails.
 */
extern riscv_vec_dct_cfg_q15_t riscv_vec_dct_init_q15(uint32_t n);

/**
 * @brief Q15 DCT function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cfg structure of the Q15 DCT configuration
 */
extern void riscv_vec_dct_q15(q15_t *dst, q15_t *src, riscv_vec_dct_cfg_q15_t cfg);

/**
 * @brief Q15 IDCT function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cfg structure of the Q15 DCT configuration
 */
extern void riscv_vec_idct_q15(q15_t *dst, q15_t *src, riscv_vec_dct_cfg_q15_t cfg);

/**
 * @brief Function to free Q15 DCT structure
 *
 * @param[in] cfg structure of the Q15 DCT configuration
 */
extern void riscv_vec_dct_free_q15(riscv_vec_dct_cfg_q15_t cfg);

/**
 * @brief Function to initialize Q31 DCT
 *
 * @param[in] n number of input samples
 * @return Structure pointer of the single-precision Q7 DCT configuration if a temporary buffer for the structure is allocated sucessfully or NULL if the buffer allocation fails.
 */
extern riscv_vec_dct_cfg_q31_t riscv_vec_dct_init_q31(uint32_t n);

/**
 * @brief Q31 DCT function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cfg structure of the Q31 DCT configuration
 */
extern void riscv_vec_dct_q31(q31_t *dst, q31_t *src, riscv_vec_dct_cfg_q31_t cfg);

/**
 * @brief Q31 IDCT function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cfg structure of the Q31 DCT configuration
 */
extern void riscv_vec_idct_q31(q31_t *dst, q31_t *src, riscv_vec_dct_cfg_q31_t cfg);

/**
 * @brief Function to free Q31 DCT structure
 *
 * @param[in] cfg structure of the Q31 DCT configuration
 */
extern void riscv_vec_dct_free_q31(riscv_vec_dct_cfg_q31_t cfg);

/** @} TransDct2 */

/**
 * @defgroup TransDct4 DCT Type IV Functions
 * @brief DCT Type IV Functions
 *
 * Discrete cosine transform type IV (DCT4) functions implement DCT transform with the following equation:
 * @image html dct4_1.gif ""
 * and IDCT4 with:
 * @image html dct4_2.gif ""
 *
 * Andes vector library supports distinct DCT4 and IDCT4 functions for the following data types: F32, Q31 and Q15. These functions are described in the subsections below. For Q31 and Q15 DCT4 and IDCT4 functions, you may apply an arithmetic right shift before calling them.
 *
 * To ensure correct results when using the DCT4 or ICDT4 function, you must initialize the structure with the corresponding DCT4 initialization function (e.g., riscv_vec_dct4_init_f32). After executing the DCT4 or IDCT4 function, release the allocated memory with the associated free function (e.g., riscv_vec_dct4_free_f32).
 *
 * The DCT4 initialization function accepts a parameter n that defines the number of input samples and uses it to allocate memory internally. The required memory size is determined according to the calculations listed in the following table. If there is not enough memory available for the allocation, the initialization function returns NULL to indicate an error.
 *
 * @image html dct4_3.gif ""
 */

/**
 * @addtogroup TransDct4
 * @{
 */

/**
 * @brief Function to initialize single-precision floating-point DCT
 *
 * @param[in] n number of input samples
 * @return Structure pointer of the single-precision floating-point DCT configuration if a temporary buffer for the structure is allocated sucessfully or NULL if the buffer allocation fails.
 */
extern riscv_vec_dct4_cfg_f32_t riscv_vec_dct4_init_f32(uint32_t n);

/**
 * @brief F32 DCT4 function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input complex vector
 * @param[in] cfg structure of the F32 DCT configuration
 */
extern void riscv_vec_dct4_f32(float32_t *dst, float32_t *src, riscv_vec_dct4_cfg_f32_t cfg);

/**
 * @brief F32 IDCT4 function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input complex vector
 * @param[in] cfg structure of the F32 DCT configuration
 */
extern void riscv_vec_idct4_f32(float32_t *dst, float32_t *src, riscv_vec_dct4_cfg_f32_t cfg);

/**
 * @brief Function to free F32 DCT structure
 *
 * @param[in] cfg structure of the F32 DCT configuration
 */
extern void riscv_vec_dct4_free_f32(riscv_vec_dct4_cfg_f32_t cfg);

/**
 * @brief Function to initialize Q15 DCT
 *
 * @param[in] n number of input samples
 * @return Structure pointer of the Q15 DCT configuration if a temporary buffer for the structure is allocated sucessfully or NULL if the buffer allocation fails.
 */
extern riscv_vec_dct4_cfg_q15_t riscv_vec_dct4_init_q15(uint32_t n);

/**
 * @brief Q15 DCT4 function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cfg structure of the Q15 DCT configuration
 */
extern void riscv_vec_dct4_q15(q15_t *dst, q15_t *src, riscv_vec_dct4_cfg_q15_t cfg);

/**
 * @brief Q15 IDCT4 function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cfg structure of the Q15 DCT configuration
 */
extern void riscv_vec_idct4_q15(q15_t *dst, q15_t *src, riscv_vec_dct4_cfg_q15_t cfg);

/**
 * @brief Function to free Q15 DCT structure
 *
 * @param[in] cfg structure of the Q15 DCT configuration
 */
extern void riscv_vec_dct4_free_q15(riscv_vec_dct4_cfg_q15_t cfg);

/**
 * @brief Function to initialize Q31 DCT
 *
 * @param[in] n number of input samples
 * @return Structure pointer of the Q31 DCT configuration if a temporary buffer for the structure is allocated sucessfully or NULL if the buffer allocation fails.
 */
extern riscv_vec_dct4_cfg_q31_t riscv_vec_dct4_init_q31(uint32_t n);

/**
 * @brief Q31 DCT4 function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cfg structure of the Q31 DCT configuration
 */
extern void riscv_vec_dct4_q31(q31_t *dst, q31_t *src, riscv_vec_dct4_cfg_q31_t cfg);

/**
 * @brief Q31 IDCT4 function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cfg structure of the Q31 DCT configuration
 */
extern void riscv_vec_idct4_q31(q31_t *dst, q31_t *src, riscv_vec_dct4_cfg_q31_t cfg);

/**
 * @brief Function to free Q31 DCT structure
 *
 * @param[in] cfg structure of the Q31 DCT configuration
 */
extern void riscv_vec_dct4_free_q31(riscv_vec_dct4_cfg_q31_t cfg);

/** @} TransDct4 */

/**
 * @defgroup TransDct2d DCT 2D 8x8 Functions
 * @brief DCT 2D 8x8 Functions
 *
 * These functions compute the 2-Dimensional (2D) Discrete Cosine Transform (DCT2D) of an 8x8 input matrix.
 *
 * Andes vector library supports distinct DCT2D_8x8 and IDCT2D_8x8 functions for the following data types: Q31 and Q15. These functions are described in the subsections below.
 *
 * To ensure correct results when using the DCT2D_8x8 or ICDT2D_8x8 function, you must initialize the structure with the corresponding DCT2D_8x8 initialization function (e.g., riscv_vec_dct2d_8x8_init_q31). After executing the DCT2D_8x8 / IDCT2D_8x8 function, release the allocated memory using the associated free function (e.g., riscv_vec_dct2d_8x8_free_q31).
 *
 * The data types for Q15 can be derived as two algorithms, for the original (riscv_vec_dct2d_8x8_q15) and fast (riscv_vec_dct2d_8x8_fast_q15). The fast algorithm only speeds up in the library with ELEN64 (-lvec_v_elen64 and -lvec_v_elen64_seg).
 *
 * The required memory size is determined according to the calculations listed in the following table. If there is not enough memory available for the allocation, the initialization function returns NULL to indicate an error.
 */

/**
 * @addtogroup TransDct2d
 * @{
 */

/**
 * @brief Function to initialize Q31 DCT2D_8x8
 *
 * @return Structure pointer of the Q31 DCT2D_8x8 configuration if a temporary buffer for the structure is allocated sucessfully or NULL if the buffer allocation fails.
 */
extern riscv_vec_dct2d_8x8_cfg_q31_t riscv_vec_dct2d_8x8_init_q31();

/**
 * @brief Q31 DCT2D_8x8 function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cfg structure of the Q31 DCT2D_8x8 configuration
 */
extern void riscv_vec_dct2d_8x8_q31(q31_t *dst, q31_t *src, riscv_vec_dct2d_8x8_cfg_q31_t cfg);

/**
 * @brief IDCT2D_8X8 function for Q31
 *
 * @param[out] dst Points to the output vector
 * @param[in] src pointer of the input vector
 * @param[in] dct_table pointer of the dct table
 * @param[in] cfg structure of the Q31 DCT2D_8x8 configuration
 */
extern void riscv_vec_idct2d_8x8_q31(u8_t *dst, q31_t *src, q31_t *dct_table, riscv_vec_dct2d_8x8_cfg_q31_t cfg);

/**
 * @brief Function to initialize Q15 DCT2D_8x8
 *
 * @return Structure pointer of the Q15 DCT2D_8x8 configuration if a temporary buffer for the structure is allocated sucessfully or NULL if the buffer allocation fails.
 */
extern riscv_vec_dct2d_8x8_cfg_q15_t riscv_vec_dct2d_8x8_init_q15();

/**
 * @brief Function to initialize Q15 DCT2D_8x8_fast
 *
 * @return Structure pointer of the Q15 DCT2D_8x8_fast configuration if a temporary buffer for the structure is allocated sucessfully or NULL if the buffer allocation fails.
 */
extern riscv_vec_dct2d_8x8_cfg_q15_t riscv_vec_dct2d_8x8_fast_init_q15();

/**
 * @brief Q15 DCT2D_8x8 function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cfg structure of the Q15 DCT2D_8x8 configuration
 */
extern void riscv_vec_dct2d_8x8_q15(q15_t *dst, q15_t *src, riscv_vec_dct2d_8x8_cfg_q15_t cfg);

/**
 * @brief Q15 DCT2D_8x8_fast function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cfg structure of the Q15 DCT2D_8x8 configuration
 */
extern void riscv_vec_dct2d_8x8_fast_q15(q15_t *dst, q15_t *src, riscv_vec_dct2d_8x8_cfg_q15_t cfg);

/**
 * @brief Q15 IDCT2D_8x8 function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] dct_table pointer of the dct table
 * @param[in] cfg structure of the Q15 DCT2D_8x8 configuration
 */
extern void riscv_vec_idct2d_8x8_q15(u8_t *dst, q15_t *src, q15_t *dct_table, riscv_vec_dct2d_8x8_cfg_q15_t cfg);

/**
 * @brief Q15 IDCT2D_8x8_fast function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] dct_table pointer of the dct table
 * @param[in] cfg structure of the Q15 DCT2D_8x8 configuration
 */
extern void riscv_vec_idct2d_8x8_fast_q15(u8_t *dst, q15_t *src, q15_t *dct_table, riscv_vec_dct2d_8x8_cfg_q15_t cfg);

/**
 * @brief Function to free Q15 DCT2D_8x8 structure
 *
 * @param[in] cfg structure of the Q15 DCT configuration
 */
extern void riscv_vec_dct2d_8x8_free_q15(riscv_vec_dct2d_8x8_cfg_q15_t cfg);

/**
 * @brief Function to free Q31 DCT2D_8x8 structure
 *
 * @param[in] cfg structure of the Q31 DCT configuration
 */
extern void riscv_vec_dct2d_8x8_free_q31(riscv_vec_dct2d_8x8_cfg_q31_t cfg);

/** @} TransDct2d */

/**
 * @defgroup TransMFCC Mel-Frequency Cepstral Coefficients (MFCCs) Transform Functions
 * @brief Mel-Frequency Cepstral Coefficients (MFCCs) Transform Functions
 *
 * Mel-Frequency Cepstral Coefficients (MFCCs) are features that collectively form a Mel-Frequency Cepstrum (MFC). They are commonly derived from a cepstral representation of an audio clip through the following process.
 *
 *     - Perform a Fourier transform on a windowed segment of a signal.
 *     - Map the resulting power spectrum onto the mel scale using triangular or cosine overlapping windows.
 *     - Take the logs of the powers at each mel frequency.
 *     - Apply the discrete cosine transform (DCT) to the list of mel log powers, treating it as a signal.
 *     - The MFCCs are the amplitudes of the resulting spectrum.
 * To ensure correct results when using the MFCC function, you must initialize the structure with the corresponding initialization function (e.g., riscv_vec_mfcc_init_f32) before calling the MFCC function (e.g., riscv_vec_mfcc_f32).
 *
 * Andes vector library supports the following data types: single- and half-precision floating-point, Q31, and Q15. These functions are described in the subsections below.
 */

/**
 * @addtogroup TransMFCC
 * @{
 */

/**
 * @brief Structure for the single-precision floating-point MFCC state
 */
typedef struct
{
    float32_t *dct_coefs;              /**< Internal DCT coefficients */
    float32_t *filter_coefs;           /**< Internal Mel filter coefficients */
    float32_t *window_coefs;           /**< Windowing coefficients */
    uint32_t *filter_pos;              /**< Internal Mel filter positions in spectrum */
    uint32_t *filter_len;              /**< Internal Mel filter  lengths */
    uint32_t fft_len;                  /**< FFT length */
    uint32_t n_mel_filters;            /**< Number of Mel filters */
    uint32_t n_dct_out;                /**< Number of DCT outputs */
    riscv_vec_rfft_cfg_f32_t rfft_cfg; /**< Points to the RFFT structure configuration */
} riscv_vec_mfcc_state_f32_t;

/**
 * @brief Structure pointer for riscv_vec_mfcc_state_f32_t
 */
typedef riscv_vec_mfcc_state_f32_t* riscv_vec_mfcc_cfg_f32_t;

/**
 * @brief Function to initialize F32 MFCC.
 * @param[in]       fft_len          RFFT length
 * @param[in]       n_mel_filters    number of Mel filters
 * @param[in]       n_dct_out        number of DCT outputs
 * @param[in]       *dct_coefs       pointer of the DCT coefficients vector
 * @param[in]       *filter_pos      pointer of the Mel filter positions vector
 * @param[in]       *filter_len      pointer of the Mel filter lengths vector
 * @param[in]       *filter_coefs    pointer of the Mel filter coefficients vector
 * @param[in]       *window_coefs   pointer of the window coefficients vector
 * @return A pointer to the F32 MFCC configuration structure if the temporary buffer is allocated successfully, or NULL if the buffer allocation fails..
 */
extern riscv_vec_mfcc_cfg_f32_t riscv_vec_mfcc_init_f32(uint32_t fft_len,
                                                        uint32_t n_mel_filters,
                                                        uint32_t n_dct_out,
                                                        float32_t *dct_coefs,
                                                        uint32_t *filter_pos,
                                                        uint32_t *filter_len,
                                                        float32_t *filter_coefs,
                                                        float32_t *window_coefs);

/**
 * @brief F32 MFCC function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input complex vector
 * @param[in] buf pointer of the buffer
 * @param[in] cfg structure of the F32 MFCC configuration
 * @return none
 *
 * To ensure correct results, initialize the structure using riscv_vec_mfcc_init_f32 before calling riscv_vec_mfcc_f32, and free the structure afterward with riscv_vec_mfcc_free_f32.
 */
extern void riscv_vec_mfcc_f32(float32_t *dst, float32_t *src, float32_t *buf, riscv_vec_mfcc_cfg_f32_t cfg);

/**
 * @brief Function to free F32 MFCC structure
 *
 * @param[in] cfg structure of the F32 MFCC configuration
 */
extern void riscv_vec_mfcc_free_f32(riscv_vec_mfcc_cfg_f32_t cfg);


/**
 * @brief Structure for the q31 floating-point MFCC state
 */
typedef struct
{
    q31_t *dct_coefs;                   /**< Internal DCT coefficients */
    q31_t *filter_coefs;                /**< Internal Mel filter coefficients */
    q31_t *window_coefs;                /**< Windowing coefficients */
    uint32_t *filter_pos;               /**< Internal Mel filter positions in spectrum */
    uint32_t *filter_len;               /**< Internal Mel filter  lengths */
    uint32_t fft_len;                   /**< FFT length */
    uint32_t log2_fft_len;              /**< log 2 of FFT length */
    uint32_t n_mel_filters;             /**< Number of Mel filters */
    uint32_t n_dct_out;                 /**< Number of DCT outputs */
    riscv_vec_rfft_cfg_q31_t rfft_cfg;  /**< Points to the RFFT structure configuration */
} riscv_vec_mfcc_state_q31_t;

/**
 * @brief Structure pointer for riscv_vec_mfcc_state_q31_t
 */
typedef riscv_vec_mfcc_state_q31_t* riscv_vec_mfcc_cfg_q31_t;

/**
 * @brief Function to initialize Q31 MFCC.
 * @param[in]       fft_len          RFFT length
 * @param[in]       n_mel_filters    number of Mel filters
 * @param[in]       n_dct_out        number of DCT outputs
 * @param[in]       *dct_coefs       pointer of the DCT coefficients vector
 * @param[in]       *filter_pos      pointer of the Mel filter positions vector
 * @param[in]       *filter_len      pointer of the Mel filter lengths vector
 * @param[in]       *filter_coefs    pointer of the Mel filter coefficients vector
 * @param[in]       *window_coefs   pointer of the window coefficients vector
 * @return A pointer to the Q31 MFCC configuration structure if the temporary buffer is allocated successfully, or NULL if the buffer allocation fails..
 */
extern riscv_vec_mfcc_cfg_q31_t riscv_vec_mfcc_init_q31(uint32_t fft_len,
                                                        uint32_t n_mel_filters,
                                                        uint32_t n_dct_out,
                                                        q31_t *dct_coefs,
                                                        uint32_t *filter_pos,
                                                        uint32_t *filter_len,
                                                        q31_t *filter_coefs,
                                                        q31_t *window_coefs);

/**
 * @brief Q31 MFCC function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input complex vector
 * @param[in] buf pointer of the buffer
 * @param[in] cfg structure of the F32 MFCC configuration
 * @return none
 *
 * To ensure correct results, initialize the structure using riscv_vec_mfcc_init_q31 before calling riscv_vec_mfcc_q31, and free the structure afterward with riscv_vec_mfcc_free_q31.
 */
extern void riscv_vec_mfcc_q31(q31_t *dst, q31_t *src, q31_t *buf, riscv_vec_mfcc_cfg_q31_t cfg);

/**
 * @brief Function to free q31 MFCC structure
 *
 * @param[in] cfg structure of the q31 MFCC configuration
 */
extern void riscv_vec_mfcc_free_q31(riscv_vec_mfcc_cfg_q31_t cfg);


/**
 * @brief Structure for the q15 floating-point MFCC state
 */
typedef struct
{
    q15_t *dct_coefs;                   /**< Internal DCT coefficients */
    q15_t *filter_coefs;                /**< Internal Mel filter coefficients */
    q15_t *window_coefs;                /**< Windowing coefficients */
    uint32_t *filter_pos;               /**< Internal Mel filter positions in spectrum */
    uint32_t *filter_len;               /**< Internal Mel filter  lengths */
    uint32_t fft_len;                   /**< FFT length */
    uint32_t log2_fft_len;              /**< log 2 of FFT length */
    uint32_t n_mel_filters;             /**< Number of Mel filters */
    uint32_t n_dct_out;                 /**< Number of DCT outputs */
    riscv_vec_rfft_cfg_q15_t rfft_cfg;  /**< Points to the RFFT structure configuration */
} riscv_vec_mfcc_state_q15_t;

/**
 * @brief Structure pointer for riscv_vec_mfcc_state_q15_t
 */
typedef riscv_vec_mfcc_state_q15_t* riscv_vec_mfcc_cfg_q15_t;

/**
 * @brief Function to initialize Q15 MFCC.
 * @param[in]       fft_len          RFFT length
 * @param[in]       n_mel_filters    number of Mel filters
 * @param[in]       n_dct_out        number of DCT outputs
 * @param[in]       *dct_coefs       pointer of the DCT coefficients vector
 * @param[in]       *filter_pos      pointer of the Mel filter positions vector
 * @param[in]       *filter_len      pointer of the Mel filter lengths vector
 * @param[in]       *filter_coefs    pointer of the Mel filter coefficients vector
 * @param[in]       **window_coefs   pointer of the window coefficients vector
 * @return A pointer to the Q15 MFCC configuration structure if the temporary buffer is allocated successfully, or NULL if the buffer allocation fails..
 */
extern riscv_vec_mfcc_cfg_q15_t riscv_vec_mfcc_init_q15(uint32_t fft_len,
                                                        uint32_t n_mel_filters,
                                                        uint32_t n_dct_out,
                                                        q15_t *dct_coefs,
                                                        uint32_t *filter_pos,
                                                        uint32_t *filter_len,
                                                        q15_t *filter_coefs,
                                                        q15_t *window_coefs);

/**
 * @brief Q15 MFCC function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input complex vector
 * @param[in] buf pointer of the buffer
 * @param[in] cfg structure of the F32 MFCC configuration
 * @return none
 *
 * To ensure correct results, initialize the structure using riscv_vec_mfcc_init_q15 before calling riscv_vec_mfcc_q15, and free the structure afterward with riscv_vec_mfcc_free_q15.
 */
extern void riscv_vec_mfcc_q15(q15_t *dst, q15_t *src, q31_t *buf, riscv_vec_mfcc_cfg_q15_t cfg);

/**
 * @brief Function to free q15 MFCC structure
 *
 * @param[in] cfg structure of the q15 MFCC configuration
 */
extern void riscv_vec_mfcc_free_q15(riscv_vec_mfcc_cfg_q15_t cfg);

#if defined (__riscv_zfh)
/**
 * @brief Structure for the half-precision floating-point MFCC state
 */
typedef struct
{
    float16_t *dct_coefs;               /**< Internal DCT coefficients */
    float16_t *filter_coefs;            /**< Internal Mel filter coefficients */
    float16_t *window_coefs;            /**< Windowing coefficients */
    uint32_t *filter_pos;               /**< Internal Mel filter positions in spectrum */
    uint32_t *filter_len;               /**< Internal Mel filter  lengths */
    uint32_t fft_len;                   /**< FFT length */
    uint32_t n_mel_filters;             /**< Number of Mel filters */
    uint32_t n_dct_out;                 /**< Number of DCT outputs */
    riscv_vec_rfft_cfg_f16_t rfft_cfg;  /**< Points to the RFFT structure configuration */
} riscv_vec_mfcc_state_f16_t;

/**
 * @brief Structure pointer for riscv_vec_mfcc_state_f16_t
 */
typedef riscv_vec_mfcc_state_f16_t* riscv_vec_mfcc_cfg_f16_t;

/**
 * @brief Function to initialize single-precision floating-point MFCC
 *
 * @param[in] n number of input samples
 * @return Structure pointer of the single-precision floating-point MFCC configuration if a temporary buffer for the structure is allocated sucessfully or NULL if the buffer allocation fails.
 */
extern riscv_vec_mfcc_cfg_f16_t riscv_vec_mfcc_init_f16(uint32_t fft_len,
                                                        uint32_t n_mel_filters,
                                                        uint32_t n_dct_out,
                                                        float16_t *dct_coefs,
                                                        uint32_t *filter_pos,
                                                        uint32_t *filter_len,
                                                        float16_t *filter_coefs,
                                                        float16_t *window_coefs);

/**
 * @brief F16 MFCC function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input complex vector
 * @param[in] buf pointer of the buffer
 * @param[in] cfg structure of the F16 MFCC configuration
 */
extern void riscv_vec_mfcc_f16(float16_t *dst, float16_t *src, float16_t *buf, riscv_vec_mfcc_cfg_f16_t cfg);

/**
 * @brief Function to free F16 MFCC structure
 *
 * @param[in] cfg structure of the F16 MFCC configuration
 */
extern void riscv_vec_mfcc_free_f16(riscv_vec_mfcc_cfg_f16_t cfg);
#endif

/** @} TransMFCC */
/** @} groupTrans */

#ifdef __cplusplus
}
#endif

#endif

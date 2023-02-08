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


/**
 * @brief Structure pointer for riscv_vec_cfft_state_f32_t
 */
typedef riscv_vec_cfft_state_f32_t* riscv_vec_cfft_cfg_f32_t;

/**
 * @brief Structure pointer for riscv_vec_cfft_state_f64_t
 */
typedef riscv_vec_cfft_state_f64_t* riscv_vec_cfft_cfg_f64_t;

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

/**
 * @brief Structure pointer for riscv_vec_rfft_state_f32_t
 */
typedef riscv_vec_rfft_state_f32_t* riscv_vec_rfft_cfg_f32_t;

/**
 * @brief Structure pointer for riscv_vec_rfft_state_f64_t
 */
typedef riscv_vec_rfft_state_f64_t* riscv_vec_rfft_cfg_f64_t;

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

/** @} */

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
 * Andes vector library supports distinct CFFT and CIFFT functions for the following data types: floating-point, Q31 and Q15. These functions are introduced in the subsections below. Note that overflow may occur in Q31 and Q15 CFFT and CIFFT functions. If you want to call any of these functions in which the input parameter scaled_flag is set to 0, make sure to do an arithmetic right shift beforehand so that the overflow can be avoided. For details about the input parameter scaled_flag for Q31 and Q15 CFFT/CIFFT functions.
 *
 * To ensure correct results, you must initialize the structure with the CFFT initialization function (e.g., riscv_vec_cfft_init_f32) before using the CFFT function and release the allocated memory of structure with CFFT free functions (e.g., riscv_vec_cfft_free_f32) in the end.
 *
 * The CFFT initialization function accepts a parameter, the number of input samples(nfft) and uses it to allocate memory internally. The needed memory sizes are listed as the following table. If there is not enough memory space to allocate, the initialization function return NULL (i.e., it means error).
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
 * @return Structure pointer of single-precision floating-point CFFT configuration
 *
 * @b Note:
 *
 * This function will allocate a temporary buffer used to store the twiddle table, temporary
calculate results and some control flags. If the buffer allocation is not successful the
function will return NULL. The buffer size will depend on the number of
input samples and the detail memory usage please refer to cfft function descriptions.
 */
extern riscv_vec_cfft_cfg_f32_t riscv_vec_cfft_init_f32(q31_t nfft);

/**
 * @brief Function to initialize double-precision floating-point CFFT
 *
 * @param[in] nfft number of input samples
 * @return Structure pointer of double-precision floating-point CFFT configuration
 *
 * @b Note:
 *
 * This function will allocate a temporary buffer used to store the twiddle table, temporary
calculate results and some control flags. If the buffer allocation is not successful the
function will return NULL. The buffer size will depend on the number of
input samples and the detail memory usage please refer to cfft function descriptions.
 */
extern riscv_vec_cfft_cfg_f64_t riscv_vec_cfft_init_f64(q31_t nfft);

/**
 * @brief Function to initialize Q31 CFFT
 *
 * @param[in] nfft number of input samples
 * @return Structure pointer of Q31 CFFT configuration
 *
 * @b Note:
 *
 * This function will allocate a temporary buffer used to store the twiddle table, temporary
calculate results and some control flags. If the buffer allocation is not successful the
function will return NULL. The buffer size will depend on the number of
input samples and the detail memory usage please refer to cfft function descriptions.
 */
extern riscv_vec_cfft_cfg_q31_t riscv_vec_cfft_init_q31(q31_t nfft);

/**
 * @brief Function to initialize Q15 CFFT
 *
 * @param[in] nfft number of input samples
 * @return Structure pointer of Q15 CFFT configuration
 *
 * @b Note:
 *
 * This function will allocate a temporary buffer used to store the twiddle table, temporary
calculate results and some control flags. If the buffer allocation is not successful the
function will return NULL. The buffer size will depend on the number of
input samples and the detail memory usage please refer to cfft function descriptions.
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
 *      riscv_vec_cfft_f32(dst, src, cfg, 0);   // FOR CFFT
 *      riscv_vec_cfft_f32(dst, src, cfg, 1);   // FOR CIFFT
 *      riscv_vec_cfft_free_f32(cfg);
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
 * Andes vector library supports distinct RFFT and RIFFT functions for the following data types: floating-point, Q31 and Q15. These functions are introduced in the subsections below. Note that overflow may occur in Q31 and Q15 RFFT and RIFFT functions. If you want to call any of these functions in which the input parameter scaled_flag is set to 0, make sure to do an arithmetic right shift beforehand so that the overflow can be avoided. For details about the input parameter scaled_flag for Q31 and Q15 RFFT /RIFFT functions.
 *
 * To ensure correct results, you must initialize the structure with the RFFT initialization function (e.g., riscv_vec_rfft_init_f32) before using the RFFT/RIFFT function and release the allocated memory of structure with RFFT free functions (e.g., riscv_vec_rfft_free_f32) in the end.
 *
 * The RFFT initialization function accepts a parameter, the number of input samples(nfft) and uses it to allocate memory internally. The needed memory sizes are listed as the following table. If there is not enough memory space to allocate, the initialization function return NULL (i.e., it means error).
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
 * @return Structure pointer of single-precision floating-point RFFT configuration
 *
 * @b Note:
 *
 * This function will allocate a temporary buffer used to store the twiddle table, temporary
calculate results and some control flags. If the buffer allocation is not successful the
function will return NULL. The buffer size will depend on the number of
input samples and the detail memory usage please refer to the RFFT function description.
 */
extern riscv_vec_rfft_cfg_f32_t riscv_vec_rfft_init_f32(q31_t nfft);

/**
 * @brief Function to initialize double-precision floating-point RFFT
 *
 * @param[in] nfft number of input samples
 * @return Structure pointer of double-precision floating-point RFFT configuration
 *
 * @b Note:
 *
 * This function will allocate a temporary buffer used to store the twiddle table, temporary
calculate results and some control flags. If the buffer allocation is not successful the
function will return NULL. The buffer size will depend on the number of
input samples and the detail memory usage please refer to the RFFT function description.
 */
extern riscv_vec_rfft_cfg_f64_t riscv_vec_rfft_init_f64(q31_t nfft);

/**
 * @brief Function to initialize Q31 RFFT
 *
 * @param[in] nfft number of input samples
 * @return Structure pointer of Q31 RFFT configuration
 *
 * @b Note:
 *
 * This function will allocate a temporary buffer used to store the twiddle table, temporary
calculate results and some control flags. If the buffer allocation is not successful the
function will return NULL. The buffer size will depend on the number of
input samples and the detail memory usage please refer to the RFFT function description.
 */
extern riscv_vec_rfft_cfg_q31_t riscv_vec_rfft_init_q31(q31_t nfft);

/**
 * @brief Function to initialize Q15 RFFT
 *
 * @param[in] nfft number of input sample
 * @return Structure pointer of Q15 RFFT configuration
 *
 * @b Note:
 *
 * This function will allocate a temporary buffer used to store the twiddle table, temporary
calculate results and some control flags. If the buffer allocation is not successful the
function will return NULL. The buffer size will depend on the number of
input samples and the detail memory usage please refer to the RFFT function description.
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
 *      riscv_vec_rfft_f32(dst, src, cfg);
 *      riscv_vec_rfft_free_f32(cfg);
 *
 * This example also serves as a reference for examples of f16, f64, Q31 and Q15 RFFT functions.
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
 * To ensure correct results, you must initialize the structure with the function riscv_vec_rfft_init_f64 before using the function riscv_vec_rfft_f64 and free the structure with another function riscv_vec_rfft_free_f64 in the end. For detailed steps/
 */
extern void  riscv_vec_rfft_f64(riscv_vec_cpx_f64_t *out,
                            float64_t *in,
                            riscv_vec_rfft_cfg_f64_t cfg);


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
 *      riscv_vec_rifft_f32(dst, src, cfg);
 *      riscv_vec_rfft_free_f32(cfg);
 *
 * This example also serves as a reference for examples of Q31 and Q15 RIFFT functions.
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

/** @} TransRfft*/

/** @} */

#ifdef __cplusplus
}
#endif

#endif

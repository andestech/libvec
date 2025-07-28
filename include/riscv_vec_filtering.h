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
#ifndef __RISCV_VEC_FILTERING_H__
#define __RISCV_VEC_FILTERING_H__

/** @file*/

#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_math_types.h"
#include <riscv_vec_types.h>

/**
 * @defgroup groupFilter Filtering Functions
 */

/**
  * @addtogroup groupFilter
  * @{
  */

/**
 * @defgroup FilterFIR Finite Impulse Response (FIR) Filter Functions
 *
 * @brief Finite impulse response (FIR) filter functions
 *
 * Finite impulse response (FIR) filter functions can be presented
 * by the following equation:
 *
 * @image html fir.gif ""
 *
 * Where b[k] is the filter coefficient and Size is the number of filter coefficients.
 *
 * Andes vector library supports distinct FIR filter functions and instance structures
 * for the following data types: single- and half-precision floating-point and Q7. Before you use FIR filter
 * functions, make sure you initialize the corresponding instance structures first.
 * These functions and their instance structures are introduced in the subsections below.
 */

 /**
  * @addtogroup FilterFIR
  * @{
  */

/**
 * @brief Instance structure of the f32 FIR filter
 */
typedef struct
{
    uint32_t coeff_size;    /**< Number of coefficients */
    float32_t *state;       /**< Pointer of the state vector whose size is <code>coeff_size + size - 1</code> */
    float32_t *coeff;       /**< Pointer of the time reversed coefficient vector whose size is <code>coeff_size</code> */
} riscv_vec_fir_f32_t;

/**
 * @brief Instance structure of the f64 FIR filter
 */
typedef struct
{
    uint32_t coeff_size;    /**< Number of coefficients */
    float64_t *state;       /**< Pointer of the state vector whose size is <code>coeff_size + size - 1</code> */
    float64_t *coeff;       /**< Pointer of the time reversed coefficient vector whose size is <code>coeff_size</code> */
} riscv_vec_fir_f64_t;


#if defined (__riscv_zfh)
/**
 * @brief Instance structure of the f16 FIR filter
 */
typedef struct
{
    uint32_t coeff_size;    /**< Number of coefficients */
    float16_t *state;       /**< Pointer of the state vector whose size is <code>coeff_size + size - 1</code> */
    float16_t *coeff;       /**< Pointer of the time reversed coefficient vector whose size is <code>coeff_size</code> */
} riscv_vec_fir_f16_t;
#endif

/**
 * @brief Instance structure of the Q7 FIR filter.
 */
typedef struct
{
    uint32_t coeff_size;    /**< Number of coefficients */
    q7_t *state;            /**< Pointer of the state vector whose size is <code>coeff_size + size - 1</code> */
    q7_t *coeff;            /**< Pointer of the time reversed coefficient vector whose size is <code>coeff_size</code> */
} riscv_vec_fir_q7_t;

/**
 * @brief Instance structure of the Q15 FIR filter.
 */
typedef struct
{
    uint32_t coeff_size;    /**< Number of coefficients */
    q15_t *state;            /**< Pointer of the state vector whose size is <code>coeff_size + size - 1</code> */
    q15_t *coeff;            /**< Pointer of the time reversed coefficient vector whose size is <code>coeff_size</code> */
} riscv_vec_fir_q15_t;

/**
 * @brief Instance structure of the Q31 FIR filter.
 */
typedef struct
{
    uint32_t coeff_size;    /**< Number of coefficients */
    q31_t *state;            /**< Pointer of the state vector whose size is <code>coeff_size + size - 1</code> */
    q31_t *coeff;            /**< Pointer of the time reversed coefficient vector whose size is <code>coeff_size</code> */
} riscv_vec_fir_q31_t;

/**
 * @brief Instance structure of the Q31 complex FIR filter.
 */
typedef struct
{
    uint32_t coeff_size;    /**< Number of coefficients */
    q31_t *state;            /**< Pointer of the state vector whose size is <code>2 * (coeff_size + size - 1)</code> */
    q31_t *coeff;            /**< Pointer of the time reversed coefficient vector whose size is <code>2 * coeff_size</code> */
} riscv_vec_cfir_q31_t;

/**
 * @brief Double-precision floating-point FIR filter function
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 *
 * @b Example
 *     <pre>
 *     \#define coeff_size          4
 *     \#define size                        6
 *     float64_t state[coeff_size + size - 1];
 *     float64_t coeff[coeff_size] = {0.3, -0.1, 0.4, 0.5};
 *     riscv_vec_fir_f64_t inst = {coeff_size, state, coeff};
 *     float64_t src[size] = {0.1, -0.2, 0.2, 0.3, 0.4, 0.1};
 *     float64_t dst[size];
 *     riscv_vec_fir_f64(&inst, src, dst, size);
 *
 *  This example also serves as a reference for the example of F32 and F16 FIR filter function.
 *     </pre>
 */
void riscv_vec_fir_f64(const riscv_vec_fir_f64_t * instance, float64_t * src, float64_t * dst, uint32_t size);

/**
 * @brief Single-precision floating-point FIR filter function
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 *
 * @b Example
 *     <pre>
 *     \#define coeff_size	    4
 *     \#define size			    6
 *     float32_t state[coeff_size + size - 1];
 *     float32_t coeff[coeff_size] = {0.3, -0.1, 0.4, 0.5};
 *     riscv_vec_fir_f32_t inst = {coeff_size, state, coeff};
 *     float32_t src[size] = {0.1, -0.2, 0.2, 0.3, 0.4, 0.1};
 *     float32_t dst[size];
 *     riscv_vec_fir_f32(&inst, src, dst, size);
 *
 *  This example also serves as a reference for the example of F16 and Q7 FIR filter function.
 *     </pre>
 */
void riscv_vec_fir_f32(const riscv_vec_fir_f32_t * instance, float32_t * src, float32_t * dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Q7 FIR filter function
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 *
 * @b Note:
 *
 * This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”, please refer to Section 1.1.
 */
void riscv_vec_fir_f16(const riscv_vec_fir_f16_t * instance, float16_t * src, float16_t * dst, uint32_t size);
#endif

/**
 * @brief Q31 FIR filter function
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 */
void riscv_vec_fir_q31(const riscv_vec_fir_q31_t * instance, q31_t * src, q31_t * dst, uint32_t size);

/**
 * @brief Q31 complex FIR filter function
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 */
void riscv_vec_cfir_q31(const riscv_vec_cfir_q31_t * instance, q31_t * src, q31_t * dst, uint32_t size);

/**
 * @brief Q15 FIR filter function
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 */
void riscv_vec_fir_q15(const riscv_vec_fir_q15_t * instance, q15_t * src, q15_t * dst, uint32_t size);

/**
 * @brief Q7 FIR filter function
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 */
void riscv_vec_fir_q7(const riscv_vec_fir_q7_t * instance, q7_t * src, q7_t * dst, uint32_t size);

/** @} */

/**
 * @defgroup FilterLMS Least Mean Square (LMS) Filter Functions
 *
 * @brief Least Mean Square (LMS) Filter Functions
 *
 * Least mean square (LMS) filter functions are adaptive filters which perform
 * the following equations to update coefficients and get the minimum mean square error:
 *
 * @image html lms.gif ""
 *
 * Where
 *  - y is the output vector.
 *  - h is the coefficients vector.
 *  - x is the input vector.
 *  - L is the filter order.
 *  - e is the error vector.
 *  - d is the desired vector.
 *  - u is the step size which can adjust coefficients.
 *
 * Andes vector library only supports LMS filter function and instance structure
 * for single-precision floating-point data. Before you use the LMS filter function, make
 * sure you initialize the corresponding instance structure first. The function and
 * its instance structure are introduced in the subsection below.
 */

 /**
  * @addtogroup FilterLMS
  * @{
  */

/**
 * @brief Instance structure of the floating-point LMS filter.
 */
typedef struct
{
    uint32_t coeff_size;  /**< Number of filter coefficients */
    float32_t *state;     /**< Pointer of the state vector whose size is <code>coeff_size + size - 1</code> */
    float32_t *coeff;     /**< Pointer of the time reversed coefficient vector whose size is <code>coeff_size</code> */
    float32_t mu;         /**< Step size which can adjust coefficients */
} riscv_vec_lms_f32_t;

/**
 * @brief Instance structure of the q31 LMS filter.
 */
typedef struct
{
    uint32_t coeff_size;  /**< Number of filter coefficients */
    q31_t *state;     /**< Pointer of the state vector whose size is <code>coeff_size + size - 1</code> */
    q31_t *coeff;     /**< Pointer of the time reversed coefficient vector whose size is <code>coeff_size</code> */
    q31_t mu;         /**< Step size which can adjust coefficients */
    q31_t shift;      /**< number of coefficient bits to be shifted */
} riscv_vec_lms_q31_t;


/**
 * @brief Instance structure of the q15 LMS filter.
 */
typedef struct
{
    uint32_t coeff_size;  /**< Number of filter coefficients */
    q15_t *state;     /**< Pointer of the state vector whose size is <code>coeff_size + size - 1</code> */
    q15_t *coeff;     /**< Pointer of the time reversed coefficient vector whose size is <code>coeff_size</code> */
    q15_t mu;         /**< Step size which can adjust coefficients */
    q15_t shift;      /**< number of coefficient bits to be shifted */
} riscv_vec_lms_q15_t;

/**
 * @brief Floating-point LMS filter function
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[in] ref pointer of the desired vector
 * @param[out] dst pointer of the output vector
 * @param[out] err pointer of the error vector
 * @param[in] size number of samples
 *
 * @b Example
 *     <pre>
 *     \#define coeff_size    5
 *     \#define size          6
 *     \#define mu            0.05
 *     float32_t state[coeff_size + size - 1] = {0.0};
 *     float32_t coeff[coeff_size] = {0.40, 0.10, 0.24, -0.40, -0.34};
 *     float32_t src[size] = {1.0, 0.5, 0.4, -0.1, -0.1, 0.3};
 *     float32_t ref[size] = {0.1, 0.2, -0.1, -0.02, -0.1, 0.2};
 *     float32_t err[size];
 *     float32_t dst[size];
 *
 *     riscv_vec_lms_f32_t instance = {coeff_size, state, coeff, mu};
 *     riscv_vec_lms_f32(&instance, src, ref, dst, err, size);
 *     </pre>
 */
void riscv_vec_lms_f32(const riscv_vec_lms_f32_t * instance, float32_t * src, float32_t * ref, float32_t * dst, float32_t * err, uint32_t size);

/**
 * @brief Q15 LMS filter function
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[in] ref pointer of the desired vector
 * @param[out] dst pointer of the output vector
 * @param[out] err pointer of the error vector
 * @param[in] size number of samples
 *
 * @b Example
 *     <pre>
 *     \#define coeff_size    5
 *     \#define size          6
 *     \#define mu            0.05
 *     q15_t state[coeff_size + size - 1] = {0.0};
 *     q15_t coeff[coeff_size] = {0.40, 0.10, 0.24, -0.40, -0.34};
 *     q15_t src[size] = {1.0, 0.5, 0.4, -0.1, -0.1, 0.3};
 *     q15_t ref[size] = {0.1, 0.2, -0.1, -0.02, -0.1, 0.2};
 *     q15_t err[size];
 *     q15_t dst[size];
 *
 *     riscv_vec_lms_q15_t instance = {coeff_size, state, coeff, mu, bitshift};
 *     riscv_vec_lms_q15(&instance, src, ref, dst, err, size);
 *     </pre>
 */
void riscv_vec_lms_q15(const riscv_vec_lms_q15_t * instance, q15_t * src, q15_t * ref, q15_t * dst, q15_t * err, uint32_t size);

/**
 * @brief Q31 LMS filter function
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[in] ref pointer of the desired vector
 * @param[out] dst pointer of the output vector
 * @param[out] err pointer of the error vector
 * @param[in] size number of samples
 *
 * @b Example
 *     <pre>
 *     \#define coeff_size    5
 *     \#define size          6
 *     \#define mu            0.05
 *     q31_t state[coeff_size + size - 1] = {0.0};
 *     q31_t coeff[coeff_size] = {0.40, 0.10, 0.24, -0.40, -0.34};
 *     q31_t src[size] = {1.0, 0.5, 0.4, -0.1, -0.1, 0.3};
 *     q31_t ref[size] = {0.1, 0.2, -0.1, -0.02, -0.1, 0.2};
 *     q31_t err[size];
 *     q31_t dst[size];
 *
 *     riscv_vec_lms_q31_t instance = {coeff_size, state, coeff, mu, bitshift};
 *     riscv_vec_lms_q31(&instance, src, ref, dst, err, size);
 *     </pre>
 */
void riscv_vec_lms_q31(const riscv_vec_lms_q31_t * instance, q31_t * src, q31_t * ref, q31_t * dst, q31_t * err, uint32_t size);

/** @} */

/**
 * @defgroup FilterNLMS Normalized Least Mean Square (NLMS) Filter Function
 *
 * @brief Normalized Least Mean Square (NLMS) Filter Functions
 *
 * The Normalized Least Mean Square (NLMS) filter function is an extension of
 * LMS filter function. It can be represented by the following equations:
 *
 * @image html nlms.gif ""
 *
 * Where
 *  - y is the output vector.
 *  - h is the coefficients vector.
 *  - x is the input vector.
 *  - L is the filter order.
 *  - e is the error vector.
 *  - d is the desired vector.
 *  - u is the step size which can adjust coefficients and is related to the normalized input signals.
 *
 * Andes vector library only supports NLMS filter function and instance structure
 * for single-precision floating point data. Before you use the NLMS filter function, make
 * sure you initialize the corresponding instance structure first. The functions and
 * its instance structure are introduced in the subsection below.
 */

 /**
  * @addtogroup FilterNLMS
  * @{
  */

/**
 * @brief Instance structure of the single-precision floating-point NLMS filter.
 */
typedef struct
{
    uint32_t coeff_size; /**< Number of filter coefficients */
    float32_t *state;    /**< Pointer of the state vector whose size is <code>coeff_size + size - 1</code> */
    float32_t *coeff;    /**< Pointer of the time reversed coefficient vector whose size is <code>coeff_size</code> */
    float32_t mu;        /**< Step size which can adjust coefficients */
    float32_t energy;    /**< Energy of the previous frame */
} riscv_vec_nlms_f32_t;

/**
 * @brief Structure for the q31 normalized LMS filter.
 */
typedef struct
{
    uint32_t coeff_size; /**<  number of filter coefficients */
    q31_t *state;        /**<  pointer of the state vector whose size is <code>coeff_size + size - 1</code>. */
    q31_t *coeff;        /**<  pointer of the time reversed coefficient vector whose size
                               is <code>coeff_size</code> */
    q31_t mu;            /**<  step size which can adjust coefficients */
    uint8_t postshift;   /**<  number of coefficient bits to be shifted */
    q31_t energy;        /**<  energy of the previous frame */
    q31_t x0;            /**<  previous input sample */
} riscv_vec_nlms_q31_t;

/**
 * @brief Structure for the q15 normalized LMS filter.
 */
typedef struct
{
    uint32_t coeff_size; /**<  number of filter coefficients */
    q15_t *state;        /**<  pointer of the state vector whose size is <code>coeff_size + size - 1</code>. */
    q15_t *coeff;        /**<  pointer of the time reversed coefficient vector whose size
                               is <code>coeff_size</code> */
    q15_t mu;            /**<  step size which can adjust coefficients */
    uint8_t postshift;   /**<  number of coefficient bits to be shifted */
    q15_t energy;        /**<  energy of the previous frame */
    q15_t x0;            /**<  previous input sample */
} riscv_vec_nlms_q15_t;

/**
 * @brief Single-precision floating-point NLMS filter function
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[in] ref pointer of the desired vector
 * @param[out] dst pointer of the output vector
 * @param[out] err pointer of the error vector
 * @param[in] size number of samples
 *
 * @b Example
 *     <pre>
 *     \#define coeff_size   5
 *     \#define size         6
 *     \#define mu           0.05
 *     \#define energy       0.1
 *     float32_t state[coeff_size + size - 1] = {0.0};
 *     float32_t coeff[coeff_size] = {0.40, 0.10, 0.24, -0.40, -0.34};
 *     float32_t src[size] = {1.0, 0.5, 0.4, -0.1, -0.1, 0.3};
 *     float32_t ref[size] = {0.1, 0.2, -0.1, -0.02, -0.1, 0.2};
 *     float32_t err[size];
 *     float32_t dst[size];
 *
 *     riscv_vec_nlms_f32_t instance = {coeff_size, state, coeff, mu, energy};
 *     riscv_vec_nlms_f32(&instance, src, ref, dst, err, size);
 *     </pre>
 */
void riscv_vec_nlms_f32(riscv_vec_nlms_f32_t * instance, float32_t * src, float32_t * ref, float32_t * dst, float32_t * err, uint32_t size);

/**
 * @brief q31 NLMS filter function
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[in] ref pointer of the desired vector
 * @param[out] dst pointer of the output vector
 * @param[out] err pointer of the error vector
 * @param[in] size number of samples
 */
void riscv_vec_nlms_q31(riscv_vec_nlms_q31_t *instance, q31_t *src,
                    q31_t *ref, q31_t *dst, q31_t *err, uint32_t size);

/**
 * @brief q15 NLMS filter function
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[in] ref pointer of the desired vector
 * @param[out] dst pointer of the output vector
 * @param[out] err pointer of the error vector
 * @param[in] size number of samples
 */
void riscv_vec_nlms_q15(riscv_vec_nlms_q15_t *instance, q15_t *src,
                    q15_t *ref, q15_t *dst, q15_t *err, uint32_t size);

/** @} FilterNLMS */

/**
 * @defgroup FilterCorr Correlation Functions
 *
 * @brief Correlation functions
 *
 * Correlation functions use two signals to form another signal (also called
 * cross-correlation) mathematically. The mathematical operations of correlation
 * functions are similar to those of convolution functions and can be presented as:
 *
 * @image html corr.gif ""
 *
 * Where src1 and src2 are input vectors, and their lengths are len1 and len2
 * separately; dst is the output vector and its size is 2 * max (len1, len2) - 1.
 *
 * Andes vector library supports distinct correlation functions for the following
 * data types: sinlge- and half-precision floating-point and Q7. These functions are introduced in the subsections below.
 */

/**
 * @addtogroup FilterCorr
 * @{
 */

/**
 * @brief Double-precision floating-point correlation function
 *
 * @param[in] src1 pointer of the first input vector
 * @param[in] len1 length of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] len2 length of the second input vector
 * @param[out] dst pointer of the output vector
 *
 * @b Example
 *     <pre>
 *     \#define len1	7
 *     \#define len2	5
 *     float64_t src1[len1] = {0.3, 0.2, -0.1, 0.2, 0.4, 0.2, -0.1};
 *     float64_t src2[len2] = {-0.1, 0.3, 0.2, -0.2, -0.2};
 *     float64_t dst[2 * len1 - 1];
 *     riscv_vec_corr_f64(src1, len1, src2, len2, dst);
 *
 * This example also serves as a reference for the example of F16 and Q7 correlation function.
 *     </pre>
 */
void riscv_vec_corr_f64(float64_t * src1, uint32_t len1,  float64_t * src2, uint32_t len2, float64_t * dst);

/**
 * @brief Single-precision floating-point correlation function
 *
 * @param[in] src1 pointer of the first input vector
 * @param[in] len1 length of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] len2 length of the second input vector
 * @param[out] dst pointer of the output vector
 *
 * @b Example
 *     <pre>
 *     \#define len1	7
 *     \#define len2	5
 *     float32_t src1[len1] = {0.3, 0.2, -0.1, 0.2, 0.4, 0.2, -0.1};
 *     float32_t src2[len2] = {-0.1, 0.3, 0.2, -0.2, -0.2};
 *     float32_t dst[2 * len1 - 1];
 *     riscv_vec_corr_f32(src1, len1, src2, len2, dst);
 *
 * This example also serves as a reference for the example of F16 and Q7 correlation function.
 *     </pre>
 */
void riscv_vec_corr_f32(float32_t * src1, uint32_t len1,  float32_t * src2, uint32_t len2, float32_t * dst);

#if defined (__riscv_zfh)
/**
 * @brief f16 correlation function
 *
 * @param[in] src1 pointer of the first input vector
 * @param[in] len1 length of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] len2 length of the second input vector
 * @param[out] dst pointer of the output vector
 *
 * @b Note:
 *
 * This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”, please refer to Section 1.1.
 */
void riscv_vec_corr_f16(float16_t * src1, uint32_t len1, float16_t * src2, uint32_t len2, float16_t * dst);
#endif

/**
  * @brief Q31 correlation function
  *
  * @param[in] src1 pointer of the first input vector
  * @param[in] len1 length of the first input vector
  * @param[in] src2 pointer of the second input vector
  * @param[in] len2 length of the second input vector
  * @param[out] dst pointer of the output vector
  */
void riscv_vec_corr_q31(q31_t * src1, uint32_t len1, q31_t * src2, uint32_t len2, q31_t * dst);

/**
  * @brief Q15 correlation function
  *
  * @param[in] src1 pointer of the first input vector
  * @param[in] len1 length of the first input vector
  * @param[in] src2 pointer of the second input vector
  * @param[in] len2 length of the second input vector
  * @param[out] dst pointer of the output vector
  */

  void riscv_vec_corr_q15(q15_t * src1, uint32_t len1, q15_t * src2, uint32_t len2, q15_t * dst);

/**
  * @brief Q7 correlation function
  *
  * @param[in] src1 pointer of the first input vector
  * @param[in] len1 length of the first input vector
  * @param[in] src2 pointer of the second input vector
  * @param[in] len2 length of the second input vector
  * @param[out] dst pointer of the output vector
  */
void riscv_vec_corr_q7(q7_t * src1, uint32_t len1, q7_t * src2, uint32_t len2, q7_t * dst);

/**
  * @brief f32 Auto-correlation function for F32.
  *
  * @param[in] src Points to the input vector
  * @param[in] len Length of the input vector
  * @param[out] dst Points to the output vector
  */
void riscv_vec_autocorr_f32(float32_t * src, uint32_t len, float32_t * dst);

/** @} FilterCorr */

/**
 * @defgroup dcmfir Decimation FIR Filter Function
 *
 * @brief Decimation FIR Filter Function
 *
 * The decimation FIR filter function implements FIR filter with decimation by
 * an integer factor M. They can be presented by the following equation:
 *
 * @image html dcmfir.gif ""
 *
 * Where b[k] is the filter coefficient, src is the input data of the length Size,
 * and dst is the output data of the length Size/M. Size must be a multiple of the
 * decimation factor M so that the number of output samples can be an integer.
 *
 * Andes vector library only supports decimation FIR filter function and instance
 * structure for the single-precision floating-point data. Before you use the decimation FIR
 * filter function, make sure you initialize the corresponding instance structure
 * first. The functions and its instance structure are introduced in the subsection below.
 */

/**
 * @addtogroup dcmfir
 * @{
 */

/**
 * @brief Instance structure of the single-precision floating-point decimator filter.
 */
typedef struct
{
    uint32_t M;             /**< decimation factor. */
    uint32_t coeff_size;    /**< number of coefficients */
    float32_t *coeff;       /**< pointer of the time reversed coefficient vector whose size is <code>coeff_size</code>. */
    float32_t *state;       /**< pointer of the state vector whose size is <code>coeff_size + size - 1</code>. */
} riscv_vec_dcmfir_f32_t;

/**
 * @brief Instance structure of the single-precision floating-point decimator filter.
 */
typedef struct
{
    uint32_t M;             /**< decimation factor. */
    uint32_t coeff_size;    /**< number of coefficients */
    q15_t *coeff;           /**< pointer of the time reversed coefficient vector whose size is <code>coeff_size</code>. */
    q15_t *state;           /**< pointer of the state vector whose size is <code>coeff_size + size - 1</code>. */
} riscv_vec_dcmfir_q15_t;

/**
 * @brief Instance structure of the single-precision floating-point decimator filter.
 */
typedef struct
{
    uint32_t M;             /**< decimation factor. */
    uint32_t coeff_size;    /**< number of coefficients */
    q31_t *coeff;           /**< pointer of the time reversed coefficient vector whose size is <code>coeff_size</code>. */
    q31_t *state;           /**< pointer of the state vector whose size is <code>coeff_size + size - 1</code>. */
} riscv_vec_dcmfir_q31_t;

/**
 * @brief Single-precision floating-point decimator FIR filters function
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 *
 * @b Example
 *     <pre>
 * With the input size as 24, the decimation factor M as 4 and the filter coefficient
 * size as 6, set the instance of the decimation FIR filter as:
 *
 *     \#define size       24
 *     \#define M           4
 *     \#define coeff_size  6
 *     float32_t coeff[coeff_size] = {…};
 *     float32_t state[coeff_size + size - 1];
 *     riscv_vec_dcmfir_f32_t inst = {M, coeff_size, coeff, state};
 *     float32_t src[size] = {0.1, -0.2, 0.2, 0.3, 0.4, 0.1, …};
 *     float32_t dst[size / M];
 *     riscv_vec_dcmfir_f32(&inst, src, dst, size);
 *     </pre>
 */
void riscv_vec_dcmfir_f32(const riscv_vec_dcmfir_f32_t * instance, float32_t * src, float32_t * dst, uint32_t size);

/**
 * @brief Single-precision floating-point decimator FIR filters function
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 */
void riscv_vec_dcmfir_q15(const riscv_vec_dcmfir_q15_t * instance, q15_t * src, q15_t * dst, uint32_t size);

/**
 * @brief Q31 decimator FIR filters function
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 */
void riscv_vec_dcmfir_q31(const riscv_vec_dcmfir_q31_t * instance, q31_t * src, q31_t * dst, uint32_t size);
/** @} dcmfir */

/**
 * @defgroup Filterupspl Upsampling FIR Filter Function
 *
 * @brief Upsampling FIR Filter Function
 *
 * The upsampling FIR filer function upsamples incoming signals to generate multi-rate
 * outputs. It is usually used in audio/sound or images processing like increasing the
 * resolution of images or adjusting the audio output sample rate from 16kbps to 32kbps.
 *
 * Andes vector library only supports upsampling FIR filter function and instance structure
 * for the single-precision floating-point data. Before you use the upsampling filter function, make
 * sure you initialize the corresponding instance structure first. The function and its
 * instance structure are introduced in the subsection below.
 */

/**
 * @addtogroup Filterupspl
 * @{
 */
/**
 * @brief Structure for the single-precision floating-point Upsampling FIR Filters.
 */
typedef struct
{
    uint32_t L;          /**< upsampling factor */
    uint32_t plen;       /**< number of coefficients */
    float32_t *coeff;    /**< pointer of the time reversed coefficient vector whose size is "L * plen" */
    float32_t *state;    /**< pointer of the state vector whose size is "plen + size - 1" */
} riscv_vec_upsplfir_f32_t;

/**
 * @brief Structure for the Q31 Upsampling FIR Filters.
 */
typedef struct
{
    uint32_t L;          /**< upsampling factor */
    uint32_t plen;       /**< number of coefficients */
    q31_t *coeff;    /**< pointer of the time reversed coefficient vector whose size is "L * plen" */
    q31_t *state;    /**< pointer of the state vector whose size is "plen + size - 1" */
} riscv_vec_upsplfir_q31_t;


/**
 * @brief Structure for the Q15 Upsampling FIR Filters.
 */
typedef struct
{
    uint32_t L;          /**< upsampling factor */
    uint32_t plen;       /**< number of coefficients */
    q15_t *coeff;    /**< pointer of the time reversed coefficient vector whose size is "L * plen" */
    q15_t *state;    /**< pointer of the state vector whose size is "plen + size - 1" */
} riscv_vec_upsplfir_q15_t;


/**
 * @brief Single-precision floating-point Upsampling FIR Filters
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 *
 * @b Example
 *     <pre>
 *     \#define L       2
 *     \#define plen    4
 *     \#define size    6
 *     float32_t state[plen + size - 1] = {0.0};
 *     float32_t coeff[L * plen] = {0.40, 0.10, 0.24, -0.40, 1.0, 0.5, 0.4, -0.1};
 *     float32_t src[size] = {1.0, 0.5, 0.4, -0.1, -0.1, 0.3};
 *     float32_t dst[size * L];
 *     riscv_vec_upsplfir_f32_t instance = {L, plen, state, coeff};
 *     riscv_vec_upsplfir_f32(&instance, src, dst, size);
 *     </pre>
 */
void riscv_vec_upsplfir_f32(const riscv_vec_upsplfir_f32_t *instance, float32_t *src, float32_t *dst, uint32_t size);

/**
 * @brief Q31 Upsampling FIR Filters
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 *
 * @b Example
 *     <pre>
 *     \#define L       2
 *     \#define plen    4
 *     \#define size    6
 *     q31_t state[plen + size - 1] = {0.0};
 *     q31_t coeff[L * plen] = {0.40, 0.10, 0.24, -0.40, 1.0, 0.5, 0.4, -0.1};
 *     q31_t src[size] = {1.0, 0.5, 0.4, -0.1, -0.1, 0.3};
 *     q31_t dst[size * L];
 *     riscv_vec_upsplfir_q31_t instance = {L, plen, state, coeff};
 *     riscv_vec_upsplfir_q31(&instance, src, dst, size);
 *     </pre>
 */
void riscv_vec_upsplfir_q31(const riscv_vec_upsplfir_q31_t *instance, q31_t *src, q31_t *dst, uint32_t size);

/**
 * @brief Q15 Upsampling FIR Filters
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 *
 * @b Example
 *     <pre>
 *     \#define L       2
 *     \#define plen    4
 *     \#define size    6
 *     q15_t state[plen + size - 1] = {0.0};
 *     q15_t coeff[L * plen] = {0.40, 0.10, 0.24, -0.40, 1.0, 0.5, 0.4, -0.1};
 *     q15_t src[size] = {1.0, 0.5, 0.4, -0.1, -0.1, 0.3};
 *     q15_t dst[size * L];
 *     riscv_vec_upsplfir_q15_t instance = {L, plen, state, coeff};
 *     riscv_vec_upsplfir_q15(&instance, src, dst, size);
 *     </pre>
 */
void riscv_vec_upsplfir_q15(const riscv_vec_upsplfir_q15_t *instance, q15_t *src, q15_t *dst, uint32_t size);


/** @} Filterupspl */

// Sparse FIR Filters
/**
 * @defgroup Filtersparse Sparse FIR Filter Function
 * @brief Sparse FIR Filter Function
 *
 * The sparse FIR filter function is the FIR filter with the majority of coefficients
 * being zero. Due to this characteristic, the sparse FIR filter only keeps the non-zero
 * coefficients and ignores the multiplication operation of signal and zero coefficients.
 *
 * Andes vector library only supports the sparse FIR filter function and instance structure
 * for the single-precision floating-point data. Before you use the sparse FIR filter function, make sure
 * you initialize the corresponding instance structure first. The function and its instance
 * structures are introduced in the subsection below.
 */

/**
 * @addtogroup Filtersparse
 * @{
 */
/**
 * @brief Structure for the single-precision flaoting-point Sparse FIR Filters.
 */
typedef struct
{
    uint16_t coeff_size;    /**< number of filter coefficients */
    uint16_t index;         /**< index of the state buffer */
    float32_t *state;       /**< pointer of the state vector whose size is <code>delay + size</code> */
    float32_t *coeff;       /**< pointer of the coefficient vector whose size is <code>coeff_size</code> */
    uint16_t delay;         /**< maximum value in the nezdelay vector */
    int32_t *nezdelay;      /**< vector which stores non-zero coefficients and the size
                                 is <code>coeff_size</code> */
} riscv_vec_spafir_f32_t;

/**
 * @brief Structure for the Q7 Sparse FIR Filters.
 */
typedef struct
{
    uint16_t coeff_size;    /**< number of filter coefficients */
    uint16_t index;         /**< index of the state buffer */
    q7_t *state;            /**< pointer of the state vector whose size is <code>delay + size</code> */
    q7_t *coeff;            /**< pointer of the coefficient vector whose size is <code>coeff_size</code> */
    uint16_t delay;         /**< maximum value in the nezdelay vector */
    int32_t *nezdelay;      /**< vector which stores non-zero coefficients and the size
                                 is <code>coeff_size</code> */
} riscv_vec_spafir_q7_t;

/**
 * @brief Structure for the Q15 Sparse FIR Filters.
 */
typedef struct
{
    uint16_t coeff_size;    /**< number of filter coefficients */
    uint16_t index;         /**< index of the state buffer */
    q15_t *state;           /**< pointer of the state vector whose size is <code>delay + size</code> */
    q15_t *coeff;           /**< pointer of the coefficient vector whose size is <code>coeff_size</code> */
    uint16_t delay;         /**< maximum value in the nezdelay vector */
    int32_t *nezdelay;      /**< vector which stores non-zero coefficients and the size
                                 is <code>coeff_size</code> */
} riscv_vec_spafir_q15_t;

/**
 * @brief Structure for the Q31 Sparse FIR Filters.
 */
typedef struct
{
    uint16_t coeff_size;    /**< number of filter coefficients */
    uint16_t index;         /**< index of the state buffer */
    q31_t *state;           /**< pointer of the state vector whose size is <code>delay + size</code> */
    q31_t *coeff;           /**< pointer of the coefficient vector whose size is <code>coeff_size</code> */
    uint16_t delay;         /**< maximum value in the nezdelay vector */
    int32_t *nezdelay;      /**< vector which stores non-zero coefficients and the size
                                 is <code>coeff_size</code> */
} riscv_vec_spafir_q31_t;

/**
 * @brief Single-precision flaoting-point Sparse FIR Filters.
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] buf pointer of the temporary vector
 * @param[in] size number of samples
 *
 * @b Note:
 *
 *  The size of temporary buffer should be equal to size, size of the input vector.
 *
 * @b Example
 *     <pre>
 * The example below can be treated as the following formula:
 *
 * <code> y(n)=coeff(0)*x(n - nezdelay(0)) + coeff(1)*x(n - nezdelay(1))+ coeff(2)*x(n - nezdelay(2)) + coeff(3)*x(n - nezdelay(3)) </code>
 *
 *       \#define coeff_size    4
 *       \#define index         1
 *       \#define size          6
 *       int32_t nezdelay[coeff_size] = {3, 0, 2, 1}
 *       \#define delay         3
 *
 *       float32_t state[delay + size] = {0.0};
 *       float32_t coeff[coeff_size] = {0.40, 0.10, 0.24, -0.40};
 *       float32_t src[size] = {1.0, 0.5, 0.4, -0.1, -0.1, 0.3};
 *       float32_t buf[size];
 *       float32_t dst[size];
 *
 *       riscv_vec_spafir_f32_t instance = {coeff_size, index, state, coeff, delay, nezdelay};
 *       riscv_vec_spafir_f32(&instance, src, dst, buf, size);
 *     </pre>
 */
void riscv_vec_spafir_f32(riscv_vec_spafir_f32_t *instance, float32_t *src, float32_t *dst, float32_t *buf, uint32_t size);

/**
 * @brief Single-precision Q7 Sparse FIR Filters.
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] buf pointer of the temporary vector
 * @param[in] buf2 pointer of the second temporary vector
 * @param[in] size number of samples
 *
 * @b Note:
 *
 *  The size of temporary buffer should be equal to size, size of the input vector.
 */
void riscv_vec_spafir_q7(riscv_vec_spafir_q7_t *instance, q7_t *src,
                         q7_t *dst, q7_t *buf, q31_t *buf2, uint32_t size);

/**
 * @brief Single-precision Q15 FIR Filters.
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] buf pointer of the temporary vector
 * @param[in] buf2 pointer of the second temporary vector
 * @param[in] size number of samples
 *
 * @b Note:
 *
 *  The size of temporary buffer should be equal to size, size of the input vector.
 */
void riscv_vec_spafir_q15(riscv_vec_spafir_q15_t *instance, q15_t *src,
                      q15_t *dst, q15_t *buf, q31_t *buf2, uint32_t size);

/**
 * @brief Single-precision Q31 Sparse FIR Filters.
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] buf pointer of the temporary vector
 * @param[in] size number of samples
 *
 * @b Note:
 *
 *  The size of temporary buffer should be equal to size, size of the input vector.
 */
void riscv_vec_spafir_q31(riscv_vec_spafir_q31_t *instance, q31_t *src, q31_t *dst, q31_t *buf, uint32_t size);


/** @} Filtersparse */

// Lattice IIR filter
/**
 * @defgroup Filteliir Lattice Infinite Impulse Response (IIR) Filter Function
 *
 * @brief Lattice IIR Filter Function
 *
 * The lattice infinite impulse response (IIR) filter function can be represented as follows:
 *
 * @image html liir.gif ""
 *
 * Where N is the number of states and z = N, N-1, ... , 1, coefficients k = {rN, rN-1, ....r1},
 * vN is the ladder coefficients and u is the state.
 *
 * Andes vector library only support lattice IIR filter function and instance structure for single-precision
 * floating-point data type. Before you use the lattice IIR filter function, make sure you
 * initialize the corresponding instance structure first. The function and its instance structures
 * are introduced in the subsection below.
 */

/**
 * @addtogroup Filteliir
 * @{
 */

/**
 * @brief Structure for the single-precision floating-point lattice IIR filter.
 */
typedef struct
{
    uint32_t nstage;    /**< number of filter stage. */
    float32_t *state;   /**< pointer of the state vector whose size is <code>nstage + size</code>. */
    float32_t *rcoeff;  /**< pointer of the time reversed reflection coefficient vector whose size is
                         <code>nstage</code>. */
    float32_t *lcoeff;  /**< pointer of the time reversed ladder coefficient vector whose size is
                         <code>nstage + 1</code>. */
} riscv_vec_liir_f32_t;

/**
 * @brief Structure for the Q15 lattice IIR filter.
 */
typedef struct
{
    uint32_t nstage;    /**< number of filter stage. */
    q15_t *state;       /**< pointer of the state vector whose size is <code>nstage + size</code>. */
    q15_t *rcoeff;      /**< pointer of the time reversed reflection coefficient vector whose size is
                         <code>nstage</code>. */
    q15_t *lcoeff;      /**< pointer of the time reversed ladder coefficient vector whose size is
                         <code>nstage + 1</code>. */
} riscv_vec_liir_q15_t;

/**
 * @brief Structure for the Q31 lattice IIR filter.
 */
typedef struct
{
    uint32_t nstage;    /**< number of filter stage. */
    q31_t *state;       /**< pointer of the state vector whose size is <code>nstage + size</code>. */
    q31_t *rcoeff;      /**< pointer of the time reversed reflection coefficient vector whose size is
                         <code>nstage</code>. */
    q31_t *lcoeff;      /**< pointer of the time reversed ladder coefficient vector whose size is
                         <code>nstage + 1</code>. */
} riscv_vec_liir_q31_t;

/**
 * @brief Single-precision floating-point lattice IIR filter
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 *
 * @b Example
 *     <pre>
 *      \#define size    8
 *      \#define nstage  6
 *      float32_t state[nstage + size];
 *      float32_t rcoeff[nstage] = {…};
 *      float32_t lcoeff[nstage + 1] = {…};
 *      riscv_vec_liir_f32_t inst = {nstage, state, rcoeff, lcoeff}.
 *      float32_t src[size] = {…};
 *      float32_t dst[size];
 *      riscv_vec_liir_f32(&inst, src, dst, size);
 *     </pre>
 */
void riscv_vec_liir_f32(const riscv_vec_liir_f32_t *instance, float32_t *src, float32_t *dst, uint32_t size);

/**
 * @brief Q15 lattice IIR filter
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 */
void riscv_vec_liir_q15(const riscv_vec_liir_q15_t *instance, q15_t *src, q15_t *dst, uint32_t size);

/**
 * @brief Q31 lattice IIR filter
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 */
void riscv_vec_liir_q31(const riscv_vec_liir_q31_t *instance, q31_t *src, q31_t *dst, uint32_t size);
/** @} Filteliir */

/**
 * @defgroup Filtelfir Lattice FIR Filter Function
 *
 * @brief Lattice FIR Filter Function
 *
 * The lattice FIR filter function can be represented by the following equations:
 *
 * @image html lfir.gif ""
 *
 * Where x is the input, y is the output, u is the state of previous inputs, 0 <= z < M and
 * M is the stage number, and Kz is the reflection coefficients.
 *
 * Andes vector library only supports the lattice FIR filter function and instance structure
 * for single-precision floating-point data. Before you use the lattice FIR filter function, make sure you
 * initialize the corresponding instance structure first. The function and its instance
 * structure are introduced in the subsection below.
 */

/**
 * @addtogroup Filtelfir
 * @{
 */
/**
 * @brief Structure for the single-precision floating-point lattice FIR filter.
 */
typedef struct
{
    uint32_t stage;    /**< number of filter stage. */
    float32_t *state;  /**< pointer of the state vector whose size is <code>stage</code>. */
    float32_t *coeff;  /**< pointer of the time reversed coefficient vector whose size is <code>stage</code>. */
} riscv_vec_lfir_f32_t;

/**
 * @brief Structure for the Q15 lattice FIR filter.
 */
typedef struct
{
    uint32_t stage;    /**< number of filter stage. */
    q15_t *state;      /**< pointer of the state vector whose size is <code>stage</code>. */
    q15_t *coeff;      /**< pointer of the time reversed coefficient vector whose size is <code>stage</code>. */
} riscv_vec_lfir_q15_t;

/**
 * @brief Structure for the Q31 lattice FIR filter.
 */
typedef struct
{
    uint32_t stage;    /**< number of filter stage. */
    q31_t *state;      /**< pointer of the state vector whose size is <code>stage</code>. */
    q31_t *coeff;      /**< pointer of the time reversed coefficient vector whose size is <code>stage</code>. */
} riscv_vec_lfir_q31_t;

/**
 * @brief Single-precision floating-point lattice FIR filter
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 *
 * @b Example
 *     <pre>
 * With the filter length as 6 and the input size as 8, set the instance of the lattice FIR filter as:
 *
 *      \#define M		6
 *      \#define size	8
 *      float32_t state[M];
 *      float32_t coeff[M] = {…};
 *      float32_t src[size] = {…};
 *      float32_t dst[size];
 *      riscv_vec_lfir_f32_t inst = {M, state, coeff};
 *      riscv_vec_lfir_f32 (&inst, src, dst, size);
 *     </pre>
 */
void riscv_vec_lfir_f32(const riscv_vec_lfir_f32_t *instance, float32_t *src, float32_t *dst, uint32_t size);

/**
 * @brief Q15 lattice FIR filter
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 */
void riscv_vec_lfir_q15(const riscv_vec_lfir_q15_t *instance, q15_t *src, q15_t *dst, uint32_t size);

/**
 * @brief Q31 lattice FIR filter
 *
 * @param[in] instance pointer of the instance structure
 * @param[in] src pointer of the input vector
 * @param[out] dst pointer of the output vector
 * @param[in] size number of samples
 */
void riscv_vec_lfir_q31(const riscv_vec_lfir_q31_t *instance, q31_t *src, q31_t *dst, uint32_t size);
/** @} Filtelfir */

/**
 * @defgroup bqm_df1 Multichannel Biquadratic Cascade (BQM) Filter Direct Form I (DF1) Functions
 *
 * @brief Multichannel Biquadratic Cascade (BQM) Filter Direct Form I (DF1) Functions
 *
 * These functions implement a biquadratic filter in direct form I (DF1), where each stage is implemented as an individual infinite impulse response (IIR) filter, as described by the following equation:
 *     dst[n] = b0 * src[n] + b1 * src[n-1] + b2 * src[n-2] + a1 * dst[n-1] + a2 * dst[n-2];
 *
 * In the equation, five coefficients (b0, b1, b2, a1, and a2) are used per stage. The coefficients b0, b1, and b2 determine the feedforward coefficients, while the coefficients a1 and a2 determine the feedback coefficients. Each stage has its own set of unique coefficients.
 *
 * The input and output signals are interleaved for multichannel data. For example, if channel = 3, the src data is arranged in the following format and the output is arranged in the same layout:
 *    { ch1_src[0] ch2_src[0], ch3_src[0], ch1_src[1] ch2_src[1], ch3_src[1], ..., ch1_src[n - 1] ch2_src[n - 1], ch3_src[n - 1] }
 *
 * To ensure correct results, you must initialize the structure with the corresponding initialization function (e.g., riscv_vec_bqm_df1_init_f32), which allocates memory to store the arranged coefficients and sets zeros in the state buffer. After executing the BQM DF1 Filter function, release the allocated memory using the associated free function (e.g., riscv_vec_bqm_df1_free_f32).
 *
 * Andes vector library supports distinct BQM DF1 filter functions and instance structures for single- and half-precision floating-point, Q31 and Q15 data types. These functions and their instance structures are introduced in the subsections below.
 */
/**
 * @addtogroup bqm_df1
 * @{
 */
/**
 * @brief Instance structure of the F32 BQM DF1 filter
 */
typedef struct
{
    uint32_t nstage;    /**< Number of filter stages */
    uint32_t nchannel;  /**< Number of channels */
    float32_t *state;   /**< Points to the state vector whose size is 4 * nstage * nchannel */
    float32_t *coeff;   /**< Points to the coefficient vector whose size is 5 * nstage * nchannel */
} riscv_vec_bqm_df1_f32_t;

/**
 * @brief BQM DF1 filter initialize function for F32.
 *
 * @param[in, out]     instance Points to the instance structure
 * @param[in] nstage   Number of filter stages
 * @param[in] state    Points to the state buffer
 * @param[in] coeff    Points to the input coefficient vector
 * @param[in] nch      Number of channels
 * @return none.
 *
 * @b Note
 * This function allocates a temporary buffer to store the rearranged coefficient vector for optimized efficiency, which size is 5 * nstage * nchannel * sizeof(float32_t).
 */
void riscv_vec_bqm_df1_init_f32(riscv_vec_bqm_df1_f32_t *instance, uint32_t nstage, float32_t *state, float32_t *coeff, uint32_t nch);

/**
 * @brief Function to free F32 BQM DF1 filter structure.
 * @param[in]     instance Points to the instance structure
 * @return none.
 */
void riscv_vec_bqm_df1_free_f32(riscv_vec_bqm_df1_f32_t *instance);

/**
 * @brief BQM DF1 filter function for F32.
 *
 * @param[in] instance Points to the instance structure
 * @param[in] src      Points to the input vector
 * @param[out] dst     Points to the output vector
 * @param[in] size     Number of samples
 * @return none.
 */
void riscv_vec_bqm_df1_f32(const riscv_vec_bqm_df1_f32_t *instance, float32_t *src, float32_t *dst, uint32_t size);

/**
 * @brief Structure for the q31 Multi-channel Biquad Filter Direct Form I.
 */
typedef struct
{
    uint32_t nstage;    /**< stages in the filter */
    uint32_t nchannel;  /**< input channels */
    q15_t *state;       /**< pointer of the state vector whose size is 4*nstage*nchannel */
    q15_t *coeff;       /**< pointer of the coefficient vector whose size is 5*nstage*nchannel */
    int8_t shift;       /**< shift bits */
} riscv_vec_bqm_df1_q15_t;

/**
 * @brief BQM DF1 filter initialize function for Q15.
 *
 * @param[in, out]     instance Points to the instance structure
 * @param[in] nstage   Number of filter stages
 * @param[in] state    Points to the state buffer
 * @param[in] coeff    Points to the input coefficient vector
 * @param[in] shift    Shift bits
 * @param[in] nch      Number of channels
 * @return none.
 *
 * @b Note
 * This function allocates a temporary buffer to store the rearranged coefficient vector for optimized efficiency, which size is 5 * nstage * nchannel * sizeof(q15_t).
 */
void riscv_vec_bqm_df1_init_q15(riscv_vec_bqm_df1_q15_t *instance, uint32_t nstage, q15_t *state, q15_t *coeff, int8_t shift, uint32_t nch);

/**
 * @brief Function to free Q15 BQM DF1 filter structure.
 * @param[in]     instance Points to the instance structure
 * @return none.
 */
void riscv_vec_bqm_df1_free_q15(riscv_vec_bqm_df1_q15_t *instance);

/**
 * @brief BQM DF1 filter function for Q15.
 *
 * @param[in] instance Points to the instance structure
 * @param[in] src      Points to the input vector
 * @param[out] dst     Points to the output vector
 * @param[in] size     Number of samples
 * @return none.
 */
void riscv_vec_bqm_df1_q15(const riscv_vec_bqm_df1_q15_t *instance, q15_t *src, q15_t *dst, uint32_t size);

/**
 * @brief Structure for the q31 Multi-channel Biquad Filter Direct Form I.
 */
typedef struct
{
    uint32_t nstage;    /**< stages in the filter */
    uint32_t nchannel;  /**< input channels */
    q31_t *state;       /**< pointer of the state vector whose size is 4*nstage*nchannel */
    q31_t *coeff;       /**< pointer of the coefficient vector whose size is 5*nstage*nchannel */
    int8_t shift;       /**< shift bits */
} riscv_vec_bqm_df1_q31_t;

/**
 * @brief BQM DF1 filter initialize function for Q31.
 *
 * @param[in, out]     instance Points to the instance structure
 * @param[in] nstage   Number of filter stages
 * @param[in] state    Points to the state buffer
 * @param[in] coeff    Points to the input coefficient vector
 * @param[in] shift    Shift bits
 * @param[in] nch      Number of channels
 * @return none.
 *
 * @b Note
 * This function allocates a temporary buffer to store the rearranged coefficient vector for optimized efficiency, which size is 5 * nstage * nchannel * sizeof(q31_t).
 */
void riscv_vec_bqm_df1_init_q31(riscv_vec_bqm_df1_q31_t *instance, uint32_t nstage, q31_t *state, q31_t *coeff, int8_t shift, uint32_t nch);

/**
 * @brief Function to free Q31 BQM DF1 filter structure.
 * @param[in]     instance Points to the instance structure
 * @return none.
 */
void riscv_vec_bqm_df1_free_q31(riscv_vec_bqm_df1_q31_t *instance);

/**
 * @brief BQM DF1 filter function for Q31.
 *
 * @param[in] instance Points to the instance structure
 * @param[in] src      Points to the input vector
 * @param[out] dst     Points to the output vector
 * @param[in] size     Number of samples
 * @return none.
 */
void riscv_vec_bqm_df1_q31(const riscv_vec_bqm_df1_q31_t *instance, q31_t *src, q31_t *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Structure for the q31 Multi-channel Biquad Filter Direct Form I.
 */
typedef struct
{
    uint32_t nstage;    /**< stages in the filter */
    uint32_t nchannel;  /**< input channels */
    float16_t *state;   /**< pointer of the state vector whose size is 4*nstage*nchannel */
    float16_t *coeff;   /**< pointer of the coefficient vector whose size is 5*nstage*nchannel */
} riscv_vec_bqm_df1_f16_t;

/**
 * @brief BQM DF1 filter initialize function for F16.
 *
 * @param[in, out]     instance Points to the instance structure
 * @param[in] nstage   Number of filter stages
 * @param[in] state    Points to the state buffer
 * @param[in] coeff    Points to the input coefficient vector
 * @param[in] nch      Number of channels
 * @return none.
 *
 * @b Note
 *  1. This function allocates a temporary buffer to store the rearranged coefficient vector for optimized efficiency, which size is 5 * nstage * nchannel * sizeof(float16_t).
 *  2. This function is supported only by toolchains with HW FPU support. Additionally, you must apply the -mzfh option in both the compiler and linker flags for applications that use this function. For details about the -mzfh linking option, see Section 1.1 or 1.2.
 */
void riscv_vec_bqm_df1_init_f16(riscv_vec_bqm_df1_f16_t *instance, uint32_t nstage, float16_t *state, float16_t *coeff, uint32_t nch);

/**
 * @brief Function to free F16 BQM DF1 filter structure.
 * @param[in]     instance Points to the instance structure
 * @return none.
 */
void riscv_vec_bqm_df1_free_f16(riscv_vec_bqm_df1_f16_t *instance);

/**
 * @brief BQM DF1 filter function for F16.
 *
 * @param[in] instance Points to the instance structure
 * @param[in] src      Points to the input vector
 * @param[out] dst     Points to the output vector
 * @param[in] size     Number of samples
 * @return none.
 */
void riscv_vec_bqm_df1_f16(const riscv_vec_bqm_df1_f16_t *instance, float16_t *src, float16_t *dst, uint32_t size);

#endif

/** @} bqm_df1 */

/**
 * @defgroup bqm_df2t Multichannel Biquadratic Cascade (BQM) Filter Direct Form II Transposed (DF2T) Functions
 *
 * @brief Multichannel Biquadratic Cascade (BQM) Filter Direct Form II Transposed (DF2T) Functions
 *
 * These functions implement a biquadratic filter in direct form II transposed (DF2T), where each stage is implemented as an individual infinite impulse response (IIR) filter, as described by the following equation:
 *   for (n = 0; n < size; n++)
 *       out[n] = b0 * in[n] + d0;
 *       d0 = b1 * in[n] + a1 * out[n] + d1;
 *       d1 = b2 * in[n] + a2 * out[n];
 *
 * In the equation, five coefficients (b0, b1, b2, a1, and a2) and two state variables (d0 and d1) are used per stage. Each stage has its own set of unique coefficients.
 *
 * The input and output signals are interleaved for multichannel data. For example, if channel = 3, the src data is arranged in the following format and the output is arranged in the same layout:
 * { ch1_src[0] ch2_src[0], ch3_src[0], ch1_src[1] ch2_src[1], ch3_src[1], …, ch1_src[n - 1] ch2_src[n - 1], ch3_src[n - 1] }
 *
 * To ensure correct results, you must initialize the structure with the corresponding initialization function (e.g., riscv_vec_bqm_df2T_init_f32), which allocates memory to store the arranged coefficients and sets zeros in the state buffer. After executing the BQM DF2T Filter function, release the allocated memory using the associated free function (e.g., riscv_vec_bqm_df2T_free_f32).
 *
 * Andes vector library supports distinct BQM DF2T filter functions and instance structures for single-, half-, and doble precision floating-point and Q31 data types. These functions and their instance structures are introduced in the subsections below.
 */
/**
 * @addtogroup bqm_df2t
 * @{
 */
/**
 * @brief Instance structure of the F32 BQM DF2T filter
 */
typedef struct
{
    uint32_t nstage;    /**< stages in the filter */
    uint32_t nchannel;  /**< input channels */
    float32_t *state;   /**< pointer of the state vector whose size is 2*nstage*nchannel */
    float32_t *coeff;   /**< pointer of the coefficient vector whose size is 5*nstage*nchannel */
} riscv_vec_bqm_df2T_f32_t;

/**
 * @brief BQM DF2T filter initialize function for F32.
 *
 * @param[in, out] instance  Points to the instance structure
 * @param[in] nstage   Number of filter stages
 * @param[in] state    Points to the state buffer
 * @param[in] coeff    Points to the input coefficient vector
 * @param[in] nch      Number of channels
 * @return none.
 *
 * @b Note
 * This function allocates a temporary buffer to store the rearranged coefficient vector for optimized efficiency, which size is 5 * nstage * nchannel * sizeof(float32_t).
 */
void riscv_vec_bqm_df2T_init_f32(riscv_vec_bqm_df2T_f32_t *instance, uint32_t nstage, float32_t *state, float32_t *coeff, uint32_t nch);

/**
 * @brief Function to free F32 BQM DF2T filter structure.
 * @param[in]     instance Points to the instance structure
 * @return none.
 */
void riscv_vec_bqm_df2T_free_f32(riscv_vec_bqm_df2T_f32_t *instance);

/**
 * @brief BQM DF2T filter function for F32.
 *
 * @param[in] instance Points to the instance structure
 * @param[in] src      Points to the input vector
 * @param[out] dst     Points to the output vector
 * @param[in] size     Number of samples
 * @return none.
 */
void riscv_vec_bqm_df2T_f32(const riscv_vec_bqm_df2T_f32_t *instance, float32_t *src, float32_t *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Instance structure of the F16 BQM DF2T filter
 */
typedef struct
{
    uint32_t nstage;    /**< stages in the filter */
    uint32_t nchannel;  /**< input channels */
    float16_t *state;   /**< pointer of the state vector whose size is 2*nstage*nchannel */
    float16_t *coeff;   /**< pointer of the coefficient vector whose size is 5*nstage*nchannel */
} riscv_vec_bqm_df2T_f16_t;

/**
 * @brief BQM DF2T filter initialize function for F16.
 *
 * @param[in, out] instance  Points to the instance structure
 * @param[in] nstage   Number of filter stages
 * @param[in] state    Points to the state buffer
 * @param[in] coeff    Points to the input coefficient vector
 * @param[in] nch      Number of channels
 * @return none.
 *
 * @b Note
 * This function allocates a temporary buffer to store the rearranged coefficient vector for optimized efficiency, which size is 5 * nstage * nchannel * sizeof(float16_t).
 */
void riscv_vec_bqm_df2T_init_f16(riscv_vec_bqm_df2T_f16_t *instance, uint32_t nstage, float16_t *state, float16_t *coeff, uint32_t nch);

/**
 * @brief Function to free F16 BQM DF2T filter structure.
 * @param[in]     instance Points to the instance structure
 * @return none.
 */
void riscv_vec_bqm_df2T_free_f16(riscv_vec_bqm_df2T_f16_t *instance);

/**
 * @brief BQM DF2T filter function for F16.
 *
 * @param[in] instance Points to the instance structure
 * @param[in] src      Points to the input vector
 * @param[out] dst     Points to the output vector
 * @param[in] size     Number of samples
 * @return none.
 */
void riscv_vec_bqm_df2T_f16(const riscv_vec_bqm_df2T_f16_t *instance, float16_t *src, float16_t *dst, uint32_t size);
#endif

/**
 * @brief Instance structure of the F64 BQM DF2T filter
 */
typedef struct
{
    uint32_t nstage;    /**< stages in the filter */
    uint32_t nchannel;  /**< input channels */
    float64_t *state;   /**< pointer of the state vector whose size is 2*nstage*nchannel */
    float64_t *coeff;   /**< pointer of the coefficient vector whose size is 5*nstage*nchannel */
} riscv_vec_bqm_df2T_f64_t;

/**
 * @brief BQM DF2T filter initialize function for F64.
 *
 * @param[in, out] instance  Points to the instance structure
 * @param[in] nstage   Number of filter stages
 * @param[in] state    Points to the state buffer
 * @param[in] coeff    Points to the input coefficient vector
 * @param[in] nch      Number of channels
 * @return none.
 *
 * @b Note
 * This function allocates a temporary buffer to store the rearranged coefficient vector for optimized efficiency, which size is 5 * nstage * nchannel * sizeof(float64_t).
 */
void riscv_vec_bqm_df2T_init_f64(riscv_vec_bqm_df2T_f64_t *instance, uint32_t nstage, float64_t *state, float64_t *coeff, uint32_t nch);

/**
 * @brief Function to free F64 BQM DF2T filter structure.
 * @param[in]     instance Points to the instance structure
 * @return none.
 */
void riscv_vec_bqm_df2T_free_f64(riscv_vec_bqm_df2T_f64_t *instance);

/**
 * @brief BQM DF2T filter function for F64.
 *
 * @param[in] instance Points to the instance structure
 * @param[in] src      Points to the input vector
 * @param[out] dst     Points to the output vector
 * @param[in] size     Number of samples
 * @return none.
 */
void riscv_vec_bqm_df2T_f64(const riscv_vec_bqm_df2T_f64_t *instance, float64_t *src, float64_t *dst, uint32_t size);

/**
 * @brief Instance structure of the Q31 BQM DF2T filter
 */
typedef struct
{
    uint32_t nstage;    /**< stages in the filter */
    uint32_t nchannel;  /**< input channels */
    q31_t *state;       /**< pointer of the state vector whose size is 2*nstage*nchannel */
    q31_t *coeff;       /**< pointer of the coefficient vector whose size is 5*nstage*nchannel */
    int8_t shift;       /**< shift bits */
} riscv_vec_bqm_df2T_q31_t;

/**
 * @brief BQM DF2T filter initialize function for Q31.
 *
 * @param[in, out]     instance Points to the instance structure
 * @param[in] nstage   Number of filter stages
 * @param[in] state    Points to the state buffer
 * @param[in] coeff    Points to the input coefficient vector
 * @param[in] shift    Shift bits
 * @param[in] nch      Number of channels
 * @return none.
 *
 * @b Note
 * This function allocates a temporary buffer to store the rearranged coefficient vector for optimized efficiency, which size is 5 * nstage * nchannel * sizeof(q31_t).
 */
void riscv_vec_bqm_df2T_init_q31(riscv_vec_bqm_df2T_q31_t *instance, uint32_t nstage, q31_t *state, q31_t *coeff, int8_t shift, uint32_t nch);

/**
 * @brief Function to free F32 BQM DF2T filter structure.
 * @param[in]     instance Points to the instance structure
 * @return none.
 */
void riscv_vec_bqm_df2T_free_q31(riscv_vec_bqm_df2T_q31_t *instance);

/**
 * @brief BQM DF2T filter function for Q31.
 *
 * @param[in] instance Points to the instance structure
 * @param[in] src      Points to the input vector
 * @param[out] dst     Points to the output vector
 * @param[in] size     Number of samples
 * @return none.
 */
void riscv_vec_bqm_df2T_q31(const riscv_vec_bqm_df2T_q31_t *instance, q31_t *src, q31_t *dst, uint32_t size);
/** @} bqm_df2t */

// Convolution
/**
 * @defgroup conv Convolution Functions
 * @brief Convolution Functions
 *
 * Convolution functions combine two signals into one signal mathematically. The convolution
 * equation can be presented as:
 *
 * @image html conv.gif ""
 *
 * Where the size of src1[n] is len1, the size of src2[n] is len2, and the size of dst[n] is len1 +
 * len2 - 1.
 *
 * Andes vector library supports distinct convolution functions for the following data types:
 * single-precision floating-point, Q31, Q15 and Q7. These functions are introduced in the subsections below.
 */
/**
 * @addtogroup conv
 * @{
 */
/**
 * @brief Convolution of the single-precision floating-point vectors.
 * @param[in]       *src1 pointer of the first input vector
 * @param[in]       len1  length of the first input vector
 * @param[in]       *src2 pointer of the second input vector
 * @param[in]       len2  length of the second input vector
 * @param[out]      *dst  pointer of the output vector
 * @return none.
 *
 * @b Example
 *     <pre>
 *      \#define len1 6
 *      \#define len2 4
 *      float32_t src1[len1] = {0.3, 0.2, -0.1, 0.2, 0.4, 0.2};
 *      float32_t src2[len2] = {-0.1, 0.3, 0.2, -0.2};
 *      float32_t dst[len1 + len2 - 1];
 *      riscv_dsp_conv_f32(src1, len1, src2, len2, dst);
 *
 * This example also serves as a reference for examples of Q31, Q15 or Q7 convolution
 * functions.
 *     </pre>
 */
void riscv_vec_conv_f32(float32_t *src1, uint32_t len1,
                    float32_t *src2, uint32_t len2, float32_t *dst);

/**
 * @brief Convolution of the q7 vectors.
 * @param[in]       *src1 pointer of the first input vector
 * @param[in]       len1  length of the first input vector
 * @param[in]       *src2 pointer of the second input vector
 * @param[in]       len2  length of the second input vector
 * @param[out]      *dst  pointer of the output vector
 * @return none.
 */
void riscv_vec_conv_q7(q7_t *src1, uint32_t len1, q7_t *src2, uint32_t len2, q7_t *dst);

/**
 * @brief Convolution of the q15 vectors.
 * @param[in]       *src1 pointer of the first input vector
 * @param[in]       len1  length of the first input vector
 * @param[in]       *src2 pointer of the second input vector
 * @param[in]       len2  length of the second input vector
 * @param[out]      *dst  pointer of the output vector
 * @return none.
 */
void riscv_vec_conv_q15(q15_t *src1, uint32_t len1, q15_t *src2, uint32_t len2, q15_t *dst);

/**
 * @brief Convolution of the q31 vectors.
 * @param[in]       *src1 pointer of the first input vector
 * @param[in]       len1  length of the first input vector
 * @param[in]       *src2 pointer of the second input vector
 * @param[in]       len2  length of the second input vector
 * @param[out]      *dst  pointer of the output vector
 * @return none.
 */
void riscv_vec_conv_q31(q31_t *src1, uint32_t len1, q31_t *src2, uint32_t len2, q31_t *dst);
/** @} conv */

/**
 * @defgroup conv_partial Partial Convolution Functions
 * @brief Partial Convolution Functions
 *
 * With a starting index and a size specified, partial convolution functions do the convolution
 * partially for two signals. The partial convolution result will be generated to the destination
 * vector in the range [start_index, … , start_index + size - 1 ]. Thus, if the partial
 * convolution result is not within the range [0, … , len1 + len2 - 2], a return value of -1 will be
 * given for the argument error
 *
 * Andes vector library supports distinct partial convolution functions for the following data types:
 * single-precision floating-point, Q31, Q15 and Q7. These functions are introduced in the subsections below.
 */
/**
 * @addtogroup conv_partial
 * @{
 */
/**
 * @brief Convolution Partial of the single-precision floating-point vectors.
 * @param[in]       *src1 pointer of the first input vector
 * @param[in]       len1  length of the first input vector
 * @param[in]       *src2 pointer of the second input vector
 * @param[in]       len2  length of the second input vector
 * @param[out]      *dst  pointer of the output vector
 * @param[in]       startindex starting index of the partial convolution
 * @param[in]       size length of the partial convolution
 * @return          Returns (0: success; -1: failure)
 *
 * @b Example
 *     <pre>
 *      \#define len1 6
 *      \#define len2 4
 *      float32_t src1[len1] = {0.3, 0.2, -0.1, 0.2, 0.4, 0.2};
 *      float32_t src2[len2] = {-0.1, 0.3, 0.2, -0.2};
 *      uint32_t dst[len1 + len2 - 1];
 *      riscv_dsp_conv_partial_f32(src1, len1, src2, len2, dst, 3, 4);
 *
 * This example also serves as a reference for examples of Q31, Q15 or Q7 partial convolution
 * functions.
 *     </pre>
 */
int32_t riscv_vec_conv_partial_f32(float32_t *src1, uint32_t len1,
                               float32_t *src2, uint32_t len2, float32_t *dst, uint32_t startindex, uint32_t size);

/**
 * @brief Convolution Partial of the q7 vectors.
 * @param[in]       *src1 pointer of the first input vector
 * @param[in]       len1  length of the first input vector
 * @param[in]       *src2 pointer of the second input vector
 * @param[in]       len2  length of the second input vector
 * @param[out]      *dst  pointer of the output vector
 * @param[in]       startindex starting index of the partial convolution
 * @param[in]       size length of the partial convolution
 * @return          Returns (0: success; -1: failure)
 */
int32_t riscv_vec_conv_partial_q7(q7_t *src1, uint32_t len1,
                               q7_t *src2, uint32_t len2, q7_t *dst, uint32_t startindex, uint32_t size);

/**
 * @brief Convolution Partial of the q15 vectors.
 * @param[in]       *src1 pointer of the first input vector
 * @param[in]       len1  length of the first input vector
 * @param[in]       *src2 pointer of the second input vector
 * @param[in]       len2  length of the second input vector
 * @param[out]      *dst  pointer of the output vector
 * @param[in]       startindex starting index of the partial convolution
 * @param[in]       size length of the partial convolution
 * @return          Returns (0: success; -1: failure)
 */
int32_t riscv_vec_conv_partial_q15(q15_t *src1, uint32_t len1,
                               q15_t *src2, uint32_t len2, q15_t *dst, uint32_t startindex, uint32_t size);

/**
 * @brief Convolution Partial of the q31 vectors.
 * @param[in]       *src1 pointer of the first input vector
 * @param[in]       len1  length of the first input vector
 * @param[in]       *src2 pointer of the second input vector
 * @param[in]       len2  length of the second input vector
 * @param[out]      *dst  pointer of the output vector
 * @param[in]       startindex starting index of the partial convolution
 * @param[in]       size length of the partial convolution
 * @return          Returns (0: success; -1: failure)
 */
int32_t riscv_vec_conv_partial_q31(q31_t *src1, uint32_t len1,
                               q31_t *src2, uint32_t len2, q31_t *dst, uint32_t startindex, uint32_t size);
/** @} conv_partial */

/** @} groupFilter */


#ifdef  __cplusplus
}
#endif
#endif

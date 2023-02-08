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
 * for the following data types: floating-point and Q7. Before you use FIR filter
 * functions, make sure you initialize the corresponding instance structures first.
 * These functions and their instance structures are introduced in the subsections below.
 */

 /**
  * @addtogroup FilterFIR
  * @{
  */

/**
 * @brief Instance structure of the floating-point FIR filter
 */
typedef struct
{
    uint32_t coeff_size;    /**< Number of coefficients */
    float32_t *state;       /**< Pointer of the state vector whose size is <code>coeff_size + size - 1</code> */
    float32_t *coeff;       /**< Pointer of the time reversed coefficient vector whose size is <code>coeff_size</code> */
} riscv_vec_fir_f32_t;

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
 * @brief Floating-point FIR filter function
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
 *  This example also serves as a reference for the example of Q7 FIR filter function.
 *     </pre>
 */
void riscv_vec_fir_f32(const riscv_vec_fir_f32_t * instance, float32_t * src, float32_t * dst, uint32_t size);

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
 * for the floating-point data type. Before you use the LMS filter function, make
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
 * for the floating point data type. Before you use the NLMS filter function, make
 * sure you initialize the corresponding instance structure first. The functions and
 * its instance structure are introduced in the subsection below.
 */

 /**
  * @addtogroup FilterNLMS
  * @{
  */

/**
 * @brief Instance structure of the floating-point NLMS filter.
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
 * @brief Floating-point NLMS filter function
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

/** @} */

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
 * data types: floating-point and Q7. These functions are introduced in the subsections below.
 */

/**
 * @addtogroup FilterCorr
 * @{
 */

/**
 * @brief Floating-point correlation function
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
 * This example also serves as a reference for the example of Q7 correlation function.
 *     </pre>
 */
void riscv_vec_corr_f32(float32_t * src1, uint32_t len1,  float32_t * src2, uint32_t len2, float32_t * dst);

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

/** @} */
/**
 * @defgroup Filterdcm Decimation FIR Filter Function
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
 * structure for the floating-point data type. Before you use the decimation FIR
 * filter function, make sure you initialize the corresponding instance structure
 * first. The functions and its instance structure are introduced in the subsection below.
 */

/**
 * @addtogroup Filterdcm
 * @{
 */

/**
 * @brief Instance structure of the floating-point decimator filter.
 */
typedef struct
{
    uint32_t M;             /**< decimation factor. */
    uint32_t coeff_size;    /**< number of coefficients */
    float32_t *coeff;       /**< pointer of the time reversed coefficient vector whose size is <code>coeff_size</code>. */
    float32_t *state;       /**< pointer of the state vector whose size is <code>coeff_size + size - 1</code>. */
} riscv_vec_dcmfir_f32_t;

/**
 * @brief Floating-point decimator FIR filters function
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
/** @} */

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
 * for the floating-point data type. Before you use the upsampling filter function, make
 * sure you initialize the corresponding instance structure first. The function and its
 * instance structure are introduced in the subsection below.
 */

/**
 * @addtogroup Filterupspl
 * @{
 */
/**
 * @brief Structure for the floating-point Upsampling FIR Filters.
 */
typedef struct
{
    uint32_t L;          /**< upsampling factor */
    uint32_t plen;       /**< number of coefficients */
    float32_t *coeff;    /**< pointer of the time reversed coefficient vector whose size is "L * plen" */
    float32_t *state;    /**< pointer of the state vector whose size is "plen + size - 1" */
} riscv_vec_upsplfir_f32_t;

/**
 * @brief Floating-point Upsampling FIR Filters
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

/** @} */

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
 * for the floating-point data type. Before you use the sparse FIR filter function, make sure
 * you initialize the corresponding instance structure first. The function and its instance
 * structures are introduced in the subsection below.
 */

/**
 * @addtogroup Filtersparse
 * @{
 */
/**
 * @brief Structure for the flaoting-point Sparse FIR Filters.
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
 * @brief flaoting-point Sparse FIR Filters.
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

/** @} */

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
 * Andes vector library only support lattice IIR filter function and instance structure for the
 * floating-point data type. Before you use the lattice IIR filter function, make sure you
 * initialize the corresponding instance structure first. The function and its instance structures
 * are introduced in the subsection below.
 */

/**
 * @addtogroup Filteliir
 * @{
 */

/**
 * @brief Structure for the floating-point lattice IIR filter.
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
 * @brief Floating-point lattice IIR filter
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
/** @} */

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
 * for floating-point. Before you use the lattice FIR filter function, make sure you
 * initialize the corresponding instance structure first. The function and its instance
 * structure are introduced in the subsection below.
 */

/**
 * @addtogroup Filtelfir
 * @{
 */
/**
 * @brief Structure for the floating-point lattice FIR filter.
 */
typedef struct
{
    uint32_t stage;    /**< number of filter stage. */
    float32_t *state;  /**< pointer of the state vector whose size is <code>stage</code>. */
    float32_t *coeff;  /**< pointer of the time reversed coefficient vector whose size is <code>stage</code>. */
} riscv_vec_lfir_f32_t;

/**
 * @brief Floating-point lattice FIR filter
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

/** @} */
/** @} */

#ifdef  __cplusplus
}
#endif
#endif

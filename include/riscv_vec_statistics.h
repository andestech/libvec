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
#ifndef __RISCV_VEC_STATISTICS_H__
#define __RISCV_VEC_STATISTICS_H__

/** @file*/

#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_math_types.h"
#include <riscv_vec_types.h>

/**
 * @defgroup statistics Statistics Functions
 *
 * @brief Vector Statistics Functions
 *
 *
 */

/**
 * @addtogroup statistics
 * @{
 */

// Maximum
/**
 * @defgroup max Maximum Functions
 * @brief Maximum Functions
 *
 * Maximum functions compare the values in a vector then return the maximum one and its index.
 *
 * Andes VEC library supports distinct maximum functions for single-precision floating-point, Q31, Q15, Q7 and
 * other data types. These functions are introduced in the subsections below.
 */
/**
 * @addtogroup max
 * @{
 */
/**
 * @brief Maximum value of the single-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Maximum value
 */
float32_t riscv_vec_max_f32(const float32_t * src, uint32_t size, uint32_t * index);

/**
 * @brief Maximum value of the double-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Maximum value
 */
float64_t riscv_vec_max_f64(const float64_t * src, uint32_t size, uint32_t * index);

/**
 * @brief Maximum value of the q7 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Maximum value
 */
q7_t riscv_vec_max_q7(const q7_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Maximum value of the q15 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Maximum value
 */
q15_t riscv_vec_max_q15(const q15_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Maximum value of the q31 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Maximum value
 */
q31_t riscv_vec_max_q31(const q31_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Maximum value of the u8 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Maximum value
 */
uint8_t riscv_vec_max_u8(const uint8_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Maximum value of the u16 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Maximum value
 */
uint16_t riscv_vec_max_u16(const uint16_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Maximum value of the u32 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Maximum value
 */
uint32_t riscv_vec_max_u32(const uint32_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Maximum value of the u64 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Maximum value
 */
uint64_t riscv_vec_max_u64(const uint64_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Maximum value of the single-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Maximum value
 */
float32_t riscv_vec_max_val_f32(const float32_t *src, uint32_t size);

/**
 * @brief Maximum value of the double-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Maximum value
 */
float64_t riscv_vec_max_val_f64(const float64_t *src, uint32_t size);

/**
 * @brief Maximum value of the q7 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Maximum value
 */
q7_t riscv_vec_max_val_q7(const q7_t *src, uint32_t size);

/**
 * @brief Maximum value of the q15 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Maximum value
 */
q15_t riscv_vec_max_val_q15(const q15_t *src, uint32_t size);

/**
 * @brief Maximum value of the q31 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Maximum value
 */
q31_t riscv_vec_max_val_q31(const q31_t *src, uint32_t size);

/**
 * @brief Maximum value of the u8 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Maximum value
 */
uint8_t riscv_vec_max_val_u8(const uint8_t *src, uint32_t size);

/**
 * @brief Maximum value of the u16 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Maximum value
 */
uint16_t riscv_vec_max_val_u16(const uint16_t *src, uint32_t size);

/**
 * @brief Maximum value of the u32 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Maximum value
 */
uint32_t riscv_vec_max_val_u32(const uint32_t *src, uint32_t size);

/**
 * @brief Maximum value of the u64 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Maximum value
 */
uint64_t riscv_vec_max_val_u64(const uint64_t *src, uint32_t size);
/** @} max */

// Minimum
/**
 * @defgroup min Minimum Functions
 * @brief Minimum Functions
 *
 * Minimum functions compare the values in a vector then return the minimum one and its index.
 *
 * Andes VEC library supports distinct minimum functions for floating-point, Q31, Q15, Q7 and
 * other data types. These functions are introduced in the subsections below.
 */
/**
 * @addtogroup min
 * @{
 */
/**
 * @brief Minimum value of the single-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the minimum value
 * @return Minimum value
 */
float32_t riscv_vec_min_f32(const float32_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Minimum value of the double-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the minimum value
 * @return Minimum value
 */
float64_t riscv_vec_min_f64(const float64_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Minimum value of the q7 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the minimum value
 * @return Minimum value
 */
q7_t riscv_vec_min_q7(const q7_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Minimum value of the q15 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the minimum value
 * @return Minimum value
 */
q15_t riscv_vec_min_q15(const q15_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Minimum value of the q31 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the minimum value
 * @return Minimum value
 */
q31_t riscv_vec_min_q31(const q31_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Minimum value of the u8 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the minimum value
 * @return Minimum value
 */
uint8_t riscv_vec_min_u8(const uint8_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Minimum value of the u16 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the minimum value
 * @return Minimum value
 */
uint16_t riscv_vec_min_u16(const uint16_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Minimum value of the u32 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the minimum value
 * @return Minimum value
 */
uint32_t riscv_vec_min_u32(const uint32_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Minimum value of the u64 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the minimum value
 * @return Minimum value
 */
uint64_t riscv_vec_min_u64(const uint64_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Minimum value of the single-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Minimum value
 */
float32_t riscv_vec_min_val_f32(const float32_t *src, uint32_t size);

/**
 * @brief Minimum value of the double-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Minimum value
 */
float64_t riscv_vec_min_val_f64(const float64_t *src, uint32_t size);

/**
 * @brief Minimum value of the q7 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Minimum value
 */
q7_t riscv_vec_min_val_q7(const q7_t *src, uint32_t size);

/**
 * @brief Minimum value of the q15 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Minimum value
 */
q15_t riscv_vec_min_val_q15(const q15_t *src, uint32_t size);

/**
 * @brief Minimum value of the q31 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Minimum value
 */
q31_t riscv_vec_min_val_q31(const q31_t *src, uint32_t size);

/**
 * @brief Minimum value of the u8 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Minimum value
 */
uint8_t riscv_vec_min_val_u8(const uint8_t *src, uint32_t size);

/**
 * @brief Minimum value of the u16 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Minimum value
 */
uint16_t riscv_vec_min_val_u16(const uint16_t *src, uint32_t size);

/**
 * @brief Minimum value of the u32 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Minimum value
 */
uint32_t riscv_vec_min_val_u32(const uint32_t *src, uint32_t size);

/**
 * @brief Minimum value of the u64 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Minimum value
 */
uint64_t riscv_vec_min_val_u64(const uint64_t *src, uint32_t size);
/** @} min */

// Absolute Maximum Functions
/**
 * @defgroup absmax Absolute Maximum Functions
 * @brief Absolute Maximum Functions
 *
 * Absolute Maximum functions compare the absolute values in a vector and return the maximum one along with its index.
 *
 * Andes VEC library supports distinct absolute maximum functions for single-precision floating-point, Q31, Q15, Q7 and
 * other data types. These functions are introduced in the subsections below.
 */
/**
 * @addtogroup absmax
 * @{
 */
/**
 * @brief Maximum absolute value of the single-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Absolute maximum value
 */
float32_t riscv_vec_absmax_f32(const float32_t* src, uint32_t size, uint32_t* index);

/**
 * @brief Maximum absolute value of the double-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Absolute maximum value
 */
float64_t riscv_vec_absmax_f64(const float64_t* src, uint32_t size, uint32_t* index);

/**
 * @brief Maximum absolute value of the q7 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Absolute maximum value
 */
q7_t riscv_vec_absmax_q7(const q7_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Maximum absolute value of the q15 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Absolute maximum value
 */
q15_t riscv_vec_absmax_q15(const q15_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Maximum absolute value of the q31 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Absolute maximum value
 */
q31_t riscv_vec_absmax_q31(const q31_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Maximum absolute value of the single-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Absolute maximum value
 */
float32_t riscv_vec_absmax_val_f32(const float32_t *src, uint32_t size);

/**
 * @brief Maximum absolute value of the double-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Absolute maximum value
 */
float64_t riscv_vec_absmax_val_f64(const float64_t *src, uint32_t size);

/**
 * @brief Maximum absolute value of the q7 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Absolute maximum value
 */
q7_t riscv_vec_absmax_val_q7(const q7_t *src, uint32_t size);

/**
 * @brief Maximum absolute value of the q15 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Absolute maximum value
 */
q15_t riscv_vec_absmax_val_q15(const q15_t *src, uint32_t size);

/**
 * @brief Maximum absolute value of the q31 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Absolute maximum value
 */
q31_t riscv_vec_absmax_val_q31(const q31_t *src, uint32_t size);
/** @} absmax */

// Absolute Minimum Functions
/**
 * @defgroup absmin Absolute absmin Functions
 * @brief Absolute Minimum Functions
 *
 * Absolute minimum functions compare the absolute values in a vector and return the minimum one along with its index.
 *
 * Andes VEC library supports distinct absolute minimum functions for single-precision floating-point, Q31, Q15, Q7 and
 * other data types. These functions are introduced in the subsections below.
 */
/**
 * @addtogroup absmin
 * @{
 */
/**
 * @brief Minimum absolute value of the single-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Absolute minimum value
 */
float32_t riscv_vec_absmin_f32(const float32_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Minimum absolute value of the double-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Absolute minimum value
 */
float64_t riscv_vec_absmin_f64(const float64_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Minimum absolute value of the q7 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Absolute minimum value
 */
q7_t riscv_vec_absmin_q7(const q7_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Minimum absolute value of the q15 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Absolute minimum value
 */
q15_t riscv_vec_absmin_q15(const q15_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Minimum absolute value of the q31 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Absolute minimum value
 */
q31_t riscv_vec_absmin_q31(const q31_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Minimum absolute value of the single-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Absolute minimum value
 */
float32_t riscv_vec_absmin_val_f32(const float32_t* src, uint32_t size);

/**
 * @brief Minimum absolute value of the double-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Absolute minimum value
 */
float64_t riscv_vec_absmin_val_f64(const float64_t* src, uint32_t size);

/**
 * @brief Minimum absolute value of the q7 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Absolute minimum value
 */
q7_t riscv_vec_absmin_val_q7(const q7_t* src, uint32_t size);

/**
 * @brief Minimum absolute value of the q15 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Absolute minimum value
 */
q15_t riscv_vec_absmin_val_q15(const q15_t* src, uint32_t size);

/**
 * @brief Minimum absolute value of the q31 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Absolute minimum value
 */
q31_t riscv_vec_absmin_val_q31(const q31_t* src, uint32_t size);
/** @} absmin */

// Mean
/**
 * @defgroup mean Mean Functions
 * @brief Mean Functions
 *
 * Mean functions calculate the arithmetic mean of a vector.
 *
 * Andes VEC library supports distinct mean functions for floating-point, Q31, Q15, Q7 and other
 * data types. These functions are introduced in the subsections below.
 */
/**
 * @addtogroup mean
 * @{
 */
/**
 * @brief Mean value of the single-precision floating-point vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Mean value.
 */
float32_t riscv_vec_mean_f32(const float32_t *src, uint32_t size);

/**
 * @brief Mean value of the double-precision floating-point vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Mean value.
 */
float64_t riscv_vec_mean_f64(const float64_t *src, uint32_t size);

/**
 * @brief Mean value of the q7 vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Mean value.
 */
q7_t riscv_vec_mean_q7(const q7_t *src, uint32_t size);

/**
 * @brief Mean value of the q15 vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Mean value.
 */
q15_t riscv_vec_mean_q15(const q15_t *src, uint32_t size);

/**
 * @brief Mean value of the q31 vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Mean value.
 */
q31_t riscv_vec_mean_q31(const q31_t *src, uint32_t size);

/**
 * @brief Mean value of the u8 vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Mean value.
 */
uint8_t riscv_vec_mean_u8(const uint8_t *src, uint32_t size);
/** @} mean */

// Root Mean Square
/**
 * @defgroup rms Root Mean Square (RMS) Functions
 * @brief RMS Functions
 *
 * Root mean square (RMS) functions compute root mean square of a vector. The behavior can be
 * defined as follows:
 *
 * @image html rms.gif ""
 *
 * For sqrt(), please refer to square root functions.
 *
 * Andes VEC library supports distinct RMS functions for the following data types: floating-point,
 * Q31, and Q15. These functions are introduced in the subsections below.
 */
/**
 * @addtogroup rms
 * @{
 */
/**
 * @brief RMS of the single-precision floating-point vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return RMS value.
 */
float32_t riscv_vec_rms_f32(const float32_t *src, uint32_t size);

/**
 * @brief RMS of the q15 vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return RMS value.
 */
q15_t riscv_vec_rms_q15(const q15_t *src, uint32_t size);

/**
 * @brief RMS of the q31 vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return RMS value.
 */
q31_t riscv_vec_rms_q31(const q31_t *src, uint32_t size);
/** @} rms */

// Variance
/**
 * @defgroup var Variance Functions
 * @brief Variance Functions
 *
 * Variance functions compute the variance value of a vector. The behavior can be defined as
 * follows:
 *
 * @image html var.gif ""
 *
 * Andes VEC library supports distinct variance functions for the following data types:
 * floating-point, Q31 and Q15. These functions are introduced in the subsections below.
 */
/**
 * @addtogroup var
 * @{
 */
/**
 * @brief Variance of the single-precision floating-point vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Variance value.
 */
float32_t riscv_vec_var_f32(const float32_t *src, uint32_t size);

/**
 * @brief Variance of the double-precision floating-point vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Variance value.
 */
float64_t riscv_vec_var_f64(const float64_t *src, uint32_t size);

/**
 * @brief Variance of the q31 vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Variance value.
 */
q63_t riscv_vec_var_q31(const q31_t *src, uint32_t size);

/**
 * @brief Variance of the q15 vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Variance value.
 */
q31_t riscv_vec_var_q15(const q15_t *src, uint32_t size);
/** @} var */

// Standard deviation
/**
 * @defgroup std Standard Deviation Functions
 * @brief Standard Deviation Functions
 *
 * Standard deviation functions compute the standard deviation value of a vector. The behavior can
 * be defined as follows:
 *
 * @image html std.gif ""
 *
 * For sqrt(), please refer to square root functions.
 *
 * Andes VEC library supports distinct standard deviation functions for floating-point, Q31, Q15
 * and other data types. These functions are introduced in the subsections below.
 */
/**
 * @addtogroup std
 * @{
 */
/**
 * @brief Standard deviation of the single-precision floating-point vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Standard deviation value.
 */
float32_t riscv_vec_std_f32(const float32_t *src, uint32_t size);

/**
 * @brief Standard deviation of the double-precision floating-point vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Standard deviation value.
 */
float64_t riscv_vec_std_f64(const float64_t *src, uint32_t size);

/**
 * @brief Standard deviation of the q15 vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Standard deviation value.
 */
q15_t riscv_vec_std_q15(const q15_t *src, uint32_t size);

/**
 * @brief Standard deviation of the q31 vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Standard deviation value.
 */
q31_t riscv_vec_std_q31(const q31_t *src, uint32_t size);

/**
 * @brief Standard deviation of the u8 vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Standard deviation value.
 */
q15_t riscv_vec_std_u8(const uint8_t *src, uint32_t size);
/** @} std */

// Mean Square Error Functions
/**
 * @defgroup mse Mean Square Error Functions
 * @brief Mean Square Error Functions
 *
 * Mean Square Error (MSE) functions calculate the MSE value of two vectors and return the MSE result
 *
 * Andes DSP library supports distinct MSE functions for single- and double-precision floating-point, Q31, Q15 and Q7 data types.
 *  These functions are introduced in the subsections below.
 */
/**
 * @addtogroup mse
 * @{
 */

/**
 * @brief Mean square error value of f32 vector
 * @param[in]     src1      pointer of the first input vector
 * @param[in]     src2      pointer of the second input vector
 * @param[in]     size      number of elements in a vector
 * @param[out]    result    pointer of the result
 */
void riscv_vec_mse_f32(const float32_t * src1, const float32_t * src2, uint32_t size, float32_t * result);

/**
 * @brief Mean square error value of f64 vector
 * @param[in]     src1      pointer of the first input vector
 * @param[in]     src2      pointer of the second input vector
 * @param[in]     size      number of elements in a vector
 * @param[out]    result    pointer of the result
 */
void riscv_vec_mse_f64(const float64_t * src1, const float64_t * src2, uint32_t size, float64_t * result);

/**
 * @brief Mean square error value of q31 vector
 * @param[in]     src1      pointer of the first input vector
 * @param[in]     src2      pointer of the second input vector
 * @param[in]     size      number of elements in a vector
 * @param[out]    result    pointer of the result
 */
void riscv_vec_mse_q31(const q31_t * src1, const q31_t * src2, uint32_t size, q31_t * result);

/**
 * @brief Mean square error value of q15 vector
 * @param[in]     src1      pointer of the first input vector
 * @param[in]     src2      pointer of the second input vector
 * @param[in]     size      number of elements in a vector
 * @param[out]    result    pointer of the result
 */
 void riscv_vec_mse_q15(const q15_t * src1, const q15_t * src2, uint32_t size, q15_t * result);

/**
 * @brief Mean square error value of q7 vector
 * @param[in]     src1      pointer of the first input vector
 * @param[in]     src2      pointer of the second input vector
 * @param[in]     size      number of elements in a vector
 * @param[out]    result    pointer of the result
 */
void riscv_vec_mse_q7(const q7_t * src1, const q7_t * src2, uint32_t size, q7_t * result);

/** @} mse */

// Power Functions
/**
 * @defgroup pwr Vector Power Functions
 * @brief Vector Power Functions
 *
 * Power functions compute the sum of squares of a vector.
 *
 * Andes VEC library supports distinct sum of squares functions for the following data types:
 * floating-point, Q31, Q15 and Q7. These functions are introduced in the subsections below.
 */

/**
 * @addtogroup pwr
 * @{
 */

/**
 * @brief Sum of the squares of the single-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Sum of squares
 */
float32_t riscv_vec_pwr_f32(const float32_t *src, uint32_t size);

/**
 * @brief Sum of the squares of the double-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Sum of squares
 */
float64_t riscv_vec_pwr_f64(const float64_t *src, uint32_t size);

/**
 * @brief Sum of the squares of the q15 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Sum of squares
 */
q63_t riscv_vec_pwr_q15(const q15_t *src, uint32_t size);

/**
 * @brief Sum of the squares of the q31 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Sum of squares
 */
q63_t riscv_vec_pwr_q31(const q31_t *src, uint32_t size);

/**
 * @brief Sum of the squares of the q7 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Sum of squares
 */
q31_t riscv_vec_pwr_q7(const q7_t *src, uint32_t size);

/** @} pwr */
/** @} statistics */

#ifdef  __cplusplus
}
#endif
#endif // __RISCV_VEC_STATISTICS_H__

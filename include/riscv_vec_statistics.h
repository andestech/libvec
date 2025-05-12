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

#if defined (__riscv_zfh)
/**
 * @brief Maximum value of the half-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Maximum value
 *
 * This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”, please refer to Section 1.1.
 */
float16_t riscv_vec_max_f16(const float16_t * src, uint32_t size, uint32_t * index);

/**
 * @brief Maximum value of the half-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Maximum value
 */
float16_t riscv_vec_max_val_f16(const float16_t *src, uint32_t size);
#endif

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

#if defined (__riscv_zfh)
/**
 * @brief Minimum value of the half-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the minimum value
 * @return Minimum value
 *
 * This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”, please refer to Section 1.1.
 */
float16_t riscv_vec_min_f16(const float16_t * src, uint32_t size, uint32_t * index);

/**
 * @brief Minimum value of the half-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Minimum value
 *
 * This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”, please refer to Section 1.1.
 */
float16_t riscv_vec_min_val_f16(const float16_t *src, uint32_t size);
#endif

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

#if defined (__riscv_zfh)
/**
 * @brief Maximum absolute value of the half-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Absolute maximum value
 */
float16_t riscv_vec_absmax_f16(const float16_t* src, uint32_t size, uint32_t* index);

/**
 * @brief Maximum absolute value of the half-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Absolute maximum value
 *
 * This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”, please refer to Section 1.1.
 */
float16_t riscv_vec_absmax_val_f16(const float16_t *src, uint32_t size);
#endif

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

#if defined (__riscv_zfh)
/**
 * @brief Minimum absolute value of the half-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @param[out]      index   index of the maximum value
 * @return Absolute minimum value
 *
 * This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”, please refer to Section 1.1.
 */
float16_t riscv_vec_absmin_f16(const float16_t *src, uint32_t size, uint32_t *index);

/**
 * @brief Minimum absolute value of the half-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Absolute minimum value
 *
 * This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”, please refer to Section 1.1.
 */
float16_t riscv_vec_absmin_val_f16(const float16_t *src, uint32_t size);
#endif

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

#if defined (__riscv_zfh)
/**
 * @brief Mean value of the half-precision floating-point vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Mean value.
 *
 * This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”, please refer to Section 1.1.
 */
float16_t riscv_vec_mean_f16(const float16_t *src, uint32_t size);
#endif

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

#if defined (__riscv_zfh)
/**
 * @brief RMS of the half-precision floating-point vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return RMS value.
 */
float16_t riscv_vec_rms_f16(const float16_t *src, uint32_t size);
#endif

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

#if defined (__riscv_zfh)
/**
 * @brief Variance of the half-precision floating-point vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Variance value.
 */
float16_t riscv_vec_var_f16(const float16_t *src, uint32_t size);
#endif

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

#if defined (__riscv_zfh)
/**
 * @brief Standard deviation of the half-precision floating-point vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Standard deviation value.
 */
float16_t riscv_vec_std_f16(const float16_t *src, uint32_t size);
#endif

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
#if defined (__riscv_zfh)
/**
 * @brief Mean square error value of f16 vector
 * @param[in]     src1      pointer of the first input vector
 * @param[in]     src2      pointer of the second input vector
 * @param[in]     size      number of elements in a vector
 * @param[out]    result    pointer of the result
 */
void riscv_vec_mse_f16(const float16_t * src1, const float16_t * src2, uint32_t size, float16_t * result);
#endif

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
 * @brief Sum of the squares of the f32 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Sum of squares
 */
float32_t riscv_vec_pwr_f32(const float32_t *src, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Sum of the squares of the half-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       size    number of elements in a vector
 * @return Sum of squares
 */
float16_t riscv_vec_pwr_f16(const float16_t *src, uint32_t size);
#endif

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
/**
 * @defgroup entropy Entropy Function
 * @brief Entropy Function
 *
 * The entropy function computes the entropy of a vector. The behavior can be defined as follows:
 *
 * @image html entropy.gif ""
 *
 * Andes VEC library only supports the entropy function for floating-point data.
 */
/**
 * @addtogroup entropy
 * @{
 */
/**
 * @brief Entropy of the floating-point vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Entropy value.
 */
float64_t riscv_vec_entropy_f64(const float64_t * src, uint32_t size);

/**
 * @brief Entropy of the floating-point vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Entropy value.
 */
float32_t riscv_vec_entropy_f32(const float32_t * src, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Entropy of the floating-point vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return Entropy value.
 */
float16_t riscv_vec_entropy_f16(const float16_t * src, uint32_t size);
#endif
/** @} entropy */

/**
 * @defgroup rel_entropy Relative Entropy Function
 * @brief Relative Entropy Function
 *
 * The relative entropy function computes the entropy of two vectors and is also called the
 * Kullback–Leibler divergence (DKL). The behavior can be defined as follows:
 *
 * @image html rel_entropy.gif ""
 *
 * Andes VEC library only supports the relative entropy function for floating-point data.
 */
/**
 * @addtogroup rel_entropy
 * @{
 */
/**
 * @brief Relative Entropy of the floating-point vector.
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[in]       size   size of elements in a vector
 * @return Relative Entropy value.
 */
float64_t riscv_vec_relative_entropy_f64(const float64_t * src1, const float64_t * src2, uint32_t size);

/**
 * @brief Relative Entropy of the floating-point vector.
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[in]       size   size of elements in a vector
 * @return Relative Entropy value.
 */
float32_t riscv_vec_relative_entropy_f32(const float32_t * src1, const float32_t * src2, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Relative Entropy of the floating-point vector.
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[in]       size   size of elements in a vector
 * @return Relative Entropy value.
 */
float16_t riscv_vec_relative_entropy_f16(const float16_t * src1, const float16_t * src2, uint32_t size);
#endif
/** @} rel_entropy */

/**
 * @defgroup lse Log-Sum-Exp (LSE) Function
 * @brief LSE Function
 *
 * The LSE function computes the logarithm of the sum of the exponentials of a vector. The
 * behavior can be defined as follows:
 *
 * @image html lse.gif ""
 *
 * Andes DSP library only supports the LSE function for floating-point data.
 */
/**
 * @addtogroup lse
 * @{
 */
/**
 * @brief Log-Sum-Exp of the floating-point vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return LSE value.
 *
 */
float32_t riscv_vec_lse_f32(const float32_t * src, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Log-Sum-Exp of the floating-point vector.
 * @param[in]       src    pointer of the input vector
 * @param[in]       size   number of elements in a vector
 * @return LSE value.
 *
 */
float16_t riscv_vec_lse_f16(const float16_t * src, uint32_t size);
#endif
/** @} lse */

/**
 * @defgroup lse_dprod Log-Sum-Exp (LSE) Dot Product Function
 * @brief LSE Dot Product Function
 *
 * @image html lse_dprod.gif ""
 *
 * Andes DSP library only supports the LSE dot product function for floating-point data.
 */
/**
 * @addtogroup lse_dprod
 * @{
 */
/**
 * @brief Dot product with Log-Sum-Exp of the floating-point vector.
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[in]       size    size of elements in a vector
 * @param[in]       buffer  pointer of the temporary vector
 * @return LSE dot product value
 *
 */
float32_t riscv_vec_lse_dprod_f32(const float32_t *src1, const float32_t *src2, uint32_t size, float32_t *buffer);

#if defined (__riscv_zfh)
/**
 * @brief Dot product with Log-Sum-Exp of the floating-point vector.
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[in]       size    size of elements in a vector
 * @param[in]       buffer  pointer of the temporary vector
 * @return LSE dot product value
 *
 */
float16_t riscv_vec_lse_dprod_f16(const float16_t *src1, const float16_t *src2, uint32_t size, float16_t *buffer);
#endif
/** @} lse_dprod */

// Naive Gaussian Bayesian Estimator
/**
 * @defgroup naivebayes Naive Gaussian Bayesian Estimation Function
 * @brief Naive Gaussian Bayesian Estimation Function
 *
 * The function implements the naive Gaussian Bayes estimator.
 *
 * Andes DSP library only supports the naive GaussianBayesian predictor function for
 * floating-point data.
 */
/**
 * @addtogroup naivebayes
 * @{
 */
/**
 * @brief instance structure of the floating-point Gaussian naïve bayes estimator,
 */

typedef struct
{
    uint32_t dimofvec;            /**< dimension of vector space */
    uint32_t numofclass;          /**< number of classes */
    const float32_t *mean;        /**< mean value for the Gaussian whose size is dimofvec*numofclass */
    const float32_t *var;         /**< variance for the Gaussian whose size is dimofvec*numofclass */
    const float32_t *classprior;  /**< class prior probabilities whose size is numofclass */
    float32_t additiveofvar;      /**< additive value of variance */
} riscv_vec_gaussian_naivebayes_f32_t;

/**
 * @brief Naive Gaussian Bayesian Estimator
 *
 * @param[in]  instance  pointer of the instance structure
 * @param[in]  src       pointer of the input vector
 * @param[in]  buf       pointer of the buffer vector
 * @return Estimated class
 *
 */
uint32_t riscv_vec_gaussian_naive_bayes_est_f32(const riscv_vec_gaussian_naivebayes_f32_t *instance,
                                              const float32_t * src,
                                              float32_t *buf);

#if defined (__riscv_zfh)
/**
 * @brief instance structure of the floating-point Gaussian naïve bayes estimator,
 */

typedef struct
{
    uint32_t dimofvec;            /**< dimension of vector space */
    uint32_t numofclass;          /**< number of classes */
    const float16_t *mean;        /**< mean value for the Gaussian whose size is dimofvec*numofclass */
    const float16_t *var;         /**< variance for the Gaussian whose size is dimofvec*numofclass */
    const float16_t *classprior;  /**< class prior probabilities whose size is numofclass */
    float16_t additiveofvar;      /**< additive value of variance */
} riscv_vec_gaussian_naivebayes_f16_t;

/**
 * @brief Naive Gaussian Bayesian Estimator
 *
 * @param[in]  instance  pointer of the instance structure
 * @param[in]  src       pointer of the input vector
 * @param[in]  buf       pointer of the buffer vector
 * @return Estimated class
 *
 */
uint32_t riscv_vec_gaussian_naive_bayes_est_f16(const riscv_vec_gaussian_naivebayes_f16_t *instance,
                                              const float16_t * src,
                                              float16_t *buf);
#endif
/** @} naivebayes */

// Accumulation functions
/**
 * @defgroup accum Accumulation functions
 * @brief Accumulation functions
 *
 * Accumulation functions calculate the accumulate value of input vectors and return the accumulate results.
 *
 * Andes vec library supports distinct accumulation functions for single-, half- and double-precision floating-point,
 * Q31, Q15 ,Q7 and U8 data types. These functions are introduced in the subsections below.
 */

/**
 * @addtogroup accum
 * @{
 */
/**
 * @brief Accumulate value of f32 vector
 * @param[in]     src      pointer of the input vector
 * @param[in]     size     number of elements in a vector
 * @return Accumulate value
 */
float32_t riscv_vec_accum_f32(const float32_t * src, uint32_t size);

/**
 * @brief Accumulate value of f64 vector
 * @param[in]     src      pointer of the input vector
 * @param[in]     size     number of elements in a vector
 * @return Accumulate value
 */
float64_t riscv_vec_accum_f64(const float64_t * src, uint32_t size);

/**
 * @brief Accumulate value of q7 vector to q31 result.
 * @param[in]     src      pointer of the input vector
 * @param[in]     size     number of elements in a vector
 * @return Accumulate value
 */
q31_t riscv_vec_accum_q7(const q7_t * src, uint32_t size);

/**
 * @brief Accumulate value of q15 vector to q63 result.
 * @param[in]     src      pointer of the input vector
 * @param[in]     size     number of elements in a vector
 * @return Accumulate value
 */
q63_t riscv_vec_accum_q15(const q15_t * src, uint32_t size);

/**
 * @brief Accumulate value of q31 vector to q63 result.
 * @param[in]     src      pointer of the input vector
 * @param[in]     size     number of elements in a vector
 * @return Accumulate value
 */
q63_t riscv_vec_accum_q31(const q31_t * src, uint32_t size);

/**
 * @brief Accumulate value of u8 vector to u32 result.
 * @param[in]     src      pointer of the input vector
 * @param[in]     size     number of elements in a vector
 * @return Accumulate value
 */
uint32_t riscv_vec_accum_u8(const u8_t * src, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Accumulate value of f16 vector
 * @param[in]     src      pointer of the input vector
 * @param[in]     size     number of elements in a vector
 * @return Accumulate value
 */
float16_t riscv_vec_accum_f16(const float16_t * src, uint32_t size);
#endif
/** @} accum */

/**
 * @brief Signal-to-noise ratio (SNR) of the f32 vector.
 * @param[in]       ref     pointer of the input reference vector
 * @param[in]       noise   pointer of the input noise vector
 * @param[in]       size    number of elements in a vector
 * @return SNR value
 */
float32_t riscv_vec_snr_f32(float32_t * ref, float32_t * noise, uint32_t size);
/** @} statistics */

#ifdef  __cplusplus
}
#endif
#endif // __RISCV_VEC_STATISTICS_H__

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
#ifndef __RISCV_VEC_UTILS_H__
#define __RISCV_VEC_UTILS_H__

#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_math_types.h"
#include <riscv_vec_types.h>
#include <stdbool.h>

/**
 * @defgroup groupUtils Utils Functions
 */
/**
 * @addtogroup groupUtils
 * @{
 */

// Convert function
/**
 * @defgroup convert Conversion Functions
 * @brief Conversion Functions
 *
 * Conversion functions convert element values in a source vector from one data type to another
 * and write the results into a destination vector. There are following data type conversions:
 *
 * @image html convert.gif ""
 *
 * These conversion functions are introduced in the subsections below.
 */
/**
 * @addtogroup convert
 * @{
 */
#if defined (__riscv_zfh)
/**
 * @brief Function to convert values from f32 to f16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_f32_f16(float32_t *src, float16_t *dst, uint32_t size);
#endif

/**
 * @brief Function to convert values from f32 to q15
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 *
 * @b Note:
 *
 * The results will be saturated to the Q15 range [0x8000, 0x7FFF].
 */
void riscv_vec_convert_f32_q15(float32_t *src, q15_t *dst, uint32_t size);

/**
 * @brief Function to convert values from f32 to q31
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 *
 * @b Note:
 *
 * The results will be saturated to the Q31 range [0x80000000, 0x7FFFFFFF].
 */
void riscv_vec_convert_f32_q31(float32_t *src, q31_t *dst, uint32_t size);

/**
 * @brief Function to convert values from f32 to q7
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 *
 * @b Note:
 *
 * The results will be saturated to the Q7 range [0x80, 0x7F].
 */
void riscv_vec_convert_f32_q7(float32_t *src, q7_t *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Function to convert values from q15 to f16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 *
 * @b Note:
 *
 * This function is only supported by the toolchain with the HW FPU support. Also, you need to apply option “-mzfh” both in compiling flags and linking flags for applications that use this function. For details about the linking option“-mzfh”.
 */
void riscv_vec_convert_q15_f16(q15_t *src, float16_t *dst, uint32_t size);
#endif

/**
 * @brief Function to convert values from q15 to f32
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_q15_f32(q15_t *src, float32_t *dst, uint32_t size);

/**
 * @brief Function to convert values from q15 to q31
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_q15_q31(q15_t *src, q31_t *dst, uint32_t size);

/**
 * @brief Function to convert values from q15 to q7
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_q15_q7(q15_t *src, q7_t *dst, uint32_t size);

/**
 * @brief Function to convert values from q31 to f32
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_q31_f32(q31_t *src, float32_t *dst, uint32_t size);

/**
 * @brief Function to convert values from q31 to q15
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_q31_q15(q31_t *src, q15_t *dst, uint32_t size);

/**
 * @brief Function to convert values from q31 to q7
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_q31_q7(q31_t *src, q7_t *dst, uint32_t size);

/**
 * @brief Function to convert values from q7 to f32
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_q7_f32(q7_t *src, float32_t *dst, uint32_t size);

/**
 * @brief Function to convert values from q7 to q15
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_q7_q15(q7_t *src, q15_t *dst, uint32_t size);

/**
 * @brief Function to convert values from q7 to q31
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_q7_q31(q7_t *src, q31_t *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Function to convert values from f16 to f32
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 *
 * @b Note:
 *
 * This function is only supported by the toolchain with the HW FPU support. Also, you need to apply option “-mzfh” both in compiling flags and linking flags for applications that use this function. For details about the linking option“-mzfh”.
 */
void riscv_vec_convert_f16_f32(float16_t *src, float32_t *dst, uint32_t size);

/**
 * @brief Function to convert values from f16 to q15
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 *
 * @b Note:
 *
 * This function is only supported by the toolchain with the HW FPU support. Also, you need to apply option “-mzfh” both in compiling flags and linking flags for applications that use this function. For details about the linking option“-mzfh”.
 */
void riscv_vec_convert_f16_q15(float16_t *src, q15_t *dst, uint32_t size);
#endif

/**
 * @brief Convert a F64 vector to F32.
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_f64_f32(float64_t *src, float32_t *dst, uint32_t size);

/**
 * @brief Convert a F64 vector to Q31.
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_f64_q31(float64_t *src, q31_t *dst, uint32_t size);

/**
 * @brief Convert a F64 vector to Q15.
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_f64_q15(float64_t *src, q15_t *dst, uint32_t size);

/**
 * @brief Convert a F64 vector to Q7.
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_f64_q7(float64_t *src, q7_t *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Convert a F64 vector to F16.
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_f64_f16(float64_t *src, float16_t *dst, uint32_t size);
#endif

/**
 * @brief Convert a F32 vector to F64.
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_f32_f64(float32_t *src, float64_t *dst, uint32_t size);

/**
 * @brief Convert a Q31 vector to F64.
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_q31_f64(q31_t *src, float64_t *dst, uint32_t size);

/**
 * @brief Convert a Q15 vector to F64.
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_q15_f64(q15_t *src, float64_t *dst, uint32_t size);

/**
 * @brief Convert a Q7 vector to F64.
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_q7_f64(q7_t *src, float64_t *dst, uint32_t size);

#if defined (__riscv_zfh)
void riscv_vec_convert_f16_f64(float16_t *src, float64_t *dst, uint32_t size);
#endif

/**
 * @brief Convert a S8 vector to S16.
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_s8_s16(s8_t *src, s16_t *dst, uint32_t size);

/**
 * @brief Convert a S16 vector to S8.
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_s16_s8(s16_t *src, s8_t *dst, uint32_t size);

/**
 * @brief Function to convert values from s8 to s4
 * @param[in]       src         pointer of the input vector
 * @param[out]      dst         pointer of the output vector
 * @param[in]       size        number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_s8_s4(s8_t *src, s8_t *dst, uint32_t size);

/**
 * @brief Function to convert values from s4 to s8
 * @param[in]       src         pointer of the input vector
 * @param[out]      dst         pointer of the output vector
 * @param[in]       size        number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_s4_s8(s8_t *src, s8_t *dst, uint32_t size);

/**
 * @brief Function to convert values from q7 to q3
 * @param[in]       src         pointer of the input vector
 * @param[out]      dst         pointer of the output vector
 * @param[in]       size        number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_q7_q3(q7_t *src, q7_t *dst, uint32_t size);

/**
 * @brief Function to convert values from q3 to q7
 * @param[in]       src         pointer of the input vector
 * @param[out]      dst         pointer of the output vector
 * @param[in]       size        number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_q3_q7(q7_t *src, q7_t *dst, uint32_t size);

#if defined (__riscv_zfbfmin)
/**
 * @brief Function to convert values from bf16 to f32
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_bf16_f32(bf16_t *src, float32_t *dst, uint32_t size);

/**
 * @brief Function to convert values from f32 to bf16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_convert_f32_bf16(float32_t *src, bf16_t *dst, uint32_t size);
#endif

/** @} convert */

/**
 * @defgroup exp Vector Exponential Functions
 * @brief Vector Exponential Functions
 *
 * Vector exponential functions compute the exponential values from elements of a source vector and write them one-by-one into a destination vector.
 *
 * Andes vector library supports distinct exponential functions for single- and half-precision floating-point data. These functions are introduced in the subsections below.
 */

/**
 * @addtogroup exp
 * @{
 */
/**
 * @brief Vector exponential function for f32
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_exp_f32(float32_t *src, float32_t *dst, uint32_t size);

/**
 * @brief Vector exponential function for f64
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_exp_f64(float64_t *src, float64_t *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Vector exponential function for f16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 *
 * @b Note:
 *
 * This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”.
 */
void riscv_vec_exp_f16(float16_t *src, float16_t *dst, uint32_t size);
#endif

#if defined (__riscv_zfbfmin)
/**
 * @brief Vector exponential function for bf16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_exp_bf16(bf16_t *src, bf16_t *dst, uint32_t size);
#endif
/** @} exp */

/**
 * @defgroup sigmoid Vector Sigmoid Functions
 * @brief Vector Sigmoid Functions
 *
 * Vector sigmoid functions compute the sigmoid values from elements of a source vector and write them one-by-one into a destination vector.
 *
 * Andes vector library supports distinct sigmoid functions for single- and half-precision floating-point data. These functions are introduced in the subsections below.
 */

/**
 * @addtogroup sigmoid
 * @{
 */
/**
 * @brief Vector sigmoid function for f32
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_sigmoid_f32(float32_t *src, float32_t *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Vector sigmoid function for f16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 *
 * @b Note:
 *
 * This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”.
 */
void riscv_vec_sigmoid_f16(float16_t *src, float16_t *dst, uint32_t size);
#endif
/** @} */

/**
 * @defgroup log Vector Natural Logarithmic Functions
 * @brief Vector Natural Logarithmic Functions
 *
 * Vector natural logarithmic functions compute natural logarithmic values from elements of a source vector and write them one-by-one into a destination vector. The input value must be large than zero.
 *
 * Andes vector library supports distinct natural logarithmic functions for single- and half-precision floating-point data. These functions are introduced in the subsections below.
 */

/**
 * @addtogroup log
 * @{
 */
/**
 * @brief Vector natural logarithmic function for f64
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_log_f64(float64_t *src, float64_t *dst, uint64_t size);

/**
 * @brief Vector natural logarithmic function for f32
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_log_f32(float32_t *src, float32_t *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Vector natural logarithmic function for f16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 *
 * @b Note:
 *
 * This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”.
 */
void riscv_vec_log_f16(float16_t *src, float16_t *dst, uint32_t size);
#endif

#if defined (__riscv_zfbfmin)
/**
 * @brief Vector natural logarithmic function for bf16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_log_bf16(bf16_t *src, bf16_t *dst, uint32_t size);
#endif

/**
 * @brief Vector natural logarithmic function for q31
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_log_q31(q31_t * src, q31_t * dst, uint32_t size);

/**
 * @brief Vector natural logarithmic function for q15
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_log_q15(q15_t * src, q15_t * dst, uint32_t size);

/** @} log */

void riscv_vec_log2_f32(float32_t * src, float32_t * dst, uint32_t size);

#if defined (__riscv_zfh)
void riscv_vec_log2_f16(float16_t * src, float16_t * dst, uint32_t size);
#endif

/**
 * @defgroup sine Vector Sine Functions
 * @brief Vector Sine Functions
 *
 * Vector sine functions compute the cosine values from elements of a source vector and write them one-by-one into a destination vector. The input values of these functions are in radians and there is no limitination for the input range.
 *
 * Andes vector library supports sine function for the following data types: single- and half-precsion floating-point data types. These functions are introduced in the subsections below.
 */

/**
 * @addtogroup sine
 * @{
 */
/**
 * @brief Vector sine function for f32
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_sin_f32(float32_t *src, float32_t *dst, uint32_t size);
/**
 * @brief Vector sine function for f64
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_sin_f64(float64_t *src, float64_t *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Vector sine function for f16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 *
 * @b Note:
 *
 * This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”.
 */
void riscv_vec_sin_f16(float16_t *src, float16_t *dst, uint32_t size);
#endif

#if defined (__riscv_zfbfmin)
/**
 * @brief Vector sine function for bf16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_sin_bf16(bf16_t *src, bf16_t *dst, uint32_t size);
#endif

/**
 * @brief Vector sine function for q31
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_sin_q31(q31_t *src, q31_t *dst, uint32_t size);

/**
 * @brief Vector sine function for q15
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_sin_q15(q15_t *src, q15_t *dst, uint32_t size);


/** @} sine */

/**
 * @defgroup cosine Vector Cosine Functions
 * @brief Vector Cosine Functions
 *
 * Vector cosine functions compute the cosine values from elements of a source vector and write them one-by-one into a destination vector. The input values of these functions are in radians and there is no limitination for the input range.
 *
 * Andes vector library supports cosine function for the following data types: single- and half-precsion floating-point data types. These functions are introduced in the subsections below.
 */

/**
 * @addtogroup cosine
 * @{
 */
/**
 * @brief Vector cosine function for f32
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_cos_f32(float32_t *src, float32_t *dst, uint32_t size);

/**
 * @brief Vector cosine function for f64
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_cos_f64(float64_t *src, float64_t *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Vector cosine function for f16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 *
 * @b Note:
 *
 * This function is only supported by toolchains with the HW FPU support. Also, you will need to apply the option “-mzfh” both in compiler and linker flags for applications that use this function. For details about the linking option “-mzfh”.
 */
void riscv_vec_cos_f16(float16_t *src, float16_t *dst, uint32_t size);
#endif

#if defined (__riscv_zfbfmin)
/**
 * @brief Vector cosine function for bf16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_cos_bf16(bf16_t *src, bf16_t *dst, uint32_t size);
#endif

/**
 * @brief Vector sine function for q31
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_cos_q31(q31_t *src, q31_t *dst, uint32_t size);

/**
 * @brief Vector sine function for q15
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_cos_q15(q15_t *src, q15_t *dst, uint32_t size);

/** @} cosine */

/**
 * @addtogroup dup
 * @{
 */
/**
 * @brief Vector dup function for f32
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_dup_f32(float32_t* src, float32_t* dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Vector dup function for f16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_dup_f16(float16_t* src, float16_t* dst, uint32_t size);
#endif

/**
 * @brief Vector dup function for f64
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_dup_f64(float64_t* src, float64_t* dst, uint32_t size);

/**
 * @brief Vector dup function for q31
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_dup_q31(q31_t* src, q31_t* dst, uint32_t size);
/**
 * @brief Vector dup function for q15
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_dup_q15(q15_t* src, q15_t* dst, uint32_t size);
/**
 * @brief Vector dup function for q7
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_dup_q7(q7_t* src, q7_t* dst, uint32_t size);
/** @} dup */

/**
 * @addtogroup sqrt
 * @{
 */
/**
 * @brief Vector sqrt function for f32
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_sqrt_f32(float32_t* src, float32_t* dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Vector sqrt function for f16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_sqrt_f16(float16_t* src, float16_t* dst, uint32_t size);
#endif

#if defined (__riscv_zfbfmin)
/**
 * @brief Vector sqrt function for bf16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_sqrt_bf16(bf16_t* src, bf16_t* dst, uint32_t size);
#endif

/**
 * @brief Vector sqrt function for q31
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_sqrt_q31(q31_t* src, q31_t* dst, uint32_t size);

/**
 * @brief Vector sqrt function for q15
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_sqrt_q15(q15_t* src, q15_t* dst, uint32_t size);
/** @} sqrt */

/**
 * @addtogroup atan
 * @{
 */
/**
 * @brief Vector atan function for f32
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_atan_f32(float32_t *src, float32_t *dst, uint32_t size);
/** @} atan */

/**
 * @addtogroup atan2
 * @{
 */
/**
 * @brief Vector atan2 function for f32
 * @param[in]       srcy pointer of the input value y vector
 * @param[in]       srcx pointer of the input value x vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_atan2_f32(float32_t *srcy, float32_t * srcx, float32_t *dst, uint32_t size);
/** @} atan2 */

/**
 * @defgroup bary Barycenter Function
 * @brief Barycenter Function
 *
 * The function calculates the barycenter of the input vector.
 * Andes DSP library only supports the barycenter function for single-precision floating-point data.
 */
/**
 * @addtogroup bary
 * @{
 */
/**
 * @brief Barycenter of the single-precision floating-point type.
 * @param[in]       src         pointer of the input vector
 * @param[in]       weights     pointer of the weighted vector
 * @param[out]      out         pointer of the output vector
 * @param[in]       numofvec    number of input vectors
 * @param[in]       dimofvec    dimension of vector space
 * @return  None
 */
void riscv_vec_barycenter_f32(const float32_t *src, const float32_t *weights, float32_t *out, uint32_t numofvec, uint32_t dimofvec);

#if defined (__riscv_zfh)
/**
 * @brief Barycenter of the f16 type.
 * @param[in]       src         pointer of the input vector
 * @param[in]       weights     pointer of the weighted vector
 * @param[out]      out         pointer of the output vector
 * @param[in]       numofvec    number of input vectors
 * @param[in]       dimofvec    dimension of vector space
 * @return  None
 */
void riscv_vec_barycenter_f16(const float16_t *src, const float16_t *weights, float16_t *out, uint32_t numofvec, uint32_t dimofvec);
#endif
/** @} bary */

/**
 * @defgroup weighted Weighted Sum Function
 * @brief Weighted Sum Function
 *
 * This function calculates the weighted sum of the input vector.
 * Andes DSP library only supports the weighted sum function for single-precision floating-point data.
 */
/**
 * @addtogroup weighted
 * @{
 */
/**
 * @brief Weighted Sum of the single-precision floating-point vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       weight  pointer of the weighted vector
 * @param[in]       size    number of elements in a vector
 * @return Weighted sum value
 */
float32_t riscv_vec_weighted_sum_f32(const float32_t *src, const float32_t *weight, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Weighted Sum of the f16 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       weight  pointer of the weighted vector
 * @param[in]       size    number of elements in a vector
 * @return Weighted sum value
 */
float16_t riscv_vec_weighted_sum_f16(const float16_t *src, const float16_t *weight, uint32_t size);
#endif
/** @} weighted */

#if defined (__riscv_zfh)
/**
 * @brief inverse of the f16 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       weight  pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_inv_f16(float16_t *src, float16_t *dst, uint32_t size);
#endif

#if defined (__riscv_zfbfmin)
/**
 * @brief inverse of the bf16 vector.
 * @param[in]       src     pointer of the input vector
 * @param[in]       weight  pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_inv_bf16(bf16_t *src, bf16_t *dst, uint32_t size);
#endif

/**
 * @addtogroup ceil
 * @{
 */

/**
 * @brief Vector ceil function for f32
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_ceil_f32(float32_t *src, float32_t *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Vector ceil function for f16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_ceil_f16(float16_t *src, float16_t *dst, uint32_t size);
#endif

#if defined (__riscv_zfbfmin)
/**
 * @brief Vector ceil function for bf16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_ceil_bf16(bf16_t *src, bf16_t *dst, uint32_t size);
#endif
/** @} ceil */

/**
 * @addtogroup floor
 * @{
 */

/**
 * @brief Vector floor function for f32
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_floor_f32(float32_t *src, float32_t *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Vector floor function for f16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_floor_f16(float16_t *src, float16_t *dst, uint32_t size);
#endif

#if defined (__riscv_zfbfmin)
/**
 * @brief Vector floor function for bf16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_floor_bf16(bf16_t *src, bf16_t *dst, uint32_t size);
#endif
/** @} floor */

/**
 * @addtogroup equal
 * @{
 */
/**
 * @brief Vector equal function for f32
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[out]      dst     pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_equal_f32(float32_t *src1, float32_t *src2, bool *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Vector equal function for f16
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[out]      dst     pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_equal_f16(float16_t *src1, float16_t *src2, bool *dst, uint32_t size);
#endif

#if defined (__riscv_zfbfmin)
/**
 * @brief Vector equal function for bf16
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[out]      dst     pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_equal_bf16(bf16_t *src1, bf16_t *src2, bool *dst, uint32_t size);
#endif
/** @} equal */

/**
 * @addtogroup less
 * @{
 */
/**
 * @brief Vector less function for f32
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[out]      dst     pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_less_f32(float32_t *src1, float32_t *src2, bool *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Vector less function for f16
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[out]      dst     pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_less_f16(float16_t *src1, float16_t *src2, bool *dst, uint32_t size);
#endif

#if defined (__riscv_zfbfmin)
/**
 * @brief Vector less function for bf16
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[out]      dst     pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_less_bf16(bf16_t *src1, bf16_t *src2, bool *dst, uint32_t size);
#endif
/** @} less */

/**
 * @addtogroup greater
 * @{
 */
/**
 * @brief Vector greater function for f32
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[out]      dst     pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_greater_f32(float32_t *src1, float32_t *src2, bool *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Vector greater function for f16
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[out]      dst     pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_greater_f16(float16_t *src1, float16_t *src2, bool *dst, uint32_t size);
#endif

#if defined (__riscv_zfbfmin)
/**
 * @brief Vector greater function for bf16
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[out]      dst     pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_greater_bf16(bf16_t *src1, bf16_t *src2, bool *dst, uint32_t size);
#endif
/** @} greater */

/**
 * @addtogroup greater_equal
 * @{
 */
/**
 * @brief Vector greater_equal function for f32
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[out]      dst     pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_greater_equal_f32(float32_t *src1, float32_t *src2, bool *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Vector greater_equal function for f16
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[out]      dst     pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_greater_equal_f16(float16_t *src1, float16_t *src2, bool *dst, uint32_t size);
#endif

#if defined (__riscv_zfbfmin)
/**
 * @brief Vector greater_equal function for bf16
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[out]      dst     pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_greater_equal_bf16(bf16_t *src1, bf16_t *src2, bool *dst, uint32_t size);
#endif
/** @} greater_equal */

/**
 * @addtogroup less_equal
 * @{
 */
/**
 * @brief Vector less_equal function for f32
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[out]      dst     pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_less_equal_f32(float32_t *src1, float32_t *src2, bool *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Vector less_equal function for f16
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[out]      dst     pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_less_equal_f16(float16_t *src1, float16_t *src2, bool *dst, uint32_t size);
#endif

#if defined (__riscv_zfbfmin)
/**
 * @brief Vector less_equal function for bf16
 * @param[in]       src1    pointer of the first input vector
 * @param[in]       src2    pointer of the second input vector
 * @param[out]      dst     pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_less_equal_bf16(bf16_t * src1, bf16_t * src2, bool * dst, uint32_t size);
#endif
/** @} less_equal */

/**
 * @addtogroup merge
 * @{
 */

/**
 * @brief Vector merge function for e32
 * @param[in]       src1    pointer of the input vector
 * @param[in]       src2    pointer of the input vector
 * @param[out]      dst     pointer of the input vector
 * @param[in]       src_select  pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_merge_e32(void * src1, void * src2, void * dst, bool * src_select, uint32_t size);

/**
 * @brief Vector merge function for e16
 * @param[in]       src1    pointer of the input vector
 * @param[in]       src2    pointer of the input vector
 * @param[out]      dst     pointer of the input vector
 * @param[in]       src_select  pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_merge_e16(void * src1, void * src2, void * dst, bool * src_select, uint32_t size);

/**
 * @brief Vector merge function for e8
 * @param[in]       src1    pointer of the input vector
 * @param[in]       src2    pointer of the input vector
 * @param[out]      dst     pointer of the input vector
 * @param[in]       src_select  pointer of the output vector
 * @param[in]       size    number of elements in a vector
 * @return none.
 */
void riscv_vec_merge_e8(void * src1, void * src2, void * dst, bool * src_select, uint32_t size);
/** @} merge */

void riscv_vec_pow_f32(float32_t * src, float32_t * dst, float32_t exponent, uint32_t size);

#if defined (__riscv_zfh)
void riscv_vec_pow_f16(float16_t * src, float16_t * dst, float16_t exponent, uint32_t size);
#endif

#if defined (__riscv_zfbfmin)
void riscv_vec_pow_bf16(bf16_t * src, bf16_t * dst, bf16_t exponent, uint32_t size);
#endif

/**
 * @addtogroup rsqrt
 * @{
 */
/**
 * @brief Vector rsqrt function for f32
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_rsqrt_f32(float32_t* src, float32_t* dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Vector rsqrt function for f16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_rsqrt_f16(float16_t* src, float16_t* dst, uint32_t size);
#endif

#if defined (__riscv_zfbfmin)
/**
 * @brief Vector rsqrt function for f16
 * @param[in]       src  pointer of the input vector
 * @param[out]      dst  pointer of the output vector
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_vec_rsqrt_bf16(bf16_t* src, bf16_t* dst, uint32_t size);
#endif
/** @} rsqrt */

/**
 * This function used to get the lib version and sha-1
 */
char * get_version_libvec(void);

/** @} groupUtils */

#ifdef  __cplusplus
}
#endif

#endif

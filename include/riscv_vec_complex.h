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
#ifndef __RISCV_VEC_COMPLEX_H__
#define __RISCV_VEC_COMPLEX_H__

#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_math_types.h"
#include <riscv_vec_types.h>

/**
 * @defgroup cmplx Complex Functions
 * @brief Complex Functions
 *
 * Complex functions provide calculations for complex vectors. In Andes DSP library, the elements
 * of complex vectors should be arranged as real and imaginary parts interleaved. That is, the
 * memory layout should look like [real, imaginary, real, imaginary, ... , real,
 * imaginary].
 */

/**
 * @addtogroup cmplx
 * @{
 */

/**
 * @defgroup cmul Complex-Multiply-Complex Function
 * @brief Complex-Multiply-Complex Function
 *
 * Complex-multiply-complex functions compute the multiplication of two complex vectors and
 * write the results into a destination vector. The behavior can be defined as follows:
 *
 * @image html cmul.gif ""
 *
 * Andes vector library supports distinct complex-multiply-complex functions for the following data types: single- and half-precision floating-point and Q15. These functions are introduced in the subsections below.
 */
/**
 * @addtogroup cmul
 * @{
 */

/**
 * @brief Complex-multiply-complex function for f32
 * @param[out]		dst  pointer of the output complex vector
 * @param[in]		src1 pointer of the first input complex vector
 * @param[in]		src2 pointer of the second input complex vector
 * @param[in]		size  number of complex elements in a vector
 * @return none.
 *
 * @b Example
 *     <pre>
 * With two sets of complex vector each containing three complex numbers like [3 + 4i, 4 -
 * 7i, -3 + 5i] and [1 - 2i, 5 - 1i, -4 + 3i], the multiplication of the two complex
 * vectors is as follows:
 *      \#define size 3
 *      float32_t src1[2*size] = {3, 4, 4, -7, -3, 5};
 *      float32_t src2[2*size] = {1, -2, 5, -1, -4, 3};
 *      float32_t dst[2*size];
 *      riscv_vec_cmul_f32(dst, src1, src2, size);
 *     </pre>
 */
void riscv_vec_cmul_f32(float32_t * dst, float32_t * src1, float32_t * src2, uint32_t size);

/**
 * @brief Complex-multiply-complex function for q15
 * @param[out]		dst  pointer of the output complex vector
 * @param[in]		src1 pointer of the first input complex vector
 * @param[in]		src2 pointer of the second input complex vector
 * @param[in]		size  number of complex elements in a vector
 * @return none.
 *
 * @b Note:
 *
 * The values written into the destination vector are in Q13 format.
 */
void riscv_vec_cmul_q15(q15_t * dst, q15_t * src1, q15_t * src2, uint32_t size);
/** @} cmul */

/**
 * @defgroup cconj Complex Conjugate Function
 * @brief Complex Conjugate Function
 *
 * Complex conjugate functions calculate conjugation values of complex numbers from a source
 * vector and write the results into a destination vector. The behavior can be defined as follows:
 *
 * @image html cconj.gif ""
 *
 * Andes vector library supports distinct complex conjugate functions for the following data types: single- and half-precision floating-point and Q15. These functions are introduced in the subsections below.
 */
/**
 * @addtogroup cconj
 * @{
 */

/**
 * @brief Complex conjugate function for f32
 * @param[out]		dst pointer of the output complex vector
 * @param[in]		src pointer of the input complex vector
 * @param[in]		size number of complex elements in a vector
 * @return none.
 *
 * @b Example
 *     <pre>
 * With a set of complex vector containing three complex elements like {3 + 4i, 4 - 7i, -3
 * + 5i}, the complex conjugation is as follows:
 *
 *      //Complex vector is arranged with real and imaginary parts interleaved.
 *      \#define size 3
 *      float32_t src[2 * size] = {3.0, 4.0, 4.0, -7.0, -3.0, 5.0};
 *      float32_t dst[2 * size];
 *      riscv_vec_cconj_f32(dst, src, size);
 *     </pre>
 */
void riscv_vec_cconj_f32(float32_t * dst, float32_t * src, uint32_t size);

/**
 * @brief Complex conjugate function for q15
 * @param[out]		dst pointer of the output complex vector
 * @param[in]		src pointer of the input complex vector
 * @param[in]		size number of complex elements in a vector
 * @return none.
 *
 * @b Note:
 *
 * If negating the input imaginary part where the value is INT16_MIN (0x8000),
the output imaginary part will be INT16_MAX (0x7FFF) after complex conjugation.
 */
void riscv_vec_cconj_q15(q15_t * dst, q15_t * src, uint32_t size);

/** @} cconj */

/**
 * @defgroup cmag Complex Magnitude Function
 * @brief Complex Magnitude Function
 *
 * Complex magnitude functions compute the magnitude of a complex vector. The behavior can be
 * defined as follows:
 *
 * @image html cmag.gif ""
 *
 * Andes vector library supports distinct complex magnitude functions for single- and half-precision floating-point data. These functions are introduced in the subsections below.
 */
/**
 * @addtogroup cmag
 * @{
 */
/**
 * @brief Complex magnitude function for f32
 * @param[out]		dst pointer of the output vector
 * @param[in]		src pointer of the input complex vector
 * @param[in]		size number of complex elements in a vector
 * @return none.
 *
 * @b Example
 *     <pre>
 * With a set of complex vector containing three complex elements like {3 + 4i, 4 – 7i, -3 + 5i},
 * the complex magnitude is as follows:
 *
 *      \#define size 3
 *      float32_t src[2 * size] = {3.0, 4.0, 4.0, -7.0, -3.0, 5.0};
 *      float32_t dst[2 * size];
 *      riscv_vec_cmag_f32(dst, src, size);
 *     </pre>
 */
void riscv_vec_cmag_f32(float32_t * dst, float32_t * src, uint32_t size);

/**
 * @brief Complex magnitude function for q15_t
 * @param[out]		dst pointer of the output vector
 * @param[in]		src pointer of the input complex vector
 * @param[in]		size number of complex elements in a vector
 * @return none.
 */
void riscv_vec_cmag_q15(q15_t * dst, q15_t * src, uint32_t size);

/** @} cmag */

/**
 * @defgroup cmag_sqr Complex Magnitude-Squared Function
 * @brief Complex Magnitude-Squared Function
 *
 * Complex magnitude-squared functions compute the magnitude squared of complex numbers
 * from a complex vector. The behavior can be defined as follows:
 *
 * @image html cmag_sqr.gif ""
 *
 * Andes vector library supports complex magnitude-squared function for the following data types: single- and half-precision floating-point and Q15. These functions are introduced in the subsections below.
 */
/**
 * @addtogroup cmag_sqr
 * @{
 */
/**
 * @brief Complex magnitude-squared function for f32
 * @param[out]		dst pointer of the output vector
 * @param[in]		src pointer of the input complex vector
 * @param[in]		size number of complex elements in a vector
 * @return none.
 *
 * @b Example
 *     <pre>
 * With a set of complex vector containing three complex elements like {3 + 4i, 4 – 7i, -3 + 5i},
 * the complex magnitude is as follows:
 *
 *      \#define size 3
 *      float32_t src[2 * size] = {3.0, 4.0, 4.0, -7.0, -3.0, 5.0};
 *      float32_t dst[2 * size];
 *      riscv_vec_cmag_sqr_f32(dst, src, size);
 *     </pre>
 */
void riscv_vec_cmag_sqr_f32(float32_t * dst, float32_t * src, uint32_t size);

/**
 * @brief Complex magnitude-squared function for q15
 * @param[out]		dst pointer of the output vector
 * @param[in]		src pointer of the input complex vector
 * @param[in]		size number of complex elements in a vector
 * @return none.
 *
 * @b Note:
 *
 * The values written into the destination vector are in Q13 format.
 */
void riscv_vec_cmag_sqr_q15(q15_t *dst, q15_t *src, uint32_t size);

/** @} cmag_sqr */

/**
 * @defgroup cdprod Complex Dot Product Functions
 * @brief Complex Dot Product Functions
 *
 * Complex dot product functions calculate the dot product of two complex vectors and write the
 * results into a destination vector. The behavior can be defined as follows:
 *
 * @image html cdprod.gif ""
 *
 * Andes vector library supports distinct complex dot product functions for the following data types: single- and half-precision floating-point and Q15. These functions are introduced in the subsections below.
 */
/**
 * @addtogroup cdprod
 * @{
 */
/**
 * @brief Complex dot product function for f32
 * @param[out]		dst  pointer of the output complex vector
 * @param[in]		src1 pointer of the first input complex vector
 * @param[in]		src2 pointer of the second input complex vector
 * @param[in]		size  number of complex elements in a vector
 * @return none.
 *
 * @b Example
 *     <pre>
 * With two sets of complex vector each containing three complex numbers like {3 + 4i, 4 -
 * 7i, -3 + 5i} and {1 - 2i, 5 - 1i, -4 + 3i}, the complex dot production is as follows:
 *
 *      \#define size 3
 *      float32_t src1[2*size] = {3, 4, 4, -7, -3, 5};
 *      float32_t src2[2*size] = {1, -2, 5, -1, -4, 3};
 *      float32_t dst[2*size];
 *      riscv_dsp_cdprod_f32(dst, src1, src2, size);
 *     </pre>
 */
void riscv_vec_cdprod_f32(float32_t * dst, float32_t * src1, float32_t * src2, uint32_t size);

/**
 * @brief Complex dot product function for q15
 * @param[out]		dst  pointer of the output complex vector
 * @param[in]		src1 pointer of the first input complex vector
 * @param[in]		src2 pointer of the second input complex vector
 * @param[in]		size  number of complex elements in a vector
 * @return none.
 *
 * @b Note:
 *
 * The values written into the destination vector are in Q13 format.
 */
void riscv_vec_cdprod_q15(q15_t * dst, q15_t * src1, q15_t * src2, uint32_t size);
/** @} cdprod */

/**
 * @defgroup cmul_real Complex-Multiply-Real Functions
 * @brief Complex-Multiply-Real Functions
 *
 * Complex-multiply-real functions compute the multiplication of a complex vector by a real vector
 * and write the results into a destination complex vector. The behavior can be defined as follows:
 *
 * @image html cmul_real.gif ""
 *
 * Andes vector library supports distinct complex-multiply-real functions for the following data types: single- and half-precision floating-point and Q15. These functions are introduced in the subsections below.
 */
/**
 * @addtogroup cmul_real
 * @{
 */
/**
 * @brief Complex dot product function for f32
 * @param[out]		dst  pointer of the output complex vector
 * @param[in]		src  pointer of the input complex vector
 * @param[in]		real pointer of the input real vector
 * @param[in]		size number of complex elements in a vector
 * @return none.
 *
 * @b Example
 *     <pre>
 * With a complex vector containing three elements like [3 + 4i, 4 - 7i, -3 + 5i] and a
 * real vector containing three elements like [1, -3, 2], the multiplication of the two vectors is as follows:
 *
 *      \#define size 		3
 *      float32_t src[2*size] = {3, 4, 4, -7, -3, 5};
 *      float32_t real[size] = {1, -3, 2};
 *      float32_t dst[2*size];
 *      riscv_vec_cmul_real_f32(dst, src, real, size);
 *     </pre>
 */
void riscv_vec_cmul_real_f32(float32_t * dst, float32_t * src, float32_t * real, uint32_t size);

/**
 * @brief Complex dot product function for q15
 * @param[out]		dst  pointer of the output complex vector
 * @param[in]		src  pointer of the input complex vector
 * @param[in]		real pointer of the input real vector
 * @param[in]		size number of complex elements in a vector
 * @return none.
 */
void riscv_vec_cmul_real_q15(q15_t * dst, q15_t * src, q15_t * real, uint32_t size);

/** @} cmul_real */

/** @} cmplx */

#ifdef  __cplusplus
}
#endif

#endif

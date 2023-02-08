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
#ifndef __RISCV_VEC_BASIC_H__
#define __RISCV_VEC_BASIC_H__

/** @file*/

#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_math_types.h"
#include <riscv_vec_types.h>

/**
 * @defgroup groupBasic Basic Vector Functions
 */

/**
 * @addtogroup groupBasic
 * @{
 */

/**
 * @defgroup BasicAbs Vector Absolute Functions
 * @brief Vector absolute functions
 *
 * Vector absolute functions get absolute values from
 * elements of a source vector and write them one-by-one into a destination vector.
 */

/**
 * @addtogroup BasicAbs
 * @{
 */

/**
 * @brief Vector absolute function for f32
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] count number of elements to be processed
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count    3
 *     float32_t src[count] = {0.1, 0.2, 0.3};
 *     float32_t dst[count];
 *     riscv_vec_abs_f32(dst, src, count);
 *     </pre>
 */
int riscv_vec_abs_f32(float32_t * dst, float32_t * src, uint32_t count);

/**
 * @brief Vector absolute function for f32x2
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x2_t.
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count    3
 *     riscv_vec_f32x2_t src[count] = {{0,1, 0.2}, {-0.1, 0.3}, {-0.2, 0.3}};
 *     riscv_vec_f32x2_t dst[count];
 *     riscv_vec_abs_f32x2(dst, src, count);
 *
 *  This example also serves as a reference for examples of f32x3 and f32x4 vector absolute functions.
 *     </pre>
 */
int riscv_vec_abs_f32x2(riscv_vec_f32x2_t * dst, riscv_vec_f32x2_t * src, uint32_t count);

/**
 * @brief Vector absolute function for f32x3
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_abs_f32x3(riscv_vec_f32x3_t * dst, riscv_vec_f32x3_t * src, uint32_t count);

/**
 * @brief Vector absolute function for f32x4
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x4_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_abs_f32x4(riscv_vec_f32x4_t * dst, riscv_vec_f32x4_t * src, uint32_t count);
/** @} */

/**
 * @defgroup BasicAdd Vector Addition Functions
 *
 * @brief Vector addition functions
 *
 * Vector addition functions add a source vector to another source vector/scalar and
 * write the results one-by-one into a destination vector.
 */

/**
 * @addtogroup BasicAdd
 * @{
 */

 /**
  * @brief Vector addition function for f64
  *
  * @param[out] dst pointer of the output vector
  * @param[in] src1 pointer of the first input vector
  * @param[in] src2 pointer of the second input vector
  * @param[in] count number of elements to be processed
  * @return Return RISCV_VEC_OK only
  *
  * @b Example
  *     <pre>
  *     \#define count    3
  *     float64_t src1[count] = {0.1, 0.2, 0.3};
  *     float64_t src2[count] = {0.2, -0.3, 0.4};
  *     float64_t dst[count];
  *     riscv_vec_add_f64(dst, src1, src2, count);
  *     </pre>
  */
int riscv_vec_add_f64(float64_t* dst, float64_t* src1, float64_t* src2, uint32_t count);

/**
 * @brief Vector addition function for f32
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count    3
 *     float32_t src1[count] = {0.1, 0.2, 0.3};
 *     float32_t src2[count] = {0.2, -0.3, 0.4};
 *     float32_t dst[count];
 *     riscv_vec_add_f32(dst, src1, src2, count);
 *     </pre>
 */
int riscv_vec_add_f32(float32_t * dst, float32_t * src1, float32_t * src2, uint32_t count);

/**
 * @brief Vector addition function for f32x2
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x2_t.
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count    3
 *     riscv_vec_f32x2_t src1[count] = {
 *         {0.1, 0.2}, {-0.1, 0.3}, {0.3, 0.1}
 *     };
 *     riscv_vec_f32x2_t src2[count] = {
 *         {0.2, -0.3}, {-0.5, 0.1}, {-0.5, 0.1}
 *     };
 *     riscv_vec_f32x2_t dst[count];
 *     riscv_vec_add_f32x2(dst, src1, src2, count);
 *
 *  This example also serves as a reference for examples of f32x3 and f32x4 vector absolute functions.
 *     </pre>
 */
int riscv_vec_add_f32x2(riscv_vec_f32x2_t * dst, riscv_vec_f32x2_t * src1, riscv_vec_f32x2_t * src2, uint32_t count);

/**
 * @brief Vector addition function for f32x3
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_add_f32x3(riscv_vec_f32x3_t * dst, riscv_vec_f32x3_t * src1, riscv_vec_f32x3_t * src2, uint32_t count);

/**
 * @brief  Vector addition function for f32x4
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x4_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_add_f32x4(riscv_vec_f32x4_t * dst, riscv_vec_f32x4_t * src1, riscv_vec_f32x4_t * src2, uint32_t count);

/**
 * @brief Vector Add constant function for f32
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of float32_t
 * @param[in] count number of elements to be processed.
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count    3
 *     float32_t src[count] = {0.1, 0.2, 0.3};
 *     const float32_t cst = 0.2;
 *     float32_t dst[count];
 *     riscv_vec_addc_f32(dst, src, &cst, count);
 *     </pre>
 */
int riscv_vec_addc_f32(float32_t * dst, float32_t * src, const float32_t cst, uint32_t count);

/**
 * @brief Vector add constant function for f32X2
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x2_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x2_t.
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count    2
 *     riscv_vec_f32x2_t src[count] = {
 *          {0.1, 0.2}, {-0.1, 0.3}, {0.3, 0.1}
 *     };
 *     riscv_vec_f32x2_t cst = {0.2, -0.3};
 *     riscv_vec_f32x2_t dst[count];
 *     riscv_vec_addc_f32x2(dst, src, &cst, count);
 *
 *  This example also serves as a reference for examples of f32x3 and f32x4 vector absolute functions.
 *     </pre>
 */
int riscv_vec_addc_f32x2(riscv_vec_f32x2_t * dst, riscv_vec_f32x2_t * src, const riscv_vec_f32x2_t * cst, uint32_t count);

/**
 * @brief Vector add constant function for f32X3
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x3_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_addc_f32x3(riscv_vec_f32x3_t * dst, riscv_vec_f32x3_t * src, const riscv_vec_f32x3_t * cst, uint32_t count);

/**
 * @brief Vector add constant function for f32X4
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x4_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x4_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_addc_f32x4(riscv_vec_f32x4_t * dst, riscv_vec_f32x4_t * src, const riscv_vec_f32x4_t * cst, uint32_t count);
/** @} */

/**
 * @defgroup BasicCross Vector Cross Product Function
 *
 * @brief The cross product function
 *
 * The cross product function calculates the cross product of two vectors and write the results into
 * a destination vector.
 */

/**
 * @addtogroup BasicCross
 * @{
 */

/**
 * @brief Vector cross product function for f32X3
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count    3
 *     riscv_vec_f32x3_t src1[count] = {
 *         {0.1, 0.2, -0.1}, {-0.1, 0.3, 0.2}, {0.3, 0.1, -0.4}
 *     };
 *     riscv_vec_f32x3_t src2[count] = {
 *         {0.2, -0.3, 0.2}, {-0.5, 0.1, 0.3}, {-0.5, 0.1, -0.2}
 *     };
 *     riscv_vec_f32x3_t dst[count];
 *     riscv_vec_cross_f32x3(dst, src1, src2, count);
 *     </pre>
 */
int riscv_vec_cross_f32x3(riscv_vec_f32x3_t * dst, riscv_vec_f32x3_t * src1, riscv_vec_f32x3_t * src2, uint32_t count);
/** @} */

/**
 * @defgroup BasicDiv Vector Division Functions
 *
 * @brief Vector division functions
 *
 * Vector division functions divide a source vector by another source vector/scalar and write the results
 * one-by-one into a destination vector.
 */

/**
  * @addtogroup BasicDiv
  * @{
  */

/**
 * @brief Vector division function for f32
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	3
 *     float32_t src1[count] = {0.1, 0.2, 0.3};
 *     float32_t src2[count] = {0.2, -0.3, 0.4};
 *     float32_t dst[count];
 *     riscv_vec_div_f32(dst, src1, src2, count);
 *     </pre>
 */
int riscv_vec_div_f32(float32_t *dst, float32_t *src1, float32_t *src2, uint32_t count);

/**
 * @brief Vector division function f32X2
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x2_t.
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	   3
 *     riscv_vec_f32x2_t src1[count] = {
 *         {0.1, 0.2}, {-0.1, 0.3}, {0.3, 0.1}
 *     };
 *     riscv_vec_f32x2_t src2[count] = {
 *         {0.2, -0.3}, {-0.5, 0.1}, {-0.5, 0.1}
 *     };
 *     riscv_vec_f32x2_t dst[count];
 *     riscv_vec_div_f32x2(dst, src1, src2, count);
 *
 *  This example also serves as a reference for examples of f32x3 and f32x4 vector absolute functions.
 *     </pre>
 */
int riscv_vec_div_f32x2(riscv_vec_f32x2_t *dst, riscv_vec_f32x2_t *src1, riscv_vec_f32x2_t *src2, uint32_t count);

/**
 * @brief Vector division function f32X3
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_div_f32x3(riscv_vec_f32x3_t *dst, riscv_vec_f32x3_t *src1, riscv_vec_f32x3_t *src2, uint32_t count);

/**
 * @brief Vector division function for f32X4
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x4_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_div_f32x4(riscv_vec_f32x4_t *dst, riscv_vec_f32x4_t *src1, riscv_vec_f32x4_t *src2, uint32_t count);

/**
 * @brief Vector divide constant function for f32
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of float32_t
 * @param[in] count number of the vector
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	3
 *     float32_t src[count] = {0.1, 0.2, 0.3};
 *     const float32_t cst = 0.2;
 *     float32_t dst[count];
 *     riscv_vec_divc_f32(dst, src, &cst, count);
 *     </pre>
 */
int riscv_vec_divc_f32(float32_t *dst, float32_t *src, const float32_t cst, uint32_t count);

/**
 * @brief Vector divide constant function for f32x2
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x2_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x2_t.
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	   2
 *     riscv_vec_f32x2_t src[count] = {
 *         {0.1, 0.2}, {-0.1, 0.3}, {0.3, 0.1}
 *     };
 *     riscv_vec_f32x2_t cst = {0.2, -0.3};
 *     riscv_vec_f32x2_t dst[count];
 *     riscv_vec_divc_f32x2(dst, src, &cst, count);
 *
 *  This example also serves as a reference for examples of f32x3 and f32x4 vector absolute functions.
 *     </pre>
 */
int riscv_vec_divc_f32x2(riscv_vec_f32x2_t *dst, riscv_vec_f32x2_t *src, const riscv_vec_f32x2_t *cst, uint32_t count);

/**
 * @brief Vector divide constant function for f32X3
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x3_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_divc_f32x3(riscv_vec_f32x3_t *dst, riscv_vec_f32x3_t *src, const riscv_vec_f32x3_t *cst, uint32_t count);

/**
 * @brief Vector divide constant function for f32X4
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x4_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x4_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_divc_f32x4(riscv_vec_f32x4_t *dst, riscv_vec_f32x4_t *src, const riscv_vec_f32x4_t *cst, uint32_t count);
/** @} */

/**
 * @defgroup BasicDprod Vector Dot Product Functions
 *
 * @brief Vector dot product functions
 *
 * Vector dot product functions calculate the dot product of two vectors and write the results into a
 * destination output.
 */

/**
  * @addtogroup BasicDprod
  * @{
  */

/**
 * @brief Vector dot product function for f32x2
 *
 * @param[out] dst Dot product of the two input vectors
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x2_t.
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	   3
 *     riscv_vec_f32x2_t src1[count] = {
 *         {0.1, 0.2}, {-0.1, 0.3}, {0.3, 0.1}
 *     };
 *     riscv_vec_f32x2_t src2[count] = {
 *         {0.2, -0.3}, {-0.5, 0.1}, {-0.5, 0.1}
 *     };
 *     riscv_vec_f32x2_t dst[count];
 *     riscv_vec_dprod_f32x2(dst, src1, src2, count);
 *
 *  This example also serves as a reference for examples of f32x3 and f32x4 vector absolute functions.
 *     </pre>
 */
int riscv_vec_dprod_f32x2(float32_t *dst, riscv_vec_f32x2_t *src1, riscv_vec_f32x2_t *src2, uint32_t count);

/**
 * @brief Vector dot product function for f32X3
 *
 * @param[out] dst Dot product of the two input vectors
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_dprod_f32x3(float32_t *dst, riscv_vec_f32x3_t *src1, riscv_vec_f32x3_t *src2, uint32_t count);

/**
 * @brief Vector dot product function for f32X4
 *
 * @param[out] dst Dot product of the two input vectors
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x4_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_dprod_f32x4(float32_t *dst, riscv_vec_f32x4_t *src1, riscv_vec_f32x4_t *src2, uint32_t count);
/** @} */

/**
 * @defgroup BasicMadd Vector Multiply and Add Functions
 *
 * @brief Vector multiply and add functions
 *
 * Vector multiply and add functions calculate the multiplication of two vectors and accumulate the
 * results into a destination vector.
 */

/**
  * @addtogroup BasicMadd
  * @{
  */

/**
 * @brief Vector multiply and add function for f32
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	3
 *     float32_t src1[count] = {0.1, 0.2, 0.3};
 *     float32_t src2[count] = {0.2, -0.3, 0.4};
 *     float32_t dst[count];
 *     riscv_vec_madd_f32(dst, src1, src2, count);
 *     </pre>
 */
int riscv_vec_madd_f32(float32_t *dst, float32_t *src1, float32_t *src2, uint32_t count);

/**
 * @brief Vector multiply and add function for f32X2
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x2_t.
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	   3
 *     riscv_vec_f32x2_t src1[count] = {
 *         {0.1, 0.2}, {-0.1, 0.3}, {0.3, 0.1}
 *     };
 *     riscv_vec_f32x2_t src2[count] = {
 *         {0.2, -0.3}, {-0.5, 0.1}, {-0.5, 0.1}
 *     };
 *     riscv_vec_f32x2_t dst[count];
 *     riscv_vec_madd_f32x2(dst, src1, src2, count);
 *
 *  This example also serves as a reference for examples of f32x3 and f32x4 vector absolute functions.
 *     </pre>
 */
int riscv_vec_madd_f32x2(riscv_vec_f32x2_t *dst, riscv_vec_f32x2_t *src1, riscv_vec_f32x2_t *src2, uint32_t count);

/**
 * @brief Vector multiply and add function for f32X3
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_madd_f32x3(riscv_vec_f32x3_t *dst, riscv_vec_f32x3_t *src1, riscv_vec_f32x3_t *src2, uint32_t count);

/**
 * @brief Vector multiply and add function for f32X4
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x4_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_madd_f32x4(riscv_vec_f32x4_t *dst, riscv_vec_f32x4_t *src1, riscv_vec_f32x4_t *src2, uint32_t count);

/**
 * @brief Vector multiply and add constant function for f32
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of float32_t
 * @param[in] count number of elements to be processed
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	3
 *     float32_t src[count] = {0.1, 0.2, 0.3};
 *     const float32_t cst = 0.2;
 *     float32_t dst[count];
 *     riscv_vec_maddc_f32(dst, src, &cst, count);
 *     </pre>
 */
int riscv_vec_maddc_f32(float32_t *dst, float32_t *src, const float32_t cst, uint32_t count);

/**
 * @brief Vector multiply and Add constant function for f32x2
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x2_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x2_t.
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	   2
 *     riscv_vec_f32x2_t src[count] = {
 *     {0.1, 0.2}, {-0.1, 0.3}, {0.3, 0.1}
 *     };
 *     riscv_vec_f32x2_t cst = {0.2, -0.3};
 *     riscv_vec_f32x2_t dst[count];
 *     riscv_vec_maddc_f32x2(dst, src, &cst, count);
 *
 *  This example also serves as a reference for examples of f32x3 and f32x4 vector absolute functions.
 *     </pre>
 */
int riscv_vec_maddc_f32x2(riscv_vec_f32x2_t *dst, riscv_vec_f32x2_t *src, const riscv_vec_f32x2_t *cst, uint32_t count);

/**
 * @brief Vector multiply and add constant function for f32x3
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x3_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_maddc_f32x3(riscv_vec_f32x3_t *dst, riscv_vec_f32x3_t *src, const riscv_vec_f32x3_t *cst, uint32_t count);

/**
 * @brief Vector multiply and add constant function for f32x4
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x4_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x4_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_maddc_f32x4(riscv_vec_f32x4_t *dst, riscv_vec_f32x4_t *src, const riscv_vec_f32x4_t *cst, uint32_t count);
/** @} */

/**
 * @defgroup BasicMag Vector Magnitude Functions
 *
 * @brief Magnitude functions
 *
 * Magnitude functions compute the magnitude of a vector and store the results into a destination vector.
 */

/**
  * @addtogroup BasicMag
  * @{
  */

/**
 * @brief Vector magnitude function for f32X2
 *
 * @param[out] dst Magnitude of the input vector
 * @param[in] src pointer of the input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x2_t.
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	   3
 *     riscv_vec_f32x2_t src[count] = {{0,1, 0.2}, {-0.1, 0.3}, {-0.2, 0.3}};
 *     float32_t dst[count];
 *     riscv_vec_mag_f32x2(dst, src, count);
 *
 *  This example also serves as a reference for examples of f32x3 and f32x4 vector absolute functions.
 *     </pre>
 */
int riscv_vec_mag_f32x2(float32_t *dst, riscv_vec_f32x2_t *src, uint32_t count);

/**
 * @brief Vector magnitude function for f32X3
 *
 * @param[out] dst Magnitude of the input vector
 * @param[in] src pointer of the input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_mag_f32x3(float32_t *dst, riscv_vec_f32x3_t *src, uint32_t count);

/**
 * @brief Vector magnitude function for f32X4
 *
 * @param[out] dst Magnitude of the input vector
 * @param[in] src pointer of the input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x4_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_mag_f32x4(float32_t *dst, riscv_vec_f32x4_t *src, uint32_t count);
/** @} */

/**
 * @defgroup BasicMul Vector Multiplication Functions
 *
 * @brief Vector multiplication functions
 *
 * Vector multiplication functions multiply a source vector by another source vector/scalar and write the
 * results one-by-one into a destination vector.
 */

/**
  * @addtogroup BasicMul
  * @{
  */

/**
 * @brief Vector multiplication function for f32
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	3
 *     float32_t src1[count] = {0.1, 0.2, 0.3};
 *     float32_t src2[count] = {0.2, -0.3, 0.4};
 *     float32_t dst[count];
 *     riscv_vec_mul_f32(dst, src1, src2, count);
 *     </pre>
 */
int riscv_vec_mul_f32(float32_t *dst, float32_t *src1, float32_t *src2, uint32_t count);

/**
 * @brief Vector multiplication function for f32X2
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x2_t.
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	   3
 *     riscv_vec_f32x2_t src1[count] = {
 *         {0.1, 0.2}, {-0.1, 0.3}, {0.3, 0.1}
 *     };
 *     riscv_vec_f32x2_t src2[count] = {
 *         {0.2, -0.3}, {-0.5, 0.1}, {-0.5, 0.1}
 *     };
 *     riscv_vec_f32x2_t dst[count];
 *     riscv_vec_mul_f32x2(dst, src1, src2, count);
 *
 *  This example also serves as a reference for examples of f32x3 and f32x4 vector absolute functions.
 *     </pre>
 */
int riscv_vec_mul_f32x2(riscv_vec_f32x2_t *dst, riscv_vec_f32x2_t *src1, riscv_vec_f32x2_t *src2, uint32_t count);

/**
 * @brief Vector multiplication function for f32X3
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_mul_f32x3(riscv_vec_f32x3_t *dst, riscv_vec_f32x3_t *src1, riscv_vec_f32x3_t *src2, uint32_t count);

/**
 * @brief Vector multiplication function for f32X4
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x4_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_mul_f32x4(riscv_vec_f32x4_t *dst, riscv_vec_f32x4_t *src1, riscv_vec_f32x4_t *src2, uint32_t count);

/**
 * @brief Vector multipy constant function for f32
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of float32_t
 * @param[in] count number of elements to be processed
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	3
 *     float32_t src[count] = {0.1, 0.2, 0.3};
 *     const float32_t cst = 0.2;
 *     float32_t dst[count];
 *     riscv_vec_mulc_f32(dst, src, &cst, count);
 *     </pre>
 */
int riscv_vec_mulc_f32(float32_t *dst, float32_t *src, const float32_t cst, uint32_t count);

/**
 * @brief Vector multipy constant function for f32X2
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x2_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x2_t.
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	   2
 *     riscv_vec_f32x2_t src[count] = {
 *         {0.1, 0.2}, {-0.1, 0.3}, {0.3, 0.1}
 *     };
 *     riscv_vec_f32x2_t cst = {0.2, -0.3};
 *     riscv_vec_f32x2_t dst[count];
 *     riscv_vec_mulc_f32x2(dst, src, &cst, count);
 *
 *  This example also serves as a reference for examples of f32x3 and f32x4 vector absolute functions.
 *     </pre>
 */
int riscv_vec_mulc_f32x2(riscv_vec_f32x2_t *dst, riscv_vec_f32x2_t *src, const riscv_vec_f32x2_t *cst, uint32_t count);

/**
 * @brief Vector multipy constant function for f32X3
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x3_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_mulc_f32x3(riscv_vec_f32x3_t *dst, riscv_vec_f32x3_t *src, const riscv_vec_f32x3_t *cst, uint32_t count);

/**
 * @brief Vector multipy constant function for f32X4
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x4_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x4_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_mulc_f32x4(riscv_vec_f32x4_t *dst, riscv_vec_f32x4_t *src, const riscv_vec_f32x4_t *cst, uint32_t count);
/** @} */

/**
 * @defgroup BasicSet Vector Set Functions
 *
 * @brief Vector set functions
 *
 * Vector set functions set all elements in a destination vector to a constant value.
 */

/**
  * @addtogroup BasicSet
  * @{
  */

/**
 * @brief Vector set constant function for f32
 *
 * @param[out] dst pointer of the output vector
 * @param[in] cst constant value with data type of float32_t
 * @param[in] count number of elements to be processed
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	3
 *     const float32_t cst = 0.2;
 *     float32_t dst[count];
 *     riscv_vec_setc_f32(dst, &cst, count);
 *     </pre>
 */
int riscv_vec_setc_f32(float32_t *dst, const float32_t cst, uint32_t count);

/**
 * @brief Vector Set constant function for f32X2
 *
 * @param[out] dst pointer of the output vector
 * @param[in] cst constant value with data type of riscv_vec_f32x2_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x2_t.
 * @return Return RISCV_VEC_OK only
 *
 *
 * @b Example
 *     <pre>
 *     \#define count	   2
 *     riscv_vec_f32x2_t cst = {0.2, -0.3};
 *     riscv_vec_f32x2_t dst[count];
 *     riscv_vec_setc_f32x2(dst, &cst, count);
 *
 *  This example also serves as a reference for examples of f32x3 and f32x4 vector absolute functions.
 *     </pre>
 */
int riscv_vec_setc_f32x2(riscv_vec_f32x2_t *dst, const riscv_vec_f32x2_t *cst, uint32_t count);

/**
 * @brief Vector set constant function for f32X3
 *
 * @param[out] dst pointer of the output vector
 * @param[in] cst constant value with data type of riscv_vec_f32x3_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_setc_f32x3(riscv_vec_f32x3_t *dst, const riscv_vec_f32x3_t *cst, uint32_t count);

/**
 * @brief Vector set constant function for f32X4
 *
 * @param[out] dst pointer of the output vector
 * @param[in] cst constant value with data type of riscv_vec_f32x4_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x4_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_setc_f32x4(riscv_vec_f32x4_t *dst, const riscv_vec_f32x4_t *cst, uint32_t count);
/** @} */

/**
 * @defgroup BasicSub Vector Subtraction Functions
 *
 * @brief Vector subtraction functions
 *
 * Vector subtraction functions subtract elements in a vector from those of the same index in
 * another vector/scalar and write the results one-by-one into a destination vector.
 */

/**
 * @addtogroup BasicSub
 * @{
 */

 /**
  * @brief Vector subtraction function for f64
  *
  * @param[out] dst pointer of the output vector
  * @param[in] src1 pointer of the first input vector
  * @param[in] src2 pointer of the second input vector
  * @param[in] count number of elements to be processed
  * @return Return RISCV_VEC_OK only
  *
  * @b Example
  *     <pre>
  *     \#define count    3
  *     float64_t src1[count] = {0.1, 0.2, 0.3};
  *     float64_t src2[count] = {0.2, -0.3, 0.4};
  *     float64_t dst[count];
  *     riscv_vec_sub_f64(dst, src1, src2, count);
  *     </pre>
  */
int riscv_vec_sub_f64(float64_t* dst, float64_t* src1, float64_t* src2, uint32_t count);

/**
 * @brief Vector subtraction function for f32
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count    3
 *     float32_t src1[count] = {0.1, 0.2, 0.3};
 *     float32_t src2[count] = {0.2, -0.3, 0.4};
 *     float32_t dst[count];
 *     riscv_vec_sub_f32(dst, src1, src2, count);
 *     </pre>
 */
int riscv_vec_sub_f32(float32_t *dst, float32_t *src1, float32_t *src2, uint32_t count);

/**
 * @brief Vector subtraction function for f32X2
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x2_t.
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	   3
 *     riscv_vec_f32x2_t src1[count] = {
 *         {0.1, 0.2}, {-0.1, 0.3}, {0.3, 0.1}
 *     };
 *     riscv_vec_f32x2_t src2[count] = {
 *         {0.2, -0.3}, {-0.5, 0.1}, {-0.5, 0.1}
 *     };
 *     riscv_vec_f32x2_t dst[count];
 *     riscv_vec_sub_f32x2(dst, src1, src2, count);
 *
 *  This example also serves as a reference for examples of f32x3 and f32x4 vector absolute functions.
 *     </pre>
 */
int riscv_vec_sub_f32x2(riscv_vec_f32x2_t *dst, riscv_vec_f32x2_t *src1, riscv_vec_f32x2_t *src2, uint32_t count);

/**
 * @brief Vector subtraction function for f32X3
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src1 pointer of the first input vector
 * @param[in] src2 pointer of the second input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_sub_f32x3(riscv_vec_f32x3_t *dst, riscv_vec_f32x3_t *src1, riscv_vec_f32x3_t *src2, uint32_t count);

/**
 * @brief Vector subtraction function for f32X4
 *
 * @param[out] dst pointer of the output 4D vector
 * @param[in] src1 pointer of the first input 4D vector
 * @param[in] src2 pointer of the second input 4D vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x4_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_sub_f32x4(riscv_vec_f32x4_t *dst, riscv_vec_f32x4_t *src1, riscv_vec_f32x4_t *src2, uint32_t count);

/**
 * @brief Vector subtract constant function for f32
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of float32_t
 * @param[in] count number of elements to be processed
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	3
 *     float32_t src[count] = {0.1, 0.2, 0.3};
 *     const float32_t cst = 0.2;
 *     float32_t dst[count];
 *     riscv_vec_subc_f32(dst, src, &cst, count);
 *     </pre>
 */
int riscv_vec_subc_f32(float32_t *dst, float32_t *src, const float32_t cst, uint32_t count);

/**
 * @brief Vector subtract constant function for f32x2
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x2_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x2_t.
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	   2
 *     riscv_vec_f32x2_t src[count] = {
 *         {0.1, 0.2}, {-0.1, 0.3}, {0.3, 0.1}
 *     };
 *     riscv_vec_f32x2_t cst = {0.2, -0.3};
 *     riscv_vec_f32x2_t dst[count];
 *     riscv_vec_subc_f32x2(dst, src, &cst, count);
 *
 *  This example also serves as a reference for examples of f32x3 and f32x4 vector absolute functions.
 *     </pre>
 */
int riscv_vec_subc_f32x2(riscv_vec_f32x2_t *dst, riscv_vec_f32x2_t *src, const riscv_vec_f32x2_t *cst, uint32_t count);

/**
 * @brief Vector subtract constant function for f32x3
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x3_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_subc_f32x3(riscv_vec_f32x3_t *dst, riscv_vec_f32x3_t *src, const riscv_vec_f32x3_t *cst, uint32_t count);

/**
 * @brief Vector subtract constant function for f32x4
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x4_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x4_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_subc_f32x4(riscv_vec_f32x4_t *dst, riscv_vec_f32x4_t *src, const riscv_vec_f32x4_t *cst, uint32_t count);

/**
 * @brief Constant subtract vector function for f32
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of float32_t
 * @param[in] count number of elements to be processed
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count 3
 *
 *     float32_t src[count] = {0.1, 0.4, -0.2};
 *     const float32_t cst = 0.2;
 *     float32_t dst[count] = {0};
 *     riscv_vec_rsubc_f32(dst, src, cst, count);
 *
 *     Outputs of the above example are as follows.
 *     dst[0] = 0.2 - 0.1 = 0.1
 *     dst[1] = 0.2 - 0.4 = -0.2
 *     dst[2] = 0.2 - (-0.2) = 0.0
 *     </pre>
 */
int riscv_vec_rsubc_f32(float32_t *dst, float32_t *src, const float32_t cst, uint32_t count);

/**
 * @brief Constant subtract vector function for f32X2
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x2_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x2_t.
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count 3
 *
 *     riscv_vec_f32x2_t src[count] = {{0.1, 0.2}, {0.4, -0.1}, {-0.2, 0.3}};
 *     const riscv_vec_f32x2_t cst = {0.2, 0.1};
 *     riscv_vec_f32x2_t dst[count];
 *     riscv_vec_rsubc_f32x2(dst, src, cst, count);
 *
 *     Outputs of the above example are as follows.
 *     dst[0] = {0.1, -0.1}
 *     dst[1] = {-0.2, 0.2}
 *     dst[2] = {0.4, -0.2}
 *
 *  This example also serves as a reference for examples of f32x3 and f32x4 vector absolute functions.
 *     </pre>
 */
int riscv_vec_rsubc_f32x2(riscv_vec_f32x2_t *dst, riscv_vec_f32x2_t *src, const riscv_vec_f32x2_t *cst, uint32_t count);

/**
 * @brief Constant subtract vector function for f32X3
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x3_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_rsubc_f32x3(riscv_vec_f32x3_t *dst, riscv_vec_f32x3_t *src, const riscv_vec_f32x3_t *cst, uint32_t count);

/**
 * @brief Constant subtract vector function for f32X4
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] cst constant value with data type of riscv_vec_f32x4_t
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x4_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_rsubc_f32x4(riscv_vec_f32x4_t *dst, riscv_vec_f32x4_t *src, const riscv_vec_f32x4_t *cst, uint32_t count);
/** @} */

/**
 * @defgroup BasicNorm Vector Normalization Functions
 *
 * @brief Vector normalization functions
 *
 * Vector normalization functions normalize source vectors and store the results into a destination vector.
 */

/**
 * @addtogroup BasicNorm
 * @{
 */

/**
 * @brief Vector normalization function for f32X2
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x2_t.
 * @return Return RISCV_VEC_OK only
 *
 * @b Example
 *     <pre>
 *     \#define count	   3
 *     riscv_vec_f32x2_t src[count] = {{0,1, 0.2}, {-0.1, 0.3}, {-0.2, 0.3}};
 *     riscv_vec_f32x2_t dst[count];
 *     riscv_vec_normalize_f32x2(dst, src, count);
 *
 *  This example also serves as a reference for examples of f32x3 and f32x4 vector absolute functions.
 *     </pre>
 */
int riscv_vec_normalize_f32x2(riscv_vec_f32x2_t *dst, riscv_vec_f32x2_t *src, uint32_t count);

/**
 * @brief Vector normalization function for f32X3
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_normalize_f32x3(riscv_vec_f32x3_t *dst, riscv_vec_f32x3_t *src, uint32_t count);

/**
 * @brief Vector normalization function for f32X4
 *
 * @param[out] dst pointer of the output vector
 * @param[in] src pointer of the input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x4_t.
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_normalize_f32x4(riscv_vec_f32x4_t *dst, riscv_vec_f32x4_t *src, uint32_t count);
/** @} */

/**
 * @defgroup neg Negate Functions
 * @brief Negate Functions
 *
 * Negate functions negate elements of a source vector and write the results one-by-one into a
 * destination vector. The behavior can be defined as follows:
 *
 * @image html neg.gif ""
 *
 * Andes vector library only supports negate function for the floating
 * point data type.
 */
/**
 * @addtogroup neg
 * @{
 */
/**
 * @brief Negation of floating-potint vectors.
 * @param[out]      dst pointer of the output vector
 * @param[in]       src pointer of the input vector
 * @param[in]       count number of elements to be processed
 * @return Return RISCV_VEC_OK only
 */
int riscv_vec_neg_f32(float32_t * dst, float32_t * src, uint32_t count);

/** @} */

/**
 * @defgroup dprod Inner Product Functions
 * @brief Inner Product Functions
 *
 * Inner product functions calculate the inner product of two vectors and return the result values. The
 * definition of inner dot production is as follows:
 *
 * @image html dprod.gif ""
 *
 * Andes vector library only supports inner product function for the floating
 * point data type.
 */
/**
 * @addtogroup dprod
 * @{
 */
/**
 * @brief Inner production of floating-point vectors.
 * @param[in]       src1 pointer of the first input vector
 * @param[in]       src2 pointer of the second input vector
 * @param[in]       count number of elements to be processed
 * @return Inner product of the two input vectors
 */
float32_t riscv_vec_dprod_f32(float32_t * src1, float32_t * src2, uint32_t count);

/** @} dprod */
/** @} basic */

#ifdef  __cplusplus
}
#endif

#endif

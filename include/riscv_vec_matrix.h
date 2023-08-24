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
#ifndef __RISCV_VEC_MATRIX_H__
#define __RISCV_VEC_MATRIX_H__

/** @file*/

#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_math_types.h"
#include <riscv_vec_types.h>

/**
 * @defgroup groupMat Matrix Functions
 *
 * @brief Vector Matrix Functions
 *
 * There are two methods to access elements of a matrix: row-major order and column-major order.
 * Andes vector Library supports both and group matrix functions into two accordingly.
 * That is, there are row-major matrix functions prefixed with "riscv_vec_rmmat_" and
 * column-major matrix functions prefixed with "riscv_vec_cmmat" in Andes vector Library.
 */

/**
 * @addtogroup groupMat
 * @{
 */

/**
 * @defgroup Rmmat Row-Major Matrix Functions
 *
 * @brief Row-Major Matrix Functions
 *
 * For row-major matrix functions, matrices are regular MATmxn with m rows and n columns and they are all stored into vectors in row-major order. That is, the size of a vector equals to the number of elements in a matrix (i.e., size = m * n).
 */

/**
 * @addtogroup Rmmat
 * @{
 */

/**
 * @defgroup Rmmatadd Row-Major Matrix Addition Function
 *
 * @brief Row-Major Matrix Addition Function
 *
 * The row-major matrix addition function adds two elements from two
 * source matrices and writes the results one-by-one in row-major order into a destination matrix.
 */
/**
 * @addtogroup Rmmatadd
 * @{
 */
/**
 * @brief Row-major matrix addition function for f32
 *
 * @param[in] src1 pointer of the first input matrix
 * @param[in] src2 pointer of the second input matrix
 * @param[out] dst pointer of the output matrix
 * @param[in] row number of rows in a matrix
 * @param[in] col number of columns in a matrix
 * @return RISCV_VEC_OK
 *
 * @b Example
 *     <pre>
 * The following equation shows the addition of two matrices and the result.
 *
 * @image html rmmat_add.gif ""
 *
 * The code example with this function is as follows:
 *
 *      \#define row	2
 *      \#define col	2
 *      float32_t src1[row * col] = {0.1, 0.4, -0.2, 0.1};
 *      float32_t src2[row * col] = {-0.2, -0.1, 0.3, 0.5};
 *      float32_t dst[row * col];
 *      riscv_vec_rmmat_add_f32(src1, src2, dst, row, col);
 *     </pre>
 */
int riscv_vec_rmmat_add_f32(const float32_t * src1, const float32_t * src2, float32_t * dst, uint32_t row, uint32_t col);

 /**
  * @brief Row-major matrix addition function for f64
  *
  * @param[in] src1 pointer of the first input matrix
  * @param[in] src2 pointer of the second input matrix
  * @param[out] dst pointer of the output matrix
  * @param[in] row number of rows in a matrix
  * @param[in] col number of columns in a matrix
  * @return RISCV_VEC_OK
  */
int riscv_vec_rmmat_add_f64(const float64_t* src1, const float64_t* src2, float64_t* dst, uint32_t row, uint32_t col);

/**
  * @brief Row-major matrix addition function for q15
  *
  * @param[in] src1 pointer of the first input matrix
  * @param[in] src2 pointer of the second input matrix
  * @param[out] dst pointer of the output matrix
  * @param[in] row number of rows in a matrix
  * @param[in] col number of columns in a matrix
  * @return none.
  */
int riscv_vec_rmmat_add_q15(const q15_t * src1, const q15_t * src2, q15_t * dst, uint32_t row, uint32_t col);

/**
  * @brief Row-major matrix addition function for q31
  *
  * @param[in] src1 pointer of the first input matrix
  * @param[in] src2 pointer of the second input matrix
  * @param[out] dst pointer of the output matrix
  * @param[in] row number of rows in a matrix
  * @param[in] col number of columns in a matrix
  * @return none.
  */
int riscv_vec_rmmat_add_q31(const q31_t * src1, const q31_t * src2, q31_t * dst, uint32_t row, uint32_t col);

/** @} Rmmatadd */

/**
 * @defgroup Rmmatsub Row-Major Matrix Subtraction Functions
 *
 * @brief Row-Major Matrix Subtraction Functions
 *
 * The row-major matrix subtraction function subtracts two matrices of
 * the same dimensions and writes the results into a destination matrix in row-major order.
 */
/**
 * @addtogroup Rmmatsub
 * @{
 */
/**
 * @brief Row-major matrix subtraction function for f32
 *
 * @param[in] src1 pointer of the first input matrix
 * @param[in] src2 pointer of the second input matrix
 * @param[out] dst pointer of the output matrix
 * @param[in] row number of rows in a matrix
 * @param[in] col number of columns in a matrix
 * @return RISCV_VEC_OK
 *
 * @b Example
 *     <pre>
 * Given two matrices and their subtraction like below,
 *
 * @image html rmmat_sub.gif ""
 *
 * the code example with this function is as follows:
 *
 * \#define row		2
 * \#define col		2
 * float32_t src1[row * col] = {0.1, 0.4, -0.2, 0.1};
 * float32_t src2[row * col] = {-0.2, -0.1, 0.3, 0.5};
 * float32_t dst[row * col];
 * riscv_vec_rmmat_sub_f32 (src1, src2, dst, row, col);
 *     </pre>
 */
int riscv_vec_rmmat_sub_f32(const float32_t * src1, const float32_t * src2, float32_t * dst, uint32_t row, uint32_t col);

 /**
  * @brief Row-major matrix subtraction function for f64
  *
  * @param[in] src1 pointer of the first input matrix
  * @param[in] src2 pointer of the second input matrix
  * @param[out] dst pointer of the output matrix
  * @param[in] row number of rows in a matrix
  * @param[in] col number of columns in a matrix
  * @return RISCV_VEC_OK
  */
int riscv_vec_rmmat_sub_f64(const float64_t* src1, const float64_t* src2, float64_t* dst, uint32_t row, uint32_t col);

/**
  * @brief Row-major matrix subtraction function for q15
  *
  * @param[in] src1 pointer of the first input matrix
  * @param[in] src2 pointer of the second input matrix
  * @param[out] dst pointer of the output matrix
  * @param[in] row number of rows in a matrix
  * @param[in] col number of columns in a matrix
  * @return none.
  */
int riscv_vec_rmmat_sub_q15(const q15_t * src1, const q15_t * src2, q15_t * dst, uint32_t row, uint32_t col);

/**
  * @brief Row-major matrix subtraction function for q31
  *
  * @param[in] src1 pointer of the first input matrix
  * @param[in] src2 pointer of the second input matrix
  * @param[out] dst pointer of the output matrix
  * @param[in] row number of rows in a matrix
  * @param[in] col number of columns in a matrix
  * @return none.
  */
int riscv_vec_rmmat_sub_q31(const q31_t * src1, const q31_t * src2, q31_t * dst, uint32_t row, uint32_t col);

/** @} Rmmatsub */

/**
 * @defgroup Rmmatmul Row-Major Matrix Multiplication Functions
 *
 * @brief Row-Major Matrix Multiplication Functions
 *
 * Row-major matrix multiplication functions compute the multiplication
 * of two source matrices and write the results into a destination matrix in row-major order.
 */
/**
 * @addtogroup Rmmatmul
 * @{
 */
/**
 * @brief Row-major matrix multiplication function for f32
 *
 * @param[in] src1 pointer of the first input matrix
 * @param[in] src2 pointer of the second input matrix
 * @param[out] dst pointer of the output matrix
 * @param[in] row number of rows in the first input matrix
 * @param[in] col number of columns in the first input matrix
 * @param[in] col2 number of columns in the second input matrix
 * @return RISCV_VEC_OK
 *
 * @b Example
 *     <pre>
 * The following equation shows the multiplication of two matrices: (A(2,3) * B(3,2) = C(2,2))
 * Given each element with a value, the result is like
 *
 * @image html rmmat_mul.gif ""
 *
 * and can be presented in code with this function as follows:
 *
 *      \#define Arow		2
 *      \#define Acol		3
 *      \#define Bcol		2
 *      float32_t src1[Arow * Acol] = {0.1, -0.1, 0.1, 0.2, -0.2, 0.3};
 *      float32_t src2[Acol * Bcol] = {0.2, 0.2, -0.1, 0.3, -0.7, -0.2};
 *      float32_t dst[Arow * Bcol];
 *      riscv_vec_rmmat_mul_f32 (src1, src2, dst, Arow, Acol, Bcol);
 *
 * This example also serves as a reference for examples of f16 and f64 row-major
 * matrix multiplication functions.
 *     </pre>
 */
int riscv_vec_rmmat_mul_f32(const float32_t * src1, const float32_t * src2, float32_t * dst, uint32_t row, uint32_t col, uint32_t col2);

/**
 * @brief Row-major matrix multiplication function for f64
 *
 * @param[in] src1 pointer of the first input matrix
 * @param[in] src2 pointer of the second input matrix
 * @param[out] dst pointer of the output matrix
 * @param[in] row number of rows in the first input matrix
 * @param[in] col number of columns in the first input matrix
 * @param[in] col2 number of columns in the second input matrix
 * @return RISCV_VEC_OK
 */
int riscv_vec_rmmat_mul_f64(const float64_t * src1, const float64_t * src2, float64_t * dst, uint32_t row, uint32_t col, uint32_t col2);

/**
 * @brief Row-major gemm matrix multiplication function for f32
 * @param[in] src1 pointer of the first input matrix
 * @param[in] src2 pointer of the second input matrix
 * @param[out] dst  pointer of the output matrix
 * @param[in] alpha const value
 * @param[in] beta const value
 * @param[in] row  number of rows in the first input matrix
 * @param[in] col  number of columns in the first input matrix
 * @param[in] col2 number of columns in the second input matrix
 * @return RISCV_VEC_OK.
 */
int riscv_vec_rmmat_gemm_f32(const float32_t * src1, const float32_t * src2, float32_t * dst, const float32_t alpha, const float32_t beta, uint32_t row, uint32_t col, uint32_t col2);

/**
 * @brief Row-major gemm matrix multiplication function for f64
 * @param[in] src1 pointer of the first input matrix
 * @param[in] src2 pointer of the second input matrix
 * @param[out] dst  pointer of the output matrix
 * @param[in] alpha const value
 * @param[in] beta const value
 * @param[in] row  number of rows in the first input matrix
 * @param[in] col  number of columns in the first input matrix
 * @param[in] col2 number of columns in the second input matrix
 * @return RISCV_VEC_OK.
 */
int riscv_vec_rmmat_gemm_f64(const float64_t * src1, const float64_t * src2, float64_t * dst, const float64_t alpha, const float64_t beta, uint32_t row, uint32_t col, uint32_t col2);
/** @} */
/**
 * @defgroup rmmat_scale Row-major Matrix Scale Functions
 * @brief Row-major Matrix Scale Functions
 *
 * Row-major matrix scale functions multiply a matrix by a constant scaling value and write the result in row-major order into a destination matrix.
 */
/**
 * @addtogroup rmmat_scale
 * @{
 */
/**
 * @brief Row-major matrix scale function for f32
 * @param[in]       src pointer of the input matrix
 * @param[in]       scale constant scaling value
 * @param[out]      dst  pointer of the output matrix
 * @param[in]       row   number of rows in a matrix
 * @param[in]       col   number of columns in a matrix
 * @return RISCV_VEC_OK
 *
 * @b Example
 *     <pre>
 * Given a matrix and a scale value 0.2, the matrix scale equation is as follows:
 *
 * @image html mat_scale_2.gif ""
 *
 * and its code example is like:
 *
 *      \#define row 2
 *      \#define col 2
 *      float32_t src[row * col] = {0.1, 0.4, -0.2, 0.1};
 *      flato32_t scale = 0.2;
 *      float32_t dst[row * col];
 *      riscv_vec_rmmat_scale_f32 (src1, scale, dst, row, col);
 *     </pre>
 */
int riscv_vec_rmmat_scale_f32(const float32_t * src, float32_t scale, float32_t * dst, uint32_t row, uint32_t col);

/**
 * @brief Row-major matrix scale function for q15
 * @param[in]       src pointer of the input matrix
 * @param[in]       scale_fract constant scaling value
 * @param[in]       shift
 * @param[out]      dst  pointer of the output matrix
 * @param[in]       row   number of rows in a matrix
 * @param[in]       col   number of columns in a matrix
 * @return none.
 */
int riscv_vec_rmmat_scale_q15(const q15_t * src, q15_t scale_fract, int32_t shift, q15_t * dst, uint32_t row, uint32_t col);

/**
 * @brief Row-major matrix scale function for q31
 * @param[in]       src pointer of the input matrix
 * @param[in]       scale_fract constant scaling value
 * @param[in]       shift
 * @param[out]      dst  pointer of the output matrix
 * @param[in]       row   number of rows in a matrix
 * @param[in]       col   number of columns in a matrix
 * @return none.
 */
int riscv_vec_rmmat_scale_q31(const q31_t * src, q31_t scale_fract, int32_t shift, q31_t * dst, uint32_t row, uint32_t col);
/** @} rmmat_scale */

/**
 * @defgroup rmmat_trans Row-major Matrix Transpose Functions
 * @brief Row-major Matrix Transpose Functions
 *
 * Row-major matrix transpose functions transpose a matrix and then write the result in row-major order into a destination matrix.
 */
/**
 * @addtogroup rmmat_trans
 * @{
 */
/**
 * @brief Row-major matrix transpose function for f32
 * @param[in]       src  pointer of the input matrix
 * @param[out]      dst  pointer of the output matrix
 * @param[in]       row   number of rows in a matrix
 * @param[in]       col   number of columns in a matrix
 * @return RISCV_VEC_OK
 *
 * @b Example
 *  <pre>
 * Given an equation of matrix transposing like below,
 *
 * @image html mat_trans_2.gif ""
 *
 * its code example is as follows:
 *
 *      \#define row 2
 *      \#define col 3
 *      float32_t src[row * col] = {0.1, -0.1, 0.1, 0.2, -0.2, 0.3};
 *      float32_t dst[col * row];
 *      riscv_vec_rmmat_trans_f32 (src, dst, row, col);
 *  </pre>
 */
int riscv_vec_rmmat_trans_f32(const float32_t * src, float32_t * dst, uint32_t row, uint32_t col);

 /**
  * @brief Row-major matrix transpose function for f64
  * @param[in]       src  pointer of the input matrix
  * @param[out]      dst  pointer of the output matrix
  * @param[in]       row   number of rows in a matrix
  * @param[in]       col   number of columns in a matrix
  * @return RISCV_VEC_OK
  */
int riscv_vec_rmmat_trans_f64(const float64_t* src, float64_t* dst, uint32_t row, uint32_t col);

/**
  * @brief Row-major matrix transpose function for q31
  * @param[in]       src  pointer of the input matrix
  * @param[out]      dst  pointer of the output matrix
  * @param[in]       row   number of rows in a matrix
  * @param[in]       col   number of columns in a matrix
  * @return none.
  */
int riscv_vec_rmmat_trans_q31(const q31_t * src, q31_t * dst, uint32_t row, uint32_t col);

/**
  * @brief Row-major matrix transpose function for q15
  * @param[in]       src  pointer of the input matrix
  * @param[out]      dst  pointer of the output matrix
  * @param[in]       row   number of rows in a matrix
  * @param[in]       col   number of columns in a matrix
  * @return none.
  */
int riscv_vec_rmmat_trans_q15(const q15_t * src, q15_t * dst, uint32_t row, uint32_t col);

/**
  * @brief Row-major matrix transpose function for q7
  * @param[in]       src  pointer of the input matrix
  * @param[out]      dst  pointer of the output matrix
  * @param[in]       row   number of rows in a matrix
  * @param[in]       col   number of columns in a matrix
  * @return none.
  */
int riscv_vec_rmmat_trans_q7(const q7_t * src, q7_t * dst, uint32_t row, uint32_t col);

/**
  * @brief Row-major matrix transpose function for u8
  * @param[in]       src  pointer of the input matrix
  * @param[out]      dst  pointer of the output matrix
  * @param[in]       row   number of rows in a matrix
  * @param[in]       col   number of columns in a matrix
  * @return none.
  */
int riscv_vec_rmmat_trans_u8(const uint8_t * src, uint8_t * dst, uint32_t row, uint32_t col);

/**
  * @brief Row-major complex matrix transpose function for f32
  * @param[in]       src  pointer of the input complex matrix
  * @param[out]      dst  pointer of the output complex matrix
  * @param[in]       row   number of rows in a complex matrix
  * @param[in]       col   number of columns in a complex matrix
  * @return none.
  */
int riscv_vec_rmcmat_trans_f32(const float32_t * src, float32_t * dst, uint32_t row, uint32_t col);

/**
  * @brief Row-major complex matrix transpose function for q31
  * @param[in]       src  pointer of the input complex matrix
  * @param[out]      dst  pointer of the output complex matrix
  * @param[in]       row   number of rows in a complex matrix
  * @param[in]       col   number of columns in a complex matrix
  * @return none.
  */
int riscv_vec_rmcmat_trans_q31(const q31_t * src, q31_t * dst, uint32_t row, uint32_t col);

/**
  * @brief Row-major complex matrix transpose function for q15
  * @param[in]       src  pointer of the input complex matrix
  * @param[out]      dst  pointer of the output complex matrix
  * @param[in]       row   number of rows in a complex matrix
  * @param[in]       col   number of columns in a complex matrix
  * @return none.
  */
int riscv_vec_rmcmat_trans_q15(const q15_t * src, q15_t * dst, uint32_t row, uint32_t col);

/** @} rmmat_trans */

/**
 * @defgroup rmmat_cholesky Row-Major Matrix Cholesky Decomposition Function
 * @brief Row-Major Matrix Cholesky Decomposition Function
 *
 * The function performs the Cholesky decomposition of an input matrix and then writes the results in row-major order into a destination matrix. It will return RISCV_VEC_FAIL if the decomposition fails.
 *
 * The Cholesky decomposition of a matrix A, is a decomposition in the form: A = LL^T
 *
 * where L is the destination matrix and a real lower triangular matrix with positive diagonal entries.
 *
 * Andes vector library only supports row-major matrix Cholesky decomposition function for single-precision floating point data.
 */
/**
 * @addtogroup rmmat_cholesky
 * @{
 */
/**
 * @brief Row-major matrix Cholesky decomposition function for f32
 * @param[in]       *src points to the input matrix.
 * @param[out]      *dst points to the output matrix.
 * @param[in]       row  number of the input matrix rows.
 * @param[in]       col  number of the input matrix columns.
 * @return RISCV_VEC_FAIL if the decomposition fails. RISCV_VEC_OK	 if the decomposition succeeds.
 */
int riscv_vec_rmmat_cholesky_f32(float32_t* src, float32_t* dst, uint32_t row, uint32_t col);
/** @} rmmat_cholesky */

/**
 * @defgroup rmmat_ldlt Row-Major Matrix LDLT Function
 * @brief Row-Major Matrix LDLT Function
 *
 * The function performs the LDLT decomposition of an input matrix and then writes the results in row-major order into destination matrices. It will return RISCV_VEC_FAIL if the decomposition fails.
 *
 * The LDLT decomposition of a matrix A, is a decomposition of the form:  A = LDL^T
 *
 * Where P is a destination permutation vector, L is a destination matrix and a real lower unit triangular matrix, and D is a destination diagonal matrix.
 *
 * Andes vector library only supports row-major matrix LDLT decomposition function for single-precision floating point data.
 */
/**
 * @addtogroup rmmat_ldlt
 * @{
 */
/**
 * @brief Row-major matrix LDLT decomposition function for f32
 * @param[in]  src    points to the input matrix.
 * @param[out] dstl   points to the output triangular matrix.
 * @param[out] dstd   points to the output diagonal matrix.
 * @param[out] pp     points to the output permutation vector.
 * @param[in]  row    number of the rows in a matrix.
 * @param[in]  col    number of the columns in a matrix.
 * @return RISCV_VEC_FAIL if the decomposition fails. RISCV_VEC_OK	 if the decomposition succeeds.
 */
int riscv_vec_rmmat_ldlt_f32(float32_t* src, float32_t* dstl, float32_t* dstd, uint16_t* pp, uint32_t row, uint32_t col);
/** @} rmmat_ldlt */

/**
 * @defgroup rmmat_lt Row-Major Matrix Solve Lower Triangular Function
 * @brief Row-Major Matrix Solve Lower Triangular Function
 *
 * The function solves the lower triangle decomposition for an input matrix and writes the results in row-major order into a destination matrix.
 *
 * The lower triangle decomposition is solved in the following form: Lx = A
 *
 * That is, the destination matrix x can be obtained if given the input matrix A and lower triangular matrix L.
 *
 * Andes vector library only supports row-major matrix solve lower triangular function for single-precision floating point data.
 */
/**
 * @addtogroup rmmat_lt
 * @{
 */
/**
 * @brief Row-major matrix solve lower triangular function for f32
 * @param[in]  lt     pointer of the input lower triangular matrix.
 * @param[in]  A      pointer of the input matrix
 * @param[out] dst    pointer of the output matrix.
 * @param[in]  row    number of the rows in a matrix.
 * @param[in]  col    number of the columns in a matrix.
 * @return RISCV_VEC_FAIL 	if the decomposition is not solved. RISCV_VEC_OK 	 if the decomposition is solved.
 */
int riscv_vec_rmmat_solve_lt_f32(float32_t* lt, float32_t* A, float32_t* dst, uint32_t row, uint32_t col);
/** @} rmmat_lt */

/**
 * @defgroup rmmat_ut Row-Major Matrix Solve Upper Triangular Function
 * @brief Row-Major Matrix Solve Upper Triangular Function
 *
 * These function solves the upper triangle decomposition for an input matrix and writes the results in row-major order into a destination matrix.
 *
 * The upper triangle decomposition is solved in the following form: Ux = A
 *
 * That is, the destination matrix x can be obtained if given the input matrix A and upper triangular matrix U.
 *
 * Andes vector library only supports row-major matrix solve upper triangular function for single-precision floating point data.
 */
/**
 * @addtogroup rmmat_ut
 * @{
 */
/**
 * @brief Row-major matrix solve upper triangular function for f32
 * @param[in]  ut     pointer of the input upper triangular matrix.
 * @param[in]  A      pointer of the input matrix
 * @param[out] dst    pointer of the output matrix.
 * @param[in]  row    number of the rows in a matrix.
 * @param[in]  col    number of the columns in a matrix.
 * @return RISCV_VEC_FAIL 	if the decomposition is not solved. RISCV_VEC_OK 	 if the decomposition is solved.
 */
int riscv_vec_rmmat_solve_ut_f32(float32_t* ut, float32_t* A, float32_t* dst, uint32_t row, uint32_t col);
/** @} rmmat_ut */

/**
 * @addtogroup rmmat_inv
 * @{
 */
/**
 * @brief Row-major matrix inverse function for f32
 * @param[in]  src    pointer of the input invertable matrix.
 * @param[out] dst    pointer of the output matrix.
 * @param[in]  size   number of the rows/cols of matrix.
 * @return RISCV_VEC_FAIL if the size is zero, otherwise return RISCV_VEC_OK .
 */
int riscv_vec_rmmat_inv_f32(float32_t * src, float32_t * dst, uint32_t size);

/**
 * @brief Row-major matrix inverse function for f64
 * @param[in]  src    pointer of the input invertable matrix.
 * @param[out] dst    pointer of the output matrix.
 * @param[in]  size   number of the rows/cols of matrix.
 * @return RISCV_VEC_FAIL if the size is zero, otherwise return RISCV_VEC_OK .
 */
int riscv_vec_rmmat_inv_f64(float64_t * src, float64_t * dst, uint32_t size);

/** @} rmmat_inv */
/** @} Rmmat */

/**
 * @defgroup Cmmat Column-Major Matrix Functions
 *
 * @brief Column-Major Matrix Functions
 *
 * Andes vector library provides column-major matrix addition, subtraction, multiplication, determinant, identity, transpose and inverse functions for three matrix types: 2x2 matrix (MAT2x2), 3x3 matrix (MAT3x3) and 4x4 matrix (MAT4x4). For these functions, all matrices are stored into vectors in column-major order and the parameter “count” represents the number of matrices to be processed. The data structures of their matrix types are listed in Section 2.4.2.1.
 */

/**
 * @addtogroup Cmmat
 * @{
 */

/**
 * @brief Structure of the floating-point 2x2 column-major matrix (MAT2x2).
 */

typedef struct
{
    riscv_vec_f32x2_t c1;   /**< column1 of the 2x2 matrix consisting of two f32 elements */
    riscv_vec_f32x2_t c2;   /**< column2 of the 2x2 matrix consisting of two f32 elements */
} riscv_vec_cmmat2x2_f32_t;

/**
 * @brief Structure of the floating-point 3x3 column-major matrix (MAT3x3).
 */
typedef struct
{
    riscv_vec_f32x3_t c1;    /**< column1 of the 3x3 matrix consisting of three f32 elements */
    riscv_vec_f32x3_t c2;    /**< column2 of the 3x3 matrix consisting of three f32 elements */
    riscv_vec_f32x3_t c3;    /**< column3 of the 3x3 matrix consisting of three f32 elements */
} riscv_vec_cmmat3x3_f32_t;

/**
 * @brief Structure of the floating-point 4x4 column-major matrix (MAT4x4).
 */
typedef struct
{
    riscv_vec_f32x4_t c1;    /**< column1 of the 4x4 matrix consisting of four f32 elements */
    riscv_vec_f32x4_t c2;    /**< column2 of the 4x4 matrix consisting of four f32 elements */
    riscv_vec_f32x4_t c3;    /**< column3 of the 4x4 matrix consisting of four f32 elements */
    riscv_vec_f32x4_t c4;    /**< column4 of the 4x4 matrix consisting of four f32 elements */
} riscv_vec_cmmat4x4_f32_t;

/**
 * @defgroup Cmmatadd Column-Major Matrix Addition Functions
 *
 * @brief Column-Major Matrix Addition Functions
 *
 * Column-major matrix addition functions add two elements from two source
 * matrices and write the results one-by-one in column-major order into a
 * destination matrix.
 */

 /**
  * @addtogroup Cmmatadd
  * @{
  */

/**
 * @brief Column-major matrix addition function for MAT2X2
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src1 pointer of the first input matrix
 * @param[in] src2 pointer of the second input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat2x2_f32_t.
 * @return RISCV_VEC_OK
 *
 * @b Example
 *     <pre>
 * The following equation shows the addition of two MAT2x2 matrices and the result.
 *
 * @image html cmmat_add.gif ""
 *
 * The code example with this function is as follows:
 *
 *      \#define count	2
 *      riscv_vec_cmmat2x2_f32_t src1[count] = {
 *          {{0.1, -0.2}, {0.4, 0.1}}, {{0.3, 0.2}, {0.4, 0.2}}
 *      };
 *      riscv_vec_cmmat2x2_f32_t src2[count] = {
 *          {{-0.2, 0.3}, {-0.1, 0.5}}, {{-0.2, 0.3}, {0.1, -0.3}}
 *      };
 *      riscv_vec_cmmat2x2_f32_t dst[count];
 *      riscv_vec_cmmat2x2_add_f32(dst, src1, src2, count);
 *
 * This example also serves as a reference for examples of MAT3x3
 * and MAT4x4 column-major matrix addition functions.
 *     </pre>
 */
int riscv_vec_cmmat2x2_add_f32(riscv_vec_cmmat2x2_f32_t * dst, riscv_vec_cmmat2x2_f32_t * src1, riscv_vec_cmmat2x2_f32_t * src2, uint32_t count);

/**
 * @brief Column-major matrix addition function for MAT3X3
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src1 pointer of the first input matrix
 * @param[in] src2 pointer of the second input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat3x3_f32_t.
 * @return RISCV_VEC_OK
 */
int riscv_vec_cmmat3x3_add_f32(riscv_vec_cmmat3x3_f32_t * dst, riscv_vec_cmmat3x3_f32_t * src1, riscv_vec_cmmat3x3_f32_t * src2, uint32_t count);

/**
 * @brief Column-major matrix addition function for MAT4X4
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src1 pointer of the first input matrix
 * @param[in] src2 pointer of the second input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat4x4_f32_t.
 * @return RISCV_VEC_OK
 */
int riscv_vec_cmmat4x4_add_f32(riscv_vec_cmmat4x4_f32_t * dst, riscv_vec_cmmat4x4_f32_t * src1, riscv_vec_cmmat4x4_f32_t * src2, uint32_t count);
/** @} */

/**
 * @defgroup Cmmatsub Column-Major Matrix Subtraction Functions
 *
 * @brief Column-Major Matrix Subtraction Functions
 *
 * Column-major matrix subtraction functions subtract two matrices
 * of the same dimensions and write the results into a destination
 * matrix in column-major order.
 */

 /**
  * @addtogroup Cmmatsub
  * @{
  */

/**
 * @brief Column-major matrix subtraction function for MAT2X2
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src1 pointer of the first input matrix
 * @param[in] src2 pointer of the second input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat2x2_f32_t.
 * @return RISCV_VEC_OK
 *
 * @b Example
 *     <pre>
 * The following equation shows the subtraction of two MAT2x2 matrices and the result.
 *
 * @image html cmmat_sub.gif ""
 *
 * The code example with this function is as follows:
 *
 *      \#define count	2
 *      riscv_vec_cmmat2x2_f32_t src1[count] = {
 *          {{0.1, -0.2}, {0.4, 0.1}}, {{0.3, 0.2}, {0.4, 0.2}}
 *      };
 *      riscv_vec_cmmat2x2_f32_t src2[count] = {
 *          {{-0.2, 0.3}, {-0.1, 0.5}}, {{-0.2, 0.3}, {0.1, -0.3}}
 *      };
 *      riscv_vec_cmmat2x2_f32_t dst[count];
 *      riscv_vec_cmmat2x2_sub_f32(dst, src1, src2, count);
 *
 * This example also serves as a reference for examples of MAT3x3
 * and MAT4x4 column-major matrix subtraction functions.
 *     </pre>
 */
int riscv_vec_cmmat2x2_sub_f32(riscv_vec_cmmat2x2_f32_t * dst, riscv_vec_cmmat2x2_f32_t * src1, riscv_vec_cmmat2x2_f32_t * src2, uint32_t count);

/**
 * @brief Column-major matrix subtraction function for MAT3X3
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src1 pointer of the first input matrix
 * @param[in] src2 pointer of the second input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat3x3_f32_t.
 * @return RISCV_VEC_OK
 */
int riscv_vec_cmmat3x3_sub_f32(riscv_vec_cmmat3x3_f32_t * dst, riscv_vec_cmmat3x3_f32_t * src1, riscv_vec_cmmat3x3_f32_t * src2, uint32_t count);

/**
 * @brief Column-major matrix subtraction function for MAT4X4
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src1 pointer of the first input matrix
 * @param[in] src2 pointer of the second input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat4x4_f32_t.
 * @return RISCV_VEC_OK
 */
int riscv_vec_cmmat4x4_sub_f32(riscv_vec_cmmat4x4_f32_t * dst, riscv_vec_cmmat4x4_f32_t * src1, riscv_vec_cmmat4x4_f32_t * src2, uint32_t count);
/** @} */

/**
 * @defgroup Cmmatmul Column-Major Matrix Multiplication Functions
 *
 * @brief Column-Major Matrix Multiplication Functions
 *
 * Column-major matrix multiplication functions compute the multiplication
 * of two source matrices and write the results into a destination matrix
 * in column-major order.
 */

 /**
  * @addtogroup Cmmatmul
  * @{
  */

/**
 * @brief Column-major matrix multiplication function for MAT2X2
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src1 pointer of the first input matrix
 * @param[in] src2 pointer of the second input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat2x2_f32_t.
 * @return RISCV_VEC_OK
 *
 * @b Example
 *     <pre>
 * The following example code presents the multiplication of four MAT2x2 matrices with this function.
 *
 *      \#define count	4
 *      riscv_vec_cmmat2x2_f32_t src1[count] = {...};
 *      riscv_vec_cmmat2x2_f32_t src2[count] = {...};
 *      riscv_vec_cmmat2x2_f32_t dst[count];
 *      riscv_vec_cmmat2x2_mul_f32(dst, src1, src2, count);
 *
 * This example also serves as a reference for examples of MAT3x3 and MAT4x4
 * column-major matrix multiplication functions.
 *     </pre>
 */
int riscv_vec_cmmat2x2_mul_f32(riscv_vec_cmmat2x2_f32_t * dst, riscv_vec_cmmat2x2_f32_t * src1, riscv_vec_cmmat2x2_f32_t * src2, uint32_t count);

/**
 * @brief Column-major matrix multiplication function for MAT3X3
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src1 pointer of the first input matrix
 * @param[in] src2 pointer of the second input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat3x3_f32_t.
 * @return RISCV_VEC_OK
 */
int riscv_vec_cmmat3x3_mul_f32(riscv_vec_cmmat3x3_f32_t * dst, riscv_vec_cmmat3x3_f32_t * src1, riscv_vec_cmmat3x3_f32_t * src2, uint32_t count);

/**
 * @brief Column-major matrix multiplication function for MAT4X4
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src1 pointer of the first input matrix
 * @param[in] src2 pointer of the second input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat4x4_f32_t.
 * @return RISCV_VEC_OK
 */
int riscv_vec_cmmat4x4_mul_f32(riscv_vec_cmmat4x4_f32_t * dst, riscv_vec_cmmat4x4_f32_t * src1, riscv_vec_cmmat4x4_f32_t * src2, uint32_t count);

/**
 * @brief Constant column-major MAT2X2 multiply vector function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] cst pointer of the constant matrix
 * @param[in] src pointer of the input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x2_t.
 * @return RISCV_VEC_OK
 *
 * @b Example
 *     <pre>
 * Given the count as 4, the example code with this function is as follows:
 *
 *      \#define count	4
 *
 *      const riscv_vec_cmmat2x2_f32_t cst = {…};
 *      riscv_vec_f32x2_t src[count] = {…};
 *      riscv_vec_f32x2_t dst[count];
 *      riscv_vec_cmmat2x2_mulc_mxv_f32 (dst, &cst, src, count);
 *
 * This example also serves as a reference for examples of constant
 * column-major MAT3x3 and MAT4x4 multiply vector functions.
 *     </pre>
 */
int riscv_vec_cmmat2x2_mulc_mxv_f32(riscv_vec_f32x2_t * dst, const riscv_vec_cmmat2x2_f32_t * cst, riscv_vec_f32x2_t * src, uint32_t count);

/**
 * @brief Constant column-major MAT3X3 multiply vector function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] cst pointer of the constant matrix
 * @param[in] src pointer of the input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return RISCV_VEC_OK
 */
int riscv_vec_cmmat3x3_mulc_mxv_f32(riscv_vec_f32x3_t * dst, const riscv_vec_cmmat3x3_f32_t * cst, riscv_vec_f32x3_t * src, uint32_t count);

/**
 * @brief Constant column-major MAT4X4 multiply vector function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] cst pointer of the constant matrix
 * @param[in] src pointer of the input vector
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_f32x3_t.
 * @return RISCV_VEC_OK
 */
int riscv_vec_cmmat4x4_mulc_mxv_f32(riscv_vec_f32x4_t * dst, const riscv_vec_cmmat4x4_f32_t * cst, riscv_vec_f32x4_t * src, uint32_t count);
/** @} */

/**
 * @defgroup Cmmmatdet Column-Major Matrix Determinant Functions
 *
 * @brief Column-Major Matrix Determinant Functions
 *
 * Column-major matrix determinant functions compute the determinant
 * of a source matrix and write the result into a destination matrix in column-major order.
 */

 /**
  * @addtogroup Cmmmatdet
  * @{
  */

/**
 * @brief Column-major matrix determinant function for MAT2X2
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src pointer of the input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat2x2_f32_t.
 * @return RISCV_VEC_OK
 *
 * @b Example
 *     <pre>
 * The following equation shows the determinant of two MAT2x2 matrices and the result.
 *
 * @image html cmmat_det.gif ""
 *
 * The code example with this function is as follows:
 *
 *      \#define count	2
 *      riscv_vec_cmmat2x2_f32_t src[count] = {
 *          {{0.1, -0.2}, {0.4, 0.1}}, {{0.3, 0.2}, {0.4, 0.2}}
 *      };
 *      float32_t dst[count];
 *      riscv_vec_cmmat2x2_det_f32(dst, src, count);
 *
 * This example also serves as a reference for examples of MAT3x3 and
 * MAT4x4 column-major matrixdeterminant functions.
 *     </pre>
 */
int riscv_vec_cmmat2x2_det_f32(float32_t * dst, riscv_vec_cmmat2x2_f32_t * src, uint32_t count);

/**
 * @brief Column-major matrix determinant function for MAT3X3
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src pointer of the input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat3x3_f32_t.
 * @return RISCV_VEC_OK
 */
int riscv_vec_cmmat3x3_det_f32(float32_t * dst, riscv_vec_cmmat3x3_f32_t * src, uint32_t count);

/**
 * @brief Column-major matrix determinant function for MAT4X4
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src pointer of the input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat4x4_f32_t.
 * @return RISCV_VEC_OK
 */
int riscv_vec_cmmat4x4_det_f32(float32_t * dst, riscv_vec_cmmat4x4_f32_t * src, uint32_t count);

/** @} */

/**
 * @defgroup MatIdent Column-Major Matrix Identity Functions
 *
 * @brief Column-Major Matrix Identity Functions
 *
 * Column-major matrix identity functions write an identity matrix into
 * a destination matrix in column-major order.
 */

/**
 * @addtogroup MatIdent
 * @{
 */

/**
 * @brief Column-major matrix identity function for MAT2X2
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat2x2_f32_t.
 * @return RISCV_VEC_OK
 *
 * @b Example
 *     <pre>
 * The following example code shows how to write two MAT2x2 identity matrices with this function.
 *
 *      \#define count	2
 *      riscv_vec_cmmat2x2_f32_t dst[count];
 *      riscv_vec_cmmat2x2_identity_f32(dst, count);
 *
 * This example also serves as a reference for examples of MAT3x3 and
 * MAT4x4 column-major identity matrix functions.
 *     </pre>
 */
int riscv_vec_cmmat2x2_identity_f32(riscv_vec_cmmat2x2_f32_t * dst, uint32_t count);

/**
 * @brief Column-major matrix identity function for MAT3X3
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat3x3_f32_t.
 * @return RISCV_VEC_OK
 */
int riscv_vec_cmmat3x3_identity_f32(riscv_vec_cmmat3x3_f32_t * dst, uint32_t count);

/**
 * @brief Column-major matrix identity function for MAT4X4
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat4x4_f32_t.
 * @return RISCV_VEC_OK
 */
int riscv_vec_cmmat4x4_identity_f32(riscv_vec_cmmat4x4_f32_t * dst, uint32_t count);
/** @} */

/**
 * @defgroup MatTrans Column-Major Matrix Transpose Functions
 *
 * @brief Column-Major Matrix Transpose Functions
 *
 * Column-major matrix transpose functions transpose a matrix and then write the
 * result into a destination matrix in column-major order.
 */

/**
 * @addtogroup MatTrans
 * @{
 */

/**
 * @brief Column-major matrix transpose function for MAT2X2
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src pointer of the input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat2x2_f32_t.
 * @return RISCV_VEC_OK
 *
 * @b Example
 *     <pre>
 * The following equation shows the matrix transpose of two MAT2x2
 * matrices and the result.
 *
 * @image html cmmat_trans.gif ""
 *
 * The code example with this function is as follows:
 *
 *      \#define count	2
 *      riscv_vec_cmmat2x2_f32_t src[count] = {
 *      {{0.1, -0.2}, {0.4, 0.1}}, {{0.3, 0.2}, {0.4, 0.2}}
 *      };
 *      riscv_vec_cmmat2x2_f32_t dst[count];
 *      riscv_vec_cmmat2x2_trans_f32(dst, src, count);
 *
 * This example also serves as a reference for examples of MAT3x3
 * and MAT4x4 column-major matrix transpose functions.
 *     </pre>
 */
int riscv_vec_cmmat2x2_trans_f32(riscv_vec_cmmat2x2_f32_t * dst, riscv_vec_cmmat2x2_f32_t * src, uint32_t count);

/**
 * @brief Column-major matrix transpose function for MAT3X3
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src pointer of the input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat3x3_f32_t.
 * @return RISCV_VEC_OK
 */
int riscv_vec_cmmat3x3_trans_f32(riscv_vec_cmmat3x3_f32_t * dst, riscv_vec_cmmat3x3_f32_t * src, uint32_t count);

/**
 * @brief Column-major matrix transpose function for MAT4X4
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src pointer of the input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat4x4_f32_t.
 * @return RISCV_VEC_OK
 */
int riscv_vec_cmmat4x4_trans_f32(riscv_vec_cmmat4x4_f32_t * dst, riscv_vec_cmmat4x4_f32_t * src, uint32_t count);
/** @} */

/**
 * @defgroup MatInv Column-Major Matrix Inverse Functions
 *
 * @brief Column-Major Matrix Inverse Functions
 *
 * Column-major matrix inverse functions return the inverse of a square matrix in column-major order.
 */

 /**
  * @addtogroup MatInv
  * @{
  */

/**
 * @brief Column-major matrix inverse function for MAT2X2
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src pointer of the input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat2x2_f32_t.
 * @return RISCV_VEC_OK
 *
 * @b Example
 *     <pre>
 * The following equation shows the matrix transpose of two MAT2x2 matrices and the result.
 *
 * @image html cmmat_inv.gif ""
 *
 * The code example with this function is as follows:
 *
 *      \#define count	2
 *      riscv_vec_cmmat2x2_f32_t src[count] = {
 *          {{0.1, -0.2}, {0.4, 0.1}}, {{0.3, 0.2}, {0.4, 0.2}}
 *      };
 *      riscv_vec_cmmat2x2_f32_t dst[count];
 *      riscv_vec_cmmat2x2_inv_f32(dst, src, count);
 *
 * This example also serves as a reference for examples of MAT3x3
 * and MAT4x4 column-major matrix inverse functions.
 *     </pre>
 *
 */
int riscv_vec_cmmat2x2_inv_f32(riscv_vec_cmmat2x2_f32_t * dst, riscv_vec_cmmat2x2_f32_t * src, uint32_t count);

/**
 * @brief Column-major matrix inverse function for MAT3X3
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src pointer of the input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat3x3_f32_t.
 * @return RISCV_VEC_OK
 */
int riscv_vec_cmmat3x3_inv_f32(riscv_vec_cmmat3x3_f32_t * dst, riscv_vec_cmmat3x3_f32_t * src, uint32_t count);

/**
 * @brief Column-major matrix inverse function for MAT4X4
 *
 * @param[out] dst pointer of the output matrix
 * @param[in] src pointer of the input matrix
 * @param[in] count number of elements to be processed. Each element has a data type of riscv_vec_cmmat4x4_f32_t.
 * @return RISCV_VEC_OK
 */
int riscv_vec_cmmat4x4_inv_f32(riscv_vec_cmmat4x4_f32_t * dst, riscv_vec_cmmat4x4_f32_t * src, uint32_t count);
/** @} MatInv */

/** @} Cmmat */
/** @} groupMat */

#ifdef  __cplusplus
}
#endif
#endif

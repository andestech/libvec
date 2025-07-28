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
#ifndef __INTERNAL_VEC_MAT_MUL_TILING_H__
#define __INTERNAL_VEC_MAT_MUL_TILING_H__

#ifdef ENA_DYNAMIC_CALC_CACHE_CONFIG
#include "nds_intrinsic.h"
#include <math.h>
#endif

typedef union
{
    struct
    {
        MM_TYPE re;
        MM_TYPE im;
    };
} UNION_CLX_T;

#ifdef ENA_MAT_MUL_VQ
typedef union
{
    struct
    {
        MM_TYPE_VQ re;
        MM_TYPE_VQ im;
    };
} UNION_CLX_VQ_T;
#endif

#ifdef ENA_PUREC_TILING

static inline void vec_mat_mul_purec_tiling(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, uint32_t row, uint32_t col, uint32_t col2, uint32_t tiling_size)
{
    //4*2*4
    int row_4 = row >> 2;
    int max_row = row_4 << 2;

    const MM_TYPE* A = src1;
    const MM_TYPE* B = src2;
    MM_TYPE* C = dst;
    int r, c, k, kk, cc;
    MM_TYPE sum1, sum2, sum3, sum4, sum5, sum6, sum7, sum8, \
            sum9, sum10, sum11, sum12, sum13, sum14, sum15, sum16;
    int kk_tiling_size, cc_tiling_size, max_cc_tiling_size, max_kk_tiling_size;
    const MM_TYPE* A11, * A12, * B11, * B12, * B13, * B14, \
                 * A21, * A22, * B21, * B22, * B23, * B24, \
                 * A31, * A32, \
                 * A41, * A42;
    MM_TYPE * C1, * C2, * C3, * C4;
    for (kk = 0; kk < col; kk += tiling_size)
    {
        kk_tiling_size = ((kk + tiling_size) > col ? col : (kk + tiling_size));
        if ((kk + tiling_size) <= col)
        {
            max_kk_tiling_size = kk_tiling_size - (tiling_size & 1);
        }
        else
        {
            max_kk_tiling_size = col - ((col - kk) & 1);
        }
        for (cc = 0; cc < col2; cc += tiling_size)
        {
            cc_tiling_size = ((cc + tiling_size) > col2 ? col2 : (cc + tiling_size));
            if ((cc + tiling_size) <= col2)
            {
                max_cc_tiling_size = cc_tiling_size - (tiling_size & 3);
            }
            else
            {
                max_cc_tiling_size = col2 - ((col2 - cc) & 3);
            }

            for (r = 0; r < max_row; r += 4)
            {
                for (c = cc; c < max_cc_tiling_size; c += 4)
                {
                    sum1 = 0.0; sum2 = 0.0; sum3 = 0.0; sum4 = 0.0;
                    sum5 = 0.0; sum6 = 0.0; sum7 = 0.0; sum8 = 0.0;
                    sum9 = 0.0; sum10 = 0.0; sum11 = 0.0; sum12 = 0.0;
                    sum13 = 0.0; sum14 = 0.0; sum15 = 0.0; sum16 = 0.0;
                    C1 = C + r * col2 + c;
                    C2 = C1 + col2;
                    C3 = C2 + col2;
                    C4 = C3 + col2;
                    A11 = A + r * col + kk; A12 = A11 + 1;
                    A21 = A11 + col; A22 = A21 + 1;
                    A31 = A21 + col; A32 = A31 + 1;
                    A41 = A31 + col; A42 = A41 + 1;
                    B11 = B + kk * col2 + c; B12 = B11 + 1; B13 = B12 + 1; B14 = B13 + 1;
                    B21 = B11 + col2; B22 = B21 + 1; B23 = B22 + 1; B24 = B23 + 1;

                    for (k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        //1st row
                        sum1 += *A11 * *B11; sum1 += *A12 * *B21;//1st column
                        sum2 += *A11 * *B12; sum2 += *A12 * *B22;//2nd column
                        sum3 += *A11 * *B13; sum3 += *A12 * *B23;//3rd column
                        sum4 += *A11 * *B14; sum4 += *A12 * *B24;//4th column
                        //2nd row
                        sum5 += *A21 * *B11; sum5 += *A22 * *B21;//1st column
                        sum6 += *A21 * *B12; sum6 += *A22 * *B22;//2nd column
                        sum7 += *A21 * *B13; sum7 += *A22 * *B23;//3rd column
                        sum8 += *A21 * *B14; sum8 += *A22 * *B24;//4th column
                        //3rd row
                        sum9 += *A31 * *B11; sum9 += *A32 * *B21;//1st column
                        sum10 += *A31 * *B12; sum10 += *A32 * *B22;//2nd column
                        sum11 += *A31 * *B13; sum11 += *A32 * *B23;//3rd column
                        sum12 += *A31 * *B14; sum12 += *A32 * *B24;//4th column
                        //4th row
                        sum13 += *A41 * *B11; sum13 += *A42 * *B21;//1st column
                        sum14 += *A41 * *B12; sum14 += *A42 * *B22;//2nd column
                        sum15 += *A41 * *B13; sum15 += *A42 * *B23;//3rd column
                        sum16 += *A41 * *B14; sum16 += *A42 * *B24;//4th column

                        A11 += 2; A12 += 2;
                        A21 += 2; A22 += 2;
                        A31 += 2; A32 += 2;
                        A41 += 2; A42 += 2;
                        B11 += 2 * col2; B12 += 2 * col2; B13 += 2 * col2; B14 += 2 * col2;
                        B21 += 2 * col2; B22 += 2 * col2; B23 += 2 * col2; B24 += 2 * col2;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        sum1 += *A11 * *B11;
                        sum2 += *A11 * *B12;
                        sum3 += *A11 * *B13;
                        sum4 += *A11 * *B14;

                        sum5 += *A21 * *B11;
                        sum6 += *A21 * *B12;
                        sum7 += *A21 * *B13;
                        sum8 += *A21 * *B14;

                        sum9 += *A31 * *B11;
                        sum10 += *A31 * *B12;
                        sum11 += *A31 * *B13;
                        sum12 += *A31 * *B14;

                        sum13 += *A41 * *B11;
                        sum14 += *A41 * *B12;
                        sum15 += *A41 * *B13;
                        sum16 += *A41 * *B14;

                        A11++;
                        A21++;
                        A31++;
                        A41++;
                        B11 += col2; B12 += col2; B13 += col2; B14 += col2;
                    }
                    *C1++ += sum1; *C1++ += sum2; *C1++ += sum3; *C1 += sum4;
                    *C2++ += sum5; *C2++ += sum6; *C2++ += sum7; *C2 += sum8;
                    *C3++ += sum9; *C3++ += sum10; *C3++ += sum11; *C3 += sum12;
                    *C4++ += sum13; *C4++ += sum14; *C4++ += sum15; *C4 += sum16;
                }
                for (; c < cc_tiling_size; c++)
                {
                    sum1 = 0.0;
                    sum5 = 0.0;
                    sum9 = 0.0;
                    sum13 = 0.0;
                    C1 = C + r * col2 + c;
                    C2 = C1 + col2;
                    C3 = C2 + col2;
                    C4 = C3 + col2;
                    A11 = A + r * col + kk; A12 = A11 + 1;
                    A21 = A11 + col; A22 = A21 + 1;
                    A31 = A21 + col; A32 = A31 + 1;
                    A41 = A31 + col; A42 = A41 + 1;
                    B11 = B + kk * col2 + c;
                    B21 = B11 + col2;
                    for (k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        //1th row
                        sum1 += *A11 * *B11; sum1 += *A12 * *B21;//1st column
                        //2th row
                        sum5 += *A21 * *B11; sum5 += *A22 * *B21;//1st column
                        //3th row
                        sum9 += *A31 * *B11; sum9 += *A32 * *B21;//1st column
                        //4th row
                        sum13 += *A41 * *B11; sum13 += *A42 * *B21;//1st column

                        A11 += 2; A12 += 2;
                        A21 += 2; A22 += 2;
                        A31 += 2; A32 += 2;
                        A41 += 2; A42 += 2;
                        B11 += 2 * col2;
                        B21 += 2 * col2;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        sum1 += *A11 * *B11;
                        sum5 += *A21 * *B11;
                        sum9 += *A31 * *B11;
                        sum13 += *A41 * *B11;

                        A11++;
                        A21++;
                        A31++;
                        A41++;
                        B11 += col2;
                    }
                    *C1 += sum1;
                    *C2 += sum5;
                    *C3 += sum9;
                    *C4 += sum13;
                }
            }
            for (; r < row; r++)
            {
                for (c = cc; c < max_cc_tiling_size; c += 4)
                {
                    sum1 = 0.0; sum2 = 0.0; sum3 = 0.0; sum4 = 0.0;
                    C1 = C + r * col2 + c; C2 = C1 + 1; C3 = C2 + 1; C4 = C3 + 1;
                    A11 = A + r * col + kk; A12 = A11 + 1;
                    B11 = B + kk * col2 + c; B12 = B11 + 1; B13 = B12 + 1; B14 = B13 + 1;
                    B21 = B11 + col2; B22 = B21 + 1; B23 = B22 + 1; B24 = B23 + 1;
                    for (k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        //1th row
                        sum1 += *A11 * *B11; sum1 += *A12 * *B21;//1st column
                        sum2 += *A11 * *B12; sum2 += *A12 * *B22;//2nd column
                        sum3 += *A11 * *B13; sum3 += *A12 * *B23;//3rd column
                        sum4 += *A11 * *B14; sum4 += *A12 * *B24;//4th column

                        A11 += 2; A12 += 2;
                        B11 += 2 * col2; B12 += 2 * col2; B13 += 2 * col2; B14 += 2 * col2;
                        B21 += 2 * col2; B22 += 2 * col2; B23 += 2 * col2; B24 += 2 * col2;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        sum1 += *A11 * *B11;
                        sum2 += *A11 * *B12;
                        sum3 += *A11 * *B13;
                        sum4 += *A11 * *B14;

                        A11++;
                        B11 += col2; B12 += col2; B13 += col2; B14 += col2;
                    }
                    *C1 += sum1; *C2 += sum2; *C3 += sum3; *C4 += sum4;
                }
                for (; c < cc_tiling_size; c++)
                {
                    sum1 = 0.0;
                    C1 = C + r * col2 + c;
                    A11 = A + r * col + kk; A12 = A11 + 1;
                    B11 = B + kk * col2 + c;
                    B21 = B11 + col2;
                    for (k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        //1th row
                        sum1 += *A11 * *B11; sum1 += *A12 * *B21;//1st column

                        A11 += 2; A12 += 2;
                        B11 += 2 * col2;
                        B21 += 2 * col2;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        sum1 += *A11 * *B11;

                        A11++;
                        B11 += col2;
                    }
                    *C1 += sum1;
                }
            }
        }
    }
}

static inline void gemm_purec_tiling(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, const MM_TYPE alpha, uint32_t row, uint32_t col, uint32_t col2, uint32_t tiling_size)
{
    //4*2*4
    int row_4 = row >> 2;
    int max_row = row_4 << 2;

    const MM_TYPE* A = src1;
    const MM_TYPE* B = src2;
    MM_TYPE* C = dst;
    int r, c, k, kk, cc;
    MM_TYPE sum1, sum2, sum3, sum4, sum5, sum6, sum7, sum8, \
            sum9, sum10, sum11, sum12, sum13, sum14, sum15, sum16;
    int kk_tiling_size, cc_tiling_size, max_cc_tiling_size, max_kk_tiling_size;
    const MM_TYPE* A11, * A12, * B11, * B12, * B13, * B14, \
                 * A21, * A22, * B21, * B22, * B23, * B24, \
                 * A31, * A32, \
                 * A41, * A42;
    MM_TYPE * C1, * C2, * C3, * C4;
    for (kk = 0; kk < col; kk += tiling_size)
    {
        kk_tiling_size = ((kk + tiling_size) > col ? col : (kk + tiling_size));
        if ((kk + tiling_size) <= col)
        {
            max_kk_tiling_size = kk_tiling_size - (tiling_size & 1);
        }
        else
        {
            max_kk_tiling_size = col - ((col - kk) & 1);
        }
        for (cc = 0; cc < col2; cc += tiling_size)
        {
            cc_tiling_size = ((cc + tiling_size) > col2 ? col2 : (cc + tiling_size));
            if ((cc + tiling_size) <= col2)
            {
                max_cc_tiling_size = cc_tiling_size - (tiling_size & 3);
            }
            else
            {
                max_cc_tiling_size = col2 - ((col2 - cc) & 3);
            }

            for (r = 0; r < max_row; r += 4)
            {
                for (c = cc; c < max_cc_tiling_size; c += 4)
                {
                    sum1 = 0.0; sum2 = 0.0; sum3 = 0.0; sum4 = 0.0;
                    sum5 = 0.0; sum6 = 0.0; sum7 = 0.0; sum8 = 0.0;
                    sum9 = 0.0; sum10 = 0.0; sum11 = 0.0; sum12 = 0.0;
                    sum13 = 0.0; sum14 = 0.0; sum15 = 0.0; sum16 = 0.0;
                    C1 = C + r * col2 + c;
                    C2 = C1 + col2;
                    C3 = C2 + col2;
                    C4 = C3 + col2;
                    A11 = A + r * col + kk; A12 = A11 + 1;
                    A21 = A11 + col; A22 = A21 + 1;
                    A31 = A21 + col; A32 = A31 + 1;
                    A41 = A31 + col; A42 = A41 + 1;
                    B11 = B + kk * col2 + c; B12 = B11 + 1; B13 = B12 + 1; B14 = B13 + 1;
                    B21 = B11 + col2; B22 = B21 + 1; B23 = B22 + 1; B24 = B23 + 1;

                    for (k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        //1st row
                        sum1 += *A11 * *B11; sum1 += *A12 * *B21;//1st column
                        sum2 += *A11 * *B12; sum2 += *A12 * *B22;//2nd column
                        sum3 += *A11 * *B13; sum3 += *A12 * *B23;//3rd column
                        sum4 += *A11 * *B14; sum4 += *A12 * *B24;//4th column
                        //2nd row
                        sum5 += *A21 * *B11; sum5 += *A22 * *B21;//1st column
                        sum6 += *A21 * *B12; sum6 += *A22 * *B22;//2nd column
                        sum7 += *A21 * *B13; sum7 += *A22 * *B23;//3rd column
                        sum8 += *A21 * *B14; sum8 += *A22 * *B24;//4th column
                        //3rd row
                        sum9 += *A31 * *B11; sum9 += *A32 * *B21;//1st column
                        sum10 += *A31 * *B12; sum10 += *A32 * *B22;//2nd column
                        sum11 += *A31 * *B13; sum11 += *A32 * *B23;//3rd column
                        sum12 += *A31 * *B14; sum12 += *A32 * *B24;//4th column
                        //4th row
                        sum13 += *A41 * *B11; sum13 += *A42 * *B21;//1st column
                        sum14 += *A41 * *B12; sum14 += *A42 * *B22;//2nd column
                        sum15 += *A41 * *B13; sum15 += *A42 * *B23;//3rd column
                        sum16 += *A41 * *B14; sum16 += *A42 * *B24;//4th column

                        A11 += 2; A12 += 2;
                        A21 += 2; A22 += 2;
                        A31 += 2; A32 += 2;
                        A41 += 2; A42 += 2;
                        B11 += 2 * col2; B12 += 2 * col2; B13 += 2 * col2; B14 += 2 * col2;
                        B21 += 2 * col2; B22 += 2 * col2; B23 += 2 * col2; B24 += 2 * col2;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        sum1 += *A11 * *B11;
                        sum2 += *A11 * *B12;
                        sum3 += *A11 * *B13;
                        sum4 += *A11 * *B14;

                        sum5 += *A21 * *B11;
                        sum6 += *A21 * *B12;
                        sum7 += *A21 * *B13;
                        sum8 += *A21 * *B14;

                        sum9 += *A31 * *B11;
                        sum10 += *A31 * *B12;
                        sum11 += *A31 * *B13;
                        sum12 += *A31 * *B14;

                        sum13 += *A41 * *B11;
                        sum14 += *A41 * *B12;
                        sum15 += *A41 * *B13;
                        sum16 += *A41 * *B14;

                        A11++;
                        A21++;
                        A31++;
                        A41++;
                        B11 += col2; B12 += col2; B13 += col2; B14 += col2;
                    }
                    *C1++ += sum1 * alpha; *C1++ += sum2 * alpha; *C1++ += sum3 * alpha; *C1 += sum4 * alpha;
                    *C2++ += sum5 * alpha; *C2++ += sum6 * alpha; *C2++ += sum7 * alpha; *C2 += sum8 * alpha;
                    *C3++ += sum9 * alpha; *C3++ += sum10 * alpha; *C3++ += sum11 * alpha; *C3 += sum12 * alpha;
                    *C4++ += sum13 * alpha; *C4++ += sum14 * alpha; *C4++ += sum15 * alpha; *C4 += sum16 * alpha;
                }
                for (; c < cc_tiling_size; c++)
                {
                    sum1 = 0.0;
                    sum5 = 0.0;
                    sum9 = 0.0;
                    sum13 = 0.0;
                    C1 = C + r * col2 + c;
                    C2 = C1 + col2;
                    C3 = C2 + col2;
                    C4 = C3 + col2;
                    A11 = A + r * col + kk; A12 = A11 + 1;
                    A21 = A11 + col; A22 = A21 + 1;
                    A31 = A21 + col; A32 = A31 + 1;
                    A41 = A31 + col; A42 = A41 + 1;
                    B11 = B + kk * col2 + c;
                    B21 = B11 + col2;
                    for (k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        //1th row
                        sum1 += *A11 * *B11; sum1 += *A12 * *B21;//1st column
                        //2th row
                        sum5 += *A21 * *B11; sum5 += *A22 * *B21;//1st column
                        //3th row
                        sum9 += *A31 * *B11; sum9 += *A32 * *B21;//1st column
                        //4th row
                        sum13 += *A41 * *B11; sum13 += *A42 * *B21;//1st column

                        A11 += 2; A12 += 2;
                        A21 += 2; A22 += 2;
                        A31 += 2; A32 += 2;
                        A41 += 2; A42 += 2;
                        B11 += 2 * col2;
                        B21 += 2 * col2;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        sum1 += *A11 * *B11;
                        sum5 += *A21 * *B11;
                        sum9 += *A31 * *B11;
                        sum13 += *A41 * *B11;

                        A11++;
                        A21++;
                        A31++;
                        A41++;
                        B11 += col2;
                    }
                    *C1 += sum1 * alpha;
                    *C2 += sum5 * alpha;
                    *C3 += sum9 * alpha;
                    *C4 += sum13 * alpha;
                }
            }
            for (; r < row; r++)
            {
                for (c = cc; c < max_cc_tiling_size; c += 4)
                {
                    sum1 = 0.0; sum2 = 0.0; sum3 = 0.0; sum4 = 0.0;
                    C1 = C + r * col2 + c; C2 = C1 + 1; C3 = C2 + 1; C4 = C3 + 1;
                    A11 = A + r * col + kk; A12 = A11 + 1;
                    B11 = B + kk * col2 + c; B12 = B11 + 1; B13 = B12 + 1; B14 = B13 + 1;
                    B21 = B11 + col2; B22 = B21 + 1; B23 = B22 + 1; B24 = B23 + 1;
                    for (k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        //1th row
                        sum1 += *A11 * *B11; sum1 += *A12 * *B21;//1st column
                        sum2 += *A11 * *B12; sum2 += *A12 * *B22;//2nd column
                        sum3 += *A11 * *B13; sum3 += *A12 * *B23;//3rd column
                        sum4 += *A11 * *B14; sum4 += *A12 * *B24;//4th column

                        A11 += 2; A12 += 2;
                        B11 += 2 * col2; B12 += 2 * col2; B13 += 2 * col2; B14 += 2 * col2;
                        B21 += 2 * col2; B22 += 2 * col2; B23 += 2 * col2; B24 += 2 * col2;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        sum1 += *A11 * *B11;
                        sum2 += *A11 * *B12;
                        sum3 += *A11 * *B13;
                        sum4 += *A11 * *B14;

                        A11++;
                        B11 += col2; B12 += col2; B13 += col2; B14 += col2;
                    }
                    *C1 += sum1 * alpha; *C2 += sum2 * alpha; *C3 += sum3 * alpha; *C4 += sum4 * alpha;
                }
                for (; c < cc_tiling_size; c++)
                {
                    sum1 = 0.0;
                    C1 = C + r * col2 + c;
                    A11 = A + r * col + kk; A12 = A11 + 1;
                    B11 = B + kk * col2 + c;
                    B21 = B11 + col2;
                    for (k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        //1th row
                        sum1 += *A11 * *B11; sum1 += *A12 * *B21;//1st column

                        A11 += 2; A12 += 2;
                        B11 += 2 * col2;
                        B21 += 2 * col2;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        sum1 += *A11 * *B11;

                        A11++;
                        B11 += col2;
                    }
                    *C1 += sum1 * alpha;
                }
            }
        }
    }
}

static inline void vec_cmat_mul_purec_tiling(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, uint32_t row, uint32_t col, uint32_t col2, uint32_t tiling_size)
{
    //2*2*2
    int row_2 = row >> 1;
    int max_row = row_2 << 1;
    int r, c, k, kk, cc;
    MM_TYPE sum_re_11, sum_im_11, sum_re_12, sum_im_12, \
            sum_re_21, sum_im_21, sum_re_22, sum_im_22;
    UNION_CLX_T *A11, *A12, \
                *A21, *A22;
    UNION_CLX_T *B11, *B12, \
                *B21, *B22;
    UNION_CLX_T *C1, \
                *C2;
    int kk_tiling_size, max_kk_tiling_size, cc_tiling_size, max_cc_tiling_size;
    for(kk = 0; kk < col; kk += tiling_size)
    {
        kk_tiling_size = ((kk + tiling_size) > col ? col : (kk + tiling_size));
        if ((kk + tiling_size) <= col)
        {
            max_kk_tiling_size = kk_tiling_size - (tiling_size & 1);
        }
        else
        {
            max_kk_tiling_size = col - ((col - kk) & 1);
        }
        for( cc = 0; cc < col2; cc += tiling_size)
        {
            cc_tiling_size = ((cc + tiling_size) > col2 ? col2 : (cc + tiling_size));
            if ((cc + tiling_size) <= col2)
            {
                max_cc_tiling_size = cc_tiling_size - (tiling_size & 1);
            }
            else
            {
                max_cc_tiling_size = col2 - ((col2 - cc) & 1);
            }
            for(r = 0; r < max_row; r += 2)
            {
                for(c = cc; c < max_cc_tiling_size; c += 2)
                {
                    sum_re_11 = 0; sum_im_11 = 0; sum_re_12 = 0; sum_im_12 = 0;
                    sum_re_21 = 0; sum_im_21 = 0; sum_re_22 = 0; sum_im_22 = 0;

                    A11 = (UNION_CLX_T *)src1 + r * col + kk; A12 = A11 + 1;
                    A21 = A11 + col; A22 = A21 + 1;
                    B11 = (UNION_CLX_T *)src2 + kk * col2 + c; B12 = B11 + 1;
                    B21 = B11 + col2; B22 = B21 + 1;
                    C1 = (UNION_CLX_T *)dst + r * col2 + c;
                    C2 = C1 + col2;
                    for(k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        sum_re_11 += A11->re * B11->re - A11->im * B11->im;
                        sum_im_11 += A11->re * B11->im + A11->im * B11->re;
                        sum_re_12 += A11->re * B12->re - A11->im * B12->im;
                        sum_im_12 += A11->re * B12->im + A11->im * B12->re;

                        sum_re_11 += A12->re * B21->re - A12->im * B21->im;
                        sum_im_11 += A12->re * B21->im + A12->im * B21->re;
                        sum_re_12 += A12->re * B22->re - A12->im * B22->im;
                        sum_im_12 += A12->re * B22->im + A12->im * B22->re;

                        sum_re_21 += A21->re * B11->re - A21->im * B11->im;
                        sum_im_21 += A21->re * B11->im + A21->im * B11->re;
                        sum_re_22 += A21->re * B12->re - A21->im * B12->im;
                        sum_im_22 += A21->re * B12->im + A21->im * B12->re;

                        sum_re_21 += A22->re * B21->re - A22->im * B21->im;
                        sum_im_21 += A22->re * B21->im + A22->im * B21->re;
                        sum_re_22 += A22->re * B22->re - A22->im * B22->im;
                        sum_im_22 += A22->re * B22->im + A22->im * B22->re;

                        A11 += 2; A12 += 2;
                        A21 += 2; A22 += 2;
                        B11 += 2 * col2; B12 += 2 * col2;
                        B21 += 2 * col2; B22 += 2 * col2;
                    }
                    for(; k < kk_tiling_size; k++)
                    {
                        sum_re_11 += A11->re * B11->re - A11->im * B11->im;
                        sum_im_11 += A11->re * B11->im + A11->im * B11->re;
                        sum_re_12 += A11->re * B12->re - A11->im * B12->im;
                        sum_im_12 += A11->re * B12->im + A11->im * B12->re;
                        sum_re_21 += A21->re * B11->re - A21->im * B11->im;
                        sum_im_21 += A21->re * B11->im + A21->im * B11->re;
                        sum_re_22 += A21->re * B12->re - A21->im * B12->im;
                        sum_im_22 += A21->re * B12->im + A21->im * B12->re;
                        A11++;
                        A21++;
                        B11 += col2; B12 += col2;
                    }
                    C1->re += sum_re_11; C1++->im += sum_im_11; C1->re += sum_re_12; C1->im += sum_im_12;
                    C2->re += sum_re_21; C2++->im += sum_im_21; C2->re += sum_re_22; C2->im += sum_im_22;
                }
                for(; c < cc_tiling_size; c++)
                {
                    sum_re_11 = 0; sum_im_11 = 0;
                    sum_re_21 = 0; sum_im_21 = 0;
                    A11 = (UNION_CLX_T *)src1 + r * col + kk; A12 = A11 + 1;
                    A21 = A11 + col; A22 = A21 + 1;
                    B11 = (UNION_CLX_T *)src2 + kk * col2 + c;
                    B21 = B11 + col2;
                    C1 = (UNION_CLX_T *)dst + r * col2 + c;
                    C2 = C1 + col2;
                    for(k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        sum_re_11 += A11->re * B11->re - A11->im * B11->im;
                        sum_im_11 += A11->re * B11->im + A11->im * B11->re;
                        sum_re_11 += A12->re * B21->re - A12->im * B21->im;
                        sum_im_11 += A12->re * B21->im + A12->im * B21->re;

                        sum_re_21 += A21->re * B11->re - A21->im * B11->im;
                        sum_im_21 += A21->re * B11->im + A21->im * B11->re;
                        sum_re_21 += A22->re * B21->re - A22->im * B21->im;
                        sum_im_21 += A22->re * B21->im + A22->im * B21->re;

                        A11 += 2; A12 += 2;
                        A21 += 2; A22 += 2;
                        B11 += 2 * col2;
                        B21 += 2 * col2;
                    }
                    for(; k < kk_tiling_size; k++)
                    {
                        sum_re_11 += A11->re * B11->re - A11->im * B11->im;
                        sum_im_11 += A11->re * B11->im + A11->im * B11->re;
                        sum_re_21 += A21->re * B11->re - A21->im * B11->im;
                        sum_im_21 += A21->re * B11->im + A21->im * B11->re;
                        A11++;
                        A21++;
                        B11 += col2;
                    }
                    C1->re += sum_re_11; C1->im += sum_im_11;
                    C2->re += sum_re_21; C2->im += sum_im_21;
                }
            }
            for(; r < row; r++)
            {
                for(c = cc; c < max_cc_tiling_size; c += 2)
                {
                    sum_re_11 = 0; sum_im_11 = 0; sum_re_12 = 0; sum_im_12 = 0;

                    A11 = (UNION_CLX_T *)src1 + r * col + kk; A12 = A11 + 1;
                    B11 = (UNION_CLX_T *)src2 + kk * col2 + c; B12 = B11 + 1;
                    B21 = B11 + col2; B22 = B21 + 1;
                    C1 = (UNION_CLX_T *)dst + r * col2 + c;
                    for(k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        sum_re_11 += A11->re * B11->re - A11->im * B11->im;
                        sum_im_11 += A11->re * B11->im + A11->im * B11->re;
                        sum_re_11 += A12->re * B21->re - A12->im * B21->im;
                        sum_im_11 += A12->re * B21->im + A12->im * B21->re;

                        sum_re_12 += A11->re * B12->re - A11->im * B12->im;
                        sum_im_12 += A11->re * B12->im + A11->im * B12->re;
                        sum_re_12 += A12->re * B22->re - A12->im * B22->im;
                        sum_im_12 += A12->re * B22->im + A12->im * B22->re;

                        A11 += 2; A12 += 2;
                        B11 += 2 * col2; B12 += 2 * col2;
                        B21 += 2 * col2; B22 += 2 * col2;
                    }
                    for(; k < kk_tiling_size; k++)
                    {
                        sum_re_11 += A11->re * B11->re - A11->im * B11->im;
                        sum_im_11 += A11->re * B11->im + A11->im * B11->re;
                        sum_re_12 += A11->re * B12->re - A11->im * B12->im;
                        sum_im_12 += A11->re * B12->im + A11->im * B12->re;
                        A11++;
                        B11 += col2; B12 += col2;
                    }
                    C1->re += sum_re_11; C1++->im += sum_im_11; C1->re += sum_re_12; C1->im += sum_im_12;
                }
                for(; c < cc_tiling_size; c++)
                {
                    sum_re_11 = 0; sum_im_11 = 0;
                    A11 = (UNION_CLX_T *)src1 + r * col + kk; A12 = A11 + 1;
                    B11 = (UNION_CLX_T *)src2 + kk * col2 + c;
                    B21 = B11 + col2;
                    C1 = (UNION_CLX_T *)dst + r * col2 + c;
                    for(k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        sum_re_11 += A11->re * B11->re - A11->im * B11->im;
                        sum_im_11 += A11->re * B11->im + A11->im * B11->re;
                        sum_re_11 += A12->re * B21->re - A12->im * B21->im;
                        sum_im_11 += A12->re * B21->im + A12->im * B21->re;

                        A11 += 2; A12 += 2;
                        B11 += 2 * col2;
                        B21 += 2 * col2;
                    }
                    for(; k < kk_tiling_size; k++)
                    {
                        sum_re_11 += A11->re * B11->re - A11->im * B11->im;
                        sum_im_11 += A11->re * B11->im + A11->im * B11->re;
                        A11++;
                        B11 += col2;
                    }
                    C1->re += sum_re_11; C1->im += sum_im_11;
                }
            }
        }
    }
}

static inline void vec_cgemm_purec_tiling(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, const MM_TYPE alpha, uint32_t row, uint32_t col, uint32_t col2, uint32_t tiling_size)
{
    //2*2*2
    int row_2 = row >> 1;
    int max_row = row_2 << 1;
    int r, c, k, kk, cc;
    MM_TYPE sum_re_11, sum_im_11, sum_re_12, sum_im_12, \
            sum_re_21, sum_im_21, sum_re_22, sum_im_22;
    UNION_CLX_T *A11, *A12, \
                *A21, *A22;
    UNION_CLX_T *B11, *B12, \
                *B21, *B22;
    UNION_CLX_T *C1, \
                *C2;
    int kk_tiling_size, max_kk_tiling_size, cc_tiling_size, max_cc_tiling_size;
    for(kk = 0; kk < col; kk += tiling_size)
    {
        kk_tiling_size = ((kk + tiling_size) > col ? col : (kk + tiling_size));
        if ((kk + tiling_size) <= col)
        {
            max_kk_tiling_size = kk_tiling_size - (tiling_size & 1);
        }
        else
        {
            max_kk_tiling_size = col - ((col - kk) & 1);
        }
        for( cc = 0; cc < col2; cc += tiling_size)
        {
            cc_tiling_size = ((cc + tiling_size) > col2 ? col2 : (cc + tiling_size));
            if ((cc + tiling_size) <= col2)
            {
                max_cc_tiling_size = cc_tiling_size - (tiling_size & 1);
            }
            else
            {
                max_cc_tiling_size = col2 - ((col2 - cc) & 1);
            }
            for(r = 0; r < max_row; r += 2)
            {
                for(c = cc; c < max_cc_tiling_size; c += 2)
                {
                    sum_re_11 = 0; sum_im_11 = 0; sum_re_12 = 0; sum_im_12 = 0;
                    sum_re_21 = 0; sum_im_21 = 0; sum_re_22 = 0; sum_im_22 = 0;

                    A11 = (UNION_CLX_T *)src1 + r * col + kk; A12 = A11 + 1;
                    A21 = A11 + col; A22 = A21 + 1;
                    B11 = (UNION_CLX_T *)src2 + kk * col2 + c; B12 = B11 + 1;
                    B21 = B11 + col2; B22 = B21 + 1;
                    C1 = (UNION_CLX_T *)dst + r * col2 + c;
                    C2 = C1 + col2;
                    for(k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        sum_re_11 += A11->re * B11->re - A11->im * B11->im;
                        sum_im_11 += A11->re * B11->im + A11->im * B11->re;
                        sum_re_12 += A11->re * B12->re - A11->im * B12->im;
                        sum_im_12 += A11->re * B12->im + A11->im * B12->re;

                        sum_re_11 += A12->re * B21->re - A12->im * B21->im;
                        sum_im_11 += A12->re * B21->im + A12->im * B21->re;
                        sum_re_12 += A12->re * B22->re - A12->im * B22->im;
                        sum_im_12 += A12->re * B22->im + A12->im * B22->re;

                        sum_re_21 += A21->re * B11->re - A21->im * B11->im;
                        sum_im_21 += A21->re * B11->im + A21->im * B11->re;
                        sum_re_22 += A21->re * B12->re - A21->im * B12->im;
                        sum_im_22 += A21->re * B12->im + A21->im * B12->re;

                        sum_re_21 += A22->re * B21->re - A22->im * B21->im;
                        sum_im_21 += A22->re * B21->im + A22->im * B21->re;
                        sum_re_22 += A22->re * B22->re - A22->im * B22->im;
                        sum_im_22 += A22->re * B22->im + A22->im * B22->re;

                        A11 += 2; A12 += 2;
                        A21 += 2; A22 += 2;
                        B11 += 2 * col2; B12 += 2 * col2;
                        B21 += 2 * col2; B22 += 2 * col2;
                    }
                    for(; k < kk_tiling_size; k++)
                    {
                        sum_re_11 += A11->re * B11->re - A11->im * B11->im;
                        sum_im_11 += A11->re * B11->im + A11->im * B11->re;
                        sum_re_12 += A11->re * B12->re - A11->im * B12->im;
                        sum_im_12 += A11->re * B12->im + A11->im * B12->re;
                        sum_re_21 += A21->re * B11->re - A21->im * B11->im;
                        sum_im_21 += A21->re * B11->im + A21->im * B11->re;
                        sum_re_22 += A21->re * B12->re - A21->im * B12->im;
                        sum_im_22 += A21->re * B12->im + A21->im * B12->re;
                        A11++;
                        A21++;
                        B11 += col2; B12 += col2;
                    }
                    C1->re += sum_re_11 * alpha; C1++->im += sum_im_11 * alpha; C1->re += sum_re_12 * alpha; C1->im += sum_im_12 * alpha;
                    C2->re += sum_re_21 * alpha; C2++->im += sum_im_21 * alpha; C2->re += sum_re_22 * alpha; C2->im += sum_im_22 * alpha;
                }
                for(; c < cc_tiling_size; c++)
                {
                    sum_re_11 = 0; sum_im_11 = 0;
                    sum_re_21 = 0; sum_im_21 = 0;
                    A11 = (UNION_CLX_T *)src1 + r * col + kk; A12 = A11 + 1;
                    A21 = A11 + col; A22 = A21 + 1;
                    B11 = (UNION_CLX_T *)src2 + kk * col2 + c;
                    B21 = B11 + col2;
                    C1 = (UNION_CLX_T *)dst + r * col2 + c;
                    C2 = C1 + col2;
                    for(k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        sum_re_11 += A11->re * B11->re - A11->im * B11->im;
                        sum_im_11 += A11->re * B11->im + A11->im * B11->re;
                        sum_re_11 += A12->re * B21->re - A12->im * B21->im;
                        sum_im_11 += A12->re * B21->im + A12->im * B21->re;

                        sum_re_21 += A21->re * B11->re - A21->im * B11->im;
                        sum_im_21 += A21->re * B11->im + A21->im * B11->re;
                        sum_re_21 += A22->re * B21->re - A22->im * B21->im;
                        sum_im_21 += A22->re * B21->im + A22->im * B21->re;

                        A11 += 2; A12 += 2;
                        A21 += 2; A22 += 2;
                        B11 += 2 * col2;
                        B21 += 2 * col2;
                    }
                    for(; k < kk_tiling_size; k++)
                    {
                        sum_re_11 += A11->re * B11->re - A11->im * B11->im;
                        sum_im_11 += A11->re * B11->im + A11->im * B11->re;
                        sum_re_21 += A21->re * B11->re - A21->im * B11->im;
                        sum_im_21 += A21->re * B11->im + A21->im * B11->re;
                        A11++;
                        A21++;
                        B11 += col2;
                    }
                    C1->re += sum_re_11 * alpha; C1->im += sum_im_11 * alpha;
                    C2->re += sum_re_21 * alpha; C2->im += sum_im_21 * alpha;
                }
            }
            for(; r < row; r++)
            {
                for(c = cc; c < max_cc_tiling_size; c += 2)
                {
                    sum_re_11 = 0; sum_im_11 = 0; sum_re_12 = 0; sum_im_12 = 0;

                    A11 = (UNION_CLX_T *)src1 + r * col + kk; A12 = A11 + 1;
                    B11 = (UNION_CLX_T *)src2 + kk * col2 + c; B12 = B11 + 1;
                    B21 = B11 + col2; B22 = B21 + 1;
                    C1 = (UNION_CLX_T *)dst + r * col2 + c;
                    for(k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        sum_re_11 += A11->re * B11->re - A11->im * B11->im;
                        sum_im_11 += A11->re * B11->im + A11->im * B11->re;
                        sum_re_11 += A12->re * B21->re - A12->im * B21->im;
                        sum_im_11 += A12->re * B21->im + A12->im * B21->re;

                        sum_re_12 += A11->re * B12->re - A11->im * B12->im;
                        sum_im_12 += A11->re * B12->im + A11->im * B12->re;
                        sum_re_12 += A12->re * B22->re - A12->im * B22->im;
                        sum_im_12 += A12->re * B22->im + A12->im * B22->re;

                        A11 += 2; A12 += 2;
                        B11 += 2 * col2; B12 += 2 * col2;
                        B21 += 2 * col2; B22 += 2 * col2;
                    }
                    for(; k < kk_tiling_size; k++)
                    {
                        sum_re_11 += A11->re * B11->re - A11->im * B11->im;
                        sum_im_11 += A11->re * B11->im + A11->im * B11->re;
                        sum_re_12 += A11->re * B12->re - A11->im * B12->im;
                        sum_im_12 += A11->re * B12->im + A11->im * B12->re;
                        A11++;
                        B11 += col2; B12 += col2;
                    }
                    C1->re += sum_re_11 * alpha; C1++->im += sum_im_11 * alpha; C1->re += sum_re_12 * alpha; C1->im += sum_im_12 * alpha;
                }
                for(; c < cc_tiling_size; c++)
                {
                    sum_re_11 = 0; sum_im_11 = 0;
                    A11 = (UNION_CLX_T *)src1 + r * col + kk; A12 = A11 + 1;
                    B11 = (UNION_CLX_T *)src2 + kk * col2 + c;
                    B21 = B11 + col2;
                    C1 = (UNION_CLX_T *)dst + r * col2 + c;
                    for(k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        sum_re_11 += A11->re * B11->re - A11->im * B11->im;
                        sum_im_11 += A11->re * B11->im + A11->im * B11->re;
                        sum_re_11 += A12->re * B21->re - A12->im * B21->im;
                        sum_im_11 += A12->re * B21->im + A12->im * B21->re;

                        A11 += 2; A12 += 2;
                        B11 += 2 * col2;
                        B21 += 2 * col2;
                    }
                    for(; k < kk_tiling_size; k++)
                    {
                        sum_re_11 += A11->re * B11->re - A11->im * B11->im;
                        sum_im_11 += A11->re * B11->im + A11->im * B11->re;
                        A11++;
                        B11 += col2;
                    }
                    C1->re += sum_re_11 * alpha; C1->im += sum_im_11 * alpha;
                }
            }
        }
    }
}

#else

static inline void vec_mat_mul_purec_original(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, uint32_t row, uint32_t col, uint32_t col2)
{
    const MM_TYPE* A = src1;
    const MM_TYPE* B = src2;
    const MM_TYPE* InA = A;
    MM_TYPE* C;
    MM_TYPE sum;
    //double sum;
    uint32_t i, colcnt, col2cnt, rowcnt;

    i = 0u;
    rowcnt = row;
    do
    {
        C = dst + i;
        B = src2;

        /* Dot product of each row in src1 with each column in src2 */
        col2cnt = col2;
        do
        {
            sum = 0.0f;
            A = InA;

            /* column loop */
            colcnt = col;
            do
            {
                sum += (*A++) * *B;
                B += col2;
                colcnt--;
            } while (colcnt != 0u);
            *C++ = sum;
            //*C++ = (MM_TYPE)sum;
            col2cnt--;
            B = src2 + (col2 - col2cnt);
        } while (col2cnt != 0u);
        i = i + col2;
        InA = A;
        rowcnt--;
    } while (rowcnt != 0u);
}

static inline void gemm_purec_original(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, const MM_TYPE alpha, const MM_TYPE beta, uint32_t row, uint32_t col, uint32_t col2)
{
    const MM_TYPE *A = src1;
    const MM_TYPE *B = src2;
    const MM_TYPE *InA = A;
    MM_TYPE *C;
    MM_TYPE sum;
    //double sum;
    uint32_t i, colcnt, col2cnt, rowcnt;
    i = 0u;
    rowcnt = row;
    do
    {
        C = dst + i;
        B = src2;

        /* Dot product of each row in src1 with each column in src2 */
        col2cnt = col2;
        do
        {
            sum = 0.0f;
            A = InA;

            /* column loop */
            colcnt = col;
            do
            {
                sum += (*A++) * *B;
                B += col2;
                colcnt--;
            }
            while (colcnt != 0u);
            *C = *C * beta + alpha * sum;
            C++;
            col2cnt--;
            B = src2 + (col2 - col2cnt);
        }
        while (col2cnt != 0u);
        i = i + col2;
        InA = A;
        rowcnt--;
    }
    while (rowcnt != 0u);
}

static inline void vec_cmat_mul_purec_original(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, uint32_t row, uint32_t col, uint32_t col2)
{
    uint32_t j;
    const MM_TYPE *Aend = src1 + (row * col * 2);
    MM_TYPE sum_re;
    MM_TYPE sum_im;
    do
    {
        const UNION_CLX_T *B = (UNION_CLX_T *)src2;
        /* Dot product of each row in src1 with each column in src2 */
        j = col2;
        do
        {
            const UNION_CLX_T *A = (UNION_CLX_T *)src1;
            const UNION_CLX_T *A2 = (UNION_CLX_T *)(A + col);
            sum_re = 0;
            sum_im = 0;
            /* column loop */
            UNION_CLX_T a, b;
            do
            {
                //  If V3_O3 and unrolling, the insts as well as cycles are 42882/45203,
                //  and the performance is better than Keil.(Keil's cycle for CM3_O3 is 45388.)
                a = *A++;
                b = *B;
                sum_re += a.re * b.re - a.im * b.im;
                sum_im += a.re * b.im + a.im * b.re;
                B += (col2);
            }
            while (A != A2);

            *dst++ = sum_re;
            *dst++ = sum_im;
            B = (UNION_CLX_T *)(src2 + 2 * (col2 - (--j)));
        }
        while (j != 0u);

        src1 += (col * 2);
    }
    while (src1 != Aend);
}

static inline void vec_cgemm_purec_original(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, const MM_TYPE alpha, const MM_TYPE beta, uint32_t row, uint32_t col, uint32_t col2)
{
    uint32_t j;
    const MM_TYPE *Aend = src1 + (row * col * 2);
    MM_TYPE sum_re;
    MM_TYPE sum_im;
    do
    {
        const UNION_CLX_T *B = (UNION_CLX_T *)src2;
        /* Dot product of each row in src1 with each column in src2 */
        j = col2;
        do
        {
            const UNION_CLX_T *A = (UNION_CLX_T *)src1;
            const UNION_CLX_T *A2 = (UNION_CLX_T *)(A + col);
            sum_re = 0;
            sum_im = 0;
            /* column loop */
            UNION_CLX_T a, b;
            do
            {
                //  If V3_O3 and unrolling, the insts as well as cycles are 42882/45203,
                //  and the performance is better than Keil.(Keil's cycle for CM3_O3 is 45388.)
                a = *A++;
                b = *B;
                sum_re += a.re * b.re - a.im * b.im;
                sum_im += a.re * b.im + a.im * b.re;
                B += (col2);
            }
            while (A != A2);

            *dst = *dst * beta + alpha * sum_re;
            dst++;
            *dst = *dst * beta + alpha * sum_im;
            dst++;
            B = (UNION_CLX_T *)(src2 + 2 * (col2 - (--j)));
        }
        while (j != 0u);

        src1 += (col * 2);
    }
    while (src1 != Aend);
}
#endif //ENA_PUREC_TILING

#ifdef ENA_MAT_MUL_VQ
static inline void vec_mat_mul_purec_original_vq(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE_VQ* dst, uint32_t row, uint32_t col, uint32_t col2)
{
    uint32_t j;
    const MM_TYPE *Aend = src1 + row * col;
    do
    {
        const MM_TYPE *B = src2;

        j = col2;
        do
        {
            const MM_TYPE *A = src1;
            const MM_TYPE *A2 = A + col;
            MM_TYPE_VQ sum = 0;

            do
            {
                sum += (MM_TYPE_VQ) (*A++) * (*B);
                B += col2;

            }
            while (A != A2);
            *dst++ = sum;
            B = src2 + col2 - (--j);
        }
        while (j != 0);

        src1 += col;
    }
    while (src1 != Aend);
}

static inline void vec_cmat_mul_purec_original_vq(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE_VQ* dst, uint32_t row, uint32_t col, uint32_t col2)
{
    uint32_t j;
    const MM_TYPE *Aend = src1 + (row * col * 2);
    MM_TYPE_VQ sum_re;
    MM_TYPE_VQ sum_im;
    do
    {
        const UNION_CLX_T *B = (UNION_CLX_T *)src2;
        /* Dot product of each row in src1 with each column in src2 */
        j = col2;
        do
        {
            const UNION_CLX_T *A = (UNION_CLX_T *)src1;
            const UNION_CLX_T *A2 = (UNION_CLX_T *)(A + col);
            sum_re = 0;
            sum_im = 0;
            /* column loop */
            UNION_CLX_T a, b;
            do
            {
                a = *A++;
                b = *B;
                sum_re += (MM_TYPE_VQ)a.re * b.re - (MM_TYPE_VQ)a.im * b.im;
                sum_im += (MM_TYPE_VQ)a.re * b.im + (MM_TYPE_VQ)a.im * b.re;
                B += (col2);
            }
            while (A != A2);

            *dst++ = sum_re;
            *dst++ = sum_im;
            B = (UNION_CLX_T *)(src2 + 2 * (col2 - (--j)));
        }
        while (j != 0u);

        src1 += (col * 2);
    }
    while (src1 != Aend);
}

static inline void vec_gemm_purec_original_vq(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE_VQ* dst, const MM_TYPE alpha, const MM_TYPE beta, uint32_t row, uint32_t col, uint32_t col2, int32_t shift)
{
    uint32_t j;
    const MM_TYPE *Aend = src1 + row * col;

    do
    {
        const MM_TYPE *B = src2;

        j = col2;
        do
        {
            const MM_TYPE *A = src1;
            const MM_TYPE *A2 = A + col;
            MM_TYPE_VQ sum = 0;

            do
            {
                sum += (MM_TYPE_VQ) (*A++) * (*B);
                B += col2;

            }
            while (A != A2);

            *dst = (q63_t)((q63_t)*dst * beta + (q63_t)alpha * sum) >> shift;
            dst++;
            B = src2 + col2 - (--j);
        }
        while (j != 0);

        src1 += col;
    }
    while (src1 != Aend);
}

static inline void vec_cgemm_purec_original_vq(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE_VQ* dst, const MM_TYPE alpha, const MM_TYPE beta, uint32_t row, uint32_t col, uint32_t col2, int32_t shift)
{
    uint32_t j;
    const MM_TYPE *Aend = src1 + (row * col * 2);
    MM_TYPE_VQ sum_re;
    MM_TYPE_VQ sum_im;
    do
    {
        const UNION_CLX_T *B = (UNION_CLX_T *)src2;
        /* Dot product of each row in src1 with each column in src2 */
        j = col2;
        do
        {
            const UNION_CLX_T *A = (UNION_CLX_T *)src1;
            const UNION_CLX_T *A2 = (UNION_CLX_T *)(A + col);
            sum_re = 0;
            sum_im = 0;
            /* column loop */
            UNION_CLX_T a, b;
            do
            {
                a = *A++;
                b = *B;
                sum_re += (MM_TYPE_VQ)a.re * b.re - (MM_TYPE_VQ)a.im * b.im;
                sum_im += (MM_TYPE_VQ)a.re * b.im + (MM_TYPE_VQ)a.im * b.re;
                B += (col2);
            }
            while (A != A2);

            *dst = (q63_t)((q63_t)*dst * beta + (q63_t)alpha * sum_re) >> shift;
            dst++;
            *dst = (q63_t)((q63_t)*dst * beta + (q63_t)alpha * sum_im) >> shift;
            dst++;
            B = (UNION_CLX_T *)(src2 + 2 * (col2 - (--j)));
        }
        while (j != 0u);

        src1 += (col * 2);
    }
    while (src1 != Aend);
}
#endif //ENA_MAT_MUL_VQ

#ifdef ENA_DYNAMIC_CALC_CACHE_CONFIG
//----- difinitions for L2C_begin -----
#define __I                     volatile const	/* 'read only' permissions      */
#define __O                     volatile        /* 'write only' permissions     */
#define __IO                    volatile        /* 'read / write' permissions   */

/*****************************************************************************
 * SMU - AE350
 ****************************************************************************/
typedef struct {
	__I  unsigned int SYSTEMVER;            /* 0x00 SYSTEM ID and Revision Register */
	     unsigned int RESERVED0[1];         /* 0x04 Reserved */
	__I  unsigned int SYSTEMCFG;            /* 0x08 SYSTEM configuration register */
	__I  unsigned int SMUVER;               /* 0x0C SMU version register */
	__IO unsigned int WRSR;                 /* 0x10 Wakeup and Reset Status Register */
	__IO unsigned int SMUCR;                /* 0x14 SMU Command Register */
	     unsigned int RESERVED1[1];         /* 0x18 Reserved */
	__IO unsigned int WRMASK;               /* 0x1C Wake up Mask Register */
	__IO unsigned int CER;                  /* 0x20 Clock Enable Register */
	__IO unsigned int CRR;                  /* 0x24 Clock Ratio Register */
	     unsigned int RESERVED2[6];         /* 0x28 ~ 0x3C Reserved Register */
	__IO unsigned int SCRATCH;              /* 0x40 Scratch Register */
	     unsigned int RESERVED3[3];         /* 0x44 ~ 0x4C Reserved */
	__IO unsigned int RESET_VECTOR;         /* 0x50 CPU Reset Vector Register */
} SMU_RegDef;

/*****************************************************************************
 * L2CACHE - AE350
 ****************************************************************************/
typedef struct {
	__I  unsigned long long CFG;            /* 0x00 Configuration Register */
	__IO unsigned long long CTL;            /* 0x08 Control Register */
	__IO unsigned long long HPMCTL0;        /* 0x10 HPM Control Register0 */
	__IO unsigned long long HPMCTL1;        /* 0x18 HPM Control Register1 */
	__IO unsigned long long HPMCTL2;        /* 0x20 HPM Control Register2 */
	__IO unsigned long long HPMCTL3;        /* 0x28 HPM Control Register3 */
	__IO unsigned long long ASYNERR;        /* 0x30 Asynchronous Error Register */
	__IO unsigned long long ERR;            /* 0x38 Error Register */
	union {
		struct {
			__IO unsigned long long CORE0CCTLCMD;   /* 0x40 Core 0 CCTL Command Register */
			__IO unsigned long long CORE0CCTLACC;   /* 0x48 Core 0 CCTL Access Line Register */
			__IO unsigned long long CORE1CCTLCMD;   /* 0x50 Core 1 CCTL Command Register */
			__IO unsigned long long CORE1CCTLACC;   /* 0x58 Core 1 CCTL Access Line Register */
			__IO unsigned long long CORE2CCTLCMD;   /* 0x60 Core 2 CCTL Command Register */
			__IO unsigned long long CORE2CCTLACC;   /* 0x68 Core 2 CCTL Access Line Register */
			__IO unsigned long long CORE3CCTLCMD;   /* 0x70 Core 3 CCTL Command Register */
			__IO unsigned long long CORE3CCTLACC;   /* 0x78 Core 3 CCTL Access Line Register */
			__I  unsigned long long CCTLSTATUS;     /* 0x80 CCTL Status Register */
			     unsigned long long RESERVED0;      /* 0x88 Reserved */
			__IO unsigned long long TGTWDATA[4];    /* 0x90 ~ 0xAF TGT Write Data 0 to 3 */
			__I  unsigned long long TGTRDATA[4];    /* 0xB0 ~ 0xCF TGT Read Data 0 to 3 */
			__IO unsigned long long TGTWECC;        /* 0xD0 TGT Write ECC Code Register */
			__I  unsigned long long TGTRECC;        /* 0xD8 TGT Read ECC Code Register */
			     unsigned long long RESERVED1[36];  /* 0xE0 ~ 0x1FF Reserved */
			__IO unsigned long long HPMCNT[32];     /* 0x200 ~ 0x2F8 HPM Counter Register 0 to 31 */
			__I  unsigned long long WAYMASK[16];    /* 0x300 ~ 0x378 Way Allocation Mask Register 0 to 15 */
		} REG;
		struct {
			__IO unsigned long long CCTLCMD;        /* 0x40 Core 0 CCTL Command Register */
			__IO unsigned long long CCTLACC;        /* 0x48 Core 0 CCTL Access Line Register */
			     unsigned long long RESERVED0[6];   /* 0x50 ~ 0x7F Reserved */
			__I  unsigned long long CCTLSTATUS;     /* 0x80 Core 0 CCTL Status Register */
			     unsigned long long RESERVED[503];  /* 0x88 Reserved */
		} CORECCTL[8];
	};
} L2C_RegDef;

// #define SMU_BASE                _IO_(0xF0100000)
#define SMU_BASE                (0xF0100000)
#define L2C_BASE                (0xE0500000)
#define AE350_SMU               ((SMU_RegDef *)  SMU_BASE)
#define AE350_L2C               ((L2C_RegDef *)  L2C_BASE)
#define DEV_SMU              AE350_SMU
#define DEV_L2C              AE350_L2C

/* CSR NDS_MCACHE_CTL */
#define DC_WARND_MSK                            (0x3 << 13)
#define DC_COHEN_MSK                            (0x1 << 19)
#define DC_COHSTA_MSK                           (0x1 << 20)

/* CSR NDS_DCM_CFG */
#define DSET_MSK                                ((1ULL << 2) | (1ULL << 1) | (1ULL << 0))
#define DWAY_MSK                                ((1ULL << 5) | (1ULL << 4) | (1ULL << 3))
#define DSIZE_MSK                               ((1ULL << 8) | (1ULL << 7) | (1ULL << 6))

/* SMU.SYSTEMCFG Configuration Register */
#define L2C_CTL_OFF                             8
#define L2C_CTL_MSK                             (0x1 << L2C_CTL_OFF)

/* Configuration Register */
#define L2C_SIZE_OFF                            7
#define L2C_SIZE_MSK                            (0x1F << L2C_SIZE_OFF)
#define L2C_SIZE_0KB                            (0x00 << L2C_SIZE_OFF)
#define L2C_SIZE_128KB                          (0x01 << L2C_SIZE_OFF)
#define L2C_SIZE_256KB                          (0x02 << L2C_SIZE_OFF)
#define L2C_SIZE_512KB                          (0x04 << L2C_SIZE_OFF)
#define L2C_SIZE_1024KB                         (0x08 << L2C_SIZE_OFF)
#define L2C_SIZE_2048KB                         (0x10 << L2C_SIZE_OFF)
#define L2C_LINE_SIZE                           32

/* L2 cache version */
#define L2C_VERSION_OFF                         24
#define L2C_VERSION_MSK                         (0xFF << L2C_VERSION_OFF)

/* Control Register */
#define L2C_ENABLE                              (0x1 << 0)
#define L2C_IPFDPT                              (0x3 << 3)
#define L2C_DPFDPT                              (0x3 << 5)

static inline void vec_calc_tiling_size_and_cache_config(uint32_t *cache_size_byte, uint32_t *tiling_size)
{
    /* NOTE: */
    /* Simulator cannot verify these code because of the CSRs below. */

    if(DEV_SMU->SYSTEMCFG & L2C_CTL_MSK) // CPU supports L2C cache
    {
        if ((DEV_L2C->CFG & L2C_SIZE_MSK) >> L2C_SIZE_OFF)
        {
            *cache_size_byte = (unsigned int)(((DEV_L2C->CFG & L2C_SIZE_MSK) >> L2C_SIZE_OFF) * 128) << 10;
        }
    }
    else if((__nds__mfsr(NDS_MDCM_CFG) >> 6) & 7) // if L1 cache exits.
    {
        // calculate cache_size
        unsigned int seth, set, way, line;
        seth = (__nds__mfsr(NDS_MDCM_CFG) >> 24) & 1;
        set = __nds__mfsr(NDS_MDCM_CFG) & 7;
        way = (__nds__mfsr(NDS_MDCM_CFG) >> 3) & 7;
        line = (__nds__mfsr(NDS_MDCM_CFG) >> 6) & 7;
        // calculate set value
        if (seth == 0)
        {
            set = 64 << set;
        }
        else if (seth == 1)
        {
            set = 32 >> set;
        }
        // calculate way value
        way++;
        // calculate line value
        if (line != 0)
        {
            line = 4 << line;
        }
        *cache_size_byte = set * way * line;
    }
    else
    {
        *cache_size_byte = 0;
    }

    //decide bsize aka. tiling_size
    int a;
    int tiling_size_arr[] = { 1,8,16,32,64,128,256,512,1024 };
    int max_tile_size_in_cache = sqrt(((*cache_size_byte) / 3) / sizeof(MM_TYPE));
    for (a = 1; a < sizeof(tiling_size_arr) / sizeof(tiling_size_arr[0]); a++)
    {
        if (tiling_size_arr[a] > max_tile_size_in_cache)
        {
            *tiling_size = tiling_size_arr[a - 1];
            break;
        }
    }
}

static inline void vec_calc_clx_tiling_size_and_cache_config(uint32_t *cache_size_byte, uint32_t *tiling_size)
{
    /* NOTE: */
    /* Simulator cannot verify these code because of the CSRs below. */

    if(DEV_SMU->SYSTEMCFG & L2C_CTL_MSK) // CPU supports L2C cache
    {
        if ((DEV_L2C->CFG & L2C_SIZE_MSK) >> L2C_SIZE_OFF)
        {
            *cache_size_byte = (unsigned int)(((DEV_L2C->CFG & L2C_SIZE_MSK) >> L2C_SIZE_OFF) * 128) << 10;
        }
    }
    else if((__nds__mfsr(NDS_MDCM_CFG) >> 6) & 7) // if L1 cache exits.
    {
        // calculate cache_size
        unsigned int seth, set, way, line;
        seth = (__nds__mfsr(NDS_MDCM_CFG) >> 24) & 1;
        set = __nds__mfsr(NDS_MDCM_CFG) & 7;
        way = (__nds__mfsr(NDS_MDCM_CFG) >> 3) & 7;
        line = (__nds__mfsr(NDS_MDCM_CFG) >> 6) & 7;
        // calculate set value
        if (seth == 0)
        {
            set = 64 << set;
        }
        else if (seth == 1)
        {
            set = 32 >> set;
        }
        // calculate way value
        way++;
        // calculate line value
        if (line != 0)
        {
            line = 4 << line;
        }
        *cache_size_byte = set * way * line;
    }
    else
    {
        *cache_size_byte = 0;
    }

    //decide bsize aka. tiling_size
    int a;
    int tiling_size_arr[] = { 1,8,16,32,64,128,256,512,1024 };
    int max_tile_size_in_cache = sqrt(((*cache_size_byte) / 3 / sizeof(MM_TYPE)) >> 1);
    for (a = 1; a < sizeof(tiling_size_arr) / sizeof(tiling_size_arr[0]); a++)
    {
        if (tiling_size_arr[a] > max_tile_size_in_cache)
        {
            *tiling_size = tiling_size_arr[a - 1];
            break;
        }
    }
    /*
    switch (cache_size)
    {
    case 1024:
        tiling_size = ;
        break;
    case 512:
        tiling_size = ;
        break;
    case 256:
        tiling_size = ;
        break;
    case 128:
        tiling_size = ;
        break;
    case 64:
        tiling_size = ;
        break;
    case 32:
        tiling_size = ;
        break;
    case 16:
        tiling_size = ;
        break;
    case 8:
        tiling_size = ;
        break;
    case 4:
        tiling_size = ;
        break;
    case 2:
        tiling_size = ;
        break;
    default:
        tiling_size = ;
        break;
    }
    */
}
#endif //ENA_DYNAMIC_CALC_CACHE_CONFIG
#endif //__INTERNAL_VEC_MAT_MUL_TILING_H__

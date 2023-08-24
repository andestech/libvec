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
#ifndef __INTERNAL_VEC_MAT_MUL_TILING_H__
#define __INTERNAL_VEC_MAT_MUL_TILING_H__

#ifdef ENA_DYNAMIC_CALC_CACHE_CONFIG
#include "nds_intrinsic.h"
#include <math.h>
#endif

#if defined(ENA_MAT_MUL_TILING)
#include "internal_vec_isa.h"

#ifndef ENA_MAT_MUL_PACKING
enum rvv_type{
    RVV_ORIGINAL,
    RVV_TILING
};
#else
enum rvv_type{
    RVV_ORIGINAL,
    RVV_TILING,
    RVV_PACKING
};
#endif

static inline void mat_mul_rvv_tiling(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, uint32_t row, uint32_t col, uint32_t col2, uint32_t tiling_size)
{

    //4*4 by 4*vl
    int row_4 = row >> 2;
    int max_row = row_4 << 2;

    const MM_TYPE* A = src1;
    const MM_TYPE* B = src2;
    const MM_TYPE* C = dst;
    int r, k, kk, cc, vl;
    int kk_tiling_size, cc_tiling_size, max_kk_tiling_size;
    const MM_TYPE* A11, * B11, \
                 * A21, * B21, \
                 * A31, * B31, \
                 * A41, * B41, \
                 * C1, * C2, * C3, * C4;
    for (kk = 0; kk < col; kk += tiling_size)
    {
        kk_tiling_size = ((kk + tiling_size) > col ? col : (kk + tiling_size));
        if ((kk + tiling_size) <= col)
        {
            max_kk_tiling_size = kk_tiling_size - (tiling_size & 3);
        }
        else
        {
            max_kk_tiling_size = col - ((col - kk) & 3);
        }
        for (cc = 0; cc < col2; cc += tiling_size)
        {
            cc_tiling_size = ((cc + tiling_size) > col2 ? col2 : (cc + tiling_size));
            for (r = 0; r < max_row; r += 4)
            {
                int mulLen = cc_tiling_size - cc;
                while (mulLen > 0)
                {
                    NDS_VEC_VSETVLI(vl, mulLen, DEF_VTYPE_SEW, NDS_VEC_VTYPE_LMUL_M4);
                    NDS_VEC_VAND_VI(NDS_VEC_V0, NDS_VEC_V0, 0); //sum
                    NDS_VEC_VAND_VI(NDS_VEC_V4, NDS_VEC_V4, 0); //sum
                    NDS_VEC_VAND_VI(NDS_VEC_V8, NDS_VEC_V8, 0); //sum
                    NDS_VEC_VAND_VI(NDS_VEC_V12, NDS_VEC_V12, 0); //sum

                    A11 = A + r * col + kk;
                    A21 = A11 + col;
                    A31 = A21 + col;
                    A41 = A31 + col;
                    B11 = B + kk * col2 + cc_tiling_size - mulLen;
                    B21 = B11 + col2;
                    B31 = B21 + col2;
                    B41 = B31 + col2;
                    C1 = C + r * col2 + cc_tiling_size - mulLen;
                    C2 = C1 + col2;
                    C3 = C2 + col2;
                    C4 = C3 + col2;
                    for (k = kk; k < max_kk_tiling_size; k += 4)
                    {
                        DEF_VLOAD_V(NDS_VEC_V16, B11);
                        DEF_VLOAD_V(NDS_VEC_V20, B21);
                        DEF_VLOAD_V(NDS_VEC_V24, B31);
                        DEF_VLOAD_V(NDS_VEC_V28, B41);
                        MM_TYPE tmpa11 = *A11++;
                        MM_TYPE tmpa21 = *A21++;
                        MM_TYPE tmpa31 = *A31++;
                        MM_TYPE tmpa41 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa21, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa31, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa41, NDS_VEC_V16);

                        MM_TYPE tmpa12 = *A11++;
                        MM_TYPE tmpa22 = *A21++;
                        MM_TYPE tmpa32 = *A31++;
                        MM_TYPE tmpa42 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa12, NDS_VEC_V20);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa22, NDS_VEC_V20);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa32, NDS_VEC_V20);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa42, NDS_VEC_V20);

                        MM_TYPE tmpa13 = *A11++;
                        MM_TYPE tmpa23 = *A21++;
                        MM_TYPE tmpa33 = *A31++;
                        MM_TYPE tmpa43 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa13, NDS_VEC_V24);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa23, NDS_VEC_V24);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa33, NDS_VEC_V24);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa43, NDS_VEC_V24);

                        MM_TYPE tmpa14 = *A11++;
                        MM_TYPE tmpa24 = *A21++;
                        MM_TYPE tmpa34 = *A31++;
                        MM_TYPE tmpa44 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa14, NDS_VEC_V28);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa24, NDS_VEC_V28);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa34, NDS_VEC_V28);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa44, NDS_VEC_V28);

                        B11 += 4 * col2;
                        B21 += 4 * col2;
                        B31 += 4 * col2;
                        B41 += 4 * col2;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        DEF_VLOAD_V(NDS_VEC_V16, B11);
                        MM_TYPE tmpa11 = *A11++;
                        MM_TYPE tmpa21 = *A21++;
                        MM_TYPE tmpa31 = *A31++;
                        MM_TYPE tmpa41 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa21, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa31, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa41, NDS_VEC_V16);

                        B11 += col2;
                    }
                    DEF_VLOAD_V(NDS_VEC_V16, C1);
                    DEF_VLOAD_V(NDS_VEC_V20, C2);
                    DEF_VLOAD_V(NDS_VEC_V24, C3);
                    DEF_VLOAD_V(NDS_VEC_V28, C4);

                    NDS_VEC_VFADD_VV(NDS_VEC_V0, NDS_VEC_V0, NDS_VEC_V16);
                    NDS_VEC_VFADD_VV(NDS_VEC_V4, NDS_VEC_V4, NDS_VEC_V20);
                    NDS_VEC_VFADD_VV(NDS_VEC_V8, NDS_VEC_V8, NDS_VEC_V24);
                    NDS_VEC_VFADD_VV(NDS_VEC_V12, NDS_VEC_V12, NDS_VEC_V28);

                    DEF_VSTORE_V(NDS_VEC_V0, C1);
                    DEF_VSTORE_V(NDS_VEC_V4, C2);
                    DEF_VSTORE_V(NDS_VEC_V8, C3);
                    DEF_VSTORE_V(NDS_VEC_V12, C4);
                    mulLen -= vl;
                }
            }
            for (; r < row; r++)
            {
                int mulLen = cc_tiling_size - cc;
                while (mulLen > 0)
                {
                    NDS_VEC_VSETVLI(vl, mulLen, DEF_VTYPE_SEW, NDS_VEC_VTYPE_LMUL_M4);
                    NDS_VEC_VAND_VI(NDS_VEC_V0, NDS_VEC_V0, 0); //sum
                    A11 = A + r * col + kk;
                    B11 = B + kk * col2 + cc_tiling_size - mulLen;
                    B21 = B11 + col2;
                    B31 = B21 + col2;
                    B41 = B31 + col2;
                    C1 = C + r * col2 + cc_tiling_size - mulLen;
                    for (k = kk; k < max_kk_tiling_size; k += 4)
                    {
                        DEF_VLOAD_V(NDS_VEC_V16, B11);
                        DEF_VLOAD_V(NDS_VEC_V20, B21);
                        DEF_VLOAD_V(NDS_VEC_V24, B31);
                        DEF_VLOAD_V(NDS_VEC_V28, B41);
                        MM_TYPE tmpa11 = *A11++;
                        MM_TYPE tmpa12 = *A11++;
                        MM_TYPE tmpa13 = *A11++;
                        MM_TYPE tmpa14 = *A11++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa12, NDS_VEC_V20);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa13, NDS_VEC_V24);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa14, NDS_VEC_V28);

                        B11 += 4 * col2;
                        B21 += 4 * col2;
                        B31 += 4 * col2;
                        B41 += 4 * col2;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        DEF_VLOAD_V(NDS_VEC_V20, B11);
                        MM_TYPE tmpa11 = *A11++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V20);
                        B11 += col2;
                    }
                    DEF_VLOAD_V(NDS_VEC_V16, C1);
                    NDS_VEC_VFADD_VV(NDS_VEC_V0, NDS_VEC_V0, NDS_VEC_V16);
                    DEF_VSTORE_V(NDS_VEC_V0, C1);
                    mulLen -= vl;
                }
            }
        }
    }
}

static inline void mat_mul_rvv_original(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, uint32_t row, uint32_t col, uint32_t col2)
{
    long curRow, curCol, vl;
    long col_4 = col >> 2;
    long res_col = col & 3;
    long row_8 = row >> 3;
    long max_row = row_8 << 3;

    for (curRow = 0; curRow < max_row; curRow += 8)
    {
        const MM_TYPE* C1 = dst + col2 * curRow;
        const MM_TYPE* C2 = C1 + col2;
        const MM_TYPE* C3 = C2 + col2;
        const MM_TYPE* C4 = C3 + col2;
        const MM_TYPE* C5 = C4 + col2;
        const MM_TYPE* C6 = C5 + col2;
        const MM_TYPE* C7 = C6 + col2;
        const MM_TYPE* C8 = C7 + col2;

        long mulLen = col2;
        while (mulLen > 0)
        {
            NDS_VEC_VSETVLI(vl, mulLen, DEF_VTYPE_SEW, NDS_VEC_VTYPE_LMUL_M2);
            NDS_VEC_VAND_VI(NDS_VEC_V0, NDS_VEC_V0, 0); //C
            NDS_VEC_VAND_VI(NDS_VEC_V2, NDS_VEC_V2, 0); //C
            NDS_VEC_VAND_VI(NDS_VEC_V4, NDS_VEC_V4, 0); //C
            NDS_VEC_VAND_VI(NDS_VEC_V6, NDS_VEC_V6, 0); //C
            NDS_VEC_VAND_VI(NDS_VEC_V8, NDS_VEC_V8, 0); //C
            NDS_VEC_VAND_VI(NDS_VEC_V10, NDS_VEC_V10, 0); //C
            NDS_VEC_VAND_VI(NDS_VEC_V12, NDS_VEC_V12, 0); //C
            NDS_VEC_VAND_VI(NDS_VEC_V14, NDS_VEC_V14, 0); //C
            const MM_TYPE* A1 = src1 + curRow * col;
            const MM_TYPE* A2 = A1 + col;
            const MM_TYPE* A3 = A2 + col;
            const MM_TYPE* A4 = A3 + col;
            const MM_TYPE* A5 = A4 + col;
            const MM_TYPE* A6 = A5 + col;
            const MM_TYPE* A7 = A6 + col;
            const MM_TYPE* A8 = A7 + col;
            const MM_TYPE* B0 = src2 + col2 - mulLen;
            const MM_TYPE* B1 = B0 + col2;
            const MM_TYPE* B2 = B1 + col2;
            const MM_TYPE* B3 = B2 + col2;

            for (curCol = 0; curCol < col_4; curCol++)
            {
                DEF_VLOAD_V(NDS_VEC_V16, B0);
                DEF_VLOAD_V(NDS_VEC_V18, B1);
                DEF_VLOAD_V(NDS_VEC_V20, B2);
                DEF_VLOAD_V(NDS_VEC_V22, B3);
                MM_TYPE tmpa00 = *A1++;
                MM_TYPE tmpa10 = *A2++;
                MM_TYPE tmpa20 = *A3++;
                MM_TYPE tmpa30 = *A4++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa00, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V2, tmpa10, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa20, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V6, tmpa30, NDS_VEC_V16);
                MM_TYPE tmpa40 = *A5++;
                MM_TYPE tmpa50 = *A6++;
                MM_TYPE tmpa60 = *A7++;
                MM_TYPE tmpa70 = *A8++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa40, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V10, tmpa50, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa60, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V14, tmpa70, NDS_VEC_V16);

                MM_TYPE tmpa01 = *A1++;
                MM_TYPE tmpa11 = *A2++;
                MM_TYPE tmpa21 = *A3++;
                MM_TYPE tmpa31 = *A4++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa01, NDS_VEC_V18);
                NDS_VEC_VFMACC_VF(NDS_VEC_V2, tmpa11, NDS_VEC_V18);
                NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa21, NDS_VEC_V18);
                NDS_VEC_VFMACC_VF(NDS_VEC_V6, tmpa31, NDS_VEC_V18);
                MM_TYPE tmpa41 = *A5++;
                MM_TYPE tmpa51 = *A6++;
                MM_TYPE tmpa61 = *A7++;
                MM_TYPE tmpa71 = *A8++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa41, NDS_VEC_V18);
                NDS_VEC_VFMACC_VF(NDS_VEC_V10, tmpa51, NDS_VEC_V18);
                NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa61, NDS_VEC_V18);
                NDS_VEC_VFMACC_VF(NDS_VEC_V14, tmpa71, NDS_VEC_V18);

                MM_TYPE tmpa02 = *A1++;
                MM_TYPE tmpa12 = *A2++;
                MM_TYPE tmpa22 = *A3++;
                MM_TYPE tmpa32 = *A4++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa02, NDS_VEC_V20);
                NDS_VEC_VFMACC_VF(NDS_VEC_V2, tmpa12, NDS_VEC_V20);
                NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa22, NDS_VEC_V20);
                NDS_VEC_VFMACC_VF(NDS_VEC_V6, tmpa32, NDS_VEC_V20);
                MM_TYPE tmpa42 = *A5++;
                MM_TYPE tmpa52 = *A6++;
                MM_TYPE tmpa62 = *A7++;
                MM_TYPE tmpa72 = *A8++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa42, NDS_VEC_V20);
                NDS_VEC_VFMACC_VF(NDS_VEC_V10, tmpa52, NDS_VEC_V20);
                NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa62, NDS_VEC_V20);
                NDS_VEC_VFMACC_VF(NDS_VEC_V14, tmpa72, NDS_VEC_V20);

                MM_TYPE tmpa03 = *A1++;
                MM_TYPE tmpa13 = *A2++;
                MM_TYPE tmpa23 = *A3++;
                MM_TYPE tmpa33 = *A4++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa03, NDS_VEC_V22);
                NDS_VEC_VFMACC_VF(NDS_VEC_V2, tmpa13, NDS_VEC_V22);
                NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa23, NDS_VEC_V22);
                NDS_VEC_VFMACC_VF(NDS_VEC_V6, tmpa33, NDS_VEC_V22);
                MM_TYPE tmpa43 = *A5++;
                MM_TYPE tmpa53 = *A6++;
                MM_TYPE tmpa63 = *A7++;
                MM_TYPE tmpa73 = *A8++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa43, NDS_VEC_V22);
                NDS_VEC_VFMACC_VF(NDS_VEC_V10, tmpa53, NDS_VEC_V22);
                NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa63, NDS_VEC_V22);
                NDS_VEC_VFMACC_VF(NDS_VEC_V14, tmpa73, NDS_VEC_V22);

                B0 += 4 * col2;
                B1 += 4 * col2;
                B2 += 4 * col2;
                B3 += 4 * col2;

            }
            for (curCol = 0; curCol < res_col; curCol++)
            {
                DEF_VLOAD_V(NDS_VEC_V16, B0);
                MM_TYPE tmpa00 = *A1++;
                MM_TYPE tmpa10 = *A2++;
                MM_TYPE tmpa20 = *A3++;
                MM_TYPE tmpa30 = *A4++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa00, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V2, tmpa10, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa20, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V6, tmpa30, NDS_VEC_V16);
                MM_TYPE tmpa40 = *A5++;
                MM_TYPE tmpa50 = *A6++;
                MM_TYPE tmpa60 = *A7++;
                MM_TYPE tmpa70 = *A8++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa40, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V10, tmpa50, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa60, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V14, tmpa70, NDS_VEC_V16);
                B0 += col2;
            }
            DEF_VSTORE_V(NDS_VEC_V0, C1);
            DEF_VSTORE_V(NDS_VEC_V2, C2);
            DEF_VSTORE_V(NDS_VEC_V4, C3);
            DEF_VSTORE_V(NDS_VEC_V6, C4);
            DEF_VSTORE_V(NDS_VEC_V8, C5);
            DEF_VSTORE_V(NDS_VEC_V10, C6);
            DEF_VSTORE_V(NDS_VEC_V12, C7);
            DEF_VSTORE_V(NDS_VEC_V14, C8);
            mulLen -= vl;
            C1 += vl;
            C2 += vl;
            C3 += vl;
            C4 += vl;
            C5 += vl;
            C6 += vl;
            C7 += vl;
            C8 += vl;
        }
    }
    for (; curRow < row; curRow++)
    {
        const MM_TYPE* C1 = dst + col2 * curRow;
        long mulLen = col2;
        while (mulLen > 0)
        {
            NDS_VEC_VSETVLI(vl, mulLen, DEF_VTYPE_SEW, NDS_VEC_VTYPE_LMUL_M2);
            NDS_VEC_VAND_VI(NDS_VEC_V0, NDS_VEC_V0, 0); //C
            const MM_TYPE* A1 = src1 + curRow * col;
            const MM_TYPE* B0 = src2 + col2 - mulLen;
            const MM_TYPE* B1 = B0 + col2;
            const MM_TYPE* B2 = B1 + col2;
            const MM_TYPE* B3 = B2 + col2;

            for (curCol = 0; curCol < col_4; curCol++)
            {
                DEF_VLOAD_V(NDS_VEC_V16, B0);
                DEF_VLOAD_V(NDS_VEC_V18, B1);
                DEF_VLOAD_V(NDS_VEC_V20, B2);
                DEF_VLOAD_V(NDS_VEC_V22, B3);
                MM_TYPE tmpa00 = *A1++;
                MM_TYPE tmpa01 = *A1++;
                MM_TYPE tmpa02 = *A1++;
                MM_TYPE tmpa03 = *A1++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa00, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa01, NDS_VEC_V18);
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa02, NDS_VEC_V20);
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa03, NDS_VEC_V22);
                B0 += 4 * col2;
                B1 += 4 * col2;
                B2 += 4 * col2;
                B3 += 4 * col2;
            }
            for (curCol = 0; curCol < res_col; curCol++)
            {
                DEF_VLOAD_V(NDS_VEC_V16, B0);
                MM_TYPE tmpa00 = *A1++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa00, NDS_VEC_V16);
                B0 += col2;
            }
            DEF_VSTORE_V(NDS_VEC_V0, C1);
            mulLen -= vl;
            C1 += vl;
        }
    }
}

static inline void gemm_rvv_tiling(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, MM_TYPE alpha, uint32_t row, uint32_t col, uint32_t col2, uint32_t tiling_size)
{
    //4*2 by 2*vl
    int row_4 = row >> 2;
    int max_row = row_4 << 2;

    const MM_TYPE* A = src1;
    const MM_TYPE* B = src2;
    const MM_TYPE* C = dst;
    int r, k, kk, cc, vl;
    int kk_tiling_size, cc_tiling_size, max_kk_tiling_size;
    const MM_TYPE* A11, * B11, \
                 * A21, * B21, \
                 * A31, \
                 * A41, \
                 * C1, * C2, * C3, * C4;
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
            for (r = 0; r < max_row; r += 4)
            {
                int mulLen = cc_tiling_size - cc;
                while (mulLen > 0)
                {
                    NDS_VEC_VSETVLI(vl, mulLen, DEF_VTYPE_SEW, NDS_VEC_VTYPE_LMUL_M4);
                    NDS_VEC_VAND_VI(NDS_VEC_V0, NDS_VEC_V0, 0); //sum
                    NDS_VEC_VAND_VI(NDS_VEC_V4, NDS_VEC_V4, 0); //sum
                    NDS_VEC_VAND_VI(NDS_VEC_V8, NDS_VEC_V8, 0); //sum
                    NDS_VEC_VAND_VI(NDS_VEC_V12, NDS_VEC_V12, 0); //sum

                    A11 = A + r * col + kk;
                    A21 = A11 + col;
                    A31 = A21 + col;
                    A41 = A31 + col;
                    B11 = B + kk * col2 + cc_tiling_size - mulLen;
                    B21 = B11 + col2;
                    C1 = C + r * col2 + cc_tiling_size - mulLen;
                    C2 = C1 + col2;
                    C3 = C2 + col2;
                    C4 = C3 + col2;
                    for (k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        DEF_VLOAD_V(NDS_VEC_V16, B11);
                        DEF_VLOAD_V(NDS_VEC_V20, B21);
                        MM_TYPE tmpa11 = *A11++;
                        MM_TYPE tmpa21 = *A21++;
                        MM_TYPE tmpa31 = *A31++;
                        MM_TYPE tmpa41 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa21, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa31, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa41, NDS_VEC_V16);

                        MM_TYPE tmpa12 = *A11++;
                        MM_TYPE tmpa22 = *A21++;
                        MM_TYPE tmpa32 = *A31++;
                        MM_TYPE tmpa42 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa12, NDS_VEC_V20);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa22, NDS_VEC_V20);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa32, NDS_VEC_V20);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa42, NDS_VEC_V20);

                        B11 += 2 * col2;
                        B21 += 2 * col2;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        DEF_VLOAD_V(NDS_VEC_V16, B11);
                        MM_TYPE tmpa11 = *A11++;
                        MM_TYPE tmpa21 = *A21++;
                        MM_TYPE tmpa31 = *A31++;
                        MM_TYPE tmpa41 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa21, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa31, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa41, NDS_VEC_V16);

                        B11 += col2;
                    }
                    DEF_VLOAD_V(NDS_VEC_V16, C1);
                    DEF_VLOAD_V(NDS_VEC_V20, C2);
                    DEF_VLOAD_V(NDS_VEC_V24, C3);
                    DEF_VLOAD_V(NDS_VEC_V28, C4);

                    NDS_VEC_VFMACC_VF(NDS_VEC_V16, alpha, NDS_VEC_V0);
                    NDS_VEC_VFMACC_VF(NDS_VEC_V20, alpha, NDS_VEC_V4);
                    NDS_VEC_VFMACC_VF(NDS_VEC_V24, alpha, NDS_VEC_V8);
                    NDS_VEC_VFMACC_VF(NDS_VEC_V28, alpha, NDS_VEC_V12);

                    DEF_VSTORE_V(NDS_VEC_V16, C1);
                    DEF_VSTORE_V(NDS_VEC_V20, C2);
                    DEF_VSTORE_V(NDS_VEC_V24, C3);
                    DEF_VSTORE_V(NDS_VEC_V28, C4);
                    mulLen -= vl;
                }
            }
            for (; r < row; r++)
            {
                int mulLen = cc_tiling_size - cc;
                while (mulLen > 0)
                {
                    NDS_VEC_VSETVLI(vl, mulLen, DEF_VTYPE_SEW, NDS_VEC_VTYPE_LMUL_M4);
                    NDS_VEC_VAND_VI(NDS_VEC_V0, NDS_VEC_V0, 0); //sum
                    A11 = A + r * col + kk;
                    B11 = B + kk * col2 + cc_tiling_size - mulLen;
                    B21 = B11 + col2;
                    C1 = C + r * col2 + cc_tiling_size - mulLen;
                    for (k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        DEF_VLOAD_V(NDS_VEC_V16, B11);
                        DEF_VLOAD_V(NDS_VEC_V20, B21);
                        MM_TYPE tmpa11 = *A11++;
                        MM_TYPE tmpa12 = *A11++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa12, NDS_VEC_V20);

                        B11 += 2 * col2;
                        B21 += 2 * col2;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        DEF_VLOAD_V(NDS_VEC_V20, B11);
                        MM_TYPE tmpa11 = *A11++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V20);
                        B11 += col2;
                    }
                    DEF_VLOAD_V(NDS_VEC_V16, C1);
                    NDS_VEC_VFMACC_VF(NDS_VEC_V16, alpha, NDS_VEC_V0);
                    DEF_VSTORE_V(NDS_VEC_V16, C1);
                    mulLen -= vl;
                }
            }
        }
    }
}

static inline void gemm_rvv_original(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, MM_TYPE alpha, MM_TYPE beta, uint32_t row, uint32_t col, uint32_t col2)
{
    long curRow, curCol, vl;
    long col_4 = col >> 2;
    long res_col = col & 3;
    long row_8 = row >> 3;
    long max_row = row_8 << 3;

    for (curRow = 0; curRow < max_row; curRow += 8)
    {
        const MM_TYPE* C1 = dst + col2 * curRow;
        const MM_TYPE* C2 = C1 + col2;
        const MM_TYPE* C3 = C2 + col2;
        const MM_TYPE* C4 = C3 + col2;
        const MM_TYPE* C5 = C4 + col2;
        const MM_TYPE* C6 = C5 + col2;
        const MM_TYPE* C7 = C6 + col2;
        const MM_TYPE* C8 = C7 + col2;

        long mulLen = col2;
        while (mulLen > 0)
        {
            NDS_VEC_VSETVLI(vl, mulLen, DEF_VTYPE_SEW, NDS_VEC_VTYPE_LMUL_M2);
            NDS_VEC_VAND_VI(NDS_VEC_V0, NDS_VEC_V0, 0); //C
            NDS_VEC_VAND_VI(NDS_VEC_V2, NDS_VEC_V2, 0); //C
            NDS_VEC_VAND_VI(NDS_VEC_V4, NDS_VEC_V4, 0); //C
            NDS_VEC_VAND_VI(NDS_VEC_V6, NDS_VEC_V6, 0); //C
            NDS_VEC_VAND_VI(NDS_VEC_V8, NDS_VEC_V8, 0); //C
            NDS_VEC_VAND_VI(NDS_VEC_V10, NDS_VEC_V10, 0); //C
            NDS_VEC_VAND_VI(NDS_VEC_V12, NDS_VEC_V12, 0); //C
            NDS_VEC_VAND_VI(NDS_VEC_V14, NDS_VEC_V14, 0); //C
            const MM_TYPE* A1 = src1 + curRow * col;
            const MM_TYPE* A2 = A1 + col;
            const MM_TYPE* A3 = A2 + col;
            const MM_TYPE* A4 = A3 + col;
            const MM_TYPE* A5 = A4 + col;
            const MM_TYPE* A6 = A5 + col;
            const MM_TYPE* A7 = A6 + col;
            const MM_TYPE* A8 = A7 + col;
            const MM_TYPE* B0 = src2 + col2 - mulLen;
            const MM_TYPE* B1 = B0 + col2;
            const MM_TYPE* B2 = B1 + col2;
            const MM_TYPE* B3 = B2 + col2;

            for (curCol = 0; curCol < col_4; curCol++)
            {
                DEF_VLOAD_V(NDS_VEC_V16, B0);
                DEF_VLOAD_V(NDS_VEC_V18, B1);
                DEF_VLOAD_V(NDS_VEC_V20, B2);
                DEF_VLOAD_V(NDS_VEC_V22, B3);
                MM_TYPE tmpa00 = *A1++;
                MM_TYPE tmpa10 = *A2++;
                MM_TYPE tmpa20 = *A3++;
                MM_TYPE tmpa30 = *A4++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa00, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V2, tmpa10, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa20, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V6, tmpa30, NDS_VEC_V16);
                MM_TYPE tmpa40 = *A5++;
                MM_TYPE tmpa50 = *A6++;
                MM_TYPE tmpa60 = *A7++;
                MM_TYPE tmpa70 = *A8++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa40, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V10, tmpa50, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa60, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V14, tmpa70, NDS_VEC_V16);

                MM_TYPE tmpa01 = *A1++;
                MM_TYPE tmpa11 = *A2++;
                MM_TYPE tmpa21 = *A3++;
                MM_TYPE tmpa31 = *A4++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa01, NDS_VEC_V18);
                NDS_VEC_VFMACC_VF(NDS_VEC_V2, tmpa11, NDS_VEC_V18);
                NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa21, NDS_VEC_V18);
                NDS_VEC_VFMACC_VF(NDS_VEC_V6, tmpa31, NDS_VEC_V18);
                MM_TYPE tmpa41 = *A5++;
                MM_TYPE tmpa51 = *A6++;
                MM_TYPE tmpa61 = *A7++;
                MM_TYPE tmpa71 = *A8++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa41, NDS_VEC_V18);
                NDS_VEC_VFMACC_VF(NDS_VEC_V10, tmpa51, NDS_VEC_V18);
                NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa61, NDS_VEC_V18);
                NDS_VEC_VFMACC_VF(NDS_VEC_V14, tmpa71, NDS_VEC_V18);

                MM_TYPE tmpa02 = *A1++;
                MM_TYPE tmpa12 = *A2++;
                MM_TYPE tmpa22 = *A3++;
                MM_TYPE tmpa32 = *A4++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa02, NDS_VEC_V20);
                NDS_VEC_VFMACC_VF(NDS_VEC_V2, tmpa12, NDS_VEC_V20);
                NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa22, NDS_VEC_V20);
                NDS_VEC_VFMACC_VF(NDS_VEC_V6, tmpa32, NDS_VEC_V20);
                MM_TYPE tmpa42 = *A5++;
                MM_TYPE tmpa52 = *A6++;
                MM_TYPE tmpa62 = *A7++;
                MM_TYPE tmpa72 = *A8++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa42, NDS_VEC_V20);
                NDS_VEC_VFMACC_VF(NDS_VEC_V10, tmpa52, NDS_VEC_V20);
                NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa62, NDS_VEC_V20);
                NDS_VEC_VFMACC_VF(NDS_VEC_V14, tmpa72, NDS_VEC_V20);

                MM_TYPE tmpa03 = *A1++;
                MM_TYPE tmpa13 = *A2++;
                MM_TYPE tmpa23 = *A3++;
                MM_TYPE tmpa33 = *A4++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa03, NDS_VEC_V22);
                NDS_VEC_VFMACC_VF(NDS_VEC_V2, tmpa13, NDS_VEC_V22);
                NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa23, NDS_VEC_V22);
                NDS_VEC_VFMACC_VF(NDS_VEC_V6, tmpa33, NDS_VEC_V22);
                MM_TYPE tmpa43 = *A5++;
                MM_TYPE tmpa53 = *A6++;
                MM_TYPE tmpa63 = *A7++;
                MM_TYPE tmpa73 = *A8++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa43, NDS_VEC_V22);
                NDS_VEC_VFMACC_VF(NDS_VEC_V10, tmpa53, NDS_VEC_V22);
                NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa63, NDS_VEC_V22);
                NDS_VEC_VFMACC_VF(NDS_VEC_V14, tmpa73, NDS_VEC_V22);

                B0 += 4 * col2;
                B1 += 4 * col2;
                B2 += 4 * col2;
                B3 += 4 * col2;

            }
            for (curCol = 0; curCol < res_col; curCol++)
            {
                DEF_VLOAD_V(NDS_VEC_V16, B0);
                MM_TYPE tmpa00 = *A1++;
                MM_TYPE tmpa10 = *A2++;
                MM_TYPE tmpa20 = *A3++;
                MM_TYPE tmpa30 = *A4++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa00, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V2, tmpa10, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa20, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V6, tmpa30, NDS_VEC_V16);
                MM_TYPE tmpa40 = *A5++;
                MM_TYPE tmpa50 = *A6++;
                MM_TYPE tmpa60 = *A7++;
                MM_TYPE tmpa70 = *A8++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa40, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V10, tmpa50, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa60, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V14, tmpa70, NDS_VEC_V16);
                B0 += col2;
            }
            DEF_VLOAD_V(NDS_VEC_V16, C1);
            DEF_VLOAD_V(NDS_VEC_V18, C2);
            DEF_VLOAD_V(NDS_VEC_V20, C3);
            DEF_VLOAD_V(NDS_VEC_V22, C4);
            DEF_VLOAD_V(NDS_VEC_V24, C5);
            DEF_VLOAD_V(NDS_VEC_V26, C6);
            DEF_VLOAD_V(NDS_VEC_V28, C7);
            DEF_VLOAD_V(NDS_VEC_V30, C8);
            NDS_VEC_VFMUL_VF(NDS_VEC_V0, NDS_VEC_V0, alpha);
            NDS_VEC_VFMUL_VF(NDS_VEC_V2, NDS_VEC_V2, alpha);
            NDS_VEC_VFMUL_VF(NDS_VEC_V4, NDS_VEC_V4, alpha);
            NDS_VEC_VFMUL_VF(NDS_VEC_V6, NDS_VEC_V6, alpha);
            NDS_VEC_VFMUL_VF(NDS_VEC_V8, NDS_VEC_V8, alpha);
            NDS_VEC_VFMUL_VF(NDS_VEC_V10, NDS_VEC_V10, alpha);
            NDS_VEC_VFMUL_VF(NDS_VEC_V12, NDS_VEC_V12, alpha);
            NDS_VEC_VFMUL_VF(NDS_VEC_V14, NDS_VEC_V14, alpha);
            NDS_VEC_VFMACC_VF(NDS_VEC_V0, beta, NDS_VEC_V16);
            NDS_VEC_VFMACC_VF(NDS_VEC_V2, beta, NDS_VEC_V18);
            NDS_VEC_VFMACC_VF(NDS_VEC_V4, beta, NDS_VEC_V20);
            NDS_VEC_VFMACC_VF(NDS_VEC_V6, beta, NDS_VEC_V22);
            NDS_VEC_VFMACC_VF(NDS_VEC_V8, beta, NDS_VEC_V24);
            NDS_VEC_VFMACC_VF(NDS_VEC_V10, beta, NDS_VEC_V26);
            NDS_VEC_VFMACC_VF(NDS_VEC_V12, beta, NDS_VEC_V28);
            NDS_VEC_VFMACC_VF(NDS_VEC_V14, beta, NDS_VEC_V30);
            DEF_VSTORE_V(NDS_VEC_V0, C1);
            DEF_VSTORE_V(NDS_VEC_V2, C2);
            DEF_VSTORE_V(NDS_VEC_V4, C3);
            DEF_VSTORE_V(NDS_VEC_V6, C4);
            DEF_VSTORE_V(NDS_VEC_V8, C5);
            DEF_VSTORE_V(NDS_VEC_V10, C6);
            DEF_VSTORE_V(NDS_VEC_V12, C7);
            DEF_VSTORE_V(NDS_VEC_V14, C8);
            mulLen -= vl;
            C1 += vl;
            C2 += vl;
            C3 += vl;
            C4 += vl;
            C5 += vl;
            C6 += vl;
            C7 += vl;
            C8 += vl;
        }
    }
    for (; curRow < row; curRow++)
    {
        const MM_TYPE* C1 = dst + col2 * curRow;
        long mulLen = col2;
        while (mulLen > 0)
        {
            NDS_VEC_VSETVLI(vl, mulLen, DEF_VTYPE_SEW, NDS_VEC_VTYPE_LMUL_M2);
            NDS_VEC_VAND_VI(NDS_VEC_V0, NDS_VEC_V0, 0); //C
            const MM_TYPE* A1 = src1 + curRow * col;
            const MM_TYPE* B0 = src2 + col2 - mulLen;
            const MM_TYPE* B1 = B0 + col2;
            const MM_TYPE* B2 = B1 + col2;
            const MM_TYPE* B3 = B2 + col2;

            for (curCol = 0; curCol < col_4; curCol++)
            {
                DEF_VLOAD_V(NDS_VEC_V16, B0);
                DEF_VLOAD_V(NDS_VEC_V18, B1);
                DEF_VLOAD_V(NDS_VEC_V20, B2);
                DEF_VLOAD_V(NDS_VEC_V22, B3);
                MM_TYPE tmpa00 = *A1++;
                MM_TYPE tmpa01 = *A1++;
                MM_TYPE tmpa02 = *A1++;
                MM_TYPE tmpa03 = *A1++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa00, NDS_VEC_V16);
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa01, NDS_VEC_V18);
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa02, NDS_VEC_V20);
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa03, NDS_VEC_V22);
                B0 += 4 * col2;
                B1 += 4 * col2;
                B2 += 4 * col2;
                B3 += 4 * col2;
            }
            for (curCol = 0; curCol < res_col; curCol++)
            {
                DEF_VLOAD_V(NDS_VEC_V16, B0);
                MM_TYPE tmpa00 = *A1++;
                NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa00, NDS_VEC_V16);
                B0 += col2;
            }
            DEF_VLOAD_V(NDS_VEC_V18, C1);
            NDS_VEC_VFMUL_VF(NDS_VEC_V2, NDS_VEC_V0, alpha);
            NDS_VEC_VFMACC_VF(NDS_VEC_V2, beta, NDS_VEC_V18);
            DEF_VSTORE_V(NDS_VEC_V2, C1);
            mulLen -= vl;
            C1 += vl;
        }
    }
}

static inline void mat_mul_rvv_changeable_tiling(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, uint32_t row, uint32_t col, uint32_t col2, uint32_t col_tiling_size, uint32_t col2_tiling_size)
{
    //4*2 by 2*vl
    int row_4 = row >> 2;
    int max_row = row_4 << 2;

    const MM_TYPE* A = src1;
    const MM_TYPE* B = src2;
    const MM_TYPE* C = dst;
    int r, k, kk, cc, vl;
    int kk_tiling_size, cc_tiling_size, max_kk_tiling_size;
    const MM_TYPE* A11, * B11, \
                 * A21, * B21, \
                 * A31, \
                 * A41, \
                 * C1, * C2, * C3, * C4;
    for (kk = 0; kk < col; kk += col_tiling_size)
    {
        kk_tiling_size = ((kk + col_tiling_size) > col ? col : (kk + col_tiling_size));
        if ((kk + col_tiling_size) <= col)
        {
            max_kk_tiling_size = kk_tiling_size - (col_tiling_size & 1);
        }
        else
        {
            max_kk_tiling_size = col - ((col - kk) & 1);
        }
        for (cc = 0; cc < col2; cc += col2_tiling_size)
        {
            cc_tiling_size = ((cc + col2_tiling_size) > col2 ? col2 : (cc + col2_tiling_size));
            for (r = 0; r < max_row; r += 4)
            {
                int mulLen = cc_tiling_size - cc;
                while (mulLen > 0)
                {
                    NDS_VEC_VSETVLI(vl, mulLen, DEF_VTYPE_SEW, NDS_VEC_VTYPE_LMUL_M4);
                    NDS_VEC_VAND_VI(NDS_VEC_V0, NDS_VEC_V0, 0); //sum
                    NDS_VEC_VAND_VI(NDS_VEC_V4, NDS_VEC_V4, 0); //sum
                    NDS_VEC_VAND_VI(NDS_VEC_V8, NDS_VEC_V8, 0); //sum
                    NDS_VEC_VAND_VI(NDS_VEC_V12, NDS_VEC_V12, 0); //sum

                    A11 = A + r * col + kk;
                    A21 = A11 + col;
                    A31 = A21 + col;
                    A41 = A31 + col;
                    B11 = B + kk * col2 + cc_tiling_size - mulLen;
                    B21 = B11 + col2;
                    C1 = C + r * col2 + cc_tiling_size - mulLen;
                    C2 = C1 + col2;
                    C3 = C2 + col2;
                    C4 = C3 + col2;
                    for (k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        DEF_VLOAD_V(NDS_VEC_V16, B11);
                        DEF_VLOAD_V(NDS_VEC_V20, B21);
                        MM_TYPE tmpa11 = *A11++;
                        MM_TYPE tmpa21 = *A21++;
                        MM_TYPE tmpa31 = *A31++;
                        MM_TYPE tmpa41 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa21, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa31, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa41, NDS_VEC_V16);

                        MM_TYPE tmpa12 = *A11++;
                        MM_TYPE tmpa22 = *A21++;
                        MM_TYPE tmpa32 = *A31++;
                        MM_TYPE tmpa42 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa12, NDS_VEC_V20);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa22, NDS_VEC_V20);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa32, NDS_VEC_V20);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa42, NDS_VEC_V20);

                        B11 += 2 * col2;
                        B21 += 2 * col2;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        DEF_VLOAD_V(NDS_VEC_V16, B11);
                        MM_TYPE tmpa11 = *A11++;
                        MM_TYPE tmpa21 = *A21++;
                        MM_TYPE tmpa31 = *A31++;
                        MM_TYPE tmpa41 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa21, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa31, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa41, NDS_VEC_V16);

                        B11 += col2;
                    }
                    DEF_VLOAD_V(NDS_VEC_V16, C1);
                    DEF_VLOAD_V(NDS_VEC_V20, C2);
                    DEF_VLOAD_V(NDS_VEC_V24, C3);
                    DEF_VLOAD_V(NDS_VEC_V28, C4);

                    NDS_VEC_VFADD_VV(NDS_VEC_V0, NDS_VEC_V0, NDS_VEC_V16);
                    NDS_VEC_VFADD_VV(NDS_VEC_V4, NDS_VEC_V4, NDS_VEC_V20);
                    NDS_VEC_VFADD_VV(NDS_VEC_V8, NDS_VEC_V8, NDS_VEC_V24);
                    NDS_VEC_VFADD_VV(NDS_VEC_V12, NDS_VEC_V12, NDS_VEC_V28);

                    DEF_VSTORE_V(NDS_VEC_V0, C1);
                    DEF_VSTORE_V(NDS_VEC_V4, C2);
                    DEF_VSTORE_V(NDS_VEC_V8, C3);
                    DEF_VSTORE_V(NDS_VEC_V12, C4);
                    mulLen -= vl;
                }
            }
            for (; r < row; r++)
            {
                int mulLen = cc_tiling_size - cc;
                while (mulLen > 0)
                {
                    NDS_VEC_VSETVLI(vl, mulLen, DEF_VTYPE_SEW, NDS_VEC_VTYPE_LMUL_M4);
                    NDS_VEC_VAND_VI(NDS_VEC_V0, NDS_VEC_V0, 0); //sum
                    A11 = A + r * col + kk;
                    B11 = B + kk * col2 + cc_tiling_size - mulLen;
                    B21 = B11 + col2;
                    C1 = C + r * col2 + cc_tiling_size - mulLen;
                    for (k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        DEF_VLOAD_V(NDS_VEC_V16, B11);
                        DEF_VLOAD_V(NDS_VEC_V20, B21);
                        MM_TYPE tmpa11 = *A11++;
                        MM_TYPE tmpa12 = *A11++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa12, NDS_VEC_V20);

                        B11 += 2 * col2;
                        B21 += 2 * col2;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        DEF_VLOAD_V(NDS_VEC_V20, B11);
                        MM_TYPE tmpa11 = *A11++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V20);
                        B11 += col2;
                    }
                    DEF_VLOAD_V(NDS_VEC_V16, C1);
                    NDS_VEC_VFADD_VV(NDS_VEC_V0, NDS_VEC_V0, NDS_VEC_V16);
                    DEF_VSTORE_V(NDS_VEC_V0, C1);
                    mulLen -= vl;
                }
            }
        }
    }
}

static inline unsigned int higher_power_2(unsigned int temp)
{
    temp--;
    temp |= temp >> 1;
    temp |= temp >> 2;
    temp |= temp >> 4;
    temp |= temp >> 8;
    temp |= temp >> 16;
    temp++;
    return temp;
}

#ifdef ENA_MAT_MUL_PACKING
#define min(a,b) (a) < (b) ? (a) : (b)
static inline int mat_mul_rvv_packing(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, uint32_t row, uint32_t col, uint32_t col2, uint32_t tiling_size)
{
    //4*4 by 4*vl and packing B
    int row_4 = row >> 2;
    int max_row = row_4 << 2;

    const MM_TYPE* A = src1;
    const MM_TYPE* B = src2;
    const MM_TYPE* C = dst;
    int r, k, kk, cc, vl;
    int kk_tiling_size, cc_tiling_size, max_kk_tiling_size;
    const MM_TYPE* A11, * B11, \
                 * A21, * B21, \
                 * A31, * B31, \
                 * A41, * B41, \
                 * C1, * C2, * C3, * C4;
    //packB
    int arrayk, arrayj, i, j;
    //MM_TYPE packB[tiling_size*tiling_size];
    MM_TYPE *packB;
    if((packB = (MM_TYPE*)malloc(sizeof(MM_TYPE) * tiling_size * tiling_size)) == NULL)
    {
        //fprintf(stderr, "packB out of memory.\n");
        return RISCV_VEC_FAIL;
    }
    for (kk = 0; kk < col; kk += tiling_size)
    {
        //row of packB
        arrayk = min(col - kk, tiling_size);

        kk_tiling_size = ((kk + tiling_size) > col ? col : (kk + tiling_size));
        if ((kk + tiling_size) <= col)
        {
            max_kk_tiling_size = kk_tiling_size - (tiling_size & 3);
        }
        else
        {
            max_kk_tiling_size = col - ((col - kk) & 3);
        }
        for (cc = 0; cc < col2; cc += tiling_size)
        {
            //col of packB
            arrayj = min(col2 - cc, tiling_size);
            for (i = 0; i < arrayk; i++)
            {
                for (j = 0; j < arrayj; j++)
                {
                    packB[i * arrayj + j] = *(src2 + (kk + i) * col2 + (cc + j));
                }
            }
            B = packB;

            cc_tiling_size = ((cc + tiling_size) > col2 ? col2 : (cc + tiling_size));
            for (r = 0; r < max_row; r += 4)
            {
                int mulLen = cc_tiling_size - cc;
                while (mulLen > 0)
                {
                    NDS_VEC_VSETVLI(vl, mulLen, DEF_VTYPE_SEW, NDS_VEC_VTYPE_LMUL_M4);
                    NDS_VEC_VAND_VI(NDS_VEC_V0, NDS_VEC_V0, 0); //sum
                    NDS_VEC_VAND_VI(NDS_VEC_V4, NDS_VEC_V4, 0); //sum
                    NDS_VEC_VAND_VI(NDS_VEC_V8, NDS_VEC_V8, 0); //sum
                    NDS_VEC_VAND_VI(NDS_VEC_V12, NDS_VEC_V12, 0); //sum

                    A11 = A + r * col + kk;
                    A21 = A11 + col;
                    A31 = A21 + col;
                    A41 = A31 + col;
                    B11 = B + ((cc_tiling_size - cc) - mulLen);
                    B21 = B11 + arrayj;
                    B31 = B21 + arrayj;
                    B41 = B31 + arrayj;
                    C1 = C + r * col2 + cc_tiling_size - mulLen;
                    C2 = C1 + col2;
                    C3 = C2 + col2;
                    C4 = C3 + col2;
                    for (k = kk; k < max_kk_tiling_size; k += 4)
                    {
                        DEF_VLOAD_V(NDS_VEC_V16, B11);
                        DEF_VLOAD_V(NDS_VEC_V20, B21);
                        DEF_VLOAD_V(NDS_VEC_V24, B31);
                        DEF_VLOAD_V(NDS_VEC_V28, B41);
                        MM_TYPE tmpa11 = *A11++;
                        MM_TYPE tmpa21 = *A21++;
                        MM_TYPE tmpa31 = *A31++;
                        MM_TYPE tmpa41 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa21, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa31, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa41, NDS_VEC_V16);

                        MM_TYPE tmpa12 = *A11++;
                        MM_TYPE tmpa22 = *A21++;
                        MM_TYPE tmpa32 = *A31++;
                        MM_TYPE tmpa42 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa12, NDS_VEC_V20);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa22, NDS_VEC_V20);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa32, NDS_VEC_V20);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa42, NDS_VEC_V20);

                        MM_TYPE tmpa13 = *A11++;
                        MM_TYPE tmpa23 = *A21++;
                        MM_TYPE tmpa33 = *A31++;
                        MM_TYPE tmpa43 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa13, NDS_VEC_V24);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa23, NDS_VEC_V24);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa33, NDS_VEC_V24);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa43, NDS_VEC_V24);

                        MM_TYPE tmpa14 = *A11++;
                        MM_TYPE tmpa24 = *A21++;
                        MM_TYPE tmpa34 = *A31++;
                        MM_TYPE tmpa44 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa14, NDS_VEC_V28);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa24, NDS_VEC_V28);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa34, NDS_VEC_V28);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa44, NDS_VEC_V28);

                        B11 += 4 * arrayj;
                        B21 += 4 * arrayj;
                        B31 += 4 * arrayj;
                        B41 += 4 * arrayj;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        DEF_VLOAD_V(NDS_VEC_V16, B11);
                        MM_TYPE tmpa11 = *A11++;
                        MM_TYPE tmpa21 = *A21++;
                        MM_TYPE tmpa31 = *A31++;
                        MM_TYPE tmpa41 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa21, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa31, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa41, NDS_VEC_V16);

                        B11 += arrayj;
                    }
                    DEF_VLOAD_V(NDS_VEC_V16, C1);
                    DEF_VLOAD_V(NDS_VEC_V20, C2);
                    DEF_VLOAD_V(NDS_VEC_V24, C3);
                    DEF_VLOAD_V(NDS_VEC_V28, C4);

                    NDS_VEC_VFADD_VV(NDS_VEC_V0, NDS_VEC_V0, NDS_VEC_V16);
                    NDS_VEC_VFADD_VV(NDS_VEC_V4, NDS_VEC_V4, NDS_VEC_V20);
                    NDS_VEC_VFADD_VV(NDS_VEC_V8, NDS_VEC_V8, NDS_VEC_V24);
                    NDS_VEC_VFADD_VV(NDS_VEC_V12, NDS_VEC_V12, NDS_VEC_V28);

                    DEF_VSTORE_V(NDS_VEC_V0, C1);
                    DEF_VSTORE_V(NDS_VEC_V4, C2);
                    DEF_VSTORE_V(NDS_VEC_V8, C3);
                    DEF_VSTORE_V(NDS_VEC_V12, C4);
                    mulLen -= vl;
                }
            }
            for (; r < row; r++)
            {
                int mulLen = cc_tiling_size - cc;
                while (mulLen > 0)
                {
                    NDS_VEC_VSETVLI(vl, mulLen, DEF_VTYPE_SEW, NDS_VEC_VTYPE_LMUL_M4);
                    NDS_VEC_VAND_VI(NDS_VEC_V0, NDS_VEC_V0, 0); //sum
                    A11 = A + r * col + kk;
                    B11 = B + ((cc_tiling_size - cc) - mulLen);
                    B21 = B11 + arrayj;
                    B31 = B21 + arrayj;
                    B41 = B31 + arrayj;
                    C1 = C + r * col2 + cc_tiling_size - mulLen;
                    for (k = kk; k < max_kk_tiling_size; k += 4)
                    {
                        DEF_VLOAD_V(NDS_VEC_V16, B11);
                        DEF_VLOAD_V(NDS_VEC_V20, B21);
                        DEF_VLOAD_V(NDS_VEC_V24, B31);
                        DEF_VLOAD_V(NDS_VEC_V28, B41);
                        MM_TYPE tmpa11 = *A11++;
                        MM_TYPE tmpa12 = *A11++;
                        MM_TYPE tmpa13 = *A11++;
                        MM_TYPE tmpa14 = *A11++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa12, NDS_VEC_V20);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa13, NDS_VEC_V24);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa14, NDS_VEC_V28);

                        B11 += 4 * arrayj;
                        B21 += 4 * arrayj;
                        B31 += 4 * arrayj;
                        B41 += 4 * arrayj;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        DEF_VLOAD_V(NDS_VEC_V20, B11);
                        MM_TYPE tmpa11 = *A11++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V20);
                        B11 += arrayj;
                    }
                    DEF_VLOAD_V(NDS_VEC_V16, C1);
                    NDS_VEC_VFADD_VV(NDS_VEC_V0, NDS_VEC_V0, NDS_VEC_V16);
                    DEF_VSTORE_V(NDS_VEC_V0, C1);
                    mulLen -= vl;
                }
            }
        }
    }
    free(packB);
    return RISCV_VEC_OK;
}

static inline int gemm_rvv_packing(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, MM_TYPE alpha, uint32_t row, uint32_t col, uint32_t col2, uint32_t tiling_size)
{
    //4*2 by 2*vl and packing B
    int row_4 = row >> 2;
    int max_row = row_4 << 2;

    const MM_TYPE* A = src1;
    const MM_TYPE* B = src2;
    const MM_TYPE* C = dst;
    int r, k, kk, cc, vl;
    int kk_tiling_size, cc_tiling_size, max_kk_tiling_size;
    const MM_TYPE* A11, * B11, \
                 * A21, * B21, \
                 * A31, \
                 * A41, \
                 * C1, * C2, * C3, * C4;
    //packB
    int arrayk, arrayj, i, j;
    //MM_TYPE packB[tiling_size*tiling_size];
    MM_TYPE *packB;
    if((packB = (MM_TYPE*)malloc(sizeof(MM_TYPE) * tiling_size * tiling_size)) == NULL)
    {
        //fprintf(stderr, "packB out of memory.\n");
        return RISCV_VEC_FAIL;
    }
    for (kk = 0; kk < col; kk += tiling_size)
    {
        //row of packB
        arrayk = min(col - kk, tiling_size);

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
            //col of packB
            arrayj = min(col2 - cc, tiling_size);
            for (i = 0; i < arrayk; i++)
            {
                for (j = 0; j < arrayj; j++)
                {
                    packB[i * arrayj + j] = *(src2 + (kk + i) * col2 + (cc + j));
                }
            }
            B = packB;

            cc_tiling_size = ((cc + tiling_size) > col2 ? col2 : (cc + tiling_size));
            for (r = 0; r < max_row; r += 4)
            {
                int mulLen = cc_tiling_size - cc;
                while (mulLen > 0)
                {
                    NDS_VEC_VSETVLI(vl, mulLen, DEF_VTYPE_SEW, NDS_VEC_VTYPE_LMUL_M4);
                    NDS_VEC_VAND_VI(NDS_VEC_V0, NDS_VEC_V0, 0); //sum
                    NDS_VEC_VAND_VI(NDS_VEC_V4, NDS_VEC_V4, 0); //sum
                    NDS_VEC_VAND_VI(NDS_VEC_V8, NDS_VEC_V8, 0); //sum
                    NDS_VEC_VAND_VI(NDS_VEC_V12, NDS_VEC_V12, 0); //sum

                    A11 = A + r * col + kk;
                    A21 = A11 + col;
                    A31 = A21 + col;
                    A41 = A31 + col;
                    B11 = B + ((cc_tiling_size - cc) - mulLen);
                    B21 = B11 + arrayj;
                    C1 = C + r * col2 + cc_tiling_size - mulLen;
                    C2 = C1 + col2;
                    C3 = C2 + col2;
                    C4 = C3 + col2;
                    for (k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        DEF_VLOAD_V(NDS_VEC_V16, B11);
                        DEF_VLOAD_V(NDS_VEC_V20, B21);
                        MM_TYPE tmpa11 = *A11++;
                        MM_TYPE tmpa21 = *A21++;
                        MM_TYPE tmpa31 = *A31++;
                        MM_TYPE tmpa41 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa21, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa31, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa41, NDS_VEC_V16);

                        MM_TYPE tmpa12 = *A11++;
                        MM_TYPE tmpa22 = *A21++;
                        MM_TYPE tmpa32 = *A31++;
                        MM_TYPE tmpa42 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa12, NDS_VEC_V20);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa22, NDS_VEC_V20);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa32, NDS_VEC_V20);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa42, NDS_VEC_V20);

                        B11 += 2 * arrayj;
                        B21 += 2 * arrayj;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        DEF_VLOAD_V(NDS_VEC_V16, B11);
                        MM_TYPE tmpa11 = *A11++;
                        MM_TYPE tmpa21 = *A21++;
                        MM_TYPE tmpa31 = *A31++;
                        MM_TYPE tmpa41 = *A41++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V4, tmpa21, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V8, tmpa31, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V12, tmpa41, NDS_VEC_V16);

                        B11 += arrayj;
                    }
                    DEF_VLOAD_V(NDS_VEC_V16, C1);
                    DEF_VLOAD_V(NDS_VEC_V20, C2);
                    DEF_VLOAD_V(NDS_VEC_V24, C3);
                    DEF_VLOAD_V(NDS_VEC_V28, C4);

                    NDS_VEC_VFMACC_VF(NDS_VEC_V16, alpha, NDS_VEC_V0);
                    NDS_VEC_VFMACC_VF(NDS_VEC_V20, alpha, NDS_VEC_V4);
                    NDS_VEC_VFMACC_VF(NDS_VEC_V24, alpha, NDS_VEC_V8);
                    NDS_VEC_VFMACC_VF(NDS_VEC_V28, alpha, NDS_VEC_V12);

                    DEF_VSTORE_V(NDS_VEC_V16, C1);
                    DEF_VSTORE_V(NDS_VEC_V20, C2);
                    DEF_VSTORE_V(NDS_VEC_V24, C3);
                    DEF_VSTORE_V(NDS_VEC_V28, C4);
                    mulLen -= vl;
                }
            }
            for (; r < row; r++)
            {
                int mulLen = cc_tiling_size - cc;
                while (mulLen > 0)
                {
                    NDS_VEC_VSETVLI(vl, mulLen, DEF_VTYPE_SEW, NDS_VEC_VTYPE_LMUL_M4);
                    NDS_VEC_VAND_VI(NDS_VEC_V0, NDS_VEC_V0, 0); //sum
                    A11 = A + r * col + kk;
                    B11 = B + ((cc_tiling_size - cc) - mulLen);
                    B21 = B11 + arrayj;
                    C1 = C + r * col2 + cc_tiling_size - mulLen;
                    for (k = kk; k < max_kk_tiling_size; k += 2)
                    {
                        DEF_VLOAD_V(NDS_VEC_V16, B11);
                        DEF_VLOAD_V(NDS_VEC_V20, B21);
                        MM_TYPE tmpa11 = *A11++;
                        MM_TYPE tmpa12 = *A11++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V16);
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa12, NDS_VEC_V20);

                        B11 += 2 * arrayj;
                        B21 += 2 * arrayj;
                    }
                    for (; k < kk_tiling_size; k++)
                    {
                        DEF_VLOAD_V(NDS_VEC_V20, B11);
                        MM_TYPE tmpa11 = *A11++;
                        NDS_VEC_VFMACC_VF(NDS_VEC_V0, tmpa11, NDS_VEC_V20);
                        B11 += arrayj;
                    }
                    DEF_VLOAD_V(NDS_VEC_V16, C1);
                    NDS_VEC_VFMACC_VF(NDS_VEC_V16, alpha, NDS_VEC_V0);
                    DEF_VSTORE_V(NDS_VEC_V16, C1);
                    mulLen -= vl;
                }
            }
        }
    }
    free(packB);
    return RISCV_VEC_OK;
}
#endif

#else

#ifdef ENA_PUREC_TILING

static inline void mat_mul_purec_tiling(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, uint32_t row, uint32_t col, uint32_t col2, uint32_t tiling_size)
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

#else

static inline void mat_mul_purec_original(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, uint32_t row, uint32_t col, uint32_t col2)
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

static inline void gemm_purec_original(const MM_TYPE* src1, const MM_TYPE* src2, MM_TYPE* dst, MM_TYPE alpha, MM_TYPE beta, uint32_t row, uint32_t col, uint32_t col2)
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

#endif //ENA_PUREC_TILING

#endif //vector

#ifdef ENA_DYNAMIC_CALC_CACHE_CONFIG
static inline void calc_tiling_size_and_cache_config(uint32_t *set, uint32_t *way, uint32_t *line, uint32_t *tiling_size)
{
    //calculate cache_size
    unsigned int seth, cache_size;
    seth = (__nds__mfsr(NDS_MDCM_CFG) >> 24) & 1;
    *set = __nds__mfsr(NDS_MDCM_CFG) & 7;
    *way = (__nds__mfsr(NDS_MDCM_CFG) >> 3) & 7;
    *line = (__nds__mfsr(NDS_MDCM_CFG) >> 6) & 7;
    // calculate set value
    if (seth == 0)
    {
        *set = 64 << *set;
    }
    else if (seth == 1)
    {
        *set = 32 >> *set;
    }
    // calculate way value
    (*way)++;
    // calculate line value
    if (*line != 0)
    {
        *line = 4 << *line;
    }
    cache_size = (*set * *way * *line) >> 10;

    //decide bsize aka. tiling_size
    int a;
    int tiling_size_arr[] = { 1,8,16,32,64,128,256,512,1024 };
    int max_tile_size_in_cache = sqrt(((cache_size) << 10) / 3 / sizeof(MM_TYPE));
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
        tiling_size = 256;
        break;
    case 512:
        tiling_size = 128;
        break;
    case 256:
        tiling_size = 128;
        break;
    case 128:
        tiling_size = 64;
        break;
    case 64:
        tiling_size = 64;
        break;
    case 32:
        tiling_size = 32;
        break;
    case 16:
        tiling_size = 32;
        break;
    case 8:
        tiling_size = 16;
        break;
    case 4:
        tiling_size = 16;
        break;
    case 2:
        tiling_size = 8;
        break;
    default:
        tiling_size = 1;
        break;
    }
    */
}
#endif //ENA_DYNAMIC_CALC_CACHE_CONFIG
#endif //__INTERNAL_VEC_MAT_MUL_TILING_H__

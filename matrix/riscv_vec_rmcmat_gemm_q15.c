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

#include "internal_nds_types.h"

typedef union
{
    struct
    {
        q15_t re;
        q15_t im;
    };
} UNION_CLX_T;

/* function description */
int riscv_vec_rmcmat_gemm_q15(const q15_t * FUNC_RESTRICT src1, const q15_t * FUNC_RESTRICT src2, q15_t * FUNC_RESTRICT dst,
                             const q15_t alpha, const q15_t beta,
                             uint32_t row, uint32_t col, uint32_t col2, int32_t shift)
{
    /**
     * complex_gemm_q15
     * The function is implemented using an internal 64-bit accumulator.
     * The complex inputs(real and image) to the multiplications are in 1.15 format, and the multiplications yield a 2.30 result.
     * The 2.30 intermediate results are accumulated in a 64-bit accumulator in 34.30 format.
     * This approach provides 33 guard bits, ensuring there is no risk of overflow.
     * The 34.30 accumulated result is multiplied by a 1.15 (alpha), and the final output is saturated into 1.15 format.
     */
    int shift1 = 30 + shift;
    uint32_t j;
    const q15_t *Aend = src1 + (row * col * 2);
    q63_t sum_re;
    q63_t sum_im;
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
                sum_re += (q63_t)a.re * b.re - (q63_t)a.im * b.im;
                sum_im += (q63_t)a.re * b.im + (q63_t)a.im * b.re;
                B += (col2);
            }
            while (A != A2);

            q63_t temp = (q63_t)((((q63_t)*dst * beta) << 15) + (q63_t)alpha * sum_re);// (q30 << 15) + q45
            *dst = NDS_ISA_SATS((temp >> shift1), 16u);
            dst++;
            temp = (q63_t)((((q63_t)*dst * beta) << 15) + (q63_t)alpha * sum_im);// (q30 << 15) + q45
            *dst = NDS_ISA_SATS((temp >> shift1), 16u);
            dst++;
            B = (UNION_CLX_T *)(src2 + 2 * (col2 - (--j)));
        }
        while (j != 0u);

        src1 += (col * 2);
    }
    while (src1 != Aend);
    return NDSV_OK;
}




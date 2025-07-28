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

/* function description */
int riscv_vec_rmmat_gemm_q7(const q7_t * FUNC_RESTRICT src1, const q7_t * FUNC_RESTRICT src2, q7_t * FUNC_RESTRICT dst,
                             const q7_t alpha, const q7_t beta,
                             uint32_t row, uint32_t col, uint32_t col2, int32_t shift)
{
    /**
     * gemm_q7
     * The function is implemented using an internal 32-bit accumulator.
     * The inputs to the multiplications are in 1.7 format, and the multiplications yield a 2.14 result.
     * The 2.14 intermediate results are accumulated in a 32-bit accumulator in 18.14 format.
     * This approach provides 17 guard bits, ensuring there is no risk of overflow.
     * The 18.14 accumulated result is multiplied by a 1.7 (alpha), and the final output is saturated into 1.7 format.
     */
    int shift1 = 14 + shift;
    uint32_t j;
    const q7_t *Aend = src1 + row * col;
    do
    {
        const q7_t *B = src2;

        j = col2;
        do
        {
            const q7_t *A = src1;
            const q7_t *A2 = A + col;
            q31_t sum = 0;

            do
            {
                sum += (q31_t) (*A++) * (*B);
                B += col2;
            }
            while (A != A2);

            q63_t temp = (q63_t)((((q63_t)*dst * beta) << 7) + (q63_t)alpha * sum);// (q14 << 7) + q21
            *dst = NDS_ISA_SATS((temp >> shift1), 8u);
            dst++;
            B = src2 + col2 - (--j);
        }
        while (j != 0);

        src1 += col;
    }
    while (src1 != Aend);
    return NDSV_OK;
}




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
void riscv_vec_rmmat_gemv_q7(const q7_t * FUNC_RESTRICT src1, const q7_t * FUNC_RESTRICT src2, q7_t * FUNC_RESTRICT dst, const q7_t alpha, const q7_t beta, uint32_t row, uint32_t col, int32_t shift)
{
    int shift1 = 14 + shift;
    const q7_t *InA = src1;
    q31_t sum;
    uint32_t colcnt, rowcnt;

    rowcnt = row;
    do
    {
        const q7_t *A = InA;
        const q7_t *B = src2;

        sum = 0;

        /* column loop */
        colcnt = col;
        do
        {
            sum += (q31_t)(*A++) * *B;
            B++;
            colcnt--;
        }
        while (colcnt != 0u);

        q63_t temp = (((q63_t)*dst * beta) << 7) + (q63_t)alpha * sum;// (q14 << 7) + q21
        *dst = NDS_ISA_SATS((q31_t)(temp >> shift1), 8u);
        dst++;
        InA = InA + col;
        rowcnt--;
    }
    while (rowcnt != 0u);
}

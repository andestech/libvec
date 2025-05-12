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

extern void riscv_vec_sqrt_q15(q15_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size);

/* function description */
q15_t riscv_vec_std_u8(const uint8_t * FUNC_RESTRICT src, uint32_t size)
{
    q31_t sum = 0;
    q31_t sos = 0;
    q31_t meansqr, sqrmean;
    q31_t tmp1;
    q15_t dst;
    uint32_t i = size;

    while (i != 0u)
    {
        tmp1 = *src++;
        //Only V3M does not have mulsr64 inst.
        //Using this instr. can save one cycle.
        sos += (q31_t) tmp1 * tmp1;
        sum += tmp1;
        i--;
    }

    // To improve accuracy, replace the original with the following code.
    meansqr = sos / (size - 1);
    sqrmean = (q31_t)(((q63_t) sum * sum) / (q63_t)(size * (size - 1)));
    tmp1 = (q15_t) NDS_ISA_SATS(((meansqr - sqrmean) >> 1), 16u);

    riscv_vec_sqrt_q15((q15_t *)&tmp1, &dst, 1);

    return dst;
}

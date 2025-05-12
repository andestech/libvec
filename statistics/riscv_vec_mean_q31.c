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
q31_t riscv_vec_mean_q31(const q31_t * FUNC_RESTRICT src, uint32_t size)
{
    union64_t sum;
    q63_t dst;
    sum.d0 = 0;
    uint32_t i = size;

    while (i != 0u)
    {
        q31_t sign, carry, val;
        uint32_t temp;

        //  If V3_O3 and unrolling, the insts as well as cycles are 6891/7139, and the performance is better than Keil.(Keil's cycle for CM3_O3 is 7892.)
        val = *src++;
        sign  = val >> 31;
        temp =  sum.b32.uw0 + (uint32_t) val;
        carry = (temp < sum.b32.uw0) ? 1 : 0;
        sum.b32.uw0 = temp;
        sum.b32.w1 += carry;
        sum.b32.w1 += sign;
        i--;
    }
    dst = (sum.d0 / size);
    return dst;
}

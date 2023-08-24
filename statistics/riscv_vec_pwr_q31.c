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

/** @file*/

//// Vector Power (Vector-Vector) Functions

#include "internal_nds_types.h"

/* function description */
q63_t riscv_vec_pwr_q31(const q31_t * FUNC_RESTRICT src, uint32_t size)
{
    NDSV_CHECK_DstSrc1Src2;
#ifdef NDS_CPU_64
    q63_t sum = 0;
    q31_t tmp;
    while (size != 0u)
    {
        tmp = *src++;
        sum += ((q63_t)tmp * tmp) >> 14u;
        size--;
    }

    return sum;
#else
    union64_t sum, temp;
    q31_t val;
    q31_t carry;
    uint32_t i = size;
    sum.d0 = 0;
    while (i > 0)
    {
        i--;
        val = *src++;
        temp.d0 = (q63_t) val * val;
        temp.d0 >>= 14;
        sum.b32.uw0 += temp.b32.uw0;
        //use slt instruction to check overflow.
        carry = (sum.b32.uw0 < temp.b32.uw0) ? 1 : 0;
        sum.b32.w1 += carry;
        sum.b32.w1 += temp.b32.w1;
    }

    return sum.d0;
#endif
}

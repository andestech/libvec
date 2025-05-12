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

/** @file*/

//// Vector Multiplication (Vector-Vector) Functions
#include "internal_nds_types.h"

int riscv_vec_mul_q15(q15_t * dst, q15_t * src1, q15_t * src2, uint32_t count)
{
    NDSV_CHECK_DstSrc1Src2;
    q15_t in11, in12;
    q15_t in21, in22;
    uint32_t rest_cnt;

    rest_cnt = count & 1;
    count -= rest_cnt;

    while (count != 0u)
    {

        in11 = *src1++;
        in12 = *src1++;
        in21 = *src2++;
        in22 = *src2++;

        *dst++ = (q15_t) NDS_ISA_KHMBB((in11), (in21));
        *dst++ = (q15_t) NDS_ISA_KHMBB((in12), (in22));

        count -= 2;
    }

    if (rest_cnt != 0u)
    {
        *dst++ = (q15_t) NDS_ISA_KHMBB((*src1++), (*src2++));
    }
    return NDSV_OK;
}

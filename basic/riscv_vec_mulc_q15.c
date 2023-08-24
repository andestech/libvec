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

//// Vector Multiplication (Vector-Scalar) Functions
#include "internal_nds_types.h"

int riscv_vec_mulc_q15(q15_t * dst, q15_t * src, const q15_t scale, const int8_t shift, uint32_t count)
{
    NDSV_CHECK_DstSrcCst;
    const int8_t shift1 = 15 - shift;
    q15_t in11, in12;
    uint32_t rest_cnt;
    rest_cnt = count & 1;
    count -= rest_cnt;
    while (count != 0u)
    {
        in11 = *src++;
        in12 = *src++;
        *dst++ = (q15_t) NDS_ISA_SATS(((q31_t) (in11) * scale) >> shift1, 16u);
        *dst++ = (q15_t) NDS_ISA_SATS(((q31_t) (in12) * scale) >> shift1, 16u);
        

        count -= 2;
    }
    if (rest_cnt != 0u)
    {
        *dst++ = (q15_t) NDS_ISA_SATS(((q31_t) (*src++) * scale) >> shift1, 16u);
        
    }
    return NDSV_OK;
}

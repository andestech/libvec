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

#include "internal_vec_utils.h"

static inline q15_t convert_q31_to_q15(q31_t x)
{
    return (q15_t)(x >> 16);
}

void riscv_vec_convert_q31_q15(q31_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size)
{
    uint32_t size2 = size >> 1;
    uint32_t rest_count = size & 1UL;
    while (size2 != 0u)
    {
        q31_t tempa, tempb;
        tempa = *src++;
        tempb = *src++;
        tempa = convert_q31_to_q15(tempa);
        tempb = convert_q31_to_q15(tempb);
        uint32_t out = (uint16_t)tempb << 16 | (uint16_t)tempa;
        *(uint32_t*)dst = out;
        dst += 2;
        size2--;
    }
    while (rest_count != 0)
    {
        *dst++ = convert_q31_to_q15(*src++);
        rest_count--;
    }
}

/**
 * @} end of convertq31
 */
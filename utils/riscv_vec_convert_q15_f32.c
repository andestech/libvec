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

#include "internal_vec_utils.h"

#define UNIT_Q15    (1.0f/32768.0f)

static inline float convert_q15_to_float(q15_t x)
{
    return x * UNIT_Q15;
}

void riscv_vec_convert_q15_f32(q15_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    while (size != 0u)
    {
        *dst++ = convert_q15_to_float(*src++);
        size--;
    }
}

/**
 * @} end of convertq15
 */

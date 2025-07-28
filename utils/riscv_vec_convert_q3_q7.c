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

void riscv_vec_convert_q3_q7(q7_t * FUNC_RESTRICT src, q7_t * FUNC_RESTRICT dst, uint32_t size)
{
    uint32_t packed_size = size >> 1;
    uint32_t size2 = size & 1;

    while (packed_size != 0u)
    {
        q7_t src44 = *src++;
        q7_t a0 = (q7_t)(src44 << 4);
        q7_t a1 = src44 & 0xF0;

        *dst++ = a0;
        *dst++ = a1;
        packed_size--;
    }
    if(size2)
    {
        q7_t src44 = *src;
        q7_t a0 = (q7_t)(src44 << 4);
        *dst++ = a0;
    }
}

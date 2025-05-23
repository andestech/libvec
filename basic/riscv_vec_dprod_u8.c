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
uint32_t riscv_vec_dprod_u8(u8_t * src1, u8_t * src2, uint32_t count)
{
    uint32_t sum = 0;
    uint32_t size2 = count >> 1;
    uint32_t rest_count = count & 1;

    while (size2 != 0)
    {
        sum += (uint32_t)(*src1++) * (*src2++);
        sum += (uint32_t)(*src1++) * (*src2++);
        size2 --;
    }

    if (rest_count != 0)
    {
        sum += (uint32_t)(*src1++) * (*src2++);
    }
    return (sum);
}

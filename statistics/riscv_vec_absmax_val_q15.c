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
q15_t riscv_vec_absmax_val_q15(const q15_t * FUNC_RESTRICT src, uint32_t size)
{
    //First, check if input size is zero?
    if (size == 0)
    {
        return (q15_t)0x80;
    }
    q15_t max_val, temp_val;
    uint32_t i;

    if(*src == (q15_t)0x8000)
    {
        max_val = (q15_t)0x7fff;
    }
    else
    {
        max_val = ((*src ^ (*src >> 15)) - (*src >> 15));
    }
    src++;

    i = 0U;
    while (i < (size - 1))
    {
        if(*src == (q15_t)0x8000)
        {
            temp_val = (q15_t)0x7fff;
        }
        else
        {
            temp_val = ((*src ^ (*src >> 15)) - (*src >> 15));
        }
        src++;
        i++;
        if (max_val < temp_val)
        {
            max_val = temp_val;
        }
    }

    return max_val;
}

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
q31_t riscv_vec_max_q31(const q31_t * FUNC_RESTRICT src, uint32_t size, uint32_t * FUNC_RESTRICT index)
{
    //First, check if input size is zero?
    if (size <= 0)
    {
        *index = 0;
        return (q31_t)0x80000000;
    }
    q31_t max_val, temp_val;
    uint32_t max_index, i;

    i = max_index = 0U;
    max_val = *src++;

    while (i < (size - 1))
    {
        temp_val = *src++;
        i++;
        if (max_val < temp_val)
        {
            max_index = i;
            max_val = temp_val;
        }
    }

    *index = max_index;
    return max_val;
}

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
float32_t riscv_vec_min_f32(const float32_t * FUNC_RESTRICT src, uint32_t size, uint32_t * FUNC_RESTRICT index)
{
    //First, check if input size is zero?
    if (size <= 0)
    {
        *index = 0;
        return (float32_t) VEC_F32_MAX;
    }
    float32_t min_val, temp_val;
    uint32_t min_index, i;

    i = min_index = 0;
    min_val = *src++;
    uint32_t cnt = (size - 1) >> 2;
    while (cnt != 0)
    {
        temp_val = *src++;
        if (temp_val < min_val)
        {
            min_index = i + 1;
            min_val = temp_val;
        }
        temp_val = *src++;
        if (temp_val < min_val)
        {
            min_index = i + 2;
            min_val = temp_val;
        }
        temp_val = *src++;
        if (temp_val < min_val)
        {
            min_index = i + 3;
            min_val = temp_val;
        }
        temp_val = *src++;
        if (temp_val < min_val)
        {
            min_index = i + 4;
            min_val = temp_val;
        }
        i += 4;
        cnt--;
    }
    uint32_t rest_cnt =  (size - 1) & 3;
    while (rest_cnt != 0)
    {
        temp_val = *src++;
        if (temp_val < min_val)
        {
            min_index = i + 1;
            min_val = temp_val;
        }
        i++;
        rest_cnt--;
    }
    *index = min_index;
    return min_val;
}

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

#include "internal_nds_types.h"
#include <math.h> //fabs

/* function description */
float64_t riscv_vec_absmin_val_f64(const float64_t * FUNC_RESTRICT src, uint32_t size)
{
    //First, check if input size is zero?
    if (size <= 0)
    {
        return (float64_t) DBL_MAX;
    }
    float64_t min_val, temp_val;
    float64_t temp_val2, temp_val3, temp_val4;

    uint32_t cnt = (size - 1) >> 2;
    min_val = fabs(*src++);

    while (cnt != 0)
    {
        temp_val = fabs(*src++);
        temp_val2 = fabs(*src++);
        temp_val3 = fabs(*src++);
        temp_val4 = fabs(*src++);
        if (min_val > temp_val)
        {
            min_val = temp_val;
        }
        if (min_val > temp_val2)
        {
            min_val = temp_val2;
        }
        if (min_val > temp_val3)
        {
            min_val = temp_val3;
        }
        if (min_val > temp_val4)
        {
            min_val = temp_val4;
        }
        cnt--;
    }
    uint32_t rest_cnt =  (size - 1) & 3;
    while (rest_cnt != 0)
    {
        temp_val = fabs(*src++);
        if (min_val > temp_val)
        {
            min_val = temp_val;
        }
        rest_cnt--;
    }
    return min_val;
}

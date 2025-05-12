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
float16_t riscv_vec_mean_f16(const float16_t * FUNC_RESTRICT src, uint32_t size)
{
    float16_t dst = (float16_t)0.0;
    if (size <= 0)
    {
        return dst;
    }
    float16_t sum = (float16_t)0.0;
    uint32_t i = size;

    do
    {
        sum += *src++;
    }
    while (--i);
    //printf("sum is: %f\n", (float)sum);
    dst = (float16_t)sum / (float16_t)size;

    return dst;
}

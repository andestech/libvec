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

/* function description */
void riscv_vec_mse_f32(const float32_t * FUNC_RESTRICT src1, const float32_t * FUNC_RESTRICT src2, uint32_t size, float32_t * result)
{
    uint32_t loop = size;
    float32_t tmp1, tmp2;
    float32_t sum = 0.0f;

    while (loop != 0u)
    {
        tmp1 = *src1++;
        tmp2 = *src2++;
        tmp1 = tmp1 - tmp2;
        sum += tmp1 * tmp1;
        loop--;
    }
    *result = sum / size;
}
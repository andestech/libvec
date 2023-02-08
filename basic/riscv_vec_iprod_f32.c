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
float32_t riscv_vec_iprod_f32(float32_t * src1, float32_t * src2, uint32_t count)
{
    float32_t sum = 0.0f;

    while (count != 0u)
    {
        /* y = x1[0] * x2[0] + x1[1] * x2[1] + x1[2] * x2[2]
         ** ..... + x1[count-1] * x2[count-1] */
        sum += (*src1++) * (*src2++);
        count--;
    }

    return (sum);
}

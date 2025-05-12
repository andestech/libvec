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
#include <math.h>       //for sqrtf

/* function description */
void riscv_vec_sqrt_f32(float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    while(size > 0)
    {
        float32_t y = *src;
        y = (y >= 0.0f) ? sqrtf(y) : -1.0f;
        *dst = y;
        size--;
        src++;
        dst++;
    }

    return;
}

/**
 * @} end of utsqrt
 */

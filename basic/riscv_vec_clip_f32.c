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

/** @file*/

//// Vector Clip Functions

#include "internal_nds_types.h"

int riscv_vec_clip_f32(float32_t * dst, float32_t * src, const float32_t low, const float32_t high, uint32_t count)
{
    NDSV_CHECK_DstSrcCst;

    float32_t input, output;
    while (count != 0u)
    {
        input = *src++;
        output = input;
        if (input > high)
            output = high;
        if (input < low)
            output = low;
        *dst++ = output;
        count--;
    }

    return NDSV_OK;
}

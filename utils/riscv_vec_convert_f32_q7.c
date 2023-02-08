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

#include "internal_vec_utils.h"

void riscv_vec_convert_f32_q7(float32_t * FUNC_RESTRICT src, q7_t * FUNC_RESTRICT dst, uint32_t size)
{
    float32_t in;
    q31_t rst;
    while (size != 0u)
    {
        in = *src++;
        in *= (float32_t)(128.0);

        if (in >= (float32_t)0)
            in += (float32_t)0.5;
        else
            in -= (float32_t)0.5;

        rst = (q31_t)in;
        *dst++ = (q7_t) NDS_ISA_SATS(rst, 8u);
        size--;
    }
}

/**
 * @} end of convertf32
 */

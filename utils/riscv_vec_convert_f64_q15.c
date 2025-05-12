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



void riscv_vec_convert_f64_q15(float64_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size)
{
    const float64_t CONSTF_COE0 = 32768.0;
    const float64_t CONSTF_0 = 0.0f;
    const float64_t CONSTF_0_5 = 0.5f;
    float64_t in;
    q31_t rst;
    long_t loop = size;

    while (loop > 0)
    {
        in = *src++;
        in *= CONSTF_COE0;
        in += in > CONSTF_0 ? CONSTF_0_5 : -CONSTF_0_5;
        rst = (q31_t)in;
        *dst++ = (q15_t) NDS_ISA_SATS(rst, 16u);
        loop--;
    }
}

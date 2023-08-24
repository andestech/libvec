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

/** @file*/

//// Vector Power (Vector-Vector) Functions
#include "internal_nds_types.h"

/* function description */
float64_t riscv_vec_pwr_f64(const float64_t * FUNC_RESTRICT src, uint32_t size)
{
    NDSV_CHECK_DstSrc1Src2;
    float64_t sum = 0.0f, tmp;
    while (size != 0u)
    {
        tmp = *src++;
        sum += tmp * tmp;
        size--;
    }
    return sum;
}

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
#include "internal_vec_utils.h"

/* function description */
void riscv_vec_cmag_hp_q15(q15_t * dst, q15_t * src, uint32_t size)
{
    q15_t a, b;
    do
    {
        /* y = sqrt(x[(2 * n) + 0] ^ 2 + x[(2 * n) + 1] ^ 2) */
        a = *src++;
        b = *src++;
        /* store the output in 2.14 format */
        q31_t acc0, acc1;
        acc0 = ((q31_t) a * a);
        acc1 = ((q31_t) b * b);
        q31_t out = vec_sqrt_q31((acc0 + acc1) >> 1);
        *dst++ = out >> 16;
        size--;
    }
    while (size != 0u);
}

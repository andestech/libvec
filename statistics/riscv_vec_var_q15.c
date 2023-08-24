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
q31_t riscv_vec_var_q15(const q15_t * FUNC_RESTRICT src, uint32_t size)
{
    q31_t sum = 0, dst;
    q31_t meansqr, sqrmean;
    uint32_t i = size;
    union64u_t sos;
    q15_t val;
    q31_t temp, carry;

    sos.d0 = 0;
    while (i > 0u)
    {
        i--;
        val = *src++;
        temp = (q31_t) val * val;
        sos.b32.w0 += temp;
        //use slt instruction to check overflow.
        carry = (sos.b32.w0 < (uint32_t)temp) ? 1 : 0;
        sos.b32.w1 += carry;
        sum += val;
    }

    // To improve accuracy, replace the original with the following code.
    meansqr = (q31_t) (sos.d0 / (uint64_t)(size - 1));
    sqrmean = (q31_t)((uint64_t)((q63_t) sum * sum) / ((uint64_t)size * (size - 1)));
    dst = (q15_t) NDS_ISA_SATS(((meansqr - sqrmean) >> 15), 16u);

    return dst;
}

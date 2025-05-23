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
q63_t riscv_vec_var_q31(const q31_t * FUNC_RESTRICT src, uint32_t size)
{
    q63_t sos = 0, dst;
    q31_t mean = 0;
    q31_t meansqr, sqrmean;
    q31_t tmp1, tmp2;
    uint32_t i;

    i = size;
    while (i != 0u)
    {
        tmp1 = *src++;
        sos += (q63_t) tmp1 * tmp1;
        mean += tmp1;
        i--;
    }
    tmp2 = 0x40000000u / (size - 1u);
    sos = sos >> 31;
    meansqr = (q31_t) ((sos * tmp2) >> 30);

    tmp2 = 0x80000000u / (size * (size - 1u));

    sqrmean = (q31_t) (((q63_t) mean * mean) >> 31);
    sqrmean = (q31_t) (((q63_t) sqrmean * tmp2) >> 31);
    dst = (q63_t) (meansqr - sqrmean);

    return dst;
}

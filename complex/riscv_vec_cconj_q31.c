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
#include "stdio.h"
/* function description */
void riscv_vec_cconj_q31(q31_t * FUNC_RESTRICT dst, q31_t * FUNC_RESTRICT src, uint32_t size)
{
    q31_t a;

    do
    {
        /* y[0] + jy[1] = x[0] + j (-1) x[1] */
        /* Saturated to INT32_MAX. if the input is INT32_MIN */
        *dst++ = *src++;
        a = *src++;
        *dst++ = (q31_t) NDS_ISA_KSUBW(0, a);
        size--;
    }
    while (size != 0u);
}

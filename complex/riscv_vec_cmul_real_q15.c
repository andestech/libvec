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
void riscv_vec_cmul_real_q15(q15_t * dst, q15_t * src, q15_t * real, uint32_t size)
{
    q15_t a;
    q15_t tmp1, tmp2;

    while (size != 0u)
    {
        /* y[2 * i] = x[2 * i] * real[i] */
        /* y[2 * i + 1] = x[2 * i + 1] * real[i] */
        tmp1 = *src++;
        tmp2 = *src++;
        a = *real++;
        *dst++ = (q15_t) NDS_ISA_KHMBB(tmp1, a);
        *dst++ = (q15_t) NDS_ISA_KHMBB(tmp2, a);
        size--;
    }
}

/**
 * @} end of clx_mul_real
 */

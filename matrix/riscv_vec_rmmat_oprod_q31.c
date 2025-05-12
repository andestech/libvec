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
void riscv_vec_rmmat_oprod_q31(const q31_t * FUNC_RESTRICT src1, const q31_t * FUNC_RESTRICT src2, q31_t * FUNC_RESTRICT dst, uint32_t size1, uint32_t size2)
{
    uint32_t i;
    for (i = 0; i < size1; i++)
    {
        q31_t tmp1 = *src1++;
        uint32_t tmpcol2 = size2;
        q31_t *tmpsrc2 = (q31_t *)src2;
        while (tmpcol2 != 0)
        {
            q31_t tmpa = *tmpsrc2++;
            *dst++ = (q31_t)(((q63_t)tmp1 * tmpa) >> 31);
            tmpcol2--;
        }
    }
}

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
int riscv_vec_rmmat_scale_q15(const q15_t * FUNC_RESTRICT src, q15_t scale_fract, int32_t shift,
                              q15_t * FUNC_RESTRICT dst, uint32_t row, uint32_t col)
{

    uint32_t size;
    int32_t totalshift = 15 - shift;
    //check if the input row or col is zero
    if ((row <= 0) || (col <= 0))
    {
        return NDSV_FAIL;
    }
    size = row * col;
    uint32_t size2 = size >> 1;
    uint32_t rest_count = size & 1UL;

    while (size2 != 0u)
    {
        /* y = x * scale and then saturated in Q15 range */
        *dst++ = (q15_t) (NDS_ISA_SATS(((q31_t) * src++ * scale_fract) >> totalshift, 16u));
        *dst++ = (q15_t) (NDS_ISA_SATS(((q31_t) * src++ * scale_fract) >> totalshift, 16u));
        size2--;
    }
    if (rest_count != 0)
    {
        *dst = (q15_t) (NDS_ISA_SATS(((q31_t) (*src) * scale_fract) >> totalshift, 16u));
    }
    return NDSV_OK;
}

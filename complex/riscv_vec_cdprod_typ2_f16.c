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
void riscv_vec_cdprod_typ2_f16(const float16_t * FUNC_RESTRICT src1, const float16_t * FUNC_RESTRICT src2,
                               uint32_t size, float16_t * FUNC_RESTRICT rout, float16_t * FUNC_RESTRICT iout)
{
    float16_t realsum = 0.0, imagsum = 0.0;
    float16_t x0, x1, y0, y1;

    do
    {
        x0 = *src1++;
        x1 = *src1++;
        y0 = *src2++;
        y1 = *src2++;
        realsum = realsum + (x0 * y0) - (x1 * y1);
        imagsum = imagsum + (x0 * y1) + (x1 * y0);

        size--;
    }
    while (size != 0u);

    *rout = realsum;
    *iout = imagsum;
}

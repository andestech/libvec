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
#include "riscv_vec_utils.h" //riscv_vec_sqrt_q15
#include <math.h>

#define F2Q(f) ((int)((f) * 0x8000))
#define SQRT2 1.4142135623730950488016887242097

/* function description */
void riscv_vec_cmag_q15(q15_t * dst, q15_t * src, uint32_t size)
{
    q15_t a, b;
    q15_t *dp = (q15_t*)malloc(size * sizeof(q15_t));
    for(int i = 0 ; i < size; i++)
    {
        a = *src++;
        b = *src++;
        dp[i] = (q15_t) ((  (q63_t)( ((q31_t)a * a) + ((q31_t)b * b) )  ) >> 17);
    }
    riscv_vec_sqrt_q15(dp, dst, size);
}

/**
 * @} end of clx_mag
 */

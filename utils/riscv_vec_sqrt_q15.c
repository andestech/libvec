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

#include "internal_vec_utils.h"
#include <math.h> //sqrt

#define F2Q(f) ((int)((f) * 0x8000))
#define MULQ(a, b) (((a) * (b)) >> 15)
#define SQRT2 1.4142135623730950488016887242097

static inline int rsqrt(int x)
{
    int y;
    if (x & 0x8000)
    {
        y = MULQ(F2Q(3.81) - x, F2Q(0.37));
    }
    else
        y = MULQ(F2Q(3.81) - (x << 1), F2Q(0.37 * SQRT2));

    int x2 = x >> 1;
    y = MULQ(y, (F2Q(1.5) - MULQ(MULQ(x2, y), y)));
    y = MULQ(y, (F2Q(1.5) - MULQ(MULQ(x2, y), y)));

    return y;
}

/* function description */
void riscv_vec_sqrt_q15(q15_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size)
{
    //C
    while (size)
    {
        int val = *src;
        if (val > 0)
        {
            unsigned signbits = NDS_ISA_CLZ(val) - 16; // - 17 + 1
            signbits &= ~0x1;
            val = val << signbits;
            val = MULQ(val, rsqrt(val));
            val = val >> (signbits >> 1);
            *dst = val;
        }
        else
        {
            *dst = (val != 0) ? -1 : 0;
        }
        src++;
        dst++;
        size--;
    }
}

/**
 * @} end of utilsset
 */

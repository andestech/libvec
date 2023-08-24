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

/** @file*/

//// Vector-Shift Functions
#include "internal_nds_types.h"

int riscv_vec_shift_q31(q31_t * dst, q31_t * src, int8_t shift, uint32_t count)
{
    NDSV_CHECK_DstSrcCst;

    q31_t tmp1;

    if (shift < 0)
    {
        while (count != 0u)
        {
            tmp1 = *src++;
            *dst++ = (tmp1 >> (-shift));
            count--;
        }
    }
    else
    {
        q31_t ret, rev_sh_val;
        while (count != 0u)
        {
            tmp1 = *src++;
            ret = tmp1 << shift;
            rev_sh_val = ret >> shift;
            if (tmp1 != rev_sh_val)
            {
                ret = (tmp1 >> 31) ^ 0x7FFFFFFF;
            }
            *dst++ = ret;
            count--;
        }
    }
    return NDSV_OK;
}

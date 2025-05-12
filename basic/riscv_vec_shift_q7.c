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

/** @file*/

//// Vector Multiplication (Vector-Shift) Functions
#include "internal_nds_types.h"

int riscv_vec_shift_q7(q7_t * dst, q7_t * src, int8_t shift, uint32_t count)
{
    NDSV_CHECK_DstSrcCst;
    if (shift < 0)
    {
        shift = -shift;
        while (count != 0u)
        {
            *dst++ = *src++ >> shift;
            count--;
        }
    }
    else
    {

        while (count != 0u)
        {
            *dst++ = (q7_t) NDS_ISA_SATS(((q31_t) * src++ << shift), 8u);
            count--;
        }
    }
    return NDSV_OK;
}

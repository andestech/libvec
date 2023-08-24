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

//// Vector Absolute Functions

#include "internal_nds_types.h"

int riscv_vec_abs_q15(q15_t * dst, q15_t * src, uint32_t count)
{
    NDSV_CHECK_DstSrc;
    q15_t a;

    while (count != 0u)
    {
        /* y = |x| */
        /* if input value is INT16_MIN saturate to INT16_MAX. */
        a = *src++;
        *dst++ = (NDS_ISA_ABS(a << 16)) >> 16;
        count--;
    }
    return NDSV_OK;
}

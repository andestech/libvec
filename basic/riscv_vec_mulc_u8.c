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

//// Vector Multiplication (Vector-Scalar) Functions
#include "internal_nds_types.h"

int riscv_vec_mulc_u8(u8_t * dst, u8_t * src, const u8_t scale, const int8_t shift, uint32_t count)
{
    NDSV_CHECK_DstSrcCst;
    const int8_t shift1 = 7 - shift;
    int size = count;

    while (size != 0u)
    {
        *dst++ = (uint8_t) (NDS_ISA_SAT(((q31_t) * src++ * scale) >> shift1, 8u));
        size--;
    }
    return NDSV_OK;
}

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

//// Vector Subtraction (Vector-Vector) Functions

#include "internal_nds_types.h"

int riscv_vec_sub_q31(q31_t * dst, q31_t * src1, q31_t * src2, uint32_t count)
{
    NDSV_CHECK_DstSrc1Src2;
    while (count != 0)
    {
        *dst++ = (q31_t) NDS_ISA_KSUBW((*src1++), (*src2++));
        count--;
    }
    return NDSV_OK;
}

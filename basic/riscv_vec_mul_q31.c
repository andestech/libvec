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

//// Vector Multiplication (Vector-Vector) Functions
#include "internal_nds_types.h"

int riscv_vec_mul_q31(q31_t * dst, q31_t * src1, q31_t * src2, uint32_t count)
{
    NDSV_CHECK_DstSrc1Src2;
    while (count != 0u)
    {
        q31_t ret;
        q31_t a = *src1++;
        q31_t b = *src2++;

        if ((a == (q31_t)0x80000000) && (b == (q31_t)0x80000000))
        {
            ret = (q31_t)0x7FFFFFFF;
        }
        else
        {
            q63_t temp = (q63_t)a * b;
            ret = ((q31_t)(temp >> 32) << 1);
            //ret |= NDS_ISA_BTST((q31_t)temp, 31);
            ret |= ((uint32_t)temp) >> 31;  //Unlike btst, this can run at v3/v3m
        }
        *dst++ = ret;
        count--;
    }
    return NDSV_OK;
}

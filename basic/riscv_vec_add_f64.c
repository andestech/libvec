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

//// Vector Addition (Vector-Vector) Functions

#include "internal_nds_types.h"

int riscv_vec_add_f64(float64_t * dst, float64_t * src1, float64_t * src2, uint32_t count)
{
    NDSV_CHECK_DstSrc1Src2;
    uint32_t itr;
    for (itr = 0; itr < count; itr++)
    {
        dst[ itr ] = src1[ itr ] + src2[ itr ];
    }
    return NDSV_OK;
}

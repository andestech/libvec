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

//// Vector Subtraction (Scalar-Vector) Functions
#include "internal_nds_types.h"

int riscv_vec_rsubc_f16(float16_t * FUNC_RESTRICT dst, float16_t * FUNC_RESTRICT src, const float16_t cst, uint32_t count)
{
    NDSV_CHECK_DstSrcCst;
    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        dst[ itr ] = cst - src[ itr ];
    }
    return NDSV_OK;
}

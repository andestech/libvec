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

//// Vector Dot Product Functions

#include "internal_nds_types.h"

int riscv_vec_dprod_f32x4(float32_t * FUNC_RESTRICT dst, riscv_vec_f32x4_t * FUNC_RESTRICT src1,  riscv_vec_f32x4_t * FUNC_RESTRICT src2, uint32_t count)
{
    NDSV_CHECK_DstSrc1Src2;
    uint32_t itr;
    for (itr = 0; itr < count; itr++ )
    {
        dst[ itr ] = src1[ itr ].x * src2[ itr ].x +
                     src1[ itr ].y * src2[ itr ].y +
                     src1[ itr ].z * src2[ itr ].z +
                     src1[ itr ].w * src2[ itr ].w;
    }
    return NDSV_OK;
}

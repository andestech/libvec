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

//// Vector Normalization Functions

//L2-Norm for vector (2-, 3- or 4-tuples)

#include "internal_nds_types.h"

#include <math.h>   //for sqrt

int riscv_vec_normalize_f32x4(riscv_vec_f32x4_t * dst, riscv_vec_f32x4_t * src, uint32_t count)
{
    NDSV_CHECK_DstSrc;
    float32_t NDS_VEC_VG4;
    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        NDS_VEC_VG4 = sqrt (src[ itr ].x * src[ itr ].x +
                            src[ itr ].y * src[ itr ].y +
                            src[ itr ].z * src[ itr ].z +
                            src[ itr ].w * src[ itr ].w);

        dst[ itr ].x = src[ itr ].x / NDS_VEC_VG4;
        dst[ itr ].y = src[ itr ].y / NDS_VEC_VG4;
        dst[ itr ].z = src[ itr ].z / NDS_VEC_VG4;
        dst[ itr ].w = src[ itr ].w / NDS_VEC_VG4;
    }
    return NDSV_OK;
}

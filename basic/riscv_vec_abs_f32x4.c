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
#include <math.h>       //for fabs

int riscv_vec_abs_f32x4(riscv_vec_f32x4_t * dst, riscv_vec_f32x4_t * src, uint32_t count)
{
    NDSV_CHECK_DstSrc;
    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        dst[ itr ].x = fabs (src[ itr ].x);
        dst[ itr ].y = fabs (src[ itr ].y);
        dst[ itr ].z = fabs (src[ itr ].z);
        dst[ itr ].w = fabs (src[ itr ].w);
    }
    return NDSV_OK;
}

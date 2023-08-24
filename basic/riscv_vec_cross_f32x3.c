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

//// Vector Cross Product Functions

#include "internal_nds_types.h"

#define SRC1_X      NDS_VEC_VG0
#define SRC1_Y      NDS_VEC_VG1
#define SRC1_Z      NDS_VEC_VG2
#define SRC2_X      NDS_VEC_VG3
#define SRC2_Y      NDS_VEC_VG4
#define SRC2_Z      NDS_VEC_VG5
#define DST_X       NDS_VEC_VG6
#define DST_Y       NDS_VEC_VG7
#define DST_Z       NDS_VEC_VG8

//dst.x = (src1.y * src2.z) - (src1.z * src2.y)
//dst.y = (src1.z * src2.x) - (src1.x * src2.z)
//dst.z = (src1.x * src2.y) - (src1.y * src2.x)

int riscv_vec_cross_f32x3(riscv_vec_f32x3_t * FUNC_RESTRICT dst, riscv_vec_f32x3_t * FUNC_RESTRICT src1, riscv_vec_f32x3_t * FUNC_RESTRICT src2, uint32_t count)
{
    NDSV_CHECK_DstSrc1Src2;
    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        dst[ itr ].x = (src1[ itr ].y * src2[ itr ].z) - (src1[ itr ].z * src2[ itr ].y);
        dst[ itr ].y = (src1[ itr ].z * src2[ itr ].x) - (src1[ itr ].x * src2[ itr ].z);
        dst[ itr ].z = (src1[ itr ].x * src2[ itr ].y) - (src1[ itr ].y * src2[ itr ].x);
    }
    return NDSV_OK;
}

#undef SRC1_X
#undef SRC1_Y
#undef SRC1_Z
#undef SRC2_X
#undef SRC2_Y
#undef SRC2_Z
#undef DST_X
#undef DST_Y
#undef DST_Z

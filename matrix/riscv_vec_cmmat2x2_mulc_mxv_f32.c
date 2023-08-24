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

#include "internal_nds_types.h"
#include "internal_vec_matrix.h"

int riscv_vec_cmmat2x2_mulc_mxv_f32(riscv_vec_f32x2_t * dst, const riscv_vec_cmmat2x2_f32_t * cst, riscv_vec_f32x2_t * src, uint32_t count)
{
#define A1 cst->c1.x
#define B1 cst->c1.y
#define C1 cst->c2.x
#define D1 cst->c2.y
    NDSV_CHECK_DstSrc1Src2;
    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        dst[ itr ].x = A1 * src[ itr ].x + C1 * src[ itr ].y;
        dst[ itr ].y = B1 * src[ itr ].x + D1 * src[ itr ].y;
    }
    return NDSV_OK;
#undef A1
#undef B1
#undef C1
#undef D1

}

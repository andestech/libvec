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

#include "internal_nds_types.h"
#include "internal_vec_matrix.h"

int riscv_vec_cmmat4x4_mulc_mxv_f32(riscv_vec_f32x4_t * dst, const riscv_vec_cmmat4x4_f32_t * cst, riscv_vec_f32x4_t * src, uint32_t count)
{
#define A1 cst->c1.x
#define B1 cst->c1.y
#define C1 cst->c1.z
#define D1 cst->c1.w
#define E1 cst->c2.x
#define F1 cst->c2.y
#define G1 cst->c2.z
#define H1 cst->c2.w
#define I1 cst->c3.x
#define J1 cst->c3.y
#define K1 cst->c3.z
#define L1 cst->c3.w
#define M1 cst->c4.x
#define N1 cst->c4.y
#define O1 cst->c4.z
#define P1 cst->c4.w
    NDSV_CHECK_DstSrc1Src2;
    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        dst[ itr ].x = A1 * src[ itr ].x + E1 * src[ itr ].y + I1 * src[ itr ].z + M1 * src[ itr ].w;
        dst[ itr ].y = B1 * src[ itr ].x + F1 * src[ itr ].y + J1 * src[ itr ].z + N1 * src[ itr ].w;
        dst[ itr ].z = C1 * src[ itr ].x + G1 * src[ itr ].y + K1 * src[ itr ].z + O1 * src[ itr ].w;
        dst[ itr ].w = D1 * src[ itr ].x + H1 * src[ itr ].y + L1 * src[ itr ].z + P1 * src[ itr ].w;
    }
    return NDSV_OK;
#undef A1
#undef B1
#undef C1
#undef D1
#undef E1
#undef F1
#undef G1
#undef H1
#undef I1
#undef J1
#undef K1
#undef L1
#undef M1
#undef N1
#undef O1
#undef P1
}

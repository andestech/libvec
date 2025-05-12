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

int riscv_vec_cmmat2x2_mul_f32(riscv_vec_cmmat2x2_f32_t * dst, riscv_vec_cmmat2x2_f32_t * src1, riscv_vec_cmmat2x2_f32_t * src2, uint32_t count)
{
#define A1 src1[ itr ].c1.x
#define A2 src2[ itr ].c1.x
#define B1 src1[ itr ].c1.y
#define B2 src2[ itr ].c1.y
#define C1 src1[ itr ].c2.x
#define C2 src2[ itr ].c2.x
#define D1 src1[ itr ].c2.y
#define D2 src2[ itr ].c2.y

    NDSV_CHECK_DstSrc1Src2;
    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        dst[ itr ].c1.x = (A1 * A2) + (C1 * B2);
        dst[ itr ].c1.y = (B1 * A2) + (D1 * B2);

        dst[ itr ].c2.x = (A1 * C2) + (C1 * D2);
        dst[ itr ].c2.y = (B1 * C2) + (D1 * D2);
    }
#undef A1
#undef A2
#undef B1
#undef B2
#undef C1
#undef C2
#undef D1
#undef D2
    return NDSV_OK;
}

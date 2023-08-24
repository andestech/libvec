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

int riscv_vec_cmmat3x3_mul_f32(riscv_vec_cmmat3x3_f32_t * dst, riscv_vec_cmmat3x3_f32_t * src1, riscv_vec_cmmat3x3_f32_t * src2, uint32_t count)
{
#define A1 src1[ itr ].c1.x
#define A2 src2[ itr ].c1.x
#define B1 src1[ itr ].c1.y
#define B2 src2[ itr ].c1.y
#define C1 src1[ itr ].c1.z
#define C2 src2[ itr ].c1.z
#define D1 src1[ itr ].c2.x
#define D2 src2[ itr ].c2.x
#define E1 src1[ itr ].c2.y
#define E2 src2[ itr ].c2.y
#define F1 src1[ itr ].c2.z
#define F2 src2[ itr ].c2.z
#define G1 src1[ itr ].c3.x
#define G2 src2[ itr ].c3.x
#define H1 src1[ itr ].c3.y
#define H2 src2[ itr ].c3.y
#define I1 src1[ itr ].c3.z
#define I2 src2[ itr ].c3.z

    NDSV_CHECK_DstSrc1Src2;
    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        dst[ itr ].c1.x = (A1 * A2) + (D1 * B2) + (G1 * C2);
        dst[ itr ].c1.y = (B1 * A2) + (E1 * B2) + (H1 * C2);
        dst[ itr ].c1.z = (C1 * A2) + (F1 * B2) + (I1 * C2);

        dst[ itr ].c2.x = (A1 * D2) + (D1 * E2) + (G1 * F2);
        dst[ itr ].c2.y = (B1 * D2) + (E1 * E2) + (H1 * F2);
        dst[ itr ].c2.z = (C1 * D2) + (F1 * E2) + (I1 * F2);

        dst[ itr ].c3.x = (A1 * G2) + (D1 * H2) + (G1 * I2);
        dst[ itr ].c3.y = (B1 * G2) + (E1 * H2) + (H1 * I2);
        dst[ itr ].c3.z = (C1 * G2) + (F1 * H2) + (I1 * I2);
    }
#undef A1
#undef A2
#undef B1
#undef B2
#undef C1
#undef C2
#undef D1
#undef D2
#undef E1
#undef E2
#undef F1
#undef F2
#undef G1
#undef G2
#undef H1
#undef H2
#undef I1
#undef I2
    return NDSV_OK;
}

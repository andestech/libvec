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

int riscv_vec_cmmat4x4_mul_f32(riscv_vec_cmmat4x4_f32_t * dst, riscv_vec_cmmat4x4_f32_t * src1, riscv_vec_cmmat4x4_f32_t * src2, uint32_t count)
{
#define A1 src1[ itr ].c1.x
#define A2 src2[ itr ].c1.x
#define B1 src1[ itr ].c1.y
#define B2 src2[ itr ].c1.y
#define C1 src1[ itr ].c1.z
#define C2 src2[ itr ].c1.z
#define D1 src1[ itr ].c1.w
#define D2 src2[ itr ].c1.w

#define E1 src1[ itr ].c2.x
#define E2 src2[ itr ].c2.x
#define F1 src1[ itr ].c2.y
#define F2 src2[ itr ].c2.y
#define G1 src1[ itr ].c2.z
#define G2 src2[ itr ].c2.z
#define H1 src1[ itr ].c2.w
#define H2 src2[ itr ].c2.w

#define I1 src1[ itr ].c3.x
#define I2 src2[ itr ].c3.x
#define J1 src1[ itr ].c3.y
#define J2 src2[ itr ].c3.y
#define K1 src1[ itr ].c3.z
#define K2 src2[ itr ].c3.z
#define L1 src1[ itr ].c3.w
#define L2 src2[ itr ].c3.w

#define M1 src1[ itr ].c4.x
#define M2 src2[ itr ].c4.x
#define N1 src1[ itr ].c4.y
#define N2 src2[ itr ].c4.y
#define O1 src1[ itr ].c4.z
#define O2 src2[ itr ].c4.z
#define P1 src1[ itr ].c4.w
#define P2 src2[ itr ].c4.w

    NDSV_CHECK_DstSrc1Src2;
    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        dst[ itr ].c1.x = (A1 * A2) + (E1 * B2) + (I1 * C2) + (M1 * D2);
        dst[ itr ].c1.y = (B1 * A2) + (F1 * B2) + (J1 * C2) + (N1 * D2);
        dst[ itr ].c1.z = (C1 * A2) + (G1 * B2) + (K1 * C2) + (O1 * D2);
        dst[ itr ].c1.w = (D1 * A2) + (H1 * B2) + (L1 * C2) + (P1 * D2);

        dst[ itr ].c2.x = (A1 * E2) + (E1 * F2) + (I1 * G2) + (M1 * H2);
        dst[ itr ].c2.y = (B1 * E2) + (F1 * F2) + (J1 * G2) + (N1 * H2);
        dst[ itr ].c2.z = (C1 * E2) + (G1 * F2) + (K1 * G2) + (O1 * H2);
        dst[ itr ].c2.w = (D1 * E2) + (H1 * F2) + (L1 * G2) + (P1 * H2);

        dst[ itr ].c3.x = (A1 * I2) + (E1 * J2) + (I1 * K2) + (M1 * L2);
        dst[ itr ].c3.y = (B1 * I2) + (F1 * J2) + (J1 * K2) + (N1 * L2);
        dst[ itr ].c3.z = (C1 * I2) + (G1 * J2) + (K1 * K2) + (O1 * L2);
        dst[ itr ].c3.w = (D1 * I2) + (H1 * J2) + (L1 * K2) + (P1 * L2);

        dst[ itr ].c4.x = (A1 * M2) + (E1 * N2) + (I1 * O2) + (M1 * P2);
        dst[ itr ].c4.y = (B1 * M2) + (F1 * N2) + (J1 * O2) + (N1 * P2);
        dst[ itr ].c4.z = (C1 * M2) + (G1 * N2) + (K1 * O2) + (O1 * P2);
        dst[ itr ].c4.w = (D1 * M2) + (H1 * N2) + (L1 * O2) + (P1 * P2);
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
#undef J1
#undef J2
#undef K1
#undef K2
#undef L1
#undef L2
#undef M1
#undef M2
#undef N1
#undef N2
#undef O1
#undef O2
#undef P1
#undef P2
    return NDSV_OK;
}

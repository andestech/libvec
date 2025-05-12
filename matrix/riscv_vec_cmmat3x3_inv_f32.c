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

int riscv_vec_cmmat3x3_inv_f32(riscv_vec_cmmat3x3_f32_t * dst, riscv_vec_cmmat3x3_f32_t * src, uint32_t count)
{
#define aa   (src[ itr ].c1.x)
#define bb   (src[ itr ].c1.y)
#define cc   (src[ itr ].c1.z)
#define dd   (src[ itr ].c2.x)
#define ee   (src[ itr ].c2.y)
#define ff   (src[ itr ].c2.z)
#define gg   (src[ itr ].c3.x)
#define hh   (src[ itr ].c3.y)
#define ii   (src[ itr ].c3.z)

    float32_t det = 0.0f;
    riscv_vec_cmmat2x2_f32_t A, B, C, D, E, F, G, H, I;

    NDSV_CHECK_DstSrc1Src2;
    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        det = vec_cmmat3x3_det_f32(&src[ itr ]);

        if (1 == IS_FLOAT_NEAR_ZERO (det))
        {
            det = 1.0f;
        }
        det = 1.0f / det;

        // Calculate the coefficients
        vec_cmmat2x2_trans_f32 (&A, ee, ff, hh, ii);
        vec_cmmat2x2_trans_f32 (&B, dd, ff, gg, ii);
        vec_cmmat2x2_trans_f32 (&C, dd, ee, gg, hh);
        vec_cmmat2x2_trans_f32 (&D, bb, cc, hh, ii);
        vec_cmmat2x2_trans_f32 (&E, aa, cc, gg, ii);
        vec_cmmat2x2_trans_f32 (&F, aa, bb, gg, hh);
        vec_cmmat2x2_trans_f32 (&G, bb, cc, ee, ff);
        vec_cmmat2x2_trans_f32 (&H, aa, cc, dd, ff);
        vec_cmmat2x2_trans_f32 (&I, aa, bb, dd, ee);

        dst[ itr ].c1.x =         det * vec_cmmat2x2_det_f32 (&A);
        dst[ itr ].c1.y = -1.0f * det * vec_cmmat2x2_det_f32 (&D);
        dst[ itr ].c1.z =         det * vec_cmmat2x2_det_f32 (&G);

        dst[ itr ].c2.x = -1.0f * det * vec_cmmat2x2_det_f32 (&B);
        dst[ itr ].c2.y =         det * vec_cmmat2x2_det_f32 (&E);
        dst[ itr ].c2.z = -1.0f * det * vec_cmmat2x2_det_f32 (&H);

        dst[ itr ].c3.x =         det * vec_cmmat2x2_det_f32 (&C);
        dst[ itr ].c3.y = -1.0f * det * vec_cmmat2x2_det_f32 (&F);
        dst[ itr ].c3.z =         det * vec_cmmat2x2_det_f32 (&I);
    }
    return NDSV_OK;

#undef aa
#undef bb
#undef cc
#undef dd
#undef ee
#undef ff
#undef gg
#undef hh
#undef ii
}

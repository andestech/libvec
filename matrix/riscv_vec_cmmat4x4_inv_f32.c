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

int riscv_vec_cmmat4x4_inv_f32(riscv_vec_cmmat4x4_f32_t * dst, riscv_vec_cmmat4x4_f32_t * src, uint32_t count)
{
#define aa   (src[ itr ].c1.x)
#define bb   (src[ itr ].c1.y)
#define cc   (src[ itr ].c1.z)
#define dd   (src[ itr ].c1.w)

#define ee   (src[ itr ].c2.x)
#define ff   (src[ itr ].c2.y)
#define gg   (src[ itr ].c2.z)
#define hh   (src[ itr ].c2.w)

#define ii   (src[ itr ].c3.x)
#define jj   (src[ itr ].c3.y)
#define kk   (src[ itr ].c3.z)
#define ll   (src[ itr ].c3.w)

#define mm   (src[ itr ].c4.x)
#define nn   (src[ itr ].c4.y)
#define oo   (src[ itr ].c4.z)
#define pp   (src[ itr ].c4.w)

    float32_t det = 0.0f;
    riscv_vec_cmmat3x3_f32_t A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P;

    NDSV_CHECK_DstSrc1Src2;
    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        det = _vec_cmmat4x4_det_f32(&src[ itr ]);

        if (1 == IS_FLOAT_NEAR_ZERO (det))
        {
            det = 1.0f;
        }
        det = 1.0f / det;

        // Calculate the coefficients
        _vec_cmmat3x3_trans_f32 (&A, ff, gg, hh, jj, kk, ll, nn, oo, pp);
        _vec_cmmat3x3_trans_f32 (&B, ee, gg, hh, ii, kk, ll, mm, oo, pp);
        _vec_cmmat3x3_trans_f32 (&C, ee, ff, hh, ii, jj, ll, mm, nn, pp);
        _vec_cmmat3x3_trans_f32 (&D, ee, ff, gg, ii, jj, kk, mm, nn, oo);
        _vec_cmmat3x3_trans_f32 (&E, bb, cc, dd, jj, kk, ll, nn, oo, pp);
        _vec_cmmat3x3_trans_f32 (&F, aa, cc, dd, ii, kk, ll, mm, oo, pp);
        _vec_cmmat3x3_trans_f32 (&G, aa, bb, dd, ii, jj, ll, mm, nn, pp);
        _vec_cmmat3x3_trans_f32 (&H, aa, bb, cc, ii, jj, kk, mm, nn, oo);
        _vec_cmmat3x3_trans_f32 (&I, bb, cc, dd, ff, gg, hh, nn, oo, pp);
        _vec_cmmat3x3_trans_f32 (&J, aa, cc, dd, ee, gg, hh, mm, oo, pp);
        _vec_cmmat3x3_trans_f32 (&K, aa, bb, dd, ee, ff, hh, mm, nn, pp);
        _vec_cmmat3x3_trans_f32 (&L, aa, bb, cc, ee, ff, gg, mm, nn, oo);
        _vec_cmmat3x3_trans_f32 (&M, bb, cc, dd, ff, gg, hh, jj, kk, ll);
        _vec_cmmat3x3_trans_f32 (&N, aa, cc, dd, ee, gg, hh, ii, kk, ll);
        _vec_cmmat3x3_trans_f32 (&O, aa, bb, dd, ee, ff, hh, ii, jj, ll);
        _vec_cmmat3x3_trans_f32 (&P, aa, bb, cc, ee, ff, gg, ii, jj, kk);

        dst[ itr ].c1.x =         det * _vec_cmmat3x3_det_f32 (&A);
        dst[ itr ].c1.y = -1.0f * det * _vec_cmmat3x3_det_f32 (&E);
        dst[ itr ].c1.z =         det * _vec_cmmat3x3_det_f32 (&I);
        dst[ itr ].c1.w = -1.0f * det * _vec_cmmat3x3_det_f32 (&M);

        dst[ itr ].c2.x = -1.0f * det * _vec_cmmat3x3_det_f32 (&B);
        dst[ itr ].c2.y =         det * _vec_cmmat3x3_det_f32 (&F);
        dst[ itr ].c2.z = -1.0f * det * _vec_cmmat3x3_det_f32 (&J);
        dst[ itr ].c2.w =         det * _vec_cmmat3x3_det_f32 (&N);

        dst[ itr ].c3.x =         det * _vec_cmmat3x3_det_f32 (&C);
        dst[ itr ].c3.y = -1.0f * det * _vec_cmmat3x3_det_f32 (&G);
        dst[ itr ].c3.z =         det * _vec_cmmat3x3_det_f32 (&K);
        dst[ itr ].c3.w = -1.0f * det * _vec_cmmat3x3_det_f32 (&O);

        dst[ itr ].c4.x = -1.0f * det * _vec_cmmat3x3_det_f32 (&D);
        dst[ itr ].c4.y =         det * _vec_cmmat3x3_det_f32 (&H);
        dst[ itr ].c4.z = -1.0f * det * _vec_cmmat3x3_det_f32 (&L);
        dst[ itr ].c4.w =         det * _vec_cmmat3x3_det_f32 (&P);
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
#undef jj
#undef kk
#undef ll
#undef mm
#undef nn
#undef oo
#undef pp
}

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

#ifndef __INTERNAL_VEC_MATRIX_H__
#define __INTERNAL_VEC_MATRIX_H__

#include <math.h>
#include "riscv_vec_matrix.h"

#define NUM_OF_MAT2X2_COL 2
#define NUM_OF_MAT2X2 4
#define NUM_OF_MAT2X2_ELEM_VLEN512 128

#define NUM_OF_MAT3X3_COL 3
#define NUM_OF_MAT3X3 9
#define NUM_OF_MAT3X3_ELEM_VLEN512 126

#define NUM_OF_MAT4X4_COL 4
#define NUM_OF_MAT4X4 16
#define NUM_OF_MAT4X4_ELEM_VLEN512 128

// This macro is used to determine floating point values that are small enough to be consiedered nearly zero
#define IS_FLOAT_NEAR_ZERO(x) ( ((fabs(x))<(1e-12)) ? 1 : 0 )

static inline float32_t _vec_cmmat2x2_det_f32(riscv_vec_cmmat2x2_f32_t *mat)
{
    // 2x2 matrix layout
    //  c.x c.x
    //  c.y c.y
    float32_t out = ((mat->c1.x * mat->c2.y) - (mat->c2.x * mat->c1.y));
    return out;
}

static inline float32_t _vec_cmmat3x3_det_f32(riscv_vec_cmmat3x3_f32_t *mat)
{
    // 3x3 matrix layout
    //  c.x c.x c.x
    //  c.y c.y c.y
    //  c.z c.z c.z

    riscv_vec_cmmat2x2_f32_t subm11 = { {mat->c2.y, mat->c2.z}, {mat->c3.y, mat->c3.z} };
    riscv_vec_cmmat2x2_f32_t subm21 = { {mat->c1.y, mat->c1.z}, {mat->c3.y, mat->c3.z} };
    riscv_vec_cmmat2x2_f32_t subm31 = { {mat->c1.y, mat->c1.z}, {mat->c2.y, mat->c2.z} };
    float32_t out = (mat->c1.x * _vec_cmmat2x2_det_f32(&subm11))
                  - (mat->c2.x * _vec_cmmat2x2_det_f32(&subm21))
                  + (mat->c3.x * _vec_cmmat2x2_det_f32(&subm31));
    return out;
}

static inline float32_t _vec_cmmat4x4_det_f32(riscv_vec_cmmat4x4_f32_t * mat)
{
    // 4x4 matrix layout
    //  c.x c.x c.x c.x
    //  c.y c.y c.y c.y
    //  c.z c.z c.z c.z
    //  c.w c.w c.w c.w

    riscv_vec_cmmat3x3_f32_t subm11 = { {mat->c2.y, mat->c2.z, mat->c2.w},
                             {mat->c3.y, mat->c3.z, mat->c3.w},
                             {mat->c4.y, mat->c4.z, mat->c4.w}};

    riscv_vec_cmmat3x3_f32_t subm21 = { {mat->c1.y, mat->c1.z, mat->c1.w},
                             {mat->c3.y, mat->c3.z, mat->c3.w},
                             {mat->c4.y, mat->c4.z, mat->c4.w} };

    riscv_vec_cmmat3x3_f32_t subm31 = { {mat->c1.y, mat->c1.z, mat->c1.w},
                             {mat->c2.y, mat->c2.z, mat->c2.w},
                             {mat->c4.y, mat->c4.z, mat->c4.w} };

    riscv_vec_cmmat3x3_f32_t subm41 = { {mat->c1.y, mat->c1.z, mat->c1.w},
                             {mat->c2.y, mat->c2.z, mat->c2.w},
                             {mat->c3.y, mat->c3.z, mat->c3.w} };

    return    (mat->c1.x * _vec_cmmat3x3_det_f32(&subm11))
            - (mat->c2.x * _vec_cmmat3x3_det_f32(&subm21))
            + (mat->c3.x * _vec_cmmat3x3_det_f32(&subm31))
            - (mat->c4.x * _vec_cmmat3x3_det_f32(&subm41));
}

static inline void _vec_cmmat2x2_trans_f32(riscv_vec_cmmat2x2_f32_t * outMat, float32_t m11, float32_t m21, float32_t m12, float32_t m22)
{
    outMat->c1.x = m11;
    outMat->c1.y = m21;
    outMat->c2.x = m12;
    outMat->c2.y = m22;
}

static inline void _vec_cmmat3x3_trans_f32 (riscv_vec_cmmat3x3_f32_t * outMat, float32_t m11, float32_t m21, float32_t m31,
        float32_t m12, float32_t m22, float32_t m32,
        float32_t m13, float32_t m23, float32_t m33)
{
    outMat->c1.x = m11;
    outMat->c1.y = m21;
    outMat->c1.z = m31;

    outMat->c2.x = m12;
    outMat->c2.y = m22;
    outMat->c2.z = m32;

    outMat->c3.x = m13;
    outMat->c3.y = m23;
    outMat->c3.z = m33;
}

static inline void _vec_cmmat4x4_trans_f32 (riscv_vec_cmmat4x4_f32_t * outMat, float32_t m11, float32_t m21, float32_t m31, float32_t m41,
        float32_t m12, float32_t m22, float32_t m32, float32_t m42,
        float32_t m13, float32_t m23, float32_t m33, float32_t m43,
        float32_t m14, float32_t m24, float32_t m34, float32_t m44)
{
    outMat->c1.x = m11;
    outMat->c1.y = m21;
    outMat->c1.z = m31;
    outMat->c1.w = m41;

    outMat->c2.x = m12;
    outMat->c2.y = m22;
    outMat->c2.z = m32;
    outMat->c2.w = m42;

    outMat->c3.x = m13;
    outMat->c3.y = m23;
    outMat->c3.z = m33;
    outMat->c3.w = m43;

    outMat->c4.x = m14;
    outMat->c4.y = m24;
    outMat->c4.z = m34;
    outMat->c4.w = m44;
}


#endif

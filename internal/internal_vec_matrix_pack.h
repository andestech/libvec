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

#ifndef __MATRIX_PACK_H__
#define __MATRIX_PACK_H__

#include "riscv_dsp_math_types.h"
#include <riscv_vec_types.h>

#define CLX_STRIDE 1
#define INDEX ((j * col + i + (size * t)) << CLX_STRIDE)

void vec_rmcmat_chd_batch_pack_f32(const float* src, float* dst, uint32_t row, uint32_t col, int batch)
{
    if ((row <= 0) || (col <= 0) || (batch <= 0))
    {
        return;
    }
    int size = row * col;
    int pos = 0;
    for(int t = 0 ; t < batch ; t++)
    {
        for(int j = 0 ; j < col ; j++)
        {
            for(int i = 0 ; i < row ; i++)
            {
                dst[INDEX] = src[pos];
                dst[INDEX + 1] = src[pos + 1];
                pos += 2;
            }
        }
    }
    return;
}

void vec_rmcmat_chd_batch_unpack_f32(float* src, float* dst, uint32_t row, uint32_t col, int batch)
{
    if ((row <= 0) || (col <= 0) || (batch <= 0))
    {
        return;
    }

    int size = row * col;
    int pos = 0;
    for(int t = 0 ; t < batch ; t++)
    {
        for(int j = 0 ; j < col ; j++)
        {
            for(int i = 0 ; i < row ; i++)
            {
                dst[pos] = src[INDEX];
                dst[pos + 1] = src[INDEX + 1];
                pos += 2;
            }
        }
    }
    return;
}

#endif //__MATRIX_PACK_H__

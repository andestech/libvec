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

#include "internal_vec_utils.h"

#define ATAN2_NB_COEFS_F32 10
#define PIHALFF32 1.5707963267948966192313f

static const float32_t atan2_coefs_f32[ATAN2_NB_COEFS_F32] =
{
    0.0f, 1.0000001638308195518f, -0.0000228941363602264f, -0.3328086544578890873f, -0.004404814619311061f,
    0.2162217461808173258f, -0.0207504842057097504f, -0.1745263362250363339f, 0.1340557235283553386f, -0.0323664125927477625f
};

static inline float32_t atan_limited_f32(float32_t x)
{
    float32_t res = atan2_coefs_f32[ATAN2_NB_COEFS_F32 - 1];
    int i = 1;
    for(i = 1 ; i < ATAN2_NB_COEFS_F32 ; i++)
    {
        res = x * res + atan2_coefs_f32[ATAN2_NB_COEFS_F32 - 1 - i];
    }

    return res;
}

/* function description */
void riscv_vec_atan_f32(float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{

    for(int i = 0 ; i < size ; i++)
    {
        int sign = 0;
        float32_t res = 0.0f;

        if (src[i] < 0.0f)
        {
            sign = 1;
            src[i] = -src[i];
        }

        if (src[i] > 1.0f)
        {
            src[i] = 1.0f / src[i];
            res = PIHALFF32 - atan_limited_f32(src[i]);
        }
        else
        {
            res += atan_limited_f32(src[i]);
        }

        if (sign)
        {
            res = -res;
        }

        dst[i] = res;
    }
    return;
}

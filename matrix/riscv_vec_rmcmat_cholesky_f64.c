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

#include <complex.h> //csqrtf, creal, cimag

/* function description */
int riscv_vec_rmcmat_cholesky_f64(const float64_t* FUNC_RESTRICT src, float64_t* FUNC_RESTRICT dst, uint32_t row, uint32_t col)
{
    //check if the input row or col is zero, and should be square matrix.
    if ((row <= 0) || (col <= 0) || (row != col))
    {
        return NDSV_FAIL;
    }
    int i, j, k;
    float64_t sum_r, sum_i;
    const float64_t* pA;
    float64_t* pG;

    pA = src;
    pG = dst;
    for (i = 0; i < col; i++)
    {
        for (j = i; j < row; j++)
        {
            pG[(j * col + i) << 1] = pA[(j * col + i) << 1];
            pG[((j * col + i) << 1) + 1] = pA[((j * col + i) << 1) + 1];
            sum_r = 0.0;
            sum_i = 0.0;
            for (k = 0; k < i; k++)
            {
                float64_t a = pG[(j * col + k) << 1], b = pG[((j * col + k) << 1) + 1];
                float64_t c = pG[(i * col + k) << 1], d = pG[((i * col + k) << 1) + 1];
                //conjugate, (a-bi)*(c+di)
                sum_r += a * c + b * d;
                sum_i += b * c - a * d;
            }

            pG[(j * col + i) << 1] -= sum_r;
            pG[((j * col + i) << 1) + 1] -= sum_i;

        }

        if (pG[(i * col + i) << 1] <= 0.0)
        {
            return NDSV_FAIL;
        }

        double complex invSqrtVj = csqrt(pG[(i * col + i) << 1] + pG[((i * col + i) << 1) + 1] * I);
        float64_t c = creal(invSqrtVj);
        float64_t d = cimag(invSqrtVj);
        float64_t divisor = 1.0 / (c * c + d * d);
        for (j = i; j < row; j++)
        {
            float64_t a = pG[(j * col + i) << 1], b = pG[((j * col + i) << 1) + 1];
            pG[(j * col + i) << 1] = (a * c + b * d) * divisor;
            pG[((j * col + i) << 1) + 1] = (b * c - a * d) * divisor;
        }
    }
    return NDSV_OK;
}

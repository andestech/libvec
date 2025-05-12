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
#include <complex.h> //csqrtf, creal, cimag for pureC

//"-DDETECT_INVALID" for fail case detection
#ifdef DETECT_INVALID
#include <string.h> //memset
#define INDEX_i_i_batch_re_detect ((i * col + i) * (batch << CLX_STRIDE) + f)
#endif

/**
 * @brief cholesky decomposition of a floating-point matrix.
 * @param[in]       *src points to the input matrix.
 * @param[out]      *dst points to the output matrix.
 * @param[in]       row  number of the input matrix rows.
 * @param[in]       col  number of the input matrix columns.
 * @param[in]       batch  number of the input matrix.
 * @return The function returns NDSV_FAIL, if the matrix can't be solved.
 * Note that the decomposition will FAIL if the matrix is not symmetric or Hermitian.
 */

//C
#define CLX_STRIDE 1
#define INDEX_j_i_batch_re ((j * col + i + (row * col * t)) << CLX_STRIDE)
#define INDEX_j_i_batch_im (INDEX_j_i_batch_re + 1)
#define INDEX_i_k_batch_re ((i * col + k + (row * col * t)) << CLX_STRIDE)
#define INDEX_i_k_batch_im (INDEX_i_k_batch_re + 1)
#define INDEX_j_k_batch_re ((j * col + k + (row * col * t)) << CLX_STRIDE)
#define INDEX_j_k_batch_im (INDEX_j_k_batch_re + 1)
#define INDEX_i_i_batch_re ((i * col + i + (row * col * t)) << CLX_STRIDE)
#define INDEX_i_i_batch_im (INDEX_i_i_batch_re + 1)

/* function description */
int riscv_vec_rmcmat_cholesky_batch_f32(const float32_t* FUNC_RESTRICT src_in, float32_t* FUNC_RESTRICT dst_out, uint32_t row, uint32_t col, int batch)
{
    //check if the input row or col is zero, and should be square matrix.
    if ((row <= 0) || (col <= 0) || (row != col))
    {
        return NDSV_FAIL;
    }

    const float32_t *src;
    float32_t *dst;
    src = src_in;
    dst = dst_out;

#ifdef DETECT_INVALID
    u8_t *fail_cases = (u8_t*)malloc(sizeof(u8_t) * batch);
    memset(fail_cases, 0, sizeof(u8_t) * batch);
#endif
    int i, j, k;
    float32_t sum_r, sum_i;
    const float32_t* pA;
    float32_t* pG;

    pA = src;
    pG = dst;
    for(int t = 0 ; t < batch ; t++)
    {
        for (i = 0; i < col; i++)
        {
            for (j = i; j < row; j++)
            {
                pG[INDEX_j_i_batch_re] = pA[INDEX_j_i_batch_re];
                pG[INDEX_j_i_batch_im] = pA[INDEX_j_i_batch_im];
                sum_r = 0.0f;
                sum_i = 0.0f;
                for (k = 0; k < i; k++)
                {
                    float a = pG[INDEX_j_k_batch_re], b = pG[INDEX_j_k_batch_im];
                    float c = pG[INDEX_i_k_batch_re], d = pG[INDEX_i_k_batch_im];
                    //conjugate, (a+bi)*(c-di)
                    sum_r += a * c + b * d;
                    sum_i += b * c - a * d;
                }
                pG[INDEX_j_i_batch_re] -= sum_r;
                pG[INDEX_j_i_batch_im] -= sum_i;
            }

#ifdef DETECT_INVALID
            if(pG[INDEX_i_i_batch_re] <= 0.0f)
            {
                fail_cases[t] = 1;
            }
#endif
            float complex invSqrtVj = csqrtf(pG[INDEX_i_i_batch_re] + pG[INDEX_i_i_batch_im] * I);
            float c = creal(invSqrtVj);
            float d = cimag(invSqrtVj);
            float divisor = 1.0 / (c * c + d * d);
            for (j = i; j < row; j++)
            {
                float a = pG[INDEX_j_i_batch_re], b = pG[INDEX_j_i_batch_im];
                pG[INDEX_j_i_batch_re] = (a * c + b * d) * divisor;
                pG[INDEX_j_i_batch_im] = (b * c - a * d) * divisor;
            }
        }
    }

    int ret = NDSV_OK;
#ifdef DETECT_INVALID
    float _NAN = 0.0 / 0.0;
    for(int t = 0 ; t < batch ; t++)
    {
        if(fail_cases[t] != 0)
        {
            for (j = 0; j < col; j++)
            {
                for (i = 0; i < row; i++)
                {
                    pG[INDEX_j_i_batch_re] = _NAN;
                    pG[INDEX_j_i_batch_im] = _NAN;
                }
            }
            ret = NDSV_FAIL;
        }
    }
#endif
    return ret;
}

/**
 * @} end of matrixtrans
 */

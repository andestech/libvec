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

#include "riscv_vec_filtering.h"
#include "internal_nds_types.h"

/* function description */
void riscv_vec_autocorr_f32(float32_t * FUNC_RESTRICT src, uint32_t len, float32_t * FUNC_RESTRICT dst)
{
    float32_t *pIn1;
    float32_t *pOut = dst;
    float32_t *pOut2 = dst + (2 * len - 2);
    float32_t sum;
    float32_t *px;
    float32_t *py;
    float32_t *pSrc1;
    long_t k = 0, count, blockSize1 = len;
    const int32_t inc = 1;

    pIn1 = (src);

    //part1
    count = 1u;
    px = pIn1;
    pSrc1 = pIn1 + (len - 1u);
    py = pSrc1;

    while (blockSize1 > 0u)
    {
        sum = 0.0f;
        k = count;
        while (k > 0u)
        {
            sum += (* px++ * *py++);
            k--;
        }
        *pOut = sum;
        *pOut2 = sum;
        pOut += inc;
        pOut2 -= inc;
        py = pSrc1 - count;
        px = pIn1;
        count++;
        blockSize1--;
    }
}

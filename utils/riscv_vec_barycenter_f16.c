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
#include <string.h>

/**
 * @ingroup utils
 */

/**
 * @brief Barycenter of the floating-potint type.
 * @param[in]       *src    points to the input vector.
 * @param[in]       *weight points to the weighted vector.
 * @param[out]      *out    points to the out vector.
 * @param[in]       numofvec    size of the vectors.
 * @param[in]       dimofvec    size of the vectors.
 * @return  None
 *
 */

/* function description */
void riscv_vec_barycenter_f16(const float16_t * FUNC_RESTRICT src, const float16_t * FUNC_RESTRICT weights, float16_t *out, uint32_t numofvec, uint32_t dimofvec)
{
    float16_t acc = 0.0f;
    uint32_t cntdim;
    memset(out, 0.0f, dimofvec * sizeof(float16_t));
    float16_t *pout;
    float16_t tmp_weight;
    while (numofvec > 0)
    {
        pout = out;
        tmp_weight = *weights;
        weights++;
        acc += tmp_weight;
        cntdim = dimofvec;
        while (cntdim > 0)
        {
            *pout = *pout + *src * tmp_weight;
            src++;
            pout++;
            cntdim--;
        }
        numofvec--;
    }
    cntdim = dimofvec;
    pout = out;
    while (cntdim > 0)
    {
        *pout = *pout / acc;
        pout++;
        cntdim--;
    }
}

/**
 * @}
 */

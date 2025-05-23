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

#include <internal_config.h>
#include "riscv_vec_svm.h"
#include "riscv_vec_utils.h"
#include "internal_vec_utils.h"
#include "math.h"

/**
 * @ingroup svm
 */

/**
 * @addtogroup groupSVM
 * @{
 */
/**
 * @brief SVM linear prediction
 * @param[in]    S          Pointer to an instance of the linear SVM structure.
 * @param[in]    src         Pointer to input vector
 * @param[out]   result    Decision value
 * @return none.
 *
 */
void riscv_vec_svm_linear_est_f32(
    const riscv_vec_svm_linear_f32_t * FUNC_RESTRICT instance,
    const float32_t * FUNC_RESTRICT src,
    int32_t * FUNC_RESTRICT result)
{
    float32_t sum = instance->intercept;
    float32_t dot = 0;
    uint32_t i, j;
    const float32_t *pvec = instance->vec;
    uint32_t cntvec = instance->numofvec;
    uint32_t cntdim = instance->dimofvec;
    const float32_t *pcoe = instance->dualcoe;

    for (i = 0; i < cntvec; i++)
    {
        dot = 0;
        for (j = 0; j < cntdim; j++)
        {
            dot = dot + src[j] * *pvec++;
        }
        sum += *pcoe++ * dot;
    }

    uint32_t idx = sum <= 0 ? 0 : 1;
    *result = instance->classes[idx];
}

/**
 * @} end of groupSVM group
 */

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
#include "internal_vec_utils.h"
#include "riscv_vec_statistics.h"
#include "math.h"

/**
 * @ingroup statistics
 */

/**
 * @addtogroup bayes
 * @{
 */
/**
 * @brief Naive Gaussian Bayesian Estimator
 *
 * @param[in]  *instance  points to a naive bayes instance
 * @param[in]  *src       points to the elements of the input vector.
 * @param[in]  *buf       points to a buffer of length numofclass
 * @return The predicted class
 *
 */
uint32_t riscv_vec_gaussian_naive_bayes_est_f16(const riscv_vec_gaussian_naivebayes_f16_t *instance,
        const float16_t * src,
        float16_t *buf)
{
    uint32_t nbclass = instance->numofclass;
    uint32_t nbdim = instance->dimofvec;
    const float16_t *pPrior = instance->classprior;
    const float16_t *pmean = instance->mean;
    const float16_t *pvar = instance->var;
    float16_t addvar = instance->additiveofvar;
    float16_t *buffer = buf;
    const float16_t *pIn = src;
    float16_t sigma;
    float16_t tmp, tmpin;
    float16_t acc1, acc2;
    uint32_t idx;
    uint32_t cntclass, cntdim;

    for (cntclass = 0; cntclass < nbclass; cntclass++)
    {
        pIn = src;
        tmp = 0.0;
        acc1 = 0.0f;
        acc2 = 0.0f;
        for (cntdim = 0; cntdim < nbdim; cntdim++)
        {
            sigma = *pvar++ + addvar;
            tmpin = *pIn - *pmean++;
            acc1 += logf(2.0f * CONST_PI * sigma);
            acc2 += tmpin * tmpin / sigma;
            pIn++;
        }
        tmp = -0.5f * acc1;
        tmp -= 0.5f * acc2;
        *buffer = tmp + logf(*pPrior++);
        buffer++;
    }
    riscv_vec_max_f16(buf, nbclass, &idx);
    return(idx);
}

/**
 * @} end of bayes.
 */
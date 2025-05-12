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

#include "internal_config.h"
#include "riscv_vec_basic.h"
#include "riscv_vec_statistics.h"
#include "riscv_vec_utils.h"
#include <math.h>

/**
 * @brief        Correlation distance between two vectors
 *
 * The input vectors are modified in place !
 *
 * @param[in]    src1         First vector
 * @param[in]    src2         Second vector
 * @param[in]    size         vector length
 * @return distance
 *
 */
float16_t riscv_vec_dist_corr_f16(const float16_t * FUNC_RESTRICT src1, const float16_t * FUNC_RESTRICT src2, uint32_t size)
{
    float16_t mean1 = riscv_vec_mean_f16(src1, size);
    float16_t mean2 = riscv_vec_mean_f16(src2, size);
    riscv_vec_addc_f16((float16_t *)src1, (float16_t *)src1, -mean1, size);
    riscv_vec_addc_f16((float16_t *)src2, (float16_t *)src2, -mean2, size);
    float16_t pwr1 = riscv_vec_pwr_f16(src1, size);
    float16_t pwr2 = riscv_vec_pwr_f16(src2, size);
    float16_t dot = riscv_vec_dprod_f16((float16_t *)src1, (float16_t *)src2, size);

    dot = dot / size;
    pwr1 = pwr1 / size;
    pwr2 = pwr2 / size;
    float16_t tmp = pwr1 * pwr2;
    float16_t sq;
    riscv_vec_sqrt_f16(&tmp, &sq, 1);
    float16_t out = 1.0f - dot / sq;
    return out;
}

/**
 * @} end of groupSVM group
 */

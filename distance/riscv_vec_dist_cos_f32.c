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
 * @brief        Cosine distance between two vectors
 *
 * The input vectors are modified in place !
 *
 * @param[in]    src1         First vector
 * @param[in]    src2         Second vector
 * @param[in]    size         vector length
 * @return distance
 *
 */
float32_t riscv_vec_dist_cos_f32(const float32_t * FUNC_RESTRICT src1, const float32_t * FUNC_RESTRICT src2, uint32_t size)
{
    float32_t pwr1 = riscv_vec_pwr_f32(src1, size);
    float32_t pwr2 = riscv_vec_pwr_f32(src2, size);
    float32_t dot = riscv_vec_dprod_f32((float32_t *)src1, (float32_t *)src2, size);
    float32_t tmp = pwr1 * pwr2;
    float32_t sq;
    riscv_vec_sqrt_f32(&tmp, &sq, 1);
    float32_t out = 1.0f - dot / sq;
    return out;
}

/**
 * @} end of groupSVM group
 */

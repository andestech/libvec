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
#include "riscv_vec_utils.h"
#include "math.h"

/**
 * @brief        Euclidean distance between two vectors
 * @param[in]    src1         First vector
 * @param[in]    src2         Second vector
 * @param[in]    size         vector length
 * @return distance
 *
 */
float16_t riscv_vec_dist_euclidean_f16(const float16_t * FUNC_RESTRICT src1, const float16_t * FUNC_RESTRICT src2, uint32_t size)
{
    float16_t accsum = 0.0f;
    float16_t tmp, out;

    while (size != 0)
    {
        tmp = *src1++ - *src2++;
        accsum += tmp * tmp;
        size--;
    }
    riscv_vec_sqrt_f16(&accsum, &out, 1);
    return out;
}

/**
 * @} end of groupSVM group
 */

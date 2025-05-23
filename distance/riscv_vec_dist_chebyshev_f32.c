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
#include <math.h>

/**
 * @brief        Chebyshev distance between two vectors
 * @param[in]    src1         First vector
 * @param[in]    src2         Second vector
 * @param[in]    size         vector length
 * @return distance
 *
 */
float32_t riscv_vec_dist_chebyshev_f32(const float32_t * FUNC_RESTRICT src1, const float32_t * FUNC_RESTRICT src2, uint32_t size)
{
    float32_t tmp1, tmp2, max, diff;

    tmp1 = *src1++;
    tmp2 = *src2++;
    diff = fabsf(tmp1 - tmp2);
    max = diff;
    size--;

    while (size != 0)
    {
        tmp1 = *src1++;
        tmp2 = *src2++;
        diff = fabsf(tmp1 - tmp2);
        max = (diff > max) ? diff : max;
        size--;
    }
    return max;
}

/**
 * @} end of groupSVM group
 */

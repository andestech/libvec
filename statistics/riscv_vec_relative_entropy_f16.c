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
#include "math.h"

/**
 * @ingroup statistics
 */

/**
 * @brief Entropy of the floating-potint vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return Entropy value.
 *
 * E = -sum (P .* ln (P))
 */

/* function description */
float16_t riscv_vec_relative_entropy_f16(const float16_t * FUNC_RESTRICT src1, const float16_t * FUNC_RESTRICT src2, uint32_t size)
{
    uint32_t cnt = size;
    float16_t acc, p1, p2, out;

    acc = 0.0f;
    while (cnt != 0)
    {
        p1 = *src1++;
        p2 = *src2++;
        acc += p1 * logf(p2 / p1);
        cnt--;
    }
    out = -acc;
    return out;
}
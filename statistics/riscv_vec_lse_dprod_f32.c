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
#include "riscv_vec_basic.h"
#include "riscv_vec_statistics.h"
#include "math.h"

/**
 * @ingroup statistics
 */

/**
 * @brief Dot product with Log-Sum-Exp of the floating-potint vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return the Log-Sum-Exp of dot product value.
 *
 */

/* function description */
float32_t riscv_vec_lse_dprod_f32(const float32_t * FUNC_RESTRICT src1, const float32_t * FUNC_RESTRICT src2, uint32_t size, float32_t * FUNC_RESTRICT buffer)
{
    riscv_vec_add_f32(buffer, (float32_t*)src1, (float32_t*)src2, size);
    float32_t out = riscv_vec_lse_f32(buffer, size);
    return(out);
}

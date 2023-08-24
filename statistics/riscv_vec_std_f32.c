/******************************************************************************
 * Copyright (C) 2010-2023 Arm Limited or its affiliates. All rights reserved.*
 * Copyright (C) 2018-2023 Andes Technology Corporation. All rights reserved. *
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

#include <math.h>
#include "internal_nds_types.h"

extern float32_t riscv_vec_var_f32(const float32_t * FUNC_RESTRICT src, uint32_t size);

/* function description */
float32_t riscv_vec_std_f32(const float32_t * FUNC_RESTRICT src, uint32_t size)
{
    float32_t dst = 0.0f;
    float32_t tmp2 = riscv_vec_var_f32(src, size);
    dst = (tmp2 >= 0) ? sqrtf(tmp2) : -1.0f;
    return dst;
}

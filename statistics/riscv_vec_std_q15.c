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

#include "internal_nds_types.h"

extern void riscv_vec_sqrt_q15(q15_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size);
extern q31_t riscv_vec_var_q15(const q15_t * FUNC_RESTRICT src, uint32_t size);

/* function description */
q15_t riscv_vec_std_q15(const q15_t * FUNC_RESTRICT src, uint32_t size)
{
    q15_t dst;
    q15_t tmp = riscv_vec_var_q15(src, size);
    riscv_vec_sqrt_q15(&tmp, &dst, 1);
    return dst;
}

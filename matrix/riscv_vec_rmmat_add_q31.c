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

#include "riscv_vec_basic.h"
#include "internal_nds_types.h"

/* function description */
int riscv_vec_rmmat_add_q31(const q31_t * FUNC_RESTRICT src1, const q31_t * FUNC_RESTRICT src2,
                            q31_t * FUNC_RESTRICT dst, uint32_t row, uint32_t col)
{
    uint32_t size = row * col;
    riscv_vec_add_q31(dst, (q31_t *)src1, (q31_t *)src2, size);
    return RISCV_VEC_OK;
}

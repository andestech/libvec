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
#define ENA_MAT_MUL_VQ
typedef q15_t MM_TYPE;//define internal_vec_mat_mul_tiling.h's data type
typedef q63_t MM_TYPE_VQ;//define internal_vec_mat_mul_tiling.h's data type
#include "internal_vec_mat_mul_tiling.h"
#include "riscv_vec_basic.h"

/* function description */
int riscv_vec_rmmat_mul_q15_q63(const MM_TYPE* FUNC_RESTRICT src1, const MM_TYPE* FUNC_RESTRICT src2, MM_TYPE_VQ* FUNC_RESTRICT dst, uint32_t row, uint32_t col, uint32_t col2)
{

    /**
     * mul_q15_q63
     * The function is implemented using an internal 64-bit accumulator.
     * The inputs to the multiplications are in 1.15 format, and the multiplications yield a 2.30 result.
     * The 2.30 intermediate results are accumulated in a 64-bit accumulator in 34.30 format.
     * This approach provides 33 guard bits, ensuring there is no risk of overflow.
     */
    vec_mat_mul_purec_original_vq(src1, src2, dst, row, col, col2);
    return NDSV_OK;
}

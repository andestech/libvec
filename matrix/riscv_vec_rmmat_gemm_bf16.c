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
typedef bf16_t MM_TYPE;//define internal_vec_mat_mul_tiling.h's data type
#include "internal_vec_mat_mul_tiling.h"

/* function description */
int riscv_vec_rmmat_gemm_bf16(const MM_TYPE* FUNC_RESTRICT src1, const MM_TYPE* FUNC_RESTRICT src2, MM_TYPE* FUNC_RESTRICT dst,
                              const MM_TYPE alpha, const MM_TYPE beta,
                              uint32_t row, uint32_t col, uint32_t col2)
{

    gemm_purec_original(src1, src2, dst, alpha, beta, row, col, col2);
    return NDSV_OK;
}

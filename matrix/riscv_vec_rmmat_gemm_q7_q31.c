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
typedef q7_t MM_TYPE;//define internal_vec_mat_mul_tiling.h's data type
typedef q31_t MM_TYPE_VQ;//define internal_vec_mat_mul_tiling.h's data type
#include "internal_vec_mat_mul_tiling.h"

extern int riscv_vec_mulc_q31(MM_TYPE_VQ * dst, MM_TYPE_VQ * src, const MM_TYPE_VQ scale, const int8_t shift, uint32_t count);

/* function description */
int riscv_vec_rmmat_gemm_q7_q31(const MM_TYPE * FUNC_RESTRICT src1, const MM_TYPE * FUNC_RESTRICT src2, MM_TYPE_VQ * FUNC_RESTRICT dst,
                                const MM_TYPE alpha, const MM_TYPE beta,
                                uint32_t row, uint32_t col, uint32_t col2, int32_t shift)
{
    int shift1 = 7 + shift;
    /**
     * gemm_q7_q31
     * The function is implemented using an internal 32-bit accumulator.
     * The inputs to the multiplications are in 1.7 format, and the multiplications yield a 2.14 result.
     * The 2.14 intermediate results are accumulated in a 32-bit accumulator in 18.14 format.
     * This approach provides 17 guard bits, ensuring there is no risk of overflow.
     * The 18.14 accumulated result is multiplied by a 1.7 (alpha), and the final output is saturated into 18.14 format.
     */
    vec_gemm_purec_original_vq(src1, src2, dst, alpha, beta, row, col, col2, shift1);// (q14 * q7 + q7 * q14) >> shift1
    return NDSV_OK;
}

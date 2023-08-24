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
typedef float64_t MM_TYPE;//define internal_vec_mat_mul_tiling.h's data type
#include "internal_vec_mat_mul_tiling.h"

extern int riscv_vec_mulc_f64(float64_t * dst, float64_t * src, const float64_t cst, uint32_t count);

int riscv_vec_rmmat_gemm_f64(const MM_TYPE* FUNC_RESTRICT src1, const MM_TYPE* FUNC_RESTRICT src2, MM_TYPE* FUNC_RESTRICT dst,
                             const MM_TYPE alpha, const MM_TYPE beta,
                             uint32_t row, uint32_t col, uint32_t col2)
{
#ifdef ENA_PUREC_TILING
#ifdef ENA_DYNAMIC_CALC_CACHE_CONFIG
    unsigned int set, way, line, tiling_size = 1;
    calc_tiling_size_and_cache_config(&set, &way, &line, &tiling_size);
#else
    //cache is 512KB, type is f64
    const unsigned int tiling_size = 128;
#endif
    uint32_t count = row * col2;
    float64_t *dst2 = dst;
    riscv_vec_mulc_f64(dst2, dst, beta, count);
    gemm_purec_tiling(src1, src2, dst2, alpha, row, col, col2, tiling_size);
#else
    gemm_purec_original(src1, src2, dst, alpha, beta, row, col, col2);
#endif
    return NDSV_OK;
}

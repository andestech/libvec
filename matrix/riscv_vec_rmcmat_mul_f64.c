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
typedef float64_t MM_TYPE;//define internal_vec_mat_mul_tiling.h's data type

#include "internal_vec_mat_mul_tiling.h"
#include "riscv_vec_basic.h"

/* function description */
int riscv_vec_rmcmat_mul_f64(const MM_TYPE * FUNC_RESTRICT src1, const MM_TYPE * FUNC_RESTRICT src2,
                             MM_TYPE * FUNC_RESTRICT dst, uint32_t row, uint32_t col, uint32_t col2)
{
#ifdef ENA_PUREC_TILING
#ifdef ENA_DYNAMIC_CALC_CACHE_CONFIG
    unsigned int set, way, line, tiling_size = 1;
    calc_clx_tiling_size_and_cache_config(&set, &way, &line, &tiling_size);
#else
    //cache is 512KB, type is f64
    const unsigned int tiling_size = 64;
#endif
    /* initail output */
    riscv_vec_setc_f64(dst, 0, row * col2 * 2);
    cmat_mul_purec_tiling(src1, src2, dst, row, col, col2, tiling_size);
#else
    vec_cmat_mul_purec_original(src1, src2, dst, row, col, col2);
#endif
    return NDSV_OK;
}

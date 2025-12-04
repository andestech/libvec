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

#include "internal_vec_transform.h"

#ifdef ENA_STOCKHAM_FFT
extern void riscv_vec_butterfly_bf16 (
    riscv_vec_cpx_bf16_t * FUNC_RESTRICT out,
    riscv_vec_cpx_bf16_t * FUNC_RESTRICT in,
    q31_t * FUNC_RESTRICT factors,
    riscv_vec_cpx_bf16_t * FUNC_RESTRICT twiddles,
    riscv_vec_cpx_bf16_t * FUNC_RESTRICT buffer);

extern void _vec_split_r2c_1d_bf16 (
    riscv_vec_cpx_bf16_t * FUNC_RESTRICT dst,
    const riscv_vec_cpx_bf16_t * FUNC_RESTRICT src,
    riscv_vec_cpx_bf16_t * FUNC_RESTRICT twiddles,
    q31_t ncfft);

extern void riscv_vec_butterfly_f16 (
    riscv_vec_cpx_f16_t * FUNC_RESTRICT out,
    riscv_vec_cpx_f16_t * FUNC_RESTRICT in,
    q31_t * FUNC_RESTRICT factors,
    riscv_vec_cpx_f16_t * FUNC_RESTRICT twiddles,
    riscv_vec_cpx_f16_t * FUNC_RESTRICT buffer);

void riscv_vec_rfft_bf16 (riscv_vec_cpx_bf16_t * FUNC_RESTRICT out,
                          bf16_t * FUNC_RESTRICT in,
                          riscv_vec_rfft_cfg_bf16_t cfg)
{
    if(cfg != NULL)
    {
        riscv_vec_cpx_bf16_t * tmpbuf = cfg->buffer;
        riscv_vec_butterfly_bf16 (tmpbuf, (riscv_vec_cpx_bf16_t*) in, cfg->factors, cfg->twiddles, out);
        _vec_split_r2c_1d_bf16 (out, tmpbuf, cfg->super_twiddles, cfg->ncfft);
    }
}
#endif  //ENA_STOCKHAM_FFT

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
extern void riscv_vec_butterfly_q31 (
    riscv_vec_cpx_q31_t * FUNC_RESTRICT Fout,
    riscv_vec_cpx_q31_t * FUNC_RESTRICT Fin,
    q31_t * FUNC_RESTRICT factors,
    riscv_vec_cpx_q31_t * FUNC_RESTRICT twiddles,
    riscv_vec_cpx_q31_t * FUNC_RESTRICT buffer,
    q31_t scaled_flag);

extern void riscv_vec_rfft_split_q31 (
    riscv_vec_cpx_q31_t * FUNC_RESTRICT dst,
    const riscv_vec_cpx_q31_t * FUNC_RESTRICT src,
    riscv_vec_cpx_q31_t * FUNC_RESTRICT twiddles,
    q31_t ncfft,
    q31_t scaled_flag);

void  riscv_vec_rfft_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT out,
                          q31_t * FUNC_RESTRICT in,
                          riscv_vec_rfft_cfg_q31_t cfg,
                          q31_t scaled_flag)
{
    if( cfg != NULL)
    {
        riscv_vec_cpx_q31_t * tmpbuf = cfg->buffer;

        riscv_vec_butterfly_q31 (tmpbuf, (riscv_vec_cpx_q31_t*) in, cfg->factors, cfg->twiddles, out, scaled_flag);
        riscv_vec_rfft_split_q31 (out, tmpbuf, cfg->super_twiddles, cfg->ncfft, scaled_flag);
    }
}
#endif  //ENA_STOCKHAM_FFT

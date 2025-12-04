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
extern void riscv_vec_inverse_butterfly_q31 (
    riscv_vec_cpx_q31_t * FUNC_RESTRICT Fout,
    riscv_vec_cpx_q31_t * FUNC_RESTRICT Fin,
    q31_t * FUNC_RESTRICT factors,
    riscv_vec_cpx_q31_t * FUNC_RESTRICT twiddles,
    riscv_vec_cpx_q31_t * FUNC_RESTRICT buffer,
    q31_t scaled_flag);

extern void riscv_vec_butterfly_q31 (
    riscv_vec_cpx_q31_t * FUNC_RESTRICT Fout,
    riscv_vec_cpx_q31_t * FUNC_RESTRICT Fin,
    q31_t * FUNC_RESTRICT factors,
    riscv_vec_cpx_q31_t * FUNC_RESTRICT twiddles,
    riscv_vec_cpx_q31_t * FUNC_RESTRICT buffer,
    q31_t scaled_flag);

void  riscv_vec_cfft_q31 (riscv_vec_cpx_q31_t * FUNC_RESTRICT out,
                          riscv_vec_cpx_q31_t * FUNC_RESTRICT in,
                          riscv_vec_cfft_cfg_q31_t cfg,
                          q31_t inverse_fft,
                          q31_t scaled_flag)
{
    if( cfg != NULL)
    {
        if (inverse_fft)
        {
            riscv_vec_inverse_butterfly_q31 (out, in, cfg->factors, cfg->twiddles, cfg->buffer, scaled_flag);
        }
        else
        {
            riscv_vec_butterfly_q31 (out, in, cfg->factors, cfg->twiddles, cfg->buffer, scaled_flag);
        }
    }
}

#endif  //ENA_STOCKHAM_FFT

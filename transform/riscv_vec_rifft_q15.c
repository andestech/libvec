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
extern void riscv_vec_inverse_butterfly_q15 (
    riscv_vec_cpx_q15_t * FUNC_RESTRICT Fout,
    riscv_vec_cpx_q15_t * FUNC_RESTRICT Fin,
    q31_t * FUNC_RESTRICT factors,
    riscv_vec_cpx_q15_t * FUNC_RESTRICT twiddles,
    riscv_vec_cpx_q15_t * FUNC_RESTRICT buffer,
    q31_t scaled_flag);

extern void riscv_vec_rifft_split_q15 (
    riscv_vec_cpx_q15_t * FUNC_RESTRICT dst,
    const riscv_vec_cpx_q15_t * FUNC_RESTRICT src,
    riscv_vec_cpx_q15_t * FUNC_RESTRICT twiddles,
    q31_t ncfft,
    q31_t scaled_flag);

#ifdef ENA_SMALL_FFT_INDV
#include "internal_vec_transform_small_fft.h"
#if !defined(ENA_HVM)
extern void riscv_vec_inverse_butterfly_16pt_q15(
    riscv_vec_cpx_q15_t * FUNC_RESTRICT Fout,
    riscv_vec_cpx_q15_t * FUNC_RESTRICT Fin,
    q31_t * FUNC_RESTRICT factors,
    riscv_vec_cpx_q15_t * FUNC_RESTRICT twiddles,
    riscv_vec_cpx_q15_t * FUNC_RESTRICT buffer,
    q31_t scaled_flag);
#endif
#endif

void  riscv_vec_rifft_q15 (q15_t * FUNC_RESTRICT out,
                          riscv_vec_cpx_q15_t * FUNC_RESTRICT in,
                          riscv_vec_rfft_cfg_q15_t cfg,
                          q31_t scaled_flag)
{
    if( cfg != NULL)
    {
#ifdef ENA_SMALL_FFT_INDV
        riscv_vec_cpx_q15_t * tmpbuf1 = cfg->buffer;
        riscv_vec_cpx_q15_t * tmpbuf2 = cfg->buffer + cfg->ncfft;
        q31_t fft_size = cfg->ncfft;
        riscv_vec_rifft_split_q15 (tmpbuf1, in, cfg->super_twiddles, cfg->ncfft, scaled_flag);
        if(fft_size < 16)
        {
            riscv_vec_inverse_butterfly_q15_small ( (riscv_vec_cpx_q15_t*) out, tmpbuf1, cfg->factors, cfg->twiddles, tmpbuf2, scaled_flag);

        }
#if !defined(ENA_HVM)
        else if(fft_size == 16)
        {
            riscv_vec_inverse_butterfly_16pt_q15 ( (riscv_vec_cpx_q15_t*) out, tmpbuf1, cfg->factors, cfg->twiddles, tmpbuf2, scaled_flag);
        }
#endif
        else
        {
            riscv_vec_inverse_butterfly_q15 ( (riscv_vec_cpx_q15_t*) out, tmpbuf1, cfg->factors, cfg->twiddles, tmpbuf2, scaled_flag);
        }

#else
        riscv_vec_cpx_q15_t * tmpbuf1 = cfg->buffer;
        riscv_vec_cpx_q15_t * tmpbuf2 = cfg->buffer + cfg->ncfft;

        riscv_vec_rifft_split_q15 (tmpbuf1, in, cfg->super_twiddles, cfg->ncfft, scaled_flag);
        riscv_vec_inverse_butterfly_q15 ( (riscv_vec_cpx_q15_t*) out, tmpbuf1, cfg->factors, cfg->twiddles, tmpbuf2, scaled_flag);
#endif
    }
}
#endif  //ENA_STOCKHAM_FFT

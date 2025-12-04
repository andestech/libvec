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

#include "riscv_vec_matrix.h"

#ifdef ENA_STOCKHAM_FFT
extern void riscv_vec_butterfly_f16 (
    riscv_vec_cpx_f16_t * FUNC_RESTRICT out,
    riscv_vec_cpx_f16_t * FUNC_RESTRICT in,
    q31_t * FUNC_RESTRICT factors,
    riscv_vec_cpx_f16_t * FUNC_RESTRICT twiddles,
    riscv_vec_cpx_f16_t * FUNC_RESTRICT buffer);

extern void _vec_split_r2c_1d_f16 (
    riscv_vec_cpx_f16_t * FUNC_RESTRICT dst,
    const riscv_vec_cpx_f16_t * FUNC_RESTRICT src,
    riscv_vec_cpx_f16_t * FUNC_RESTRICT twiddles,
    q31_t ncfft);

void riscv_vec_rfft2d_f16 (riscv_vec_cpx_f16_t * FUNC_RESTRICT out,
                           float16_t * FUNC_RESTRICT in,
                           riscv_vec_rfft2d_cfg_f16_t cfg)
{
    q31_t tmp_idx = 0;
    riscv_vec_cpx_f16_t *tmp_1d = (riscv_vec_cpx_f16_t*)&cfg->buffer_1d[0];
    riscv_vec_cpx_f16_t *tmp_buff = (riscv_vec_cpx_f16_t*)&cfg->buffer_2d[0];
    riscv_vec_cpx_f16_t *tmp_out = NULL;
    float16_t *data_in = NULL;
    riscv_vec_cpx_f16_t *c_data_in = NULL;
    q31_t m = cfg->m;
    q31_t n = cfg->ncfft;
    float16_t *trans_in = NULL;
    float16_t *trans_out = NULL;

    if(cfg != NULL)
    {
        // 1st: process n-dim 1d-rfft for m-times
        for(tmp_idx = 0 ; tmp_idx < m ; tmp_idx++)
        {
            tmp_out = (riscv_vec_cpx_f16_t*) &tmp_buff[tmp_idx * (n + 1)];
            data_in = (float16_t*) &in[tmp_idx * n * 2];
            riscv_vec_butterfly_f16 (tmp_1d, (riscv_vec_cpx_f16_t*) data_in, cfg->factors_n, cfg->twiddles_n, tmp_out);
            _vec_split_r2c_1d_f16 (tmp_out, tmp_1d, cfg->super_twiddles, n);
        }
        trans_in = (float16_t*) &tmp_buff[0].r;
        trans_out = (float16_t*) &out[0].r;
        riscv_vec_rmcmat_trans_f16(trans_in, trans_out, m, (n + 1));
        // 2nd: process m-dim 1d-cfft for (ncfft+1) times
        for(tmp_idx = 0; tmp_idx < n + 1 ; tmp_idx++)
        {
            tmp_out = (riscv_vec_cpx_f16_t*) &tmp_buff[tmp_idx * m];
            c_data_in = (riscv_vec_cpx_f16_t*) &out[tmp_idx * m];
            riscv_vec_butterfly_f16(tmp_out, c_data_in, cfg->factors_m, cfg->twiddles_m, tmp_1d);
        }
        trans_in = (float16_t*) &tmp_buff[0].r;
        trans_out = (float16_t *) &out[0].r;
        riscv_vec_rmcmat_trans_f16(trans_in, trans_out, (n + 1), m);
    }
}

#endif  //ENA_STOCKHAM_FFT

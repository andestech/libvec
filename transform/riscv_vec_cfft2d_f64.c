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
extern void riscv_vec_inverse_butterfly_f64 (
    riscv_vec_cpx_f64_t *out,
    riscv_vec_cpx_f64_t *in,
    q31_t *factors,
    riscv_vec_cpx_f64_t *twiddles,
    riscv_vec_cpx_f64_t *buffer);

extern void riscv_vec_butterfly_f64 (
    riscv_vec_cpx_f64_t * FUNC_RESTRICT out,
    riscv_vec_cpx_f64_t * FUNC_RESTRICT in,
    q31_t * FUNC_RESTRICT factors,
    riscv_vec_cpx_f64_t * FUNC_RESTRICT twiddles,
    riscv_vec_cpx_f64_t * FUNC_RESTRICT buffer);

void riscv_vec_cfft2d_f64 (riscv_vec_cpx_f64_t * FUNC_RESTRICT out,
                           riscv_vec_cpx_f64_t * FUNC_RESTRICT in,
                           riscv_vec_cfft2d_cfg_f64_t cfg,
                           q31_t inverse_fft)
{
    q31_t tmp_idx = 0;
    riscv_vec_cpx_f64_t *tmp_1d = (riscv_vec_cpx_f64_t*)&cfg->buffer_1d[0];
    riscv_vec_cpx_f64_t *tmp_buff = (riscv_vec_cpx_f64_t*)&cfg->buffer_2d[0];
    riscv_vec_cpx_f64_t *tmp_out = NULL;
    riscv_vec_cpx_f64_t *data_in = NULL;
    q31_t m = cfg->m;
    q31_t n = cfg->n;
    float64_t *trans_in = NULL;
    float64_t *trans_out = NULL;
    if(cfg != NULL)
    {
        if(inverse_fft)
        {
            // 1st: process n-dim 1d-ifft for m-times
            for(tmp_idx = 0 ; tmp_idx < m ; tmp_idx++)
            {
                tmp_out = (riscv_vec_cpx_f64_t*) &tmp_buff[tmp_idx * n];
                data_in = (riscv_vec_cpx_f64_t*) &in[tmp_idx * n];
                riscv_vec_inverse_butterfly_f64 (tmp_out, data_in, cfg->factors_n, cfg->twiddles_n, tmp_1d);
            }
            trans_in = (float64_t*) &tmp_buff[0].r;
            trans_out = (float64_t*) &out[0].r;
            riscv_vec_rmcmat_trans_f64(trans_in, trans_out, m, n);
            // 2nd: process m-dim 1d-fft for n times
            for(tmp_idx = 0; tmp_idx < n ; tmp_idx++)
            {
                tmp_out = (riscv_vec_cpx_f64_t*) &tmp_buff[tmp_idx * m];
                data_in = (riscv_vec_cpx_f64_t*) &out[tmp_idx * m];
                riscv_vec_inverse_butterfly_f64(tmp_out, data_in, cfg->factors_m, cfg->twiddles_m, tmp_1d);
            }
            trans_in = (float64_t*) &tmp_buff[0].r;
            trans_out = (float64_t *) &out[0].r;
            riscv_vec_rmcmat_trans_f64(trans_in, trans_out, n, m);
        }
        else
        {
            // 1st: process n-dim 1d-fft for m-times
            for(tmp_idx = 0 ; tmp_idx < m ; tmp_idx++)
            {
                tmp_out = (riscv_vec_cpx_f64_t*) &tmp_buff[tmp_idx * n];
                data_in = (riscv_vec_cpx_f64_t*) &in[tmp_idx * n];
                riscv_vec_butterfly_f64 (tmp_out, data_in, cfg->factors_n, cfg->twiddles_n, tmp_1d);

            }
            trans_in = (float64_t*) &tmp_buff[0].r;
            trans_out = (float64_t*) &out[0].r;
            riscv_vec_rmcmat_trans_f64(trans_in, trans_out, m, n);
            // 2nd: process m-dim 1d-fft for n times
            for(tmp_idx = 0; tmp_idx < n ; tmp_idx++)
            {
                tmp_out = (riscv_vec_cpx_f64_t*) &tmp_buff[tmp_idx * m];
                data_in = (riscv_vec_cpx_f64_t*) &out[tmp_idx * m];
                riscv_vec_butterfly_f64 (tmp_out, data_in, cfg->factors_m, cfg->twiddles_m, tmp_1d);
            }

            trans_in = (float64_t*) &tmp_buff[0].r;
            trans_out = (float64_t *) &out[0].r;
            riscv_vec_rmcmat_trans_f64(trans_in, trans_out, n, m);
        }
    } //(cfg != NULL)
}

#endif  //ENA_STOCKHAM_FFT

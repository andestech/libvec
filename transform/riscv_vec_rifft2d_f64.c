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
extern void riscv_vec_inverse_butterfly_f64 (
    riscv_vec_cpx_f64_t * FUNC_RESTRICT out,
    riscv_vec_cpx_f64_t * FUNC_RESTRICT in,
    q31_t * FUNC_RESTRICT factors,
    riscv_vec_cpx_f64_t * FUNC_RESTRICT twiddles,
    riscv_vec_cpx_f64_t * FUNC_RESTRICT buffer);

extern void _vec_split_c2r_1d_f64 (
    riscv_vec_cpx_f64_t *dst,
    const riscv_vec_cpx_f64_t *src,
    riscv_vec_cpx_f64_t *twiddles,
    q31_t ncfft);

void riscv_vec_rifft2d_f64 (float64_t * FUNC_RESTRICT out,
                            riscv_vec_cpx_f64_t * FUNC_RESTRICT in,
                            riscv_vec_rfft2d_cfg_f64_t cfg)
{
    q31_t tmp_idx = 0, tmp_cp = 0;
    riscv_vec_cpx_f64_t *tmp_1d = (riscv_vec_cpx_f64_t*)&cfg->buffer_1d[0];
    riscv_vec_cpx_f64_t *tmp_buff = (riscv_vec_cpx_f64_t*)&cfg->buffer_2d[0];
    riscv_vec_cpx_f64_t *tmp_out = NULL;
    riscv_vec_cpx_f64_t *tmp_in = (riscv_vec_cpx_f64_t*)&cfg->tmp_1d_input[0];
    riscv_vec_cpx_f64_t *data_in = NULL;
    float64_t *data_out = NULL;
    riscv_vec_cpx_f64_t *cp_data_in = in;
    q31_t m = cfg->m;
    q31_t n = cfg->ncfft;
    if( cfg != NULL)
    {
        // 1st: process (n+1) times cifft for m-pts
        for(tmp_idx = 0; tmp_idx < n + 1 ; tmp_idx++)
        {
            tmp_out = (riscv_vec_cpx_f64_t*) &tmp_buff[tmp_idx * (m)];
            cp_data_in = &in[tmp_idx];
            data_in = tmp_in;
            for(tmp_cp = 0 ; tmp_cp < m ; tmp_cp++)
            {
                data_in[tmp_cp].r = cp_data_in[tmp_cp * (n + 1)].r;
                data_in[tmp_cp].i = cp_data_in[tmp_cp * (n + 1)].i;
            }

            riscv_vec_inverse_butterfly_f64 (tmp_out, data_in, cfg->factors_m, cfg->twiddles_m, tmp_1d);
        }

        // 2nd: process m-times rifft for n-pts
        for(tmp_idx = 0 ; tmp_idx < m ; tmp_idx++)
        {
            data_out = (float64_t*) &out[tmp_idx * (2 * n)];
            cp_data_in = &tmp_buff[tmp_idx];
            data_in = tmp_in;
            riscv_vec_cpx_f64_t * tmp_split = cfg-> tmp_split;
            for(tmp_cp = 0 ; tmp_cp < (n + 1) ; tmp_cp++)
            {
                data_in[tmp_cp].r = cp_data_in[tmp_cp * (m)].r;
                data_in[tmp_cp].i = cp_data_in[tmp_cp * (m)].i;
            }

            _vec_split_c2r_1d_f64 (tmp_split, data_in, cfg->super_twiddles, cfg->ncfft);
            riscv_vec_inverse_butterfly_f64 ((riscv_vec_cpx_f64_t*) data_out, tmp_split, cfg->factors_n, cfg->twiddles_n, tmp_1d);
        }
    }
}

#endif  //ENA_STOCKHAM_FFT

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

/** @file*/

//// Vector Power (Vector-Vector) Functions

#include "internal_nds_types.h"
#include "internal_vec_utils.h"   // _log_f32
#include <math.h>
#define LOG10_E 0.43429448190325181667f

/* function description */
float32_t riscv_vec_snr_f32(float32_t * FUNC_RESTRICT signal, float32_t * FUNC_RESTRICT noise, uint32_t size)
{
    float32_t SNR = 0.0f;
    float pwr_signal = 0.0, pwr_noise = 0.0;
    while (size != 0u)
    {
        float tmp_signal = *signal++;
        float tmp_noise = *noise++;
        pwr_signal += tmp_signal * tmp_signal;
        pwr_noise += tmp_noise * tmp_noise;
        size--;
    }
    SNR = 10 * log10f(pwr_signal / pwr_noise);
    return SNR;
}

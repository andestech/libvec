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
//#include "internal_nds_types.h"

#ifdef ENA_MEASURE_HEAP
#include <stdio.h>
#endif

#ifdef ENA_STOCKHAM_FFT
/*
 * This function outputs a factor buffer ('facbuf') that decomposes an FFT of input size
 * n into a number of radix-r butterfly calculations (for r in some set of radix values).
 *
 * Factor buffer layout:
 *     index 0: buffer init info
 *     index 1: stride for the first stage (also for the info of fft_size)
 *     index 2: RFFT flag ( 1: RFFT / 0: CFFT)
 *
 * for C version, stockham FFT using radix-2 only.
 * for vector version, stockham fft using radix-4 + radix-2.
 *    example:
 *        fft size 1024 (2^10) -> radix 4 for all stage
 *        ffr size 2048 (2^11) -> radix 2 for the last stage and radix-4 for the others.
 *
 * Only the leading 42 int32 is used to store factors.
 * The left can be used as algorithm flags, or status flags.
 * Even the leading bits of stage number can be reused.
 */

q31_t riscv_vec_create_factor (q31_t n,
                               q31_t * FUNC_RESTRICT facbuf,
                               q31_t vec_factor_flags)
{
    q31_t p = 2;
    q31_t stage_num = 0;
    q31_t stride_max = n;
    if( n > 1)
    {
        do
        {
            n /= p;
            stage_num++;
        }
        while (n > 1);
    }
    facbuf[OUT_INDEX] = stage_num;
    facbuf[SIZE_FIRST_STAGE] = stride_max / p;
    // facbuf[2 * i] = alg_flag;
    return NDSV_OK;
}
#endif  //ENA_SOTCLHAM_FFT

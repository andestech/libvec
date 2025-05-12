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
#include "riscv_vec_utils.h"

/* function description */
q31_t riscv_vec_rms_q31(const q31_t * FUNC_RESTRICT src, uint32_t size)
{
    if(size <= 0)
    {
        return 0;
    }
    q63_t sum = 0;
    q31_t tmp, dst;
    uint32_t i;

    i = size;
    while (i != 0u)
    {
        tmp = *src++;
        sum += (q63_t) tmp * tmp;
        i--;
    }
#ifdef __NDS32_EXT_PERF__
    tmp = NDS_ISA_SATS(sum >> 31, 31u);
#else
    //It is well for only checking if larger than (INT32_MAX >> 1) due to the value of sum is always positive.
    //The format of tmp is Q31.
    tmp = (q31_t)(sum >> 31) > (INT32_MAX >> 1) ? (INT32_MAX >> 1) : (q31_t)(sum >> 31);
#endif
    tmp = (int32_t)((uint32_t) tmp / (uint32_t) size);
    riscv_vec_sqrt_q31(&tmp, &dst, 1);

    return dst;
}

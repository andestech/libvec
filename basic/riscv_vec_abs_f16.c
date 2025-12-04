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

//// Vector Absolute Functions
#include "internal_nds_types.h"
#include <math.h>       //for fabs

int riscv_vec_abs_f16(float16_t * FUNC_RESTRICT dst, float16_t * FUNC_RESTRICT src, uint32_t count)
{
    NDSV_CHECK_DstSrc;
    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        dst[itr] = fabs (src[itr]);     // cycle 196,644

        //float16_t zero = (float16_t)0.0;
        //dst[itr] = (src[itr] > zero) ? src[itr] : -src[itr];      // cycle 542,474
    }
    return NDSV_OK;
}

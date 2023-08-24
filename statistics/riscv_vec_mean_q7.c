/******************************************************************************
 * Copyright (C) 2010-2023 Arm Limited or its affiliates. All rights reserved.*
 * Copyright (C) 2018-2023 Andes Technology Corporation. All rights reserved. *
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

/* function description */
q7_t riscv_vec_mean_q7(const q7_t * FUNC_RESTRICT src, uint32_t size)
{
    q31_t sum = 0, dst;
    uint32_t i = size;
    q31_t *temp_src = (q31_t *) src;

    //Check alignment of src pointer
    if (((size_t)src & 0x3) == 0)
    {
        while (i >= 4u)
        {
            q31_t temp;
            temp = *temp_src++;
            sum += (temp >> 24) + ((temp << 8) >> 24) \
                   + ((temp << 16) >> 24) + ((temp << 24) >> 24);
            i -= 4u;
        }

    }

    src = (q7_t *) temp_src;
    while (i != 0u)
    {
        sum += *src++;
        i--;
    }
    dst = (q7_t)(sum / (int32_t) size);
    return dst;
}

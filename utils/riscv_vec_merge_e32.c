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

#include "internal_vec_utils.h"
#include <stdbool.h>

void riscv_vec_merge_e32(void * FUNC_RESTRICT src1, void * FUNC_RESTRICT src2, void * FUNC_RESTRICT dst, bool * FUNC_RESTRICT src_select, uint32_t size)
{
    int32_t *src_tmp1 = src1;
    int32_t *src_tmp2 = src2;
    int32_t *dst_tmp = dst;
    bool *select_tmp = src_select;

    while(size > 0)
    {
        if(*select_tmp)
        {
            *dst_tmp = *src_tmp1;
        }
        else
        {
            *dst_tmp = *src_tmp2;
        }

        src_tmp1++;
        src_tmp2++;
        dst_tmp++;
        select_tmp++;
        size--;
    }
}

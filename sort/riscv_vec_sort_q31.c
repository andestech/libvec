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

#include "riscv_vec_sort.h"
#include "internal_nds_types.h"

extern void riscv_vec_sort_descend_q31(q31_t * src, q31_t * dst, uint32_t size);
extern void riscv_vec_sort_ascend_q31(q31_t * src, q31_t * dst, uint32_t size);

int riscv_vec_sort_q31(q31_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT dst, uint32_t size, riscv_vec_sort_order order)
{
    if (order == RISCV_VEC_SORT_DESCENDING)
    {
        riscv_vec_sort_descend_q31(src, dst, size);
    }
    else if (order == RISCV_VEC_SORT_ASCENDING)
    {
        riscv_vec_sort_ascend_q31(src, dst, size);
    }
    else
    {
        return RISCV_VEC_FAIL;
    }
    return RISCV_VEC_OK;
}

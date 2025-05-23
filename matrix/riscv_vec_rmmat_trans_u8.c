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

/* function description */
int riscv_vec_rmmat_trans_u8(const uint8_t * FUNC_RESTRICT src, uint8_t * FUNC_RESTRICT dst, uint32_t row, uint32_t col)
{
    //check if the input row or col is zero
    if ((row <= 0) || (col <= 0))
    {
        return RISCV_VEC_FAIL;
    }
    uint8_t *C;
    uint32_t i, colcnt, rowcnt;

    i = 0u;
    rowcnt = row;

    do
    {
        C = dst + i;

        colcnt = col;
        do
        {
            *C = *src++;
            C += row;
            colcnt--;
        }
        while (colcnt != 0u);

        i++;
        rowcnt--;
    }
    while (rowcnt != 0u);
    return RISCV_VEC_OK;
}

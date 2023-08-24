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
int riscv_vec_rmmat_trans_f32(const float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t row, uint32_t col)
{
    //check if the input row or col is zero
    if ((row <= 0) || (col <= 0))
    {
        return NDSV_FAIL;
    }
    float32_t *C;
    uint32_t i = 0, colcnt, rowcnt;
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
    return NDSV_OK;
}

/**
 * @} end of matrixtrans
 */

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
#include "riscv_vec_interp.h"

/* function description */
void riscv_vec_linear_interp_q15(riscv_vec_linear_interp_q15_t * FUNC_RESTRICT instance, q31_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size)
{
    const q15_t *py_tbl = instance->tbl_data;
    int32_t table_size = instance->tbl_size;

    const int32_t COE_LOWER_Q20 = 0x000FFFFF;
    const int32_t COE_UPPER_Q12 = 0xFFF00000;

    q63_t y;            /* output */
    q31_t fract;        /* fractional part */
    q31_t in_x;
    int32_t idx;
    q15_t y0, y1, out;  /* Nearest output values */

    for(uint32_t cnt = 0; cnt < size; cnt++)
    {
        in_x = *src++;

        /* Input is in 12.20 format */
        /* 12 bits for the table index */
        /* Index value calculation */
        idx = ((in_x & (int32_t)COE_UPPER_Q12) >> 20);

        if ((int32_t)idx >= (int32_t)(table_size - 1))
        {
            out = py_tbl[table_size - 1];
        }
        else if (idx < 0)
        {
            out = py_tbl[0];
        }
        else
        {
            /* 20 bits for the fractional part */
            /* fract is in 12.20 format */
            fract = (in_x & COE_LOWER_Q20);

            /* Read two nearest output values from the index */
            y0 = py_tbl[idx];
            y1 = py_tbl[idx + 1];

            /* Calculation of y0 * (1-fract) and y is in 13.35 format */
            y = ((q63_t) y0 * (COE_LOWER_Q20 - fract));

            /* Calculation of (y0 * (1-fract) + y1 * fract) and y is in 13.35 format */
            y += ((q63_t) y1 * (fract));

            /* convert y to 1.15 format */
            out = (q15_t) (y >> 20);
        }
        *dst++ = out;
    }
}

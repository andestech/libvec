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

#include "internal_vec_utils.h"

/* function description */
void riscv_vec_sqrt_q31(q31_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT dst, uint32_t size)
{
    //C
    while (size != 0)
    {
        q31_t val, temp1, var1, signbit, halfval, out;

        val = *src++;
        if (val > 0)
        {
            signbit = NDS_ISA_CLZ(val) - 1;
            signbit &= ~0x1;
            val = val << signbit;
            halfval = val >> 1;
            temp1 = val;

            // The following formulas are derived from the above transformation.
            if (val <= 0x40000000) //input=16, cycle is 3,139,583
                var1 = (q31_t)(0x9BACEF80UL - (uint32_t) val);  // var1 + val = 2611801984 (0x9BACEF80)

            else if (val < 0x4AC00000)
                var1 = (q31_t)(0x9E4CEF80UL - (uint32_t) val);  // var1 + val ~= 2655842176 (0x9E4CEF80)

            else if (val < 0x56400001)
                var1 = (q31_t)(0xA40CEF80UL - (uint32_t) val);  // var1 + val ~= 2752311168 (0xA40CEF80)

            else if (val < 0x62400001)
                var1 = (q31_t)(0xAA0CEF80UL - (uint32_t) val);  // var1 + val ~= 2852974464 (0xAA0CEF80)

            else if (val < 0x6E400001)
                var1 = (q31_t)(0xB00CEF80UL - (uint32_t) val);  // var1 + val ~= 2953637760 (0xB00CEF80)

            else if (val < 0x79000000)
                var1 = (q31_t)(0xB5ECEF80UL - (uint32_t) val);  // var1 + val ~= 3052203904 (0xB5ECEF80)

            else
                var1 = halfval;    // var1 is the half of val. halfval = val >> 1;

            /* After three iterations:                     */
            /* Error: max is 68. (3.17 * 10^ -8, about 25 bits ) */
            /* The inst count is 75, The cycle count is 82.     */

            /* After four iterations:                     */
            /* Error: max is 7. (3.3 * 10^ -9, about 29 bits ) */
            /* The inst count is 89, The cycle count is 96.     */

            /* Iteration */
            var1 = ((q31_t)((q63_t)var1
                            * (0x30000000 - ((q31_t)((((q31_t)(((q63_t)var1
                                             * var1) >> 31)) * (q63_t)halfval) >> 31))) >> 31)) << 2;

            var1 = ((q31_t)((q63_t)var1
                            * (0x30000000 - ((q31_t)((((q31_t)(((q63_t)var1
                                             * var1) >> 31)) * (q63_t)halfval) >> 31))) >> 31)) << 2;

            var1 = ((q31_t)((q63_t)var1
                            * (0x30000000 - ((q31_t)((((q31_t)(((q63_t)var1
                                             * var1) >> 31)) * (q63_t)halfval) >> 31))) >> 31)) << 2;
            var1 = ((q31_t)((q63_t)var1
                            * (0x30000000 - ((q31_t)((((q31_t)(((q63_t)var1
                                             * var1) >> 31)) * (q63_t)halfval) >> 31))) >> 31)) << 2;
#ifdef ENA_HIGHER_PERFORMANCE
            var1 = ((q31_t)(((q63_t)temp1 * var1) >> 32)) << 2;
#else
            var1 = ((q31_t)(((q63_t)temp1 * var1) >> 31)) << 1;
#endif

            var1 = var1 >> (signbit / 2);
            *dst++ = var1;
        }
        else
        {
            out = (val == 0) ? 0 : -1;
            *dst++ = out;
        }
        size--;

    }
}

/**
 * @} end of utilsset
 */

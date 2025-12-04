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

#include "riscv_vec_filtering.h"
#include "internal_nds_types.h"

/* function description */
void riscv_vec_lms_q15(const riscv_vec_lms_q15_t * FUNC_RESTRICT instance, q15_t *  FUNC_RESTRICT src, q15_t * FUNC_RESTRICT ref,
                       q15_t * FUNC_RESTRICT dst, q15_t * FUNC_RESTRICT err, uint32_t size)
{
    q15_t *state = instance->state;
    q15_t *px, *pf, *scurr;
    q15_t *pOut = dst;
    q15_t mu = instance->mu;
    q15_t w = 0, e = 0;
    uint32_t coeff_size = instance->coeff_size;
    long_t tapcnt;
    q63_t sum = 0;

    q31_t acc_l, acc_h;
    int32_t lshift = (15 - (int32_t) instance->shift);
    int32_t ushift = (32 - lshift);

    scurr = &(instance->state[(coeff_size - 1u)]);
    /* block process */
    while (size != 0u)
    {
        *scurr++ = *src++;
        px = state;
        pf = instance->coeff;
        /* filter phase */
        tapcnt = coeff_size;
        sum = 0;
        while (tapcnt != 0u)
        {
            sum += ((q63_t)(*px++) * (q63_t)(*pf++));
            tapcnt--;
        }
        acc_l = sum & 0xffffffff;
        acc_h = (sum >> 32) & 0xffffffff;
        sum = (uint32_t) acc_l >> lshift | acc_h << ushift;
        sum = NDS_ISA_SATS(sum, 16u);
        *pOut++ = (q15_t) sum;
        /* update phase */
        e = (q15_t) * ref++ - (q15_t)sum;
        e = (q15_t)NDS_ISA_SATS(e, 16u);
        *err++ = e;
        w = (q15_t)((q31_t)e * (q31_t) mu >> 15);
        px = state;
        pf = instance->coeff;

        tapcnt = coeff_size;
        while (tapcnt != 0u)
        {
            sum = (q63_t) (*pf + (q15_t) (((q31_t) w * (q31_t)(*px++)) >> 15));
            *pf = (q15_t)NDS_ISA_SATS(sum, 16u);
            //*pf = *pf +(q15_t) (((q31_t) w * (*px++)) >> 15);
            pf++;
            tapcnt--;
        }
        state = state + 1;
        size--;
    }
    /* end of block process */

    /* clean up for next frame */
    scurr = instance->state;
    tapcnt = (coeff_size - 1u);
    while (tapcnt != 0u)
    {
        *scurr++ = *state++;
        tapcnt--;
    }
}

/**
 * @} end of lms
 */

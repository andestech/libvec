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
#include <math.h>

/* function description */
void riscv_vec_fir_q31(const riscv_vec_fir_q31_t * FUNC_RESTRICT instance, q31_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT dst, uint32_t size)
{

    q31_t *state = instance->state;
    q31_t *px, *pf, *scurr;
    q63_t acc0;
    q31_t x0, x1, x2, x3;
    q31_t c0;
    q63_t acc2, acc3;
    long coeff_size = instance->coeff_size;
    long i=0, tapcnt, blkCnt;

    scurr = &(instance->state[(coeff_size - 1u)]);

    /* block loop */
    q63_t acc1;

    blkCnt = size >> 2;
    while (blkCnt != 0u)
    {
        *scurr++ = *src++;
        *scurr++ = *src++;
        *scurr++ = *src++;
        *scurr++ = *src++;

        // Initialize filter coefficient and state pointer
        px = state;
        pf = instance->coeff;

        acc0 = 0;
        acc1 = 0;
        acc2 = 0;
        acc3 = 0;

        x0 = *(px++);
        x1 = *(px++);
        x2 = *(px++);

        tapcnt = coeff_size >> 2;
        i = tapcnt;

        while (i > 0u)
        {
            c0 = *pf;
            x3 = *px;
            acc0 += ((q63_t) x0 * c0);
            acc1 += ((q63_t) x1 * c0);
            acc2 += ((q63_t) x2 * c0);
            acc3 += ((q63_t) x3 * c0);
            c0 = *(pf + 1u);
            x0 = *(px + 1u);

            acc0 += ((q63_t) x1 * c0);
            acc1 += ((q63_t) x2 * c0);
            acc2 += ((q63_t) x3 * c0);
            acc3 += ((q63_t) x0 * c0);
            c0 = *(pf + 2u);
            x1 = *(px + 2u);

            acc0 += ((q63_t) x2 * c0);
            acc1 += ((q63_t) x3 * c0);
            acc2 += ((q63_t) x0 * c0);
            acc3 += ((q63_t) x1 * c0);

            c0 = *(pf + 3u);
            x2 = *(px + 3u);

            acc0 += ((q63_t) x3 * c0);
            acc1 += ((q63_t) x0 * c0);
            acc2 += ((q63_t) x1 * c0);
            acc3 += ((q63_t) x2 * c0);
            pf += 4u;
            px += 4u;
            i--;
        }

        i = coeff_size - (tapcnt << 2u);
        while (i > 0u)
        {
            c0 = *(pf++);
            x3 = *(px++);
            acc0 += ((q63_t) x0 * c0);
            acc1 += ((q63_t) x1 * c0);
            acc2 += ((q63_t) x2 * c0);
            acc3 += ((q63_t) x3 * c0);
            x0 = x1;
            x1 = x2;
            x2 = x3;
            i--;
        }
        state = state + 4;

        acc0 = (q31_t) (acc0 >> 31u);
        acc1 = (q31_t) (acc1 >> 31u);
        acc2 = (q31_t) (acc2 >> 31u);
        acc3 = (q31_t) (acc3 >> 31u);
        *dst++ = acc0;
        *dst++ = acc1;
        *dst++ = acc2;
        *dst++ = acc3;
        blkCnt--;
    }

    blkCnt = size % 4u;
    while (blkCnt > 0u)
    {
        *scurr++ = *src++;
        acc0 = 0;

        px = state;
        pf = instance->coeff;

        i = coeff_size;
        while (i > 0u)
        {
            acc0 += (q63_t) * (px++) * (*(pf++));
            i--;
        }
        *dst++ = (q31_t) (acc0 >> 31u);
        state = state + 1;
        blkCnt--;
    }

    scurr = instance->state;
    tapcnt = (coeff_size - 1u);
    while (tapcnt != 0u)
    {
        *scurr++ = *state++;
        tapcnt--;
    }
}

/**
 * @} end of fir
 */

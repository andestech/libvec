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

/**
 * @ingroup filtering
 */

/**
 * @defgroup lms Least Mean Square (LMS) Filters
 *
 * The LMS algorithm performs the following operations to update the
 * coefficients of an adaptive filter: With input signal array
 * <code>src[n]</code> of length <code>size</code>and filter coefficient
 * array <code>f[k]</code> of length <code>coeff_size</code>, for k = 0, 1,
 * 2, ..., coeff_size - 1,
 *
 * 1> Calculate the output signal dst[n] from the adaptive filter as
 *          dst[n] = f[0] * src[n] + f[1] * dst[n - 1] + ...
 *                  + f[coeff_size - 1] * dst[n - coeff_size + 1]
 * 2> Calculate the error signal
 *          e[n] = d[n] (reference signal) - dst[n].
 * 3> Update the filter coefficients
 *          b[k] = b[k - 1] + mu * e[n] * x[n - k]
 *                  for k = 0, 1, 2, ..., coeff_size - 1.
 * where <code>mu</code> is the step size and controls the rate of
 * coefficient convergence.
 *
 * The coefficients and state variables for a filter are stored together in
 * an instance data structure which is defined as follow. The state
 * variables are updated after each block of data is processed.
 *
 * <pre>
 *  For the floating-point instruction:
 *      struct {
 *          uint32_t  coeff_size;
 *          float32_t *state;
 *          float32_t *coeff;
 *          float32_t  mu;
 *      } nds_lms_f32_t;
 *
 * For the q15 and q31 instruction:
 *      struct {
 *          uint32_t  coeff_size;
 *          (type) *state;
 *          (type) *coeff;
 *          (type) mu;
 *          (type) shift;
 *      } nds_lms_(type)_t;
 *
 * where coeff_size number of filter coefficients.
 *           *state points to the state variable array of size
 *                  <code>coeff_size + size - 1</code>.
 *       Ex: {src[n-coeff_size+1], src[n-coeff_size], src[n-coeff_size-1],
 *              src[n-coeff_size-2]....src[0], src[1], ..., x[size-1]}
 *           *coeff points to the coefficient array of size
 *                  <code>coeff_size</code> and the coefficients are stored
 *                  in time reversed order.
 *       Ex: {b[coeff_size-1], b[coeff_size-2], b[N-2], ..., b[1], b[0]}
 *               mu step size that controls filter coefficient updates.
 *            shift bit shift applied to coefficients.
 * </pre>
 */

/**
 * @addtogroup lms
 * @{
 */

/**
 * @brief Function for the q31 LMS filter.
 * @param[in]       *instance points to an instance of the LMS structure.
 * @param[in]       *src      points to the input block data.
 * @param[in]       *ref      points to the reference data.
 * @param[out]      *dst      points to the output data.
 * @param[out]      *err      points to the error data.
 * @param[in]       size      number of the blocksize.
 * @return none.
 */

/* function description */
void riscv_vec_lms_q31(const riscv_vec_lms_q31_t * FUNC_RESTRICT instance, q31_t *  FUNC_RESTRICT src, q31_t * FUNC_RESTRICT ref,
                       q31_t * FUNC_RESTRICT dst, q31_t * FUNC_RESTRICT err, uint32_t size)
{
    q31_t *state = instance->state;
    q31_t *px, *pf, *scurr;
    q31_t *pOut = dst;
    q31_t mu = instance->mu;
    q31_t w = 0, e = 0;
    uint32_t coeff_size = instance->coeff_size;
    long_t tapcnt;
    q63_t sum = 0;
    q31_t acc_l, acc_h;
    int32_t ushift = ((uint32_t) instance->shift + 1u);
    int32_t lshift = 32u - ushift;

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
        //sum = NDS_ISA_SATS(sum, 32u);
        *pOut++ = (q31_t) sum;
        /* update phase */
        e = *ref++ - (q31_t)sum;
        //e = (q31_t)NDS_ISA_SATS(e, 16u);
        *err++ = (q31_t) e;
        w = (q31_t)((q63_t)e * (q63_t) mu >> 31);
        px = state;
        pf = instance->coeff;

        tapcnt = coeff_size;
        while (tapcnt != 0u)
        {
            sum = (q63_t) (*pf + (q31_t) (((q63_t) w * (q63_t)(*px++)) >> 31));
            *pf = (q31_t) sum;
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

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

#include "internal_vec_filtering.h"

/**
 * @ingroup filtering
 */

/**
 * @defgroup nlms Normalized LMS Filter
 *
 * The NLMS algorithm It is related to the Least Mean Square (LMS) adaptive
 * filter and includes an additional normalization factor which increases
 * the the adaptation rate of the filter.With input signal array
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
 *          b[k] = b[k - 1] + u[n] * e[n] * x[n - k]
 *          where u[n] = mu / energy and energy = src[n] ^ 2
 *                  + src[n - 1] ^ 2 + ... + src[n - coeff_size + 1] ^ 2.
 *          for k = 0, 1, 2, ..., coeff_size - 1.
 * where <code>mu</code> is the step size and controls the rate of
 * coefficient convergence.
 *
 * The coefficients and state variables for a filter are stored together in
 * an instance data structure which is defined as follow. The state
 * variables are updated after each block of data is processed.
 *
 * <pre>
 *  For the Q31 instruction:
 *      struct {
 *          uint32_t  coeff_size;
 *          q31_t *state;
 *          q31_t *coeff;
 *          q31_t  mu;
 *          q31_t energy;
 *      } nds_nlms_q31_t;
 *
 *      struct {
 *          uint32_t  coeff_size;
 *          (type) *state;
 *          (type) *coeff;
 *          (type) mu;
 *          uint8_t shift;
 *          (type) *recip;
 *          (type) energy;
 *      } nds_nlms_(type)_t;
 *
 * where coeff_size number of filter coefficients in the filter.
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
 *           *recip points to the reciprocal initial value table.
 *           energy previous frame energy.
 * </pre>
 */

/**
 * @addtogroup nlms
 * @{
 */

/* Instance Structue */

/**
 * @brief Function for the q31 NLMS filter.
 * @param[in]       *instance points to an instance of the LMS structure.
 * @param[in]       *src      points to the input block data.
 * @param[in]       *ref      points to the reference data.
 * @param[out]      *dst      points to the output data.
 * @param[out]      *err      points to the error data.
 * @param[in]       size      number of the blocksize.
 * @return none.
 */

/* function description */
void riscv_vec_nlms_q31(riscv_vec_nlms_q31_t * FUNC_RESTRICT instance,
                        q31_t * FUNC_RESTRICT src,
                        q31_t * FUNC_RESTRICT ref,
                        q31_t * FUNC_RESTRICT dst,
                        q31_t * FUNC_RESTRICT err,
                        uint32_t size)
{
    q31_t *pState = instance->state;
    q31_t *px, *pb, *scurr;
    q31_t mu = instance->mu;
    uint32_t coeff_size = instance->coeff_size;
    long tapcnt;
    q31_t w = 0;
    q31_t in;
    q31_t x0;
    q31_t postshift;
    q31_t onebyenergy = 0;
    q63_t energy;
    q31_t errormu = 0 ;
    q31_t e = 0;
    q63_t sum;
    int32_t uShift = ((uint32_t) instance->postshift + 1u);
    energy = (q63_t) instance->energy;

    int32_t lShift = 32u - uShift;
    q31_t acc_l, acc_h;
    q63_t coef;

    scurr = &(instance->state[(coeff_size - 1u)]);
    x0 = instance->x0;
    /* block process */
    while (size != 0u)
    {
        *scurr++ = *src;
        px = pState;
        pb = instance->coeff;

        /* filter phase */
        in = *src++;
        energy = (q31_t) ((((q63_t) energy << 32) - (((q63_t) x0 * (q63_t) x0) << 1)) >> 32);
        energy = (q31_t) (((((q63_t) in * (q63_t) in) << 1) + (energy << 32)) >> 32);
        sum = 0;
        tapcnt = coeff_size;
        while (tapcnt != 0u)
        {
            sum += ((q63_t)(*px++) * (q63_t)(*pb++));
            tapcnt--;
        }
        acc_l = sum & 0xffffffff;
        acc_h = (sum >> 32) & 0xffffffff;
        sum = (uint32_t) acc_l >> lShift | acc_h << uShift;
        *dst++ = (q31_t) sum;

        /* update phase */
        e = (q31_t)((*ref++) - (q31_t)sum);
        *err++ = e;
        postshift = vec_recip_q31((energy + DELTA_Q31), &onebyenergy);
        errormu = (q31_t) (((q63_t) e * (q63_t) mu) >> 31);
        sum = ((q63_t) errormu * (q63_t)onebyenergy) >> (31 - postshift);
        w = vec_truncate_q63_to_q31(sum);
        px = pState;
        pb = instance->coeff;

        tapcnt = coeff_size;
        while (tapcnt != 0u)
        {
            coef = (((q63_t) w * (q63_t)(*px)) >> (32));
            *pb  = vec_truncate_q63_to_q31((q63_t) * pb + (coef << 1u));
            pb++;
            px++;
            tapcnt--;
        }
        x0 = *pState++;
        size--;
    }

    /* clean up for next frame */
    instance->energy = (q31_t)energy;
    instance->x0 = x0;
    scurr = instance->state;
    tapcnt = (coeff_size - 1u);
    while (tapcnt != 0u)
    {
        *scurr++ = *pState++;
        tapcnt--;
    }
}

/**
 * @} end of nlms
 */

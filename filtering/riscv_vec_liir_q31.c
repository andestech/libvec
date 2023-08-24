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
#include "riscv_vec_filtering.h"
#include "internal_nds_types.h"
#include "internal_vec_filtering.h"
/**
 * @ingroup filtering
 */

/**
 * @defgroup liir Lattice Infinite impulse response(IIR) Filters
 *
 * The lattice IIR filters are used in a variety of adaptive filter
 * applications. The filter structure has feedforward and feedback components
 * and the net impulse response is infinite length. The following
 * difference equations are implemented:
 *
 * This function operate on blocks of input data <code>src[n]</code> and
 * output data for the length <code>size</code> and each call to the
 * function processes.
 *
 * <pre>
 *      fN[n] = src[n]
 *      fm-1[n] = fm[n] - rm * gm-1[n-1] for m = N, N-1, ...1
 *      gm[n] = rm * fm-1[n] + gm-1[n-1] for m = N, N-1, ...1
 *      dst[n] = vN * gN(n) + vN-1 * gN-1(n) + ...+ v0 * g0(n)
 * </pre>
 * where N is number of stages, Reflection Coefficients <code>rm</code> are
 * stored in {rN, rN-1, ....r1} of the length <code>nstage</code>. The
 * <code>vm[n]</code> is the ladder coefficients of the length
 * <code>nstage + 1</code>. The state variables array <code>gm[n]</code> has
 * the length <code>nstage + size</code> sizes.
 *
 * The coefficients and state variables for a filter are stored together in
 * an instance data structure which is defined as follow. The state
 * variables are updated after each block of data is processed and the
 * coefficients are untouched.
 *
 * <pre>
 *      struct {
 *          uint32_t nstage;
 *          (type) *state;
 *          (type) *rcoeff;
 *          (type) *lcoeff;
 *      } nds_liir_(type)_t;
 *
 * where *state  points to the state variable array of size
 *               <code>nstage + size</code>.
 *       *rcoeff points to the reflection coefficient array of size
 *               <code>nstage</code> and the coefficients are stored
 *               in time reversed order.
 *          Ex:  {r[nstage-1], r[nstage-2], ..., r[1]}
 *       *lcoeff points to the ladder coefficient array of size
 *               <code>nstage + 1</code>.
 *          Ex:  {v[nstage-1], v[nstage-2], ..., v[1], v[0]}
 *        nstage number of stages in the filter.
 * </pre>
 */

/**
 * @addtogroup liir
 * @{
 */

/**
 * @brief Function for the q31 lattice IIR filter.
 * @param[in]       *instance points to an instance of the lattice
 *                            IIR structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       size      number of the blocksize.
 * @return none.
 */

/* function description */
void riscv_vec_liir_q31(const riscv_vec_liir_q31_t * FUNC_RESTRICT instance,
                        q31_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT dst, uint32_t size)
{
    q63_t fcurr = 0, gcurr = 0, fnext = 0, gnext = 0;
    q63_t acc;
    uint32_t blkcnt, tapcnt;
    q31_t *pst, *ref, *lad;
    uint32_t nstage = instance->nstage;
    q31_t *state, *stateCurnt;
    q31_t val_ref, val_lad;
    state = &instance->state[0];
    blkcnt = size;

    while (blkcnt != 0u)
    {
        fcurr = *src++;
        pst = state;
        acc = 0;
        ref = &instance->rcoeff[0];
        lad = &instance->lcoeff[0];
        tapcnt = nstage;
        /* stage loop */
        while (tapcnt != 0u)
        {
            gcurr = *pst;

            val_ref = *ref;
            val_lad = *lad;
            fnext = (q63_t)fcurr - (((q63_t)gcurr * (q63_t) val_ref)>>31);
            gnext = (q63_t)gcurr + (((q63_t)fnext * (q63_t) val_ref)>>31);
            acc += ((q63_t)gnext * (q63_t)val_lad);
            lad++;
            ref++;
            *pst = (q31_t)gnext;
            fcurr = fnext;
            pst++;
            tapcnt--;
        }
        acc += ((q63_t)fnext *(q63_t) (*lad));
        //lad++;
        *pst =  fnext;
        *dst++ =(q31_t)( acc >> 31); //NDS_ISA_SATS(acc >> 31, 32u); //acc;
        state++;
        blkcnt--;
    }

    stateCurnt = &instance->state[0];
    state = &instance->state[size];
    tapcnt = nstage;
    while (tapcnt != 0u)
    {
        *stateCurnt++ = *state++;
        tapcnt--;
    }
}

/**
 * @} end of liir
 */

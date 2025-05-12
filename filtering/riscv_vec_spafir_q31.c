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
 * @defgroup spafir Sparse FIR Filter
 *
 * This functions implement the Sparse Fir Filter. Sparse Fir Filter is
 * means a filter with the majority of the coefficients being zero  and are
 * used for simulating reflections in communications and audio applications.
 *
 * In the sparse fir instance, there is an array of buf indices
 * <code>nezdelay</code> which specifies the locations of the non-zero
 * coefficients. This is in addition to the coefficient array <code>b</code>.
 * The implementation skips the multiplications by zero and leads to an
 * efficient realization.
 *
 * The functions operate on blocks of input and output data. Both the input
 * <code>src</code> and output <code>dst</code> array have the length
 * <code>size</code> The functions use a polyphase filter structure:
 *
 * <pre>
 *		dst[n] = b[0] * x[n - nezdelay[0]] + b[1] * x[n - nezdelay[1]]
 *				+ b[2] * x[n - nezdelay[2]] + ...
 *					+ b[coeff_size-1] * x[n-nezdelay[coeff_size-1]]
 *		wher b[n] is the filter coefficients.
 * </pre>
 *
 * The coefficients and state variables for a filter are stored together in
 * an instance data structure which is defined as follow. The state
 * variables are updated after each block of data is processed and the
 * coefficients are untouched.
 *
 * <pre>
 *		struct {
 *			uint32_t coeff_size;
 *			uint32_t index;
 *			(type) *state;
 *			(type) *coeff;
 *			uint32_t delay;
 *			int32_t *nezdelay;
 *		} nds_spafir_(type)_t;
 *
 * where *state points to the state variable array of size
 *				<code>delay + size</code>.
 *		 *coeff points to the coefficient array of size
 *				<code>coeff_size</code>.
 *	 coeff_size number of filter coefficients.
 *		  index is the state buffer index.
 *		  delay is the largest offset value that is ever used in the
 *				nezdelay array.
 *	   nezdelay points to an array of nonzero indices and is also of length
 *				<code>coeff_size</code>.
 * </pre>
 */

/**
 * @addtogroup spafir
 * @{
 */

/**
 * @brief Function for the q31 Sparse FIR filter.
 * @param[in]       *instance points to an instance of the Sparse FIR
 *                            structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 & @param[in]       *buf      points to a temporary buffer of length size.
 * @param[in]       size      number of the size.
 * @return none.
 */

/* function description */
void riscv_vec_spafir_q31(riscv_vec_spafir_q31_t * FUNC_RESTRICT instance, q31_t * FUNC_RESTRICT src,
                          q31_t * FUNC_RESTRICT dst, q31_t * FUNC_RESTRICT buf, uint32_t size)
{
    q63_t out_tmp = 0 ;
    q31_t *state = instance->state;
    q31_t *coeff = instance->coeff;
    q31_t *px, *py = state, *pb = buf;
    q31_t *out = dst;
    int32_t *nezdelay = instance->nezdelay;
    uint32_t delsize = instance->delay + size;
    uint16_t coeff_size = instance->coeff_size;
    int32_t rinx;
    long_t tapcnt, blkcnt;
    q31_t coe = *coeff++;
    vec_cirwrite_f32((int32_t *) py, delsize, &instance->index, 1, (int32_t *) src, 1, size);

    rinx = ((int32_t) instance->index - (int32_t) size) - *nezdelay++;

    if (rinx < 0)
    {
        rinx += (int32_t) delsize;
    }

    py = state;
    vec_cirread_f32((int32_t *) py, delsize, &rinx, 1, (int32_t *) pb, (int32_t *) pb, size, 1, size);

    px = pb;
    out = dst;

    blkcnt = size;
    while (blkcnt != 0u)
    {
        out_tmp = (q63_t)(((q63_t) * px++ * (q63_t)coe) >> 32);
        *out++ = (q31_t) out_tmp;
        blkcnt--;
    }
    coe = *coeff++;
    rinx = ((int32_t) instance->index - (int32_t) size) - *nezdelay++;

    if (rinx < 0)
    {
        rinx += (int32_t) delsize;
    }

    tapcnt = (uint32_t) coeff_size - 1u;
    while (tapcnt != 0u)
    {
        py = state;
        vec_cirread_f32((int32_t *) py, delsize, &rinx, 1, (int32_t *) pb, (int32_t *) pb, size, 1, size);

        px = pb;
        out = dst;

        blkcnt = size;
        while (blkcnt != 0u)
        {
            out_tmp = (q63_t) * out;
            out_tmp += (q63_t)(((q63_t) * px++ * (q63_t) coe) >> 32);
            *out++ = (q31_t) out_tmp;
            blkcnt--;
        }

        coe = *coeff++;
        rinx = ((int32_t) instance->index - (int32_t) size) - *nezdelay++;
        if (rinx < 0)
        {
            rinx += (int32_t) delsize;
        }
        tapcnt--;
    }
    out = dst;
    blkcnt = size;
    while (blkcnt != 0u)
    {
        out_tmp = (q63_t) (*out << 1);
        *out++ = (q31_t) out_tmp ;
        blkcnt--;
    }

}

/**
 * @} end of spafir
 */

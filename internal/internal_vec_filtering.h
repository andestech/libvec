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

#ifndef __INTERNAL_VEC_MATRIX_H__
#define __INTERNAL_VEC_MATRIX_H__

#include "riscv_vec_filtering.h"
#include "nds_intrinsic.h"

#define DELTA_Q15   (0x5)
#define DELTA_Q31   (0x100)

// Internal used circular functions
// Floating-point Circular write function
static inline void nds_cirwrite_f32(int32_t *buf, int32_t len,
                                      uint16_t *offset, int32_t binc, const int32_t *src,
                                      int32_t sinc, uint32_t size)
{
    uint32_t i = 0u;
    int32_t woffset;

    woffset = *offset;
    i = size;
    do
    {
        buf[woffset] = *src;
        src += sinc;
        woffset += binc;

        if (woffset >= len)
            woffset -= len;
    }
    while (--i != 0u);
    *offset = woffset;
}

/**
 * @brief Circular Read function.
 */
// Flaoting-point Circular Read function
static inline void nds_cirread_f32(int32_t *buf, int32_t len,
                                     int32_t *offset, int32_t binc, int32_t *dst, int32_t *dbase,
                                     int32_t dlen, int32_t dinc, uint32_t size)
{
    uint32_t i = 0u;
    int32_t roffset;
    int32_t *dend;

    roffset = *offset;
    dend = dbase + dlen;
    i = size;
    do
    {
        *dst = buf[roffset];
        dst += dinc;

        if (dst == dend)
        {
            dst = dbase;
        }
        roffset += binc;

        if (roffset >= len)
            roffset -= len;
    }
    while (--i != 0u);
    *offset = roffset;
}

// Internal used circular functions
// Q7 Circular write function
static inline void nds_cirwrite_q7(q7_t *buf, int32_t len,
                                      uint16_t *offset, int32_t binc, const q7_t *src,
                                      int32_t sinc, uint32_t size)
{
    uint32_t i = 0u;
    int32_t woffset;

    woffset = *offset;
    i = size;
    do
    {
        buf[woffset] = *src;
        src += sinc;
        woffset += binc;

        if (woffset >= len)
            woffset -= len;
    }
    while (--i != 0u);
    *offset = woffset;
}

// Floating-point Circular write function
static inline void nds_cirwrite_q15(q15_t *buf, int32_t len,
                                      uint16_t *offset, int32_t binc, const q15_t *src,
                                      int32_t sinc, uint32_t size)
{
    uint32_t i = 0u;
    int32_t woffset;

    woffset = *offset;
    i = size;
    do
    {
        buf[woffset] = *src;
        src += sinc;
        woffset += binc;

        if (woffset >= len)
            woffset -= len;
    }
    while (--i != 0u);
    *offset = woffset;
}

/**
 * @brief Circular Read function.
 */
// Q7 Circular Read function
static inline void nds_cirread_q7(q7_t *buf, int32_t len,
                                     int32_t *offset, int32_t binc, q7_t *dst, q7_t *dbase,
                                     int32_t dlen, int32_t dinc, uint32_t size)
{
    uint32_t i = 0u;
    int32_t roffset;
    q7_t *dend;

    roffset = *offset;
    dend = dbase + dlen;
    i = size;
    do
    {
        *dst = buf[roffset];
        dst += dinc;

        if (dst == dend)
        {
            dst = dbase;
        }
        roffset += binc;

        if (roffset >= len)
            roffset -= len;
    }
    while (--i != 0u);
    *offset = roffset;
}

// Q15 Circular Read function
static inline void nds_cirread_q15(q15_t *buf, int32_t len,
                                     int32_t *offset, int32_t binc, q15_t *dst, q15_t *dbase,
                                     int32_t dlen, int32_t dinc, uint32_t size)
{
    uint32_t i = 0u;
    int32_t roffset;
    q15_t *dend;

    roffset = *offset;
    dend = dbase + dlen;
    i = size;
    do
    {
        *dst = buf[roffset];
        dst += dinc;

        if (dst == dend)
        {
            dst = dbase;
        }
        roffset += binc;

        if (roffset >= len)
            roffset -= len;
    }
    while (--i != 0u);
    *offset = roffset;

}

// Q63 to Q(destination type)
static inline q31_t _vec_truncate_q63_to_q31(q63_t src)
{
#ifdef NDS_CPU_64
    return src != (q31_t)src ?
           ((INT32_MAX ^ ((q31_t) (src >> 63)))) : (q31_t) src;
#else
    return ((q31_t) (src >> 32) != ((q31_t) src >> 31)) ?
           ((INT32_MAX ^ ((q31_t) (src >> 63)))) : (q31_t) src;
#endif
}

// Count the log2 of 32-bit value
static inline int32_t _vec_log2_32b(uint32_t v)
{
    uint32_t r, shift;

    r = (v > 0xFFFF) << 4;
    v >>= r;
    shift = (v > 0xFF) << 3;
    v >>= shift;
    r |= shift;
    shift = (v > 0xF) << 2;
    v >>= shift;
    r |= shift;
    shift = (v > 0x3) << 1;
    v >>= shift;
    r |= shift;
    r |= (v >> 1);

    return (int32_t)r;
}

static inline uint32_t _vec_ghb_32b(q31_t data)
{
    uint32_t count;
#if defined(__NDS32_EXT_PERF__)
    count = 32 - __nds32__clz(data);
#else
    count = data;
    if (data != 0)
    {
        count = _vec_log2_32b(data) + 1;
    }
#endif
    return count;
}

static inline uint32_t _vec_clz(q31_t data)
{
    uint32_t count;
#if defined(__NDS32_EXT_PERF__)
    count = __nds32__clz(data);
#else
    count = 32 - _vec_ghb_32b(data);
#endif
    return count;
}

static inline uint32_t _vec_recip_q15(q15_t src, q15_t *dst)
{
    q15_t out = 0;
    uint32_t val = 0, sign = 0;

    sign = (src > 0) ? (_vec_clz(src) - 17) : (_vec_clz(-src) - 17);
    src = src << sign;

    //out = 48 / 17 - (32 / 17 * src);
    out = (0x2D2D6000 - 0x3C3C * src) >> 14;

    /* two iterations*/
    val = (q15_t) (((q31_t) src * out) >> 15);
    val = 0x7FFF - val;
    out = (q15_t) (((q31_t) out * val) >> 14);

    val = (q15_t) (((q31_t) src * out) >> 15);
    val = 0x7FFF - val;
    out = (q15_t) (((q31_t) out * val) >> 14);

    *dst = out;
    return (sign + 1);
}

static inline uint32_t _vec_recip_q31(q31_t src, q31_t *dst)
{
    q31_t out = 0;
    uint32_t val = 0, sign = 0;

    sign = (src > 0) ? (_vec_clz(src) - 1) : (_vec_clz(-src) - 1);
    src = src << sign;

    out  = (0x7FFFFFFF - src) + 0x40000000; //convert src to the range 0.5 ~ 1

    /* two iterations*/
    val = (q31_t) (((q63_t) src * out) >> 31u);
    val = 0x7FFFFFFF - val;
    out = (q31_t) _vec_truncate_q63_to_q31(((q63_t) out * val) >> 30u);

    val = (q31_t) (((q63_t) src * out) >> 31u);
    val = 0x7FFFFFFF - val;
    out = (q31_t) _vec_truncate_q63_to_q31(((q63_t) out * val) >> 30u);

    *dst = out;
    return (sign + 1u);
}

#endif

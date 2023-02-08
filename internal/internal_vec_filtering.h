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

#endif

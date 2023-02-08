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

#ifndef __INTERNAL_VEC_MAT_MUL_H__
#define __INTERNAL_VEC_MAT_MUL_H__

#include "stdio.h"
#include "nds_intrinsic.h"

typedef enum
{
    keepa_2x2x64_f64 = 0,
    keepb_2x2x64_f64 = 1,
    keepa_8x4x16_f64 = 2,
    keepb_8x4x16_f64 = 3,
    keepa_4x4x32_f64 = 4,
    keepb_4x4x32_f64 = 5
}mat_mul_alg;

static inline void bestperf(uint64_t tmpinst, uint64_t tmpcycle,
                            uint64_t * retinst, uint64_t * retcycle,
                            int * cnt, int * idx)
{
    int nowcnt = *cnt;
    uint64_t mincycle = *retcycle;
    printf("%d inst: %lu cycle %lu\n", nowcnt, (long unsigned int)tmpinst, (long unsigned int)tmpcycle);
    if(mincycle > tmpcycle)
    {
        *retcycle = tmpcycle;
        *retinst = tmpinst;
        *idx = nowcnt;
    }
    nowcnt++;
    *cnt = nowcnt;
}

static inline int findpwr2(const int target)
{
    int temp = target - 1;
    temp |= temp >> 1;
    temp |= temp >> 2;
    temp |= temp >> 4;
    temp |= temp >> 8;
    temp |= temp >> 16;
    int final = (temp < 0) ? 1 : temp + 1;
    if (final > target)
        return (final >> 1);
    else
        return final;
}

#ifndef DCACHE_SIZE
#define DCACHE_SIZE 32
#endif

#define DCACHE_BYTE (DCACHE_SIZE * 1024)
#define DCACHE_HALF_BYTE (DCACHE_BYTE/2)
static inline void get_keepB_2x2x64_slice(uint32_t row, uint32_t *row_slice, uint32_t *col2_slice)
{
    uint32_t dcache_byte = DCACHE_BYTE;
    uint32_t total_byte = 2 * row * row * sizeof(float64_t);
    uint32_t tmp;
    if (total_byte < dcache_byte)
    {
        tmp = row >> 1;
        *row_slice = tmp;
        *col2_slice = tmp;
    }
    else
    {
        tmp = (DCACHE_HALF_BYTE / row) / sizeof(float64_t);
        if (tmp < 64)
        {
            *col2_slice = 64;
            *row_slice = 2;
        }
        else
        {
            *col2_slice = 64;
            uint32_t b_size = 64 * sizeof(float64_t) * row;
            uint32_t a_size = DCACHE_BYTE - b_size;
            uint32_t a_slice = (a_size / row) / sizeof(float64_t);
            a_slice = findpwr2(a_slice);
            *row_slice = a_slice;
        }
    }
}

static inline void get_keepB_8x4x16_slice(uint32_t row, uint32_t *row_slice, uint32_t *col2_slice)
{
    uint32_t dcache_byte = DCACHE_BYTE;
    uint32_t total_byte = 2 * row * row * sizeof(float64_t);
    uint32_t tmp;
    if (total_byte < dcache_byte)
    {
        tmp = row >> 1;
        *row_slice = tmp;
        *col2_slice = tmp;
    }
    else
    {
        tmp = (DCACHE_HALF_BYTE / row) / sizeof(float64_t);
        if (tmp < 64)
        {
            *col2_slice = 16;
            *row_slice = 8;
        }
        else
        {
            *col2_slice = 64;
            uint32_t b_size = 64 * sizeof(float64_t) * row;
            uint32_t a_size = DCACHE_BYTE - b_size;
            uint32_t a_slice = (a_size / row) / sizeof(float64_t);
            a_slice = findpwr2(a_slice);
            *row_slice = a_slice;
        }
    }
}
static inline void get_keepB_4x4x32_slice(uint32_t row, uint32_t *row_slice, uint32_t *col2_slice)
{
    uint32_t dcache_byte = DCACHE_BYTE;
    uint32_t total_byte = 2 * row * row * sizeof(float64_t);
    uint32_t tmp;
    if (total_byte < dcache_byte)
    {
        tmp = row >> 1;
        *row_slice = tmp;
        *col2_slice = tmp;
    }
    else
    {
        tmp = (DCACHE_HALF_BYTE / row) / sizeof(float64_t);
        if (tmp < 64)
        {
            *col2_slice = 64;
            *row_slice = 4;
        }
        else
        {
            *col2_slice = 64;
            uint32_t b_size = 64 * sizeof(float64_t) * row;
            uint32_t a_size = DCACHE_BYTE - b_size;
            uint32_t a_slice = (a_size / row) / sizeof(float64_t);
            a_slice = findpwr2(a_slice);
            *row_slice = a_slice;
        }
    }
}
#endif  // __INTERNAL_VEC_MAT_MUL_H__

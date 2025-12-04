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
#ifndef __INTERNAL_VEC_SORT_H__
#define __INTERNAL_VEC_SORT_H__

#include <math.h>  /// for log2
// quick sort pure c template
#include "string.h"
#define VEC_SORT_QS_DESCEND \
    do \
    { \
        i++; \
    } \
    while (src[i] > pivot && i < last); \
    do \
    { \
        j--; \
    } \
    while (src[j] < pivot);

#define VEC_SORT_QS_ASCEND \
    do \
    { \
        i++; \
    } \
    while (src[i] < pivot && i < last); \
    do \
    { \
        j--; \
    } \
    while (src[j] > pivot);

static inline uint32_t vec_quick_sort_partition(SORT_TYPE *src, int32_t first, int32_t last)
{
    int32_t i, j, pivot_index;
    SORT_TYPE pivot;
    SORT_TYPE temp;

    /* The first element is the pivot */
    pivot_index = first;
    pivot = src[pivot_index];

    /* Initialize indices for do-while loops */
    i = first - 1;
    j = last + 1;

    while (i < j)
    {
        QUICK_SORT_PARTITION;
        if (i < j)
        {
            temp = src[i];
            src[i] = src[j];
            src[j] = temp;
        }
    }
    return j;
}

static inline void vec_quick_sort_core(SORT_TYPE *src, int32_t first, int32_t last)
{
    /* If the array [first ... last] has more than one element */
    if (first < last)
    {
        int32_t pivot;

        /* Compute pivot */
        pivot = vec_quick_sort_partition(src, first, last);

        /* Iterate algorithm with two sub-arrays [first ... pivot] and [pivot+1 ... last] */
        vec_quick_sort_core(src, first,   pivot);
        vec_quick_sort_core(src, pivot + 1, last);
    }
}


static inline void quick_sort_purec(SORT_TYPE* src, SORT_TYPE* dst, uint32_t size)
{
    SORT_TYPE * pA;

    /* Out-of-place */
    if (src != dst)
    {
        memcpy(dst, src, size * sizeof(SORT_TYPE) );
        pA = dst;
    }
    else
        pA = src;

    vec_quick_sort_core(pA, 0, size - 1);
}

#endif // __INTERNAL_VEC_SORT_H__

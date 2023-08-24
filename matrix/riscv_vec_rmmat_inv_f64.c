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

#include <string.h> //memset

#include "internal_nds_types.h"

/* function description */
int riscv_vec_rmmat_inv_f64(float64_t * FUNC_RESTRICT src, float64_t * FUNC_RESTRICT dst, uint32_t size)
{
    if (size == 0)
    {
        return NDSV_FAIL;
    }
    float64_t *in = src;
    float64_t *pt1, *pt2, *pt3, *pt4;
    float64_t *diagsrc, *rtsrc, *diagdst, *rtdst;
    float64_t tmp1 = 0.0f;
    uint32_t i, j, k, l;
    uint32_t rowcnt, loopcnt;
    //Create an identity matrix in dst.
    pt2 = dst;
    rowcnt = size;

    memset(pt2, 0, sizeof(float64_t)*size * size);
    for (i = 0; i < rowcnt; i++)
    {
        *pt2 = 1.0f;
        pt2 += (rowcnt + 1);
    }

    loopcnt = size;
    l = 0u; //l is col pointer
    do
    {
        pt1 = in + (l * size);
        pt3 = dst + (l * size);

        //Check the pivot is zero or not. If yes then swap with next row which pivot is not zero.
        tmp1 = *pt1;
        k = 0u;
        if (tmp1 == 0.0f)
        {
            //Now pivot is zero
            int32_t swapRow = 0;
            i = l;
            while (i != size)
            {
                pt2 = pt1 + (size * k);
                pt4 = pt3 + (size * k);
                k++;
                i++;
                if (*pt2 != 0.0f)
                {
                    j = 0u;
                    do
                    {
                        float64_t tmp = *pt2;
                        *pt2++ = *pt1;
                        *pt1++ = tmp;
                        j++;
                    }
                    while (j != (size - l));

                    j = 0u;
                    do
                    {
                        float64_t tmp = *pt4;
                        *pt4++ = *pt3;
                        *pt3++ = tmp;
                        j++;
                    }
                    while (j != size);
                    swapRow = 1;
                    break;
                }
            }

            if (swapRow != 1)
            {
                return NDSV_FAIL;
            }
        }

        //Begin elementary row operation
        pt1 = diagsrc = in + (l * size);
        pt2 = diagdst = dst + (l * size);
        tmp1 = *pt1;

        //Do row division to let pivot become 1.

        j = 0u;
        do
        {
            float64_t tmp2 = *pt1 / tmp1;
            *pt1++ = tmp2;
            j++;
        }
        while (j != (size - l));

        j = 0u;
        do
        {
            float64_t tmp2 = *pt2 / tmp1;
            *pt2++ = tmp2;
            j++;
        }
        while (j != size);
        //Do row operation to let pivot column at other rows become zero
        pt1 = in;
        pt2 = dst;

        i = 0u;
        do
        {
            if (i == l)
            {
                pt1 += size - l;
                pt2 += size;
            }
            else
            {
                tmp1 = *pt1;
                rtsrc = diagsrc;
                rtdst = diagdst;
                j = 0u;
                do
                {
                    float64_t tmp2 = *pt1 - (tmp1 * *rtsrc++);
                    *pt1++ = tmp2;
                    j++;
                }
                while (j != (size - l));

                j = 0u;
                do
                {
                    float64_t tmp2 = *pt2 - (tmp1 * *rtdst++);
                    *pt2++ = tmp2;
                    j++;
                }
                while (j != size);
            }
            pt1 = pt1 + l;
            i++;
        }
        while (i != size);
        in++;
        loopcnt--;
        l++;
    }
    while (loopcnt != 0u);

    return NDSV_OK;
}

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

/* function description */
void riscv_vec_corr_q31(q31_t * FUNC_RESTRICT src1, uint32_t len1,
                        q31_t * FUNC_RESTRICT src2, uint32_t len2, q31_t * FUNC_RESTRICT dst)
{
    q31_t *pIn1;
    q31_t *pIn2;
    q31_t *pOut = dst;
    q63_t sum;
    q31_t *px;
    q63_t acc0, acc1, acc2, acc3;
    q31_t x0, x1, x2, c0, x3;
    q31_t *py;
    q31_t *pSrc1;
    long_t j, k = 0, count, blkCnt, outBlockSize, blockSize1, blockSize2, blockSize3;
    int32_t inc = 1;

    if (len1 >= len2)
    {
        pIn1 = (src1);
        pIn2 = (src2);
        outBlockSize = (2u * len1) - 1u;
        j = outBlockSize - (len1 + (len2 - 1u));
        pOut += j;

    }
    else
    {
        pIn1 = (src2);
        pIn2 = (src1);
        j = len2;
        len2 = len1;
        len1 = j;
        pOut = dst + ((len1 + len2) - 2u);
        inc = -1;

    }
    blockSize1 = len2 - 1u;
    blockSize2 = len1 - (len2 - 1u);
    blockSize3 = blockSize1;

    //part1
    count = 1u;
    px = pIn1;
    pSrc1 = pIn2 + (len2 - 1u);
    py = pSrc1;

    while (blockSize1 > 0u)
    {
        sum = 0;
        k = count;
        while (k > 0u)
        {
            sum += ((q63_t) * px++ * (q63_t) * py++);
            k--;
        }

        //*pOut = NDS_ISA_SATS((sum >> 31u), 32u) ;
        *pOut = (q31_t)(sum >> 31u);
        pOut += inc;
        py = pSrc1 - count;
        px = pIn1;
        count++;
        blockSize1--;
    }
    px = pIn1;
    py = pIn2;
    count = 0u;

    if (len2 >= 4u)
    {
        blkCnt = blockSize2 >> 2;
        while (blkCnt > 0u)
        {
            acc0 = 0;
            acc1 = 0;
            acc2 = 0;
            acc3 = 0;

            x0 = *(px++);
            x1 = *(px++);
            x2 = *(px++);
            k = len2 >> 2;

            do
            {
                c0 = *(py);
                x3 = *(px);
                acc0 += ((q63_t)x0 * (q63_t)c0);
                acc1 += ((q63_t)x1 * (q63_t)c0);
                acc2 += ((q63_t)x2 * (q63_t)c0);
                acc3 += ((q63_t)x3 * (q63_t)c0);
                c0 = *(py + 1u);
                x0 = *(px + 1u);
                acc0 += ((q63_t)x1 * (q63_t)c0);
                acc1 += ((q63_t)x2 * (q63_t)c0);
                acc2 += ((q63_t)x3 * (q63_t)c0);
                acc3 += ((q63_t)x0 * (q63_t)c0);

                c0 = *(py + 2u);
                x1 = *(px + 2u);
                acc0 += ((q63_t)x2 * (q63_t)c0);
                acc1 += ((q63_t)x3 * (q63_t)c0);
                acc2 += ((q63_t)x0 * (q63_t)c0);
                acc3 += ((q63_t)x1 * (q63_t)c0);

                c0 = *(py + 3u);
                x2 = *(px + 3u);
                acc0 += ((q63_t)x3 * (q63_t)c0);
                acc1 += ((q63_t)x0 * (q63_t)c0);
                acc2 += ((q63_t)x1 * (q63_t)c0);
                acc3 += ((q63_t)x2 * (q63_t)c0);

                px += 4u;
                py += 4u;
            }
            while (--k);
            k = len2 & 0x3u;

            while (k > 0u)
            {
                c0 = *(py++);
                x3 = *(px++);

                acc0 += ((q63_t)x0 * (q63_t)c0);
                acc1 += ((q63_t)x1 * (q63_t)c0);
                acc2 += ((q63_t)x2 * (q63_t)c0);
                acc3 += ((q63_t)x3 * (q63_t)c0);
                x0 = x1;
                x1 = x2;
                x2 = x3;
                k--;
            }
            //*pOut = NDS_ISA_SATS((acc0 >> 15u), 16);
            *pOut = (q31_t)(acc0 >> 31);
            pOut += inc;

            //*pOut = NDS_ISA_SATS((acc1 >> 15u), 16);
            *pOut = (q31_t)(acc1 >> 31);
            pOut += inc;

            //*pOut = NDS_ISA_SATS((acc2 >> 15u), 16);
            *pOut = (q31_t)(acc2 >> 31);
            pOut += inc;

            //*pOut = NDS_ISA_SATS((acc3 >> 15u), 16);
            *pOut = (q31_t)(acc3 >> 31);
            pOut += inc;

            count += 4u;

            px = pIn1 + count;
            py = pIn2;
            blkCnt--;
        }


        blkCnt = blockSize2 & 0x3u;
        while (blkCnt > 0u)
        {
            sum = 0;
            k = len2;
            while (k > 0u)
            {
                sum += ((q63_t) * px++ * (q63_t)(*py++));
                k--;
            }

            //*pOut = NDS_ISA_SATS((sum >> 15u), 16);
            *pOut = (q31_t)(sum >> 31);
            pOut += inc;
            count++;
            px = pIn1 + count;
            py = pIn2;
            blkCnt--;
        }
    }
    else
    {
        blkCnt = blockSize2;

        while (blkCnt > 0u)
        {
            sum = 0;
            k = len2;
            while (k > 0u)
            {
                sum += ((q63_t) * px++ * (q63_t)(*py++));
                k--;
            }

            //*pOut = NDS_ISA_SATS((sum >> 15u), 16);
            *pOut = (q31_t)(sum >> 31);
            pOut += inc;
            count++;
            px = pIn1 + count;
            py = pIn2;
            blkCnt--;
        }
    }
    //part3
    count = len2 - 1u;
    pSrc1 = (pIn1 + len1) - (len2 - 1u);
    px = pSrc1;
    py = pIn2;

    while (blockSize3 > 0u)
    {
        sum = 0;
        k = count;
        while (k > 0u)
        {
            sum += ((q63_t) * px++ * (q63_t) * py++);
            k--;
        }
        //*pOut = NDS_ISA_SATS((sum >> 15u), 16);
        *pOut = (q31_t)(sum >> 31);
        pOut += inc;
        px = ++pSrc1;
        py = pIn2;
        count--;
        blockSize3--;
    }
}

/**
 * @} end of corr
 */

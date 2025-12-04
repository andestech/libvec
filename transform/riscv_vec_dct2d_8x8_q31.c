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
#include "internal_vec_transform.h"
// #include "internal_nds_types.h"
#include "riscv_vec_utils.h"

#define PIPE_RESCHE

#define FDCT_PASS1_BITS 2
#define FDCT_COS_BITS 13
#define FDCT_COS_MUL (1 << FDCT_COS_BITS)
#define FDCT_RES_SHIFT (2 * FDCT_COS_BITS)
// #define FDCT_PASS1_RES_ROUND (1<<(FDCT_COS_BITS-1))
// #define FDCT_PASS2_RES_ROUND (1<<(FDCT_COS_BITS+FDCT_PASS1_BITS-1))
#define FDCT_RES_ROUND (1 << (FDCT_RES_SHIFT - 1))

#define FIX_0_298631336 ((int)2446)  /* FIX(0.298631336) */
#define FIX_0_390180644 ((int)3196)  /* FIX(0.390180644) */
#define FIX_0_541196100 ((int)4433)  /* FIX(0.541196100) */
#define FIX_0_765366865 ((int)6270)  /* FIX(0.765366865) */
#define FIX_0_899976223 ((int)7373)  /* FIX(0.899976223) */
#define FIX_1_175875602 ((int)9633)  /* FIX(1.175875602) */
#define FIX_1_501321110 ((int)12299) /* FIX(1.501321110) */
#define FIX_1_847759065 ((int)15137) /* FIX(1.847759065) */
#define FIX_1_961570560 ((int)16069) /* FIX(1.961570560) */
#define FIX_2_053119869 ((int)16819) /* FIX(2.053119869) */
#define FIX_2_562915447 ((int)20995) /* FIX(2.562915447) */
#define FIX_3_072711026 ((int)25172) /* FIX(3.072711026) */

#define DCTSIZE 8
#define DCTSIZE2 DCTSIZE *DCTSIZE
#define MAXJSAMPLE 255
#define CENTERJSAMPLE 128

#define MULTIPLY(var, const) ((var) * (const))

#define DEQUANTIZE(coef, quantval) (((int32_t)(coef)) * (quantval))

#define RANGE_MASK (MAXJSAMPLE * 4 + 3) /* 2 bits wider than legal samples */

#define LEFT_SHIFT(a, b) ((int32_t)((unsigned long)(a) << (b)))

#define RIGHT_SHIFT(x, shft) ((x) >> (shft))

#define DESCALE(x, n) RIGHT_SHIFT((x) + ((int32_t)1 << ((n)-1)), n)

riscv_vec_dct2d_8x8_cfg_q31_t riscv_vec_dct2d_8x8_init_q31()
{
    u32_t memneeded = sizeof(riscv_vec_dct2d_8x8_state_q31_t)
                      + sizeof(u8_t) * 1024     /* idct_limit */
                      + sizeof(q31_t) * 64      /* buffer */
                      + NDSV_ALIGNMENT;         /* 64-bit alignment */
                      
    riscv_vec_dct2d_8x8_cfg_q31_t ret = NDSV_MALLOC(memneeded);
    if (ret == NULL)
    {
        return NULL;
    }

    uintptr_t addr = (uintptr_t)ret + sizeof(riscv_vec_dct2d_8x8_state_q31_t);
    NDSV_ALIGN(addr, NDSV_ALIGNMENT);
    ret->idct_limit = (u8_t *)addr;
    ret->buffer = (q31_t *)(ret->idct_limit + 1024);

    for (int i = 0; i < 512; i++)
    {
        int val = i + 128;

        if (val < 0)
            ret->idct_limit[i] = 0;
        else if (val > 255)
            ret->idct_limit[i] = 255;
        else
            ret->idct_limit[i] = val;
    }
    for (int i = 512; i < 896; i++)
    {
        ret->idct_limit[i] = 0;
    }
    for (int i = 896; i < 1024; i++)
    {
        int val = i - 896;
        ret->idct_limit[i] = val;
    }

    return ret;
}

void riscv_vec_dct2d_8x8_q31(q31_t *dst, q31_t *src, riscv_vec_dct2d_8x8_cfg_q31_t cfg)
{
    if (cfg == NULL)
    {
        return;
    }      
#ifdef JPEG_9E
    int32_t tmp0, tmp1, tmp2, tmp3;
    int32_t tmp10, tmp11, tmp12, tmp13;
    int32_t z1;
    q31_t *dataptr;
    q31_t *elemptr;
    int ctr;

    /* Pass 1: process rows.
     * Note results are scaled up by sqrt(8) compared to a true DCT;
     * furthermore, we scale the results by 2**FDCT_PASS1_BITS.
     * cK represents sqrt(2) * cos(K*pi/16).
     */

    dataptr = dst;
    for (ctr = 0; ctr < 8; ctr++)
    {
        elemptr = &src[ctr * 8];
        /* Even part per LL&M figure 1 --- note that published figure is faulty;
         * rotator "c1" should be "c6".
         */

        tmp0 = elemptr[0] + elemptr[7];
        tmp1 = elemptr[1] + elemptr[6];
        tmp2 = elemptr[2] + elemptr[5];
        tmp3 = elemptr[3] + elemptr[4];

        tmp10 = tmp0 + tmp3;
        tmp12 = tmp0 - tmp3;
        tmp11 = tmp1 + tmp2;
        tmp13 = tmp1 - tmp2;

        tmp0 = (elemptr[0]) - (elemptr[7]);
        tmp1 = (elemptr[1]) - (elemptr[6]);
        tmp2 = (elemptr[2]) - (elemptr[5]);
        tmp3 = (elemptr[3]) - (elemptr[4]);

        /* Apply unsigned->signed conversion. */
        // dataptr[0] = (q31_t) ((tmp10 + tmp11 - 8 * CENTERJSAMPLE) << FDCT_PASS1_BITS);
        dataptr[0] = (q31_t)((tmp10 + tmp11) << FDCT_PASS1_BITS);
        dataptr[4] = (q31_t)((tmp10 - tmp11) << FDCT_PASS1_BITS);

        z1 = MULTIPLY(tmp12 + tmp13, FIX_0_541196100); /* c6 */
        /* Add fudge factor here for final descale. */
        z1 += 1 << (FDCT_COS_BITS - FDCT_PASS1_BITS - 1);

        dataptr[2] = (q31_t)
            RIGHT_SHIFT(z1 + MULTIPLY(tmp12, FIX_0_765366865), /* c2-c6 */
                        FDCT_COS_BITS - FDCT_PASS1_BITS);
        dataptr[6] = (q31_t)
            RIGHT_SHIFT(z1 - MULTIPLY(tmp13, FIX_1_847759065), /* c2+c6 */
                        FDCT_COS_BITS - FDCT_PASS1_BITS);

        /* Odd part per figure 8 --- note paper omits factor of sqrt(2).
         * i0..i3 in the paper are tmp0..tmp3 here.
         */

        tmp12 = tmp0 + tmp2;
        tmp13 = tmp1 + tmp3;

        z1 = MULTIPLY(tmp12 + tmp13, FIX_1_175875602); /*  c3 */
        /* Add fudge factor here for final descale. */
        z1 += 1 << (FDCT_COS_BITS - FDCT_PASS1_BITS - 1);

        tmp12 = MULTIPLY(tmp12, -FIX_0_390180644); /* -c3+c5 */
        tmp13 = MULTIPLY(tmp13, -FIX_1_961570560); /* -c3-c5 */
        tmp12 += z1;
        tmp13 += z1;

        z1 = MULTIPLY(tmp0 + tmp3, -FIX_0_899976223); /* -c3+c7 */
        tmp0 = MULTIPLY(tmp0, FIX_1_501321110);       /*  c1+c3-c5-c7 */
        tmp3 = MULTIPLY(tmp3, FIX_0_298631336);       /* -c1+c3+c5-c7 */
        tmp0 += z1 + tmp12;
        tmp3 += z1 + tmp13;

        z1 = MULTIPLY(tmp1 + tmp2, -FIX_2_562915447); /* -c1-c3 */
        tmp1 = MULTIPLY(tmp1, FIX_3_072711026);       /*  c1+c3+c5-c7 */
        tmp2 = MULTIPLY(tmp2, FIX_2_053119869);       /*  c1+c3-c5+c7 */
        tmp1 += z1 + tmp13;
        tmp2 += z1 + tmp12;

        dataptr[1] = (q31_t)RIGHT_SHIFT(tmp0, FDCT_COS_BITS - FDCT_PASS1_BITS);
        dataptr[3] = (q31_t)RIGHT_SHIFT(tmp1, FDCT_COS_BITS - FDCT_PASS1_BITS);
        dataptr[5] = (q31_t)RIGHT_SHIFT(tmp2, FDCT_COS_BITS - FDCT_PASS1_BITS);
        dataptr[7] = (q31_t)RIGHT_SHIFT(tmp3, FDCT_COS_BITS - FDCT_PASS1_BITS);

        dataptr += 8; /* advance pointer to next row */
    }

    /* Pass 2: process columns.
     * We remove the FDCT_PASS1_BITS scaling, but leave the results scaled up
     * by an overall factor of 8.
     * cK represents sqrt(2) * cos(K*pi/16).
     */

    dataptr = dst;
    for (ctr = 8 - 1; ctr >= 0; ctr--)
    {
        /* Even part per LL&M figure 1 --- note that published figure is faulty;
         * rotator "c1" should be "c6".
         */

        tmp0 = dataptr[8 * 0] + dataptr[8 * 7];
        tmp1 = dataptr[8 * 1] + dataptr[8 * 6];
        tmp2 = dataptr[8 * 2] + dataptr[8 * 5];
        tmp3 = dataptr[8 * 3] + dataptr[8 * 4];

        /* Add fudge factor here for final descale. */
        tmp10 = tmp0 + tmp3 + (1 << (FDCT_PASS1_BITS - 1));
        tmp12 = tmp0 - tmp3;
        tmp11 = tmp1 + tmp2;
        tmp13 = tmp1 - tmp2;

        tmp0 = dataptr[8 * 0] - dataptr[8 * 7];
        tmp1 = dataptr[8 * 1] - dataptr[8 * 6];
        tmp2 = dataptr[8 * 2] - dataptr[8 * 5];
        tmp3 = dataptr[8 * 3] - dataptr[8 * 4];

        dataptr[8 * 0] = (q31_t)RIGHT_SHIFT(tmp10 + tmp11, FDCT_PASS1_BITS);
        dataptr[8 * 4] = (q31_t)RIGHT_SHIFT(tmp10 - tmp11, FDCT_PASS1_BITS);

        z1 = MULTIPLY(tmp12 + tmp13, FIX_0_541196100); /* c6 */
        /* Add fudge factor here for final descale. */
        z1 += 1 << (FDCT_COS_BITS + FDCT_PASS1_BITS - 1);

        dataptr[8 * 2] = (q31_t)
            RIGHT_SHIFT(z1 + MULTIPLY(tmp12, FIX_0_765366865), /* c2-c6 */
                        FDCT_COS_BITS + FDCT_PASS1_BITS);
        dataptr[8 * 6] = (q31_t)
            RIGHT_SHIFT(z1 - MULTIPLY(tmp13, FIX_1_847759065), /* c2+c6 */
                        FDCT_COS_BITS + FDCT_PASS1_BITS);

        /* Odd part per figure 8 --- note paper omits factor of sqrt(2).
         * i0..i3 in the paper are tmp0..tmp3 here.
         */

        tmp12 = tmp0 + tmp2;
        tmp13 = tmp1 + tmp3;

        z1 = MULTIPLY(tmp12 + tmp13, FIX_1_175875602); /*  c3 */
        /* Add fudge factor here for final descale. */
        z1 += 1 << (FDCT_COS_BITS + FDCT_PASS1_BITS - 1);

        tmp12 = MULTIPLY(tmp12, -FIX_0_390180644); /* -c3+c5 */
        tmp13 = MULTIPLY(tmp13, -FIX_1_961570560); /* -c3-c5 */
        tmp12 += z1;
        tmp13 += z1;

        z1 = MULTIPLY(tmp0 + tmp3, -FIX_0_899976223); /* -c3+c7 */
        tmp0 = MULTIPLY(tmp0, FIX_1_501321110);       /*  c1+c3-c5-c7 */
        tmp3 = MULTIPLY(tmp3, FIX_0_298631336);       /* -c1+c3+c5-c7 */
        tmp0 += z1 + tmp12;
        tmp3 += z1 + tmp13;

        z1 = MULTIPLY(tmp1 + tmp2, -FIX_2_562915447); /* -c1-c3 */
        tmp1 = MULTIPLY(tmp1, FIX_3_072711026);       /*  c1+c3+c5-c7 */
        tmp2 = MULTIPLY(tmp2, FIX_2_053119869);       /*  c1+c3-c5+c7 */
        tmp1 += z1 + tmp13;
        tmp2 += z1 + tmp12;

        dataptr[8 * 1] = (q31_t)RIGHT_SHIFT(tmp0, FDCT_COS_BITS + FDCT_PASS1_BITS);
        dataptr[8 * 3] = (q31_t)RIGHT_SHIFT(tmp1, FDCT_COS_BITS + FDCT_PASS1_BITS);
        dataptr[8 * 5] = (q31_t)RIGHT_SHIFT(tmp2, FDCT_COS_BITS + FDCT_PASS1_BITS);
        dataptr[8 * 7] = (q31_t)RIGHT_SHIFT(tmp3, FDCT_COS_BITS + FDCT_PASS1_BITS);
        dataptr++; /* advance pointer to next column */
    }
#else  // jpeg-turbo _jpeg_fdct_islow
    q31_t tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
    q31_t tmp10, tmp11, tmp12, tmp13;
    q31_t z1, z2, z3, z4, z5;
    q31_t *dataptr;
    q31_t *outptr;
    q31_t ctr;

    /* Pass 1: process rows. */
    /* Note results are scaled up by sqrt(8) compared to a true DCT; */
    /* furthermore, we scale the results by 2**FDCT_PASS1_BITS. */

    dataptr = src;
    outptr = dst;
    for (ctr = DCTSIZE - 1; ctr >= 0; ctr--)
    {
        tmp0 = dataptr[0] + dataptr[7];
        tmp7 = dataptr[0] - dataptr[7];
        tmp1 = dataptr[1] + dataptr[6];
        tmp6 = dataptr[1] - dataptr[6];
        tmp2 = dataptr[2] + dataptr[5];
        tmp5 = dataptr[2] - dataptr[5];
        tmp3 = dataptr[3] + dataptr[4];
        tmp4 = dataptr[3] - dataptr[4];

        /* Even part per LL&M figure 1 --- note that published figure is faulty;
         * rotator "sqrt(2)*c1" should be "sqrt(2)*c6".
         */

        tmp10 = tmp0 + tmp3;
        tmp13 = tmp0 - tmp3;
        tmp11 = tmp1 + tmp2;
        tmp12 = tmp1 - tmp2;

        outptr[0] = (q31_t)LEFT_SHIFT(tmp10 + tmp11, FDCT_PASS1_BITS);
        outptr[4] = (q31_t)LEFT_SHIFT(tmp10 - tmp11, FDCT_PASS1_BITS);

        z1 = MULTIPLY(tmp12 + tmp13, FIX_0_541196100);
        outptr[2] = (q31_t)DESCALE(z1 + MULTIPLY(tmp13, FIX_0_765366865),
                                   FDCT_COS_BITS - FDCT_PASS1_BITS);
        outptr[6] = (q31_t)DESCALE(z1 + MULTIPLY(tmp12, -FIX_1_847759065),
                                   FDCT_COS_BITS - FDCT_PASS1_BITS);

        /* Odd part per figure 8 --- note paper omits factor of sqrt(2).
         * cK represents cos(K*pi/16).
         * i0..i3 in the paper are tmp4..tmp7 here.
         */

        z1 = tmp4 + tmp7;
        z2 = tmp5 + tmp6;
        z3 = tmp4 + tmp6;
        z4 = tmp5 + tmp7;
        z5 = MULTIPLY(z3 + z4, FIX_1_175875602); /* sqrt(2) * c3 */

        tmp4 = MULTIPLY(tmp4, FIX_0_298631336); /* sqrt(2) * (-c1+c3+c5-c7) */
        tmp5 = MULTIPLY(tmp5, FIX_2_053119869); /* sqrt(2) * ( c1+c3-c5+c7) */
        tmp6 = MULTIPLY(tmp6, FIX_3_072711026); /* sqrt(2) * ( c1+c3+c5-c7) */
        tmp7 = MULTIPLY(tmp7, FIX_1_501321110); /* sqrt(2) * ( c1+c3-c5-c7) */
        z1 = MULTIPLY(z1, -FIX_0_899976223);    /* sqrt(2) * ( c7-c3) */
        z2 = MULTIPLY(z2, -FIX_2_562915447);    /* sqrt(2) * (-c1-c3) */
        z3 = MULTIPLY(z3, -FIX_1_961570560);    /* sqrt(2) * (-c3-c5) */
        z4 = MULTIPLY(z4, -FIX_0_390180644);    /* sqrt(2) * ( c5-c3) */

        z3 += z5;
        z4 += z5;

        outptr[7] = (q31_t)DESCALE(tmp4 + z1 + z3, FDCT_COS_BITS - FDCT_PASS1_BITS);
        outptr[5] = (q31_t)DESCALE(tmp5 + z2 + z4, FDCT_COS_BITS - FDCT_PASS1_BITS);
        outptr[3] = (q31_t)DESCALE(tmp6 + z2 + z3, FDCT_COS_BITS - FDCT_PASS1_BITS);
        outptr[1] = (q31_t)DESCALE(tmp7 + z1 + z4, FDCT_COS_BITS - FDCT_PASS1_BITS);

        dataptr += DCTSIZE; /* advance pointer to next row */
        outptr += DCTSIZE;
    }

    /* Pass 2: process columns.
     * We remove the FDCT_PASS1_BITS scaling, but leave the results scaled up
     * by an overall factor of 8.
     */

    outptr = dst;
    for (ctr = DCTSIZE - 1; ctr >= 0; ctr--)
    {
        tmp0 = outptr[DCTSIZE * 0] + outptr[DCTSIZE * 7];
        tmp7 = outptr[DCTSIZE * 0] - outptr[DCTSIZE * 7];
        tmp1 = outptr[DCTSIZE * 1] + outptr[DCTSIZE * 6];
        tmp6 = outptr[DCTSIZE * 1] - outptr[DCTSIZE * 6];
        tmp2 = outptr[DCTSIZE * 2] + outptr[DCTSIZE * 5];
        tmp5 = outptr[DCTSIZE * 2] - outptr[DCTSIZE * 5];
        tmp3 = outptr[DCTSIZE * 3] + outptr[DCTSIZE * 4];
        tmp4 = outptr[DCTSIZE * 3] - outptr[DCTSIZE * 4];

        /* Even part per LL&M figure 1 --- note that published figure is faulty;
         * rotator "sqrt(2)*c1" should be "sqrt(2)*c6".
         */

        tmp10 = tmp0 + tmp3;
        tmp13 = tmp0 - tmp3;
        tmp11 = tmp1 + tmp2;
        tmp12 = tmp1 - tmp2;

        outptr[DCTSIZE * 0] = (q31_t)DESCALE(tmp10 + tmp11, FDCT_PASS1_BITS);
        outptr[DCTSIZE * 4] = (q31_t)DESCALE(tmp10 - tmp11, FDCT_PASS1_BITS);

        z1 = MULTIPLY(tmp12 + tmp13, FIX_0_541196100);
        outptr[DCTSIZE * 2] =
            (q31_t)DESCALE(z1 + MULTIPLY(tmp13, FIX_0_765366865),
                           FDCT_COS_BITS + FDCT_PASS1_BITS);
        outptr[DCTSIZE * 6] =
            (q31_t)DESCALE(z1 + MULTIPLY(tmp12, -FIX_1_847759065),
                           FDCT_COS_BITS + FDCT_PASS1_BITS);

        /* Odd part per figure 8 --- note paper omits factor of sqrt(2).
         * cK represents cos(K*pi/16).
         * i0..i3 in the paper are tmp4..tmp7 here.
         */

        z1 = tmp4 + tmp7;
        z2 = tmp5 + tmp6;
        z3 = tmp4 + tmp6;
        z4 = tmp5 + tmp7;
        z5 = MULTIPLY(z3 + z4, FIX_1_175875602); /* sqrt(2) * c3 */

        tmp4 = MULTIPLY(tmp4, FIX_0_298631336); /* sqrt(2) * (-c1+c3+c5-c7) */
        tmp5 = MULTIPLY(tmp5, FIX_2_053119869); /* sqrt(2) * ( c1+c3-c5+c7) */
        tmp6 = MULTIPLY(tmp6, FIX_3_072711026); /* sqrt(2) * ( c1+c3+c5-c7) */
        tmp7 = MULTIPLY(tmp7, FIX_1_501321110); /* sqrt(2) * ( c1+c3-c5-c7) */
        z1 = MULTIPLY(z1, -FIX_0_899976223);    /* sqrt(2) * ( c7-c3) */
        z2 = MULTIPLY(z2, -FIX_2_562915447);    /* sqrt(2) * (-c1-c3) */
        z3 = MULTIPLY(z3, -FIX_1_961570560);    /* sqrt(2) * (-c3-c5) */
        z4 = MULTIPLY(z4, -FIX_0_390180644);    /* sqrt(2) * ( c5-c3) */

        z3 += z5;
        z4 += z5;

        outptr[DCTSIZE * 7] = (q31_t)DESCALE(tmp4 + z1 + z3,
                                             FDCT_COS_BITS + FDCT_PASS1_BITS);
        outptr[DCTSIZE * 5] = (q31_t)DESCALE(tmp5 + z2 + z4,
                                             FDCT_COS_BITS + FDCT_PASS1_BITS);
        outptr[DCTSIZE * 3] = (q31_t)DESCALE(tmp6 + z2 + z3,
                                             FDCT_COS_BITS + FDCT_PASS1_BITS);
        outptr[DCTSIZE * 1] = (q31_t)DESCALE(tmp7 + z1 + z4,
                                             FDCT_COS_BITS + FDCT_PASS1_BITS);

        outptr++; /* advance pointer to next column */
    }

#endif
}

void riscv_vec_idct2d_8x8_q31(u8_t *dst, q31_t *src, q31_t *dct_table, riscv_vec_dct2d_8x8_cfg_q31_t cfg)
{
    if (cfg == NULL)
    {
        return;
    }     
    long tmp0, tmp1, tmp2, tmp3;
    long tmp10, tmp11, tmp12, tmp13;
    long z1, z2, z3, z4, z5;
    q31_t *inptr;
    q31_t *quantptr;
    int32_t *wsptr;
    u8_t *outptr;
    u8_t *range_limit = cfg->idct_limit;
    int32_t ctr;
    q31_t *workspace = cfg->buffer; /* buffers data between passes */

    /* Pass 1: process columns from input, store into work array. */
    /* Note results are scaled up by sqrt(8) compared to a true IDCT; */
    /* furthermore, we scale the results by 2**FDCT_PASS1_BITS. */

    inptr = src;
    quantptr = dct_table;
    wsptr = workspace;
    for (ctr = DCTSIZE; ctr > 0; ctr--)
    {
        /* Due to quantization, we will usually find that many of the input
         * coefficients are zero, especially the AC terms.    We can exploit this
         * by short-circuiting the IDCT calculation for any column in which all
         * the AC terms are zero.    In that case each output is equal to the
         * DC coefficient (with scale factor as needed).
         * With typical images and quantization tables, half or more of the
         * column DCT calculations can be simplified this way.
         */

        if (inptr[DCTSIZE * 1] == 0 && inptr[DCTSIZE * 2] == 0 &&
            inptr[DCTSIZE * 3] == 0 && inptr[DCTSIZE * 4] == 0 &&
            inptr[DCTSIZE * 5] == 0 && inptr[DCTSIZE * 6] == 0 &&
            inptr[DCTSIZE * 7] == 0)
        {
            /* AC terms all zero */
            int dcval = LEFT_SHIFT(DEQUANTIZE(inptr[DCTSIZE * 0],
                                              quantptr[DCTSIZE * 0]),
                                   FDCT_PASS1_BITS);

            wsptr[DCTSIZE * 0] = dcval;
            wsptr[DCTSIZE * 1] = dcval;
            wsptr[DCTSIZE * 2] = dcval;
            wsptr[DCTSIZE * 3] = dcval;
            wsptr[DCTSIZE * 4] = dcval;
            wsptr[DCTSIZE * 5] = dcval;
            wsptr[DCTSIZE * 6] = dcval;
            wsptr[DCTSIZE * 7] = dcval;

            inptr++; /* advance pointers to next column */
            quantptr++;
            wsptr++;
            continue;
        }

        /* Even part: reverse the even part of the forward DCT. */
        /* The rotator is sqrt(2)*c(-6). */

        z2 = DEQUANTIZE(inptr[DCTSIZE * 2], quantptr[DCTSIZE * 2]);
        z3 = DEQUANTIZE(inptr[DCTSIZE * 6], quantptr[DCTSIZE * 6]);

        z1 = MULTIPLY(z2 + z3, FIX_0_541196100);
        tmp2 = z1 + MULTIPLY(z3, -FIX_1_847759065);
        tmp3 = z1 + MULTIPLY(z2, FIX_0_765366865);

        z2 = DEQUANTIZE(inptr[DCTSIZE * 0], quantptr[DCTSIZE * 0]);
        z3 = DEQUANTIZE(inptr[DCTSIZE * 4], quantptr[DCTSIZE * 4]);

        tmp0 = LEFT_SHIFT(z2 + z3, FDCT_COS_BITS);
        tmp1 = LEFT_SHIFT(z2 - z3, FDCT_COS_BITS);

        tmp10 = tmp0 + tmp3;
        tmp13 = tmp0 - tmp3;
        tmp11 = tmp1 + tmp2;
        tmp12 = tmp1 - tmp2;

        /* Odd part per figure 8; the matrix is unitary and hence its
         * transpose is its inverse.    i0..i3 are y7,y5,y3,y1 respectively.
         */

        tmp0 = DEQUANTIZE(inptr[DCTSIZE * 7], quantptr[DCTSIZE * 7]);
        tmp1 = DEQUANTIZE(inptr[DCTSIZE * 5], quantptr[DCTSIZE * 5]);
        tmp2 = DEQUANTIZE(inptr[DCTSIZE * 3], quantptr[DCTSIZE * 3]);
        tmp3 = DEQUANTIZE(inptr[DCTSIZE * 1], quantptr[DCTSIZE * 1]);

        z1 = tmp0 + tmp3;
        z2 = tmp1 + tmp2;
        z3 = tmp0 + tmp2;
        z4 = tmp1 + tmp3;
        z5 = MULTIPLY(z3 + z4, FIX_1_175875602); /* sqrt(2) * c3 */

        tmp0 = MULTIPLY(tmp0, FIX_0_298631336); /* sqrt(2) * (-c1+c3+c5-c7) */
        tmp1 = MULTIPLY(tmp1, FIX_2_053119869); /* sqrt(2) * ( c1+c3-c5+c7) */
        tmp2 = MULTIPLY(tmp2, FIX_3_072711026); /* sqrt(2) * ( c1+c3+c5-c7) */
        tmp3 = MULTIPLY(tmp3, FIX_1_501321110); /* sqrt(2) * ( c1+c3-c5-c7) */
        z1 = MULTIPLY(z1, -FIX_0_899976223);    /* sqrt(2) * ( c7-c3) */
        z2 = MULTIPLY(z2, -FIX_2_562915447);    /* sqrt(2) * (-c1-c3) */
        z3 = MULTIPLY(z3, -FIX_1_961570560);    /* sqrt(2) * (-c3-c5) */
        z4 = MULTIPLY(z4, -FIX_0_390180644);    /* sqrt(2) * ( c5-c3) */

        z3 += z5;
        z4 += z5;

        tmp0 += z1 + z3;
        tmp1 += z2 + z4;
        tmp2 += z2 + z3;
        tmp3 += z1 + z4;

        /* Final output stage: inputs are tmp10..tmp13, tmp0..tmp3 */

        wsptr[DCTSIZE * 0] = (int)DESCALE(tmp10 + tmp3, FDCT_COS_BITS - FDCT_PASS1_BITS);
        wsptr[DCTSIZE * 7] = (int)DESCALE(tmp10 - tmp3, FDCT_COS_BITS - FDCT_PASS1_BITS);
        wsptr[DCTSIZE * 1] = (int)DESCALE(tmp11 + tmp2, FDCT_COS_BITS - FDCT_PASS1_BITS);
        wsptr[DCTSIZE * 6] = (int)DESCALE(tmp11 - tmp2, FDCT_COS_BITS - FDCT_PASS1_BITS);
        wsptr[DCTSIZE * 2] = (int)DESCALE(tmp12 + tmp1, FDCT_COS_BITS - FDCT_PASS1_BITS);
        wsptr[DCTSIZE * 5] = (int)DESCALE(tmp12 - tmp1, FDCT_COS_BITS - FDCT_PASS1_BITS);
        wsptr[DCTSIZE * 3] = (int)DESCALE(tmp13 + tmp0, FDCT_COS_BITS - FDCT_PASS1_BITS);
        wsptr[DCTSIZE * 4] = (int)DESCALE(tmp13 - tmp0, FDCT_COS_BITS - FDCT_PASS1_BITS);

        inptr++; /* advance pointers to next column */
        quantptr++;
        wsptr++;
    }

    /* Pass 2: process rows from work array, store into output array. */
    /* Note that we must descale the results by a factor of 8 == 2**3, */
    /* and also undo the FDCT_PASS1_BITS scaling. */

    wsptr = workspace;
    for (ctr = 0; ctr < DCTSIZE; ctr++)
    {
        outptr = dst + DCTSIZE * ctr;
        /* Rows of zeroes can be exploited in the same way as we did with columns.
         * However, the column calculation has created many nonzero AC terms, so
         * the simplification applies less often (typically 5% to 10% of the time).
         * On machines with very fast multiplication, it's possible that the
         * test takes more time than it's worth.    In that case this section
         * may be commented out.
         */

        /* Even part: reverse the even part of the forward DCT. */
        /* The rotator is sqrt(2)*c(-6). */

        z2 = (long)wsptr[2];
        z3 = (long)wsptr[6];

        z1 = MULTIPLY(z2 + z3, FIX_0_541196100);
        tmp2 = z1 + MULTIPLY(z3, -FIX_1_847759065);
        tmp3 = z1 + MULTIPLY(z2, FIX_0_765366865);

        tmp0 = LEFT_SHIFT((long)wsptr[0] + (long)wsptr[4], FDCT_COS_BITS);
        tmp1 = LEFT_SHIFT((long)wsptr[0] - (long)wsptr[4], FDCT_COS_BITS);

        tmp10 = tmp0 + tmp3;
        tmp13 = tmp0 - tmp3;
        tmp11 = tmp1 + tmp2;
        tmp12 = tmp1 - tmp2;

        /* Odd part per figure 8; the matrix is unitary and hence its
         * transpose is its inverse.    i0..i3 are y7,y5,y3,y1 respectively.
         */

        tmp0 = (long)wsptr[7];
        tmp1 = (long)wsptr[5];
        tmp2 = (long)wsptr[3];
        tmp3 = (long)wsptr[1];

        z1 = tmp0 + tmp3;
        z2 = tmp1 + tmp2;
        z3 = tmp0 + tmp2;
        z4 = tmp1 + tmp3;
        z5 = MULTIPLY(z3 + z4, FIX_1_175875602); /* sqrt(2) * c3 */

        tmp0 = MULTIPLY(tmp0, FIX_0_298631336); /* sqrt(2) * (-c1+c3+c5-c7) */
        tmp1 = MULTIPLY(tmp1, FIX_2_053119869); /* sqrt(2) * ( c1+c3-c5+c7) */
        tmp2 = MULTIPLY(tmp2, FIX_3_072711026); /* sqrt(2) * ( 1+c3+c5-c7) */
        tmp3 = MULTIPLY(tmp3, FIX_1_501321110); /* sqrt(2) * ( c1+c3-c5-c7) */
        z1 = MULTIPLY(z1, -FIX_0_899976223);    /* sqrt(2) * ( c7-c3) */
        z2 = MULTIPLY(z2, -FIX_2_562915447);    /* sqrt(2) * (-c1-c3) */
        z3 = MULTIPLY(z3, -FIX_1_961570560);    /* sqrt(2) * (-c3-c5) */
        z4 = MULTIPLY(z4, -FIX_0_390180644);    /* sqrt(2) * ( c5-c3) */

        z3 += z5;
        z4 += z5;

        tmp0 += z1 + z3;
        tmp1 += z2 + z4;
        tmp2 += z2 + z3;
        tmp3 += z1 + z4;

        /* Final output stage: inputs are tmp10..tmp13, tmp0..tmp3 */
        outptr[0] = range_limit[(int)DESCALE(tmp10 + tmp3, FDCT_COS_BITS + FDCT_PASS1_BITS + 3) & RANGE_MASK];
        outptr[7] = range_limit[(int)DESCALE(tmp10 - tmp3, FDCT_COS_BITS + FDCT_PASS1_BITS + 3) & RANGE_MASK];
        outptr[1] = range_limit[(int)DESCALE(tmp11 + tmp2, FDCT_COS_BITS + FDCT_PASS1_BITS + 3) & RANGE_MASK];
        outptr[6] = range_limit[(int)DESCALE(tmp11 - tmp2, FDCT_COS_BITS + FDCT_PASS1_BITS + 3) & RANGE_MASK];
        outptr[2] = range_limit[(int)DESCALE(tmp12 + tmp1, FDCT_COS_BITS + FDCT_PASS1_BITS + 3) & RANGE_MASK];
        outptr[5] = range_limit[(int)DESCALE(tmp12 - tmp1, FDCT_COS_BITS + FDCT_PASS1_BITS + 3) & RANGE_MASK];
        outptr[3] = range_limit[(int)DESCALE(tmp13 + tmp0, FDCT_COS_BITS + FDCT_PASS1_BITS + 3) & RANGE_MASK];
        outptr[4] = range_limit[(int)DESCALE(tmp13 - tmp0, FDCT_COS_BITS + FDCT_PASS1_BITS + 3) & RANGE_MASK];

        wsptr += DCTSIZE; /* advance pointer to next row */
    }
}

void riscv_vec_dct2d_8x8_free_q31(riscv_vec_dct2d_8x8_cfg_q31_t cfg)
{
    NDSV_FREE(cfg);
}

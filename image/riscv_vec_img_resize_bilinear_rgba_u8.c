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

#include "internal_nds_types.h"
#include <string.h>

#define INTER_RESIZE_COEF_BITS  11
#define INTER_RESIZE_COEF_SCALE (1 << 11)
#define MAX_ESIZE          16

static inline u32_t riscv_vec_align_size (q31_t sz, q31_t n)
{
    return (sz + n - 1) & -n;
}

static inline q31_t riscv_vec_floor (float32_t a)
{
    return ( ( (a) >= 0) ? ( (q31_t) a) : ( (q31_t) a - 1));
}

static inline q31_t riscv_vec_clip (q31_t x, q31_t a, q31_t b)
{
    return (x >= a ? (x < b ? x : b - 1) : a);
}
static void riscv_vec_img_hresize_linear (const u8_t** src,
        q31_t** dst,
        q31_t count,
        const q31_t* xofs,
        const q15_t* alpha,
        q31_t swidth,
        q31_t dwidth,
        q31_t cn,
        q31_t xmin,
        q31_t xmax)
{
    q31_t k;

    q31_t dx;
    q31_t dx0 = 0;

    //for (k = 0; k <= count - 2; k++)
    if (count == 2)
    {
        k = 0;
        const u8_t *S0 = src[k], *S1 = src[k + 1];
        q31_t *D0 = dst[k], *D1 = dst[k + 1];
        for (dx = dx0; dx < xmax; dx++)
        {
            q31_t sx = xofs[dx];
            q31_t a0 = alpha[dx * 2], a1 = alpha[dx * 2 + 1];
            q31_t t0 = S0[sx] * a0 + S0[sx + cn] * a1;
            q31_t t1 = S1[sx] * a0 + S1[sx + cn] * a1;
            D0[dx] = t0;
            D1[dx] = t1;
        }

        for (; dx < dwidth; dx++)
        {
            q31_t sx = xofs[dx];
            D0[dx] = (q31_t) S0[sx] * INTER_RESIZE_COEF_SCALE;
            D1[dx] = (q31_t) S1[sx] * INTER_RESIZE_COEF_SCALE;
        }
    }
    //for (; k < count; k++)
    if (count == 1)
    {
        k = 0;
        const u8_t *S = src[k];
        q31_t *D = dst[k];
        for (dx = 0; dx < xmax; dx++)
        {
            q31_t sx = xofs[dx];
            D[dx] = S[sx] * alpha[dx * 2] + S[sx + cn] * alpha[dx * 2 + 1];
        }

        for (dx = xmax; dx < dwidth; dx++)
        {
            D[dx] = (q31_t) S[xofs[dx]] * INTER_RESIZE_COEF_SCALE;
        }
    }
}
static inline u8_t riscv_vec_cast_op (q31_t val)
{
    q31_t bits = INTER_RESIZE_COEF_BITS * 2;
    q31_t SHIFT = bits;
    q31_t DELTA = 1 << (bits - 1) ;
    q31_t temp = NDSV_MIN (255, NDSV_MAX (0, (val + DELTA) >> SHIFT));
    return (u8_t) (temp);
};
void riscv_vec_img_vresize_linear (const q31_t** src, u8_t* dst, const q15_t* beta, q31_t width)
{
    q31_t b0 = beta[0], b1 = beta[1];
    const q31_t *S0 = src[0], *S1 = src[1];

    q31_t x;
    for (x = 0; x < width; x += 4)
    {
        q31_t t0, t1;
        t0 = S0[x] * b0 + S1[x] * b1;
        t1 = S0[x + 1] * b0 + S1[x + 1] * b1;
        dst[x] = riscv_vec_cast_op (t0);
        dst[x + 1] = riscv_vec_cast_op (t1);
        t0 = S0[x + 2] * b0 + S1[x + 2] * b1;
        t1 = S0[x + 3] * b0 + S1[x + 3] * b1;
        dst[x + 2] = riscv_vec_cast_op (t0);
        dst[x + 3] = riscv_vec_cast_op (t1);
    }
}

static void riscv_vec_img_resize_generic_linear (u8_t* src,
        u8_t* dst,
        const q31_t* xofs,
        const q15_t* _alpha,
        const q31_t* yofs,
        const q15_t* _beta,
        q31_t xmin,
        q31_t xmax,
        q31_t ksize,
        q31_t srcw,
        q31_t srch,
        q31_t srcstep,
        q31_t dstw,
        q31_t dsth,
        q31_t channels)
{

    const q15_t* alpha = _alpha;
    const q15_t* beta = _beta;
    q31_t cn = channels;
    srcw *= cn;
    dstw *= cn;

    q31_t bufstep = (q31_t) riscv_vec_align_size (dstw, 16);
    q31_t dststep = (q31_t) riscv_vec_align_size (dstw, 4);

    q31_t *buf = (q31_t*) NDSV_MALLOC (bufstep * ksize * sizeof (q31_t));

    const u8_t* srows[MAX_ESIZE];
    q31_t* rows[MAX_ESIZE];
    q31_t prev_sy[MAX_ESIZE];
    q31_t k, dy;
    xmin *= cn;
    xmax *= cn;

    for (k = 0; k < ksize; k++)
    {
        prev_sy[k] = -1;
        rows[k] = (q31_t*) buf + bufstep * k;
    }

    // image resize is a separable operation. In case of not too strong
    for (dy = 0; dy < dsth; dy++, beta += ksize)
    {
        q31_t sy0 = yofs[dy], k, k0 = ksize, k1 = 0, ksize2 = ksize / 2;

        for (k = 0; k < ksize; k++)
        {
            q31_t sy = riscv_vec_clip (sy0 - ksize2 + 1 + k, 0, srch);
            for (k1 = NDSV_MAX (k1, k); k1 < ksize; k1++)
            {
                if (sy == prev_sy[k1])  // if the sy-th row has been computed already, reuse it.
                {
                    if (k1 > k)
                        memcpy (rows[k], rows[k1], bufstep * sizeof (rows[0][0]));
                    break;
                }
            }
            if (k1 == ksize)
                k0 = NDSV_MIN (k0, k); // remember the first row that needs to be computed
            srows[k] = (const u8_t*) (src + srcstep * sy);
            prev_sy[k] = sy;
        }

        if (k0 < ksize)
            riscv_vec_img_hresize_linear (srows + k0, rows + k0, ksize - k0, xofs, alpha,
                                          srcw, dstw, cn, xmin, xmax);

        riscv_vec_img_vresize_linear ( (const q31_t**) rows, (u8_t*) (dst + dststep * dy), beta, dstw);
    }

    NDSV_FREE (buf);
}

static void riscv_vec_img_resize_cal_offset_linear (q31_t* xofs,
        q15_t* ialpha,
        q31_t* yofs,
        q15_t* ibeta,
        q31_t *xmin,
        q31_t *xmax,
        q31_t ksize,
        q31_t ksize2,
        q31_t srcw,
        q31_t srch,
        q31_t dstw,
        q31_t dsth,
        q31_t channels)
{
    float32_t inv_scale_x = (float32_t) dstw / srcw;
    float32_t inv_scale_y = (float32_t) dsth / srch;

    q31_t cn = channels;
    float32_t scale_x = 1.0f / inv_scale_x;
    float32_t scale_y = 1.0f / inv_scale_y;
    q31_t k, sx, sy, dx, dy;


    float32_t fx, fy;

    float32_t cbuf[MAX_ESIZE];

    for (dx = 0; dx < dstw; dx++)
    {
        fx = (float32_t) ( (dx + 0.5f) * scale_x - 0.5f);
        sx = riscv_vec_floor (fx);
        fx -= sx;

        if (sx < ksize2 - 1)
        {
            *xmin = dx + 1;
            if (sx < 0)
                fx = 0, sx = 0;
        }

        if (sx + ksize2 >= srcw)
        {
            *xmax = NDSV_MIN (*xmax, dx);
            if (sx >= srcw - 1)
                fx = 0, sx = srcw - 1;
        }

        for (k = 0, sx *= cn; k < cn; k++)
            xofs[dx * cn + k] = sx + k;

        cbuf[0] = 1.f - fx;
        cbuf[1] = fx;

        for (k = 0; k < ksize; k++)
        {
            ialpha[dx * cn * ksize + k] = (q15_t) (cbuf[k] * INTER_RESIZE_COEF_SCALE);
        }
        for (; k < cn * ksize; k++)
        {
            ialpha[dx * cn * ksize + k] = ialpha[dx * cn * ksize + k - ksize];
        }
    }

    for (dy = 0; dy < dsth; dy++)
    {
        fy = (float32_t) ( (dy + 0.5f) * scale_y - 0.5f);
        sy = riscv_vec_floor (fy);
        fy -= sy;

        yofs[dy] = sy;

        cbuf[0] = 1.0f - fy;
        cbuf[1] = fy;

        for (k = 0; k < ksize; k++)
            ibeta[dy * ksize + k] = (q15_t) (cbuf[k] * INTER_RESIZE_COEF_SCALE);

    }

}

void riscv_vec_img_resize_bilinear_rgba_u8(u8_t* dst,
        u32_t dst_width,
        u32_t dst_height,
        u8_t* src,
        u32_t src_width,
        u32_t src_height,
        u32_t src_stride)
{
    q31_t dstw = dst_width;
    q31_t dsth = dst_height;
    q31_t srcw = src_width;
    q31_t srch = src_height;

    q31_t cn = 4;
    q31_t xmin = 0;
    q31_t xmax = dstw;
    q31_t width = dstw * cn;

    q31_t ksize = 0, ksize2;
    ksize = 2;
    ksize2 = ksize / 2;

    // (4 * dstw + dsth) * (4 + 4 * 2);
    u8_t *buf = (u8_t*) NDSV_MALLOC ( (width + dsth) * (sizeof (q31_t) + sizeof (float32_t) * ksize));

    q31_t* xofs = (q31_t*) buf;
    q31_t* yofs = xofs + width;
    q15_t* ialpha = (q15_t*) (yofs + dsth);
    q15_t* ibeta = ialpha + width * ksize;

    riscv_vec_img_resize_cal_offset_linear(xofs, ialpha, yofs, ibeta, &xmin, &xmax, ksize, ksize2, srcw, srch, dstw, dsth, cn);
    riscv_vec_img_resize_generic_linear(src, dst, xofs, ialpha, yofs, ibeta, xmin, xmax, ksize, srcw, srch, src_stride, dstw, dsth, cn);
    NDSV_FREE (buf);
}

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

#ifndef __INTERNAL_VEC_IMAGE_RESIZE_TEMPLATE_PUREC_U8_H__
#define __INTERNAL_VEC_IMAGE_RESIZE_TEMPLATE_PUREC_U8_H__

#include "internal_nds_types.h"
// #include <string.h>
// #include <math.h>
#include <stdint.h> //uintptr_t

static inline q31_t vec_floor (double value)
{
    q31_t i = (q31_t) value;
    return i - (i > value);
}
static inline q31_t vec_floorf (float32_t a)
{
    return ( ( (a) >= 0) ? ( (q31_t) a) : ( (q31_t) a - 1));
}

static inline uintptr_t vec_align_size (uintptr_t sz, q31_t n)
{
    return (sz + n - 1) & -n;
}

static inline q31_t vec_clip (q31_t x, q31_t a, q31_t b)
{
    return (x >= a ? (x < b ? x : b - 1) : a);
}

// Nearest
static inline void resizeNN(
    const u8_t* src,
    u8_t* dst,
    q31_t *x_ofs,
    const q31_t swidth, const q31_t sheight, const q31_t sstride,
    const q31_t dwidth, const q31_t dheight,
    const q31_t pix_size,
    const double ify )
{
    int y, x;
    for( y = 0; y < dheight ; y++ )
    {
        u8_t* D = dst + dwidth * pix_size * y;
        int sy = NDSV_MIN(vec_floor(y * ify), sheight - 1);
        const u8_t* S = src + sstride * sy;

        switch( pix_size )
        {
            case 1:
            {
                for( x = 0; x <= dwidth - 2; x += 2 )
                {
                    u8_t t0 = S[x_ofs[x]];
                    u8_t t1 = S[x_ofs[x+1]];
                    D[x] = t0;
                    D[x+1] = t1;
                }

                for( ; x < dwidth; x++ )
                    D[x] = S[x_ofs[x]];
                break;
            }
            case 2:
                for( x = 0; x < dwidth; x++ )
                    *(u16_t*)(D + x*2) = *(u16_t*)(S + x_ofs[x]);
                break;
            case 3:
                for( x = 0; x < dwidth; x++, D += 3 )
                {
                    const u8_t* _tS = S + x_ofs[x];
                    D[0] = _tS[0]; D[1] = _tS[1]; D[2] = _tS[2];
                }
                break;
            case 4:
                for( x = 0; x < dwidth; x++ )
                    *(int*)(D + x*4) = *(int*)(S + x_ofs[x]);
                break;
            // case 6:
            //     for( x = 0; x < dwidth; x++, D += 6 )
            //     {
            //         const ushort* _tS = (const ushort*)(S + x_ofs[x]);
            //         ushort* _tD = (ushort*)D;
            //         _tD[0] = _tS[0]; _tD[1] = _tS[1]; _tD[2] = _tS[2];
            //     }
            //     break;
            // case 8:
            //     for( x = 0; x < dwidth; x++, D += 8 )
            //     {
            //         const int* _tS = (const int*)(S + x_ofs[x]);
            //         int* _tD = (int*)D;
            //         _tD[0] = _tS[0]; _tD[1] = _tS[1];
            //     }
            //     break;
            // case 12:
            //     for( x = 0; x < dwidth; x++, D += 12 )
            //     {
            //         const int* _tS = (const int*)(S + x_ofs[x]);
            //         int* _tD = (int*)D;
            //         _tD[0] = _tS[0]; _tD[1] = _tS[1]; _tD[2] = _tS[2];
            //     }
            //     break;
            default:
                for( x = 0; x < dwidth; x++, D += pix_size )
                {
                    const u8_t* _tS = S + x_ofs[x];
                    for (int k = 0; k < pix_size; k++)
                        D[k] = _tS[k];
                }
        }
    }
}

static inline void vec_img_resize_nearest_u8(
    u8_t* FUNC_RESTRICT dst,
    u32_t dst_width,
    u32_t dst_height,
    u8_t* src,
    u32_t src_width,
    u32_t src_height,
    u32_t src_stride,
    u32_t channel)
{
    q31_t pix_size = channel * sizeof(u8_t);

    double inv_scale_x = (double) dst_width / src_width;
    double inv_scale_y = (double) dst_height / src_height;

    double ifx = 1.0f / inv_scale_x;
    double ify = 1.0f / inv_scale_y;

    q31_t bufstep = (q31_t) vec_align_size (dst_width, 64); //align 64-byte for cache friendly
    q31_t *x_ofs = (q31_t*) NDSV_MALLOC (bufstep * sizeof (q31_t));

    int x;
    for( x = 0; x < dst_width; x++ )
    {
        int sx = vec_floor(x * ifx);
        x_ofs[x] = NDSV_MIN(sx, src_width - 1) * pix_size;
    }

    resizeNN(src, dst, x_ofs, src_width, src_height, src_stride, dst_width, dst_height, pix_size, ify);

    NDSV_FREE (x_ofs);
}

// Nearest exact
static void resizeNN_bitexact(
    const u8_t* src,
    u8_t* dst,
    q31_t *x_ofse,
    const q31_t swidth, const q31_t sheight, const q31_t sstride,
    const q31_t dwidth, const q31_t dheight,
    const q31_t pix_size,
    const q31_t ify,
    const q31_t ify0)
{
    int y;
    for( y = 0; y < dheight; y++ )
    {
        u8_t* D = dst + dwidth * pix_size * y;
        int _sy = (ify * y + ify0) >> 16;
        int sy = NDSV_MIN(_sy, sheight-1);
        const u8_t* S = src + sstride * sy;

        int x = 0;
        switch( pix_size )
        {
        case 1:
            for( ; x < dwidth; x++ )
                D[x] = S[x_ofse[x]];
            break;
        case 2:
            for( ; x < dwidth; x++ )
                *((u16_t*)D + x) = *((u16_t*)(S + x_ofse[x]));
            break;
        case 3:
            for( ; x < dwidth; x++, D += 3 )
            {
                const u8_t* _tS = S + x_ofse[x];
                D[0] = _tS[0]; D[1] = _tS[1]; D[2] = _tS[2];
            }
            break;
        case 4:
            for( ; x < dwidth; x++ )
                *((uint32_t*)D + x) = *((uint32_t*)(S + x_ofse[x]));
            break;
        // case 6:
        //     for( ; x < dwidth; x++, D += 6 )
        //     {
        //         const u16_t* _tS = (const u16_t*)(S + x_ofse[x]);
        //         u16_t* _tD = (u16_t*)D;
        //         _tD[0] = _tS[0]; _tD[1] = _tS[1]; _tD[2] = _tS[2];
        //     }
        //     break;
        // case 8:
        //     for( ; x < dwidth; x++ )
        //         *((uint64_t*)D + x) = *((uint64_t*)S + x_ofse[x]);
        //     break;
        // case 12:
        //     for( ; x < dwidth; x++, D += 12 )
        //     {
        //         const int* _tS = (const int*)(S + x_ofse[x]);
        //         int* _tD = (int*)D;
        //         _tD[0] = _tS[0]; _tD[1] = _tS[1]; _tD[2] = _tS[2];
        //     }
        //     break;
        default:
            for( x = 0; x < dwidth; x++, D += pix_size )
            {
                const u8_t* _tS = S + x_ofse[x];
                for (int k = 0; k < pix_size; k++)
                    D[k] = _tS[k];
            }
            break;
        }
    }
}

static inline void vec_img_resize_nearest_exact_u8(
    u8_t* FUNC_RESTRICT dst,
    u32_t dst_width,
    u32_t dst_height,
    u8_t* src,
    u32_t src_width,
    u32_t src_height,
    u32_t src_stride,
    u32_t channel)
{
    q31_t pix_size = channel * sizeof(u8_t);

    int ifx = ((src_width << 16) + dst_width / 2) / dst_width; // 16bit fixed-point arithmetic
    int ifx0 = ifx / 2 - src_width % 2;                            // This method uses center pixel coordinate as Pillow and scikit-images do.
    int ify = ((src_height << 16) + dst_height / 2) / dst_height;
    int ify0 = ify / 2 - src_height % 2;

    q31_t bufstep = (q31_t) vec_align_size (dst_width, 64); //align 64-byte for cache friendly
    q31_t *x_ofse = (q31_t*) NDSV_MALLOC (bufstep * sizeof (q31_t));

     for( int x = 0; x < dst_width; x++ )
    {
        int sx = (ifx * x + ifx0) >> 16;
        x_ofse[x] = NDSV_MIN(sx, src_width-1) * pix_size;    // offset in element (not byte)
    }

    resizeNN_bitexact(src, dst, x_ofse, src_width, src_height, src_stride, dst_width, dst_height, pix_size, ify, ify0);

    NDSV_FREE (x_ofse);
}

// bilinear
#define CHANGE

#define INTER_RESIZE_COEF_BITS  11
#define INTER_RESIZE_COEF_SCALE (1 << 11)
#define MAX_ESIZE               16

static inline q15_t vec_round(float32_t value)
{
    int32_t intpart = (int32_t)(value);
    float32_t fractpart = value - intpart;
    if ((fractpart != 0.5 && fractpart != -0.5) || ((intpart % 2) != 0))
        intpart = (int32_t)(value + (value >= 0 ? 0.5 : -0.5));

    return (q15_t)((uint32_t)(intpart - SHRT_MIN) <= (uint32_t) USHRT_MAX ? intpart : intpart > 0 ? SHRT_MAX : SHRT_MIN);
}

static void vec_img_hresize_linear (
    const u8_t** src,
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

static inline void vec_img_vresize_linear (const q31_t** src, u8_t* dst, const q15_t* beta, q31_t width)
{
    q31_t b0 = beta[0], b1 = beta[1];
    const q31_t *S0 = src[0], *S1 = src[1];

    q31_t x;
    for (x = 0; x <= width - 4; x += 4)
    {
        dst[x + 0] = (u8_t)(( ((b0 * (S0[x + 0] >> 4)) >> 16) + ((b1 * (S1[x + 0] >> 4)) >> 16) + 2) >> 2);
        dst[x + 1] = (u8_t)(( ((b0 * (S0[x + 1] >> 4)) >> 16) + ((b1 * (S1[x + 1] >> 4)) >> 16) + 2) >> 2);
        dst[x + 2] = (u8_t)(( ((b0 * (S0[x + 2] >> 4)) >> 16) + ((b1 * (S1[x + 2] >> 4)) >> 16) + 2) >> 2);
        dst[x + 3] = (u8_t)(( ((b0 * (S0[x + 3] >> 4)) >> 16) + ((b1 * (S1[x + 3] >> 4)) >> 16) + 2) >> 2);
    }
    for (; x < width; x++)
    {
        dst[x] = (u8_t)(( ((b0 * (S0[x] >> 4)) >> 16) + ((b1 * (S1[x] >> 4)) >> 16) + 2) >> 2);
    }
}

static void vec_img_resize_generic_linear (
    u8_t* src,
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
#ifdef CHANGE
    int dirty = 1;
#endif
    q31_t bufstep = (q31_t) vec_align_size (dstw, 64); //align 64-byte for cache friendly
    q31_t *buf = (q31_t*) NDSV_MALLOC (bufstep * ksize * sizeof (q31_t) + 64);

    const u8_t* srows[MAX_ESIZE];
    q31_t* rows[MAX_ESIZE];
    q31_t prev_sy[MAX_ESIZE];
    q31_t k, dy;
    xmin *= cn;
    xmax *= cn;
    for (k = 0; k < ksize; k++)
    {
        prev_sy[k] = -1;
        //rows[k] = (q31_t*) buf + bufstep * k;
        rows[k] = (q31_t*) (uintptr_t) vec_align_size ((uintptr_t) buf, 64) + bufstep * k;
    }

    // image resize is a separable operation. In case of not too strong
    for (dy = 0; dy < dsth; dy++, beta += ksize)
    {
        q31_t sy0 = yofs[dy], k, k0 = ksize, k1 = 0, ksize2 = ksize / 2;
        for (k = 0; k < ksize; k++)
        {
            q31_t sy = vec_clip(sy0 - ksize2 + 1 + k, 0, srch);//sy0=0~srch-1, k=0~1
            for (k1 = NDSV_MAX (k1, k); k1 < ksize; k1++)
            {
                if (sy == prev_sy[k1])  // if the sy-th row has been computed already, reuse it.
                {
                    if (k1 > k)
                    {
#ifdef CHANGE
                        q31_t* temp = rows[k];
                        rows[k] = rows[k1];
                        rows[k1] = temp;
#else
                        memcpy(rows[k], rows[k1], bufstep * sizeof(rows[0][0])); // k=0, k1=1
#endif
                    }
                    break;
                }
            }
            if (k1 == ksize)
                k0 = NDSV_MIN (k0, k); // remember the first row that needs to be computed
            srows[k] = (const u8_t*) (src + srcstep * sy);
            prev_sy[k] = sy;
        }

#ifdef CHANGE
        if (k0 < ksize)
        {
            vec_img_hresize_linear(srows + k0, rows + k0, ksize - k0, xofs, alpha,
                                   srcw, dstw, cn, xmin, xmax);
            dirty = 1;
        }
        else if (prev_sy[0] == prev_sy[1] && dirty)
        {
            memcpy(rows[1], rows[0], bufstep * sizeof(rows[0][0]));
            dirty = 0;
        }
#else
        if (k0 < ksize)
            vec_img_hresize_linear(srows + k0, rows + k0, ksize - k0, xofs, alpha,
                                   srcw, dstw, cn, xmin, xmax);
#endif
        vec_img_vresize_linear ( (const q31_t**) rows, (u8_t*) (dst + dstw * dy), beta, dstw);
    }

    NDSV_FREE (buf);
}

static void vec_img_resize_cal_offset_linear (
    q31_t* xofs,
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
    double inv_scale_x = (double) dstw / srcw;
    double inv_scale_y = (double) dsth / srch;

    q31_t cn = channels;
    double scale_x = 1.0f / inv_scale_x;
    double scale_y = 1.0f / inv_scale_y;
    q31_t k, sx, sy, dx, dy;

    float32_t fx, fy;

    float32_t cbuf[MAX_ESIZE];

    for (dx = 0; dx < dstw; dx++)
    {
        fx = (float32_t) ( (dx + 0.5f) * scale_x - 0.5f);
        sx = vec_floorf (fx);
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

        cbuf[0] = 1.f - fx;
        cbuf[1] = fx;

        for (k = 0, sx *= cn; k < cn; k++)
            xofs[dx * cn + k] = sx + k;

        for (k = 0; k < ksize; k++)
        {
            ialpha[dx * cn * ksize + k] = vec_round(cbuf[k] * INTER_RESIZE_COEF_SCALE);
        }
        for (; k < cn * ksize; k++)
        {
            ialpha[dx * cn * ksize + k] = ialpha[dx * cn * ksize + k - ksize];
        }
    }
    for (dy = 0; dy < dsth; dy++)
    {
        fy = (float32_t) ( (dy + 0.5f) * scale_y - 0.5f);
        sy = vec_floorf (fy);
        fy -= sy;
        yofs[dy] = sy;

        cbuf[0] = 1.0f - fy;
        cbuf[1] = fy;

        for (k = 0; k < ksize; k++)
            ibeta[dy * ksize + k] = vec_round(cbuf[k] * INTER_RESIZE_COEF_SCALE);
    }
}

static inline void vec_img_resize_bilinear_u8(
    u8_t* FUNC_RESTRICT dst,
    u32_t dst_width,
    u32_t dst_height,
    u8_t* src,
    u32_t src_width,
    u32_t src_height,
    u32_t src_stride,
    u32_t channel)
{
    q31_t dstw = dst_width;
    q31_t dsth = dst_height;
    q31_t srcw = src_width;
    q31_t srch = src_height;

    q31_t cn = channel;
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

    vec_img_resize_cal_offset_linear(xofs, ialpha, yofs, ibeta, &xmin, &xmax, ksize, ksize2, srcw, srch, dstw, dsth, cn);
    vec_img_resize_generic_linear(src, dst, xofs, ialpha, yofs, ibeta, xmin, xmax, ksize, srcw, srch, src_stride, dstw, dsth, cn);
    NDSV_FREE (buf);
}

// bicubic
static void vec_img_hresize_cubic (
    const u8_t** src,
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
    for( int k = 0; k < count; k++ )
    {
        const u8_t *S = src[k];
        q31_t *D = dst[k];
        int dx = 0, limit = xmin;
        for(;;)
        {
            for( ; dx < limit; dx++, alpha += 4 )
            {
                int j, sx = xofs[dx] - cn;
                q31_t v = 0;
                for( j = 0; j < 4; j++ )
                {
                    int sxj = sx + j*cn;
                    if( (unsigned)sxj >= (unsigned)swidth )
                    {
                        while( sxj < 0 )
                            sxj += cn;
                        while( sxj >= swidth )
                            sxj -= cn;
                    }
                    v += S[sxj]*alpha[j];
                }
                D[dx] = v;
            }
            if( limit == dwidth )
                break;
            for( ; dx < xmax; dx++, alpha += 4 )
            {
                int sx = xofs[dx];
                D[dx] = S[sx-cn]*alpha[0] + S[sx]*alpha[1] +
                        S[sx+cn]*alpha[2] + S[sx+cn*2]*alpha[3];
            }
            limit = dwidth;
        }
        alpha -= dwidth*4;
    }
}

static inline u8_t vec_cast_op (q31_t val)
{
    q31_t bits = INTER_RESIZE_COEF_BITS * 2;
    q31_t SHIFT = bits;
    q31_t DELTA = 1 << (bits - 1) ;
    q31_t temp = NDSV_MIN (255, NDSV_MAX (0, (val + DELTA) >> SHIFT)); //Rounding
    return (u8_t) (temp);
}

static inline void vec_img_vresize_cubic (const q31_t** src, u8_t* dst, const q15_t* beta, q31_t width)
{
    q31_t b0 = beta[0], b1 = beta[1], b2 = beta[2], b3 = beta[3];
    const q31_t *S0 = src[0], *S1 = src[1], *S2 = src[2], *S3 = src[3];
    for(int x = 0 ; x < width; x++ )
    {
        dst[x] = vec_cast_op (S0[x]*b0 + S1[x]*b1 + S2[x]*b2 + S3[x]*b3);
    }
}

static inline void vec_img_resize_generic_cubic (
    u8_t* src,
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

    q31_t bufstep = (q31_t) vec_align_size (dstw, 64); //align 64-byte for cache friendly
    q31_t *buf = (q31_t*) NDSV_MALLOC (bufstep * ksize * sizeof (q31_t) + 64);

    const u8_t* srows[MAX_ESIZE];
    q31_t* rows[MAX_ESIZE];
    q31_t prev_sy[MAX_ESIZE];
    q31_t k, dy;
    xmin *= cn;
    xmax *= cn;

    for(k = 0; k < ksize; k++ )
    {
        prev_sy[k] = -1;
        // rows[k] = _buffer.data() + bufstep*k;
        rows[k] = (q31_t*) (uintptr_t) vec_align_size ((uintptr_t) buf, 64) + bufstep * k;
    }

    for (dy = 0; dy < dsth; dy++, beta += ksize)
    {
        q31_t sy0 = yofs[dy], k0 = ksize, k1 = 0, ksize2 = ksize/2;
#ifdef CHANGE
        q31_t swapk = -1, swapk1 = -1;
#endif
        for(k = 0; k < ksize; k++ )
        {
            int sy = vec_clip (sy0 - ksize2 + 1 + k, 0, srch);

            for( k1 = NDSV_MAX (k1, k); k1 < ksize; k1++ )
            {
                if( sy == prev_sy[k1] ) // if the sy-th row has been computed already, reuse it.
                {
                    if( k1 > k )
                    {
                        // printf("swap %d to %d\n", k1, k);
#ifdef CHANGE
                        q31_t* temp = rows[k];
                        rows[k] = rows[k1];
                        rows[k1] = temp;
                        swapk = k; swapk1 = k1;
#else
                        memcpy( rows[k], rows[k1], bufstep*sizeof(rows[0][0]) );
#endif
                    }
                    break;
                }
            }
            if( k1 == ksize )
                k0 = NDSV_MIN(k0, k); // remember the first row that needs to be computed
            srows[k] = (const u8_t*) (src + srcstep * sy);
            prev_sy[k] = sy;
        }
#ifdef CHANGE
        // printf("%d, %d, %d, %d     c%d, k%d, k1 %d\n", prev_sy[0], prev_sy[1], prev_sy[2], prev_sy[3], ksize - k0, swapk, swapk1);
        if (k0 < ksize)
        {
            vec_img_hresize_cubic (srows + k0, rows + k0, ksize - k0, xofs, alpha,
                                   srcw, dstw, cn, xmin, xmax);
        }
        else if (swapk > 0 && prev_sy[swapk] == prev_sy[swapk1])
        {
            // printf("copy %d to %d\n", swapk, swapk1);
            memcpy( rows[swapk1], rows[swapk], bufstep*sizeof(rows[0][0]) );
        }
#else
        // printf("%d, %d, %d, %d, %d\n", prev_sy[0], prev_sy[1], prev_sy[2], prev_sy[3], ksize - k0);
        if( k0 < ksize ) // ksize - k0 : how many rows need to compute
            vec_img_hresize_cubic (srows + k0, rows + k0, ksize - k0, xofs, alpha,
                                   srcw, dstw, cn, xmin, xmax);
#endif
        vec_img_vresize_cubic ( (const q31_t**) rows, (u8_t*) (dst + dstw * dy), beta, dstw);
    }

    NDSV_FREE (buf);
}

static inline void interpolateCubic( float x, float* coeffs )
{
    const float A = -0.75f;

    coeffs[0] = ((A*(x + 1) - 5*A)*(x + 1) + 8*A)*(x + 1) - 4*A;
    coeffs[1] = ((A + 2)*x - (A + 3))*x*x + 1;
    coeffs[2] = ((A + 2)*(1 - x) - (A + 3))*(1 - x)*(1 - x) + 1;
    coeffs[3] = 1.f - coeffs[0] - coeffs[1] - coeffs[2];
}

static inline void vec_img_resize_cal_offset_cubic (
    q31_t* xofs,
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
    double inv_scale_x = (double) dstw / srcw;
    double inv_scale_y = (double) dsth / srch;

    q31_t cn = channels;
    double scale_x = 1.0f / inv_scale_x;
    double scale_y = 1.0f / inv_scale_y;
    q31_t k, sx, sy, dx, dy;

    float32_t fx, fy;

    float32_t cbuf[MAX_ESIZE];

    for( dx = 0; dx < dstw; dx++ )
    {
        fx = (float32_t)((dx+0.5)*scale_x - 0.5);
        sx = vec_floorf (fx);
        fx -= sx;

        if( sx < ksize2-1 )
        {
            *xmin = dx+1;
        }

        if( sx + ksize2 >= srcw )
        {
            *xmax = NDSV_MIN( *xmax, dx );
        }

        interpolateCubic( fx, cbuf );

        for( k = 0, sx *= cn; k < cn; k++ )
            xofs[dx*cn + k] = sx + k;

        for( k = 0; k < ksize; k++ )
        {
            ialpha[dx*cn*ksize + k] = vec_round(cbuf[k]*INTER_RESIZE_COEF_SCALE);
        }
        for( ; k < cn*ksize; k++ )
        {
            ialpha[dx*cn*ksize + k] = ialpha[dx*cn*ksize + k - ksize];
        }
    }

    for( dy = 0; dy < dsth; dy++ )
    {
        fy = (float32_t)((dy+0.5)*scale_y - 0.5);
        sy = vec_floorf(fy);
        fy -= sy;

        yofs[dy] = sy;

        interpolateCubic( fy, cbuf );

        for( k = 0; k < ksize; k++ )
        {
            ibeta[dy*ksize + k] = vec_round(cbuf[k]*INTER_RESIZE_COEF_SCALE);
        }
    }
}

void vec_img_resize_bicubic_u8(
    u8_t* FUNC_RESTRICT dst,
    u32_t dst_width,
    u32_t dst_height,
    u8_t* src,
    u32_t src_width,
    u32_t src_height,
    u32_t src_stride,
    u32_t channel)
{
    q31_t dstw = dst_width;
    q31_t dsth = dst_height;
    q31_t srcw = src_width;
    q31_t srch = src_height;

    q31_t cn = channel;
    q31_t xmin = 0;
    q31_t xmax = dstw;
    q31_t width = dstw * cn;

    q31_t ksize = 4;
    q31_t ksize2 = ksize / 2;

    // (4 * dstw + dsth) * (4 + 4 * 4);
    u8_t *buf = (u8_t*) NDSV_MALLOC ( (width + dsth) * (sizeof (q31_t) + sizeof (float32_t) * ksize));

    q31_t* xofs = (q31_t*) buf;
    q31_t* yofs = xofs + width;
    q15_t* ialpha = (q15_t*) (yofs + dsth);
    q15_t* ibeta = ialpha + width * ksize;

    vec_img_resize_cal_offset_cubic(xofs, ialpha, yofs, ibeta, &xmin, &xmax, ksize, ksize2, srcw, srch, dstw, dsth, cn);
    vec_img_resize_generic_cubic(src, dst, xofs, ialpha, yofs, ibeta, xmin, xmax, ksize, srcw, srch, src_stride, dstw, dsth, cn);
    NDSV_FREE (buf);
}

#endif // __INTERNAL_VEC_IMAGE_RESIZE_TEMPLATE_PUREC_U8_H__

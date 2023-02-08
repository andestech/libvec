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

#ifndef __INTERNAL_VEC_UTILS_H__
#define __INTERNAL_VEC_UTILS_H__

#include "internal_nds_types.h"
#include "stdio.h"

#define F32_HMT_FUNC_ATTR   inline
#define ENA_HIGHER_PERFORMANCE_CONVERT_F32  //Using not checking inf, -inf, NAN for higher performance.

typedef struct
{
    uint32_t e;
    int32_t m;
} st754;

//This fucntion extracts the elements of IEEE754 format
//and save elements into st754 structure.
//Note:
//  The sign bit will be applied on mantissa to do signed-extension.
F32_HMT_FUNC_ATTR
static st754
Xtr754(float32_t a)
{
    union32_t uni_a;
    st754 ret;

    uni_a.f0 = a;
    ret.e = (uni_a.u0 << 1) >> (32 - 8);
    ret.m = 0;
    if (uni_a.u0 != 0)
        ret.m = ((uni_a.u0 | ((uint32_t)1 << 23)) << 8) >> 8;
    ret.m = (uni_a.w0 < 0 ) ? -ret.m : ret.m;
    return ret;
}

//This function recovers back to IEEE754 format from st754 structure.
F32_HMT_FUNC_ATTR
static float32_t
To754(st754 a)
{
    union32_t ret;
    int32_t adj;
    uint32_t zcount, e, s, m;

    ret.f0 = 0.0F;
    if (a.m != 0)
    {
        s = a.m & 0x80000000UL;
        m = s ? -a.m : a.m;
        zcount = NDS_ISA_CLZ(m);
        adj = 9 - zcount - 1;
        e = a.e + adj;

        if (adj < 0)
        {
            m <<= (-adj);
        }
        else if (adj > 0)
        {
            m >>= adj;
        }

        m &= ~(1UL << 23);
        ret.u0 = s | m | (e << 23);
    }

    return ret.f0;
}

F32_HMT_FUNC_ATTR
static st754
Mul754(st754 a, st754 b)
{
    st754 ret;

    //1.23*1.23 >> 23 = 2.23
    ret.m = ((int64_t)a.m * b.m) >> 23;
    ret.e = 0;

    if (ret.m != 0)
    {
        ret.e = 127 + a.e - 127 + b.e - 127;
    }

    return ret;
}

//This function does addition between two st754 structures.
//Before adds two mantissa, the bigger exponent will be
//normalized to small one and the adjustment will be applied to its mantissa.
//This method also avoids overflow problem.
F32_HMT_FUNC_ATTR
static st754
Add754(st754 a, st754 b)
{
    st754 ret;
    int32_t m, adj;

    adj = (int32_t)a.e - (int32_t)b.e;
    if (adj < 0)
    {
        ret.e = b.e;
        m = a.m >> (-adj);
        ret.m = m + b.m;
    }
    else
    {
        ret.e = a.e;
        m = b.m >> adj;
        ret.m = a.m + m;
    }

    return ret;
}

//This function does subtraction between two st754 structures.
//Before subtracts two mantissa, it needs to do nomalization like Add754 function.
F32_HMT_FUNC_ATTR
static st754
Sub754(st754 a, st754 b)
{
    st754 ret;
    int32_t m, adj;

    adj = (int32_t)a.e - (int32_t)b.e;
    if (adj < 0)
    {
        ret.e = b.e;
        m = a.m >> (-adj);
        ret.m = m - b.m;
    }
    else
    {
        ret.e = a.e;
        m = b.m >> adj;
        ret.m = a.m - m;
    }
    return ret;
}

//This function compares the values between two st754 structures.
//if a > b, then return 1
//if a = b, then return 0
//if a < b, then return -1
F32_HMT_FUNC_ATTR
static int
Cmp754(st754 a, st754 b, st754 *st_result)
{
    int ret = 1;
    *st_result = Sub754(a, b);
    if (st_result->m < 0)
        ret = -1;
    else if (st_result->m == 0)
        ret = 0;
    return ret;
}

// ------------- Compute library ------------
#include "math.h"
typedef union
{
    float32_t f32;
    int32_t   i32;
    uint32_t  u32;
} _union32_t;

static const float CONST_LN2          = 0.6931471805f;     // ln(2)

// Compute library of exp_f32
//--- const values for taylor_poly_f32 ---
// Exponent polynomial coefficients
static const float EXP_CONST_COE0 = 1.f;
static const float EXP_CONST_COE1 = 0.0416598916054f;
static const float EXP_CONST_COE2 = 0.500000596046f;
static const float EXP_CONST_COE3 = 0.0014122662833f;
static const float EXP_CONST_COE4 = 1.00000011921f;
static const float EXP_CONST_COE5 = 0.00833693705499f;
static const float EXP_CONST_COE6 = 0.166665703058f;
static const float EXP_CONST_COE7 = 0.000195780929062f;

//--- const values for exp_f32 ---
static const float EXP_CONST_INV_LN2      = 1.4426950408f;     // 1/ln(2)
static const float EXP_CONST_INF          = INFINITY;
static const float EXP_CONST_MAX_INPUT    = 88.7f;
static const float CONST_0            = 0.f;
static const int   EXP_CONST_NEGATIVE_126 = -126;

static inline float taylor_poly_exp_f32(float x)
{
    float A = EXP_CONST_COE0 + EXP_CONST_COE4 * x;
    float B = EXP_CONST_COE2 + EXP_CONST_COE6 * x;
    float C = EXP_CONST_COE1 + EXP_CONST_COE5 * x;
    float D = EXP_CONST_COE3 + EXP_CONST_COE7 * x;
    float x2 = x * x;
    float x4 = x2 * x2;
    float res = (A + B * x2) + (C + D * x2) * x4;
    return res;
}

static inline float32_t _exp_f32(float32_t x)
{
#ifdef ENA_HIGHER_PERFORMANCE_VEXP_F32
    // Perform range reduction [-log(2),log(2)]
    int m = x * EXP_CONST_INV_LN2;
    float val = x - (float)m * CONST_LN2;

    // Polynomial Approximation
    _union32_t poly;
    poly.f32 = taylor_poly_exp_f32(val);

    // Reconstruct
#ifdef ENA_DSP_ISA
    int m2 = NDS_VEC_SATS((m << 23), 32);
    poly.i32 = NDS_VEC_KADDW(poly.i32, m2);
#else
    int m2 = NDS_ISA_SATS((m << 23), 32);
    poly.i32 = NDS_ISA_KADDW(poly.i32, m2);
#endif

    // Handle overflow
    poly.f32 = (m < EXP_CONST_NEGATIVE_126) ? CONST_0 : poly.f32;
    poly.f32 = (x > EXP_CONST_MAX_INPUT) ? EXP_CONST_INF : poly.f32;

    return poly.f32;
#else
    return expf(x);
#endif
}

// Compute library of log_f32
static const float LOG_CONST_COE0 = -2.29561495781f;
static const float LOG_CONST_COE1 = -2.47071170807f;
static const float LOG_CONST_COE2 = -5.68692588806f;
static const float LOG_CONST_COE3 = -0.165253549814f;
static const float LOG_CONST_COE4 = 5.17591238022f;
static const float LOG_CONST_COE5 = 0.844007015228f;
static const float LOG_CONST_COE6 = 4.58445882797f;
static const float LOG_CONST_COE7 = 0.0141278216615f;
static const int   CONST_127  = 127;

static inline float taylor_poly_log_f32(float x)
{
    float A = LOG_CONST_COE0 + LOG_CONST_COE4 * x;
    float B = LOG_CONST_COE2 + LOG_CONST_COE6 * x;
    float C = LOG_CONST_COE1 + LOG_CONST_COE5 * x;
    float D = LOG_CONST_COE3 + LOG_CONST_COE7 * x;
    float x2 = x * x;
    float x4 = x2 * x2;
    float res = (A + B * x2) + (C + D * x2) * x4;
    return res;
}
static inline float _log_f32(float x)
{
#ifdef ENA_HIGHER_PERFORMANCE_VLOG_F32
    // Extract exponent
    _union32_t val, tmp;
    val.f32 = x;
    tmp.u32 = val.u32 >> 23;
    int m = tmp.i32 - CONST_127;
    int m2 = m << 23;
    val.i32 = val.i32 - m2;

    // Polynomial Approximation
    float out = taylor_poly_log_f32(val.f32);

    // Reconstruct
    out = out + (float)m * CONST_LN2;

    return out;
#else
    return logf(x);
#endif
}
// end of log_f32

// Compute library of sigmoid_f32
static const float32_t CONST_1 = 1.f;
static const float32_t CONST_MIN_SIGMOID = -10.f;
static const float32_t CONST_MAX_SIGMOID = 10.f;

static inline float32_t _sigmoid_f32(float32_t x)
{
    x = (x < CONST_MIN_SIGMOID) ? CONST_MIN_SIGMOID : x;
    x = (x > CONST_MAX_SIGMOID) ? CONST_MAX_SIGMOID : x;
    float32_t num = _exp_f32(x);
    float32_t den = num + CONST_1;
    float32_t out = num / den;
    return out;
}
// end of sigmoid_f32

// Compute library of sin_f32
#define CONST_PI (3.14159265358979323846)
static const float32_t pi_v     = CONST_PI;
static const float32_t pio2_v   = (CONST_PI / 2);
static const float32_t ipi_v    = (1 / CONST_PI);
static const float32_t te_sin_coeff2 = 0.166666666666f; // 1/(2*3)
static const float32_t te_sin_coeff3 = 0.05f;           // 1/(4*5)
static const float32_t te_sin_coeff4 = 0.023809523810f; // 1/(6*7)
static const float32_t te_sin_coeff5 = 0.013888888889f; // 1/(8*9)
static const int INT_ZERO = 0;

static inline float _sin_f32(float x)
{
#ifdef ENA_HIGHER_PERFORMANCE_VSIN_F32
    // algorithm from compute library
    _union32_t c_v;

    //Find positive or negative
#ifdef ENA_XTR_754
    st754 st_x = Xtr754(x);
    st754 st_ipi_v = Xtr754(ipi_v);
    st754 st_out;
    st_out = Mul754(st_x, st_ipi_v);
    float32_t tmp = To754(st_out);
    c_v.i32 = NDS_ISA_ABS((int)tmp);
#else
    c_v.i32 = NDS_ISA_ABS((int)x * ipi_v);
#endif
#ifdef ENA_XTR_754
    st754 st_pre;
    st754 st_0f = Xtr754(CONST_0);
    uint32_t sign_v;
    if (Cmp754(st_x, st_0f, &st_pre) < 0)
        sign_v = 1;
    else
        sign_v = 0;
#else
    uint32_t sign_v = (x < 0) ? 1 : 0;
#endif
    uint32_t odd_v  = c_v.u32 & 1;
    uint32_t neg_v = odd_v ^ sign_v;

    // Modulus a - (n * int(a*(1/n)))
    float32_t absx = fabsf(x);
#ifdef ENA_XTR_754
    float tmpc_v = (float)c_v.i32;
    st754 st_tmpc_v = Xtr754(tmpc_v);
    st754 st_pi_v = Xtr754(pi_v);
    st754 st_absx = Xtr754(absx);
    st_out = Mul754(st_pi_v, st_tmpc_v);
    st754 st_ma;
    st_ma = Sub754(st_absx, st_out);
    float32_t ma = To754(st_ma);
#else
    float32_t ma = absx - pi_v * (float)c_v.i32;
#endif
    const uint32_t reb_v = (ma >= pio2_v) ? 1 : 0;

    //Rebase a between 0 and pi/2
    ma = (reb_v != 0) ? (pi_v - ma) : ma;

    // Taylor series
    const float32_t ma2 = ma * ma;

    //2nd elem: x^3 / 3!
    float32_t elem = (ma * ma2) * te_sin_coeff2;
    float32_t res = ma - elem;

    //3rd elem: x^5 / 5!
    elem = (elem * ma2) * te_sin_coeff3;
    res = res + elem;

    //4th elem: x^7 / 7!float32x2_t vsin_f32(float32x2_t val)
    elem = (elem * ma2) * te_sin_coeff4;
    res = res - elem;

    //5th elem: x^9 / 9!
    elem = (elem * ma2) * te_sin_coeff5;
    res = res + elem;

    //Change of sign
    neg_v = neg_v << 31;
    _union32_t ures, out;
    ures.f32 = res;
    out.u32 = (ures.u32 ^ neg_v);
    return (out.f32);
#else
    return sinf(x);
#endif
}

static inline float _cos_f32(float x)
{
#ifdef ENA_HIGHER_PERFORMANCE_VCOS_F32
    float32_t val = x + pio2_v;
    return (_sin_f32(val));
#else
    return cosf(x);
#endif
}

#endif  // __INTERNAL_VEC_UTILS_H__

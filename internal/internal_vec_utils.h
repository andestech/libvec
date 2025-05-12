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

#ifndef __INTERNAL_VEC_UTILS_H__
#define __INTERNAL_VEC_UTILS_H__

#include "internal_nds_types.h"
#include "stdio.h"

#define F32_HMT_FUNC_ATTR   inline
#define ENA_HIGHER_PERFORMANCE_CONVERT_F32  //Using not checking inf, -inf, NAN for higher performance.
#define VEC_SQUARE(x) (x * x)

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
    float64_t f64;
    int64_t   i64;
    uint64_t  u64;
} union64_utils_t;

typedef union
{
    float32_t f32;
    int32_t   i32;
    uint32_t  u32;
} union32_utils_t;

static const double F64_CONST_LN2          = 0.6931471805f;     // ln(2)
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

#if defined (__riscv_zfh)
static const float16_t F16_CONST_LN2          = 0.6931471805f;     // ln(2)
static const float16_t EXP_F16_CONST_COE0 = 1.f;
static const float16_t EXP_F16_CONST_COE1 = 0.0416598916054f;
static const float16_t EXP_F16_CONST_COE2 = 0.500000596046f;
static const float16_t EXP_F16_CONST_COE3 = 0.0014122662833f;
static const float16_t EXP_F16_CONST_COE4 = 1.00000011921f;
static const float16_t EXP_F16_CONST_COE5 = 0.00833693705499f;
static const float16_t EXP_F16_CONST_COE6 = 0.166665703058f;
static const float16_t EXP_F16_CONST_COE7 = 0.000195780929062f;

//--- const values for exp_f32 ---
static const float16_t EXP_F16_CONST_INV_LN2      = 1.4426950408f;     // 1/ln(2)
static const float16_t EXP_F16_CONST_INF          = INFINITY;
static const float16_t EXP_F16_CONST_MAX_INPUT    = 88.7f;
static const float16_t F16_CONST_0            = 0.f;
static const int   EXP_F16_CONST_NEGATIVE_15 = -15;
#endif

static inline float vec_taylor_poly_exp_f32(float x)
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

static inline float32_t vec_exp_f32(float32_t x)
{
#ifdef ENA_HIGHER_PERFORMANCE_VEXP_F32
    // Perform range reduction [-log(2),log(2)]
    int m = x * EXP_CONST_INV_LN2;
    float val = x - (float)m * CONST_LN2;

    // Polynomial Approximation
    union32_utils_t poly;
    poly.f32 = vec_taylor_poly_exp_f32(val);

    // Reconstruct
    int m2 = NDS_ISA_SATS((m << 23), 32);
    poly.i32 = NDS_ISA_KADDW(poly.i32, m2);

    // Handle overflow
    poly.f32 = (m < EXP_CONST_NEGATIVE_126) ? CONST_0 : poly.f32;
    poly.f32 = (x > EXP_CONST_MAX_INPUT) ? EXP_CONST_INF : poly.f32;

    return poly.f32;
#else
    return expf(x);
#endif
}

#if defined (__riscv_zfh)
static inline float16_t vec_taylor_poly_exp_f16(float16_t x)
{
    float16_t A = EXP_F16_CONST_COE0 + EXP_F16_CONST_COE4 * x;
    float16_t B = EXP_F16_CONST_COE2 + EXP_F16_CONST_COE6 * x;
    float16_t C = EXP_F16_CONST_COE1 + EXP_F16_CONST_COE5 * x;
    float16_t D = EXP_F16_CONST_COE3 + EXP_F16_CONST_COE7 * x;
    float16_t x2 = x * x;
    float16_t x4 = x2 * x2;
    float16_t res = (A + B * x2) + (C + D * x2) * x4;
    return res;
}

typedef union
{
    float16_t f16;
    int16_t   i16;
    uint16_t  u16;
} union16_utils_t;

static inline float16_t vec_exp_f16(float16_t x)
{
    // Perform range reduction [-log(2),log(2)]
    int m = x * EXP_F16_CONST_INV_LN2;
    float16_t val = x - (float16_t)m * F16_CONST_LN2;

    // Polynomial Approximation
    union16_utils_t poly;
    poly.f16 = vec_taylor_poly_exp_f16(val);

    // Reconstruct
    int m2 = NDS_ISA_SATS((m << 10), 16);
    poly.i16 = NDS_ISA_KADDH(poly.i16, m2);

    // Handle overflow
    poly.f16 = (m < EXP_F16_CONST_NEGATIVE_15) ? F16_CONST_0 : poly.f16;
    poly.f16 = (x > EXP_F16_CONST_MAX_INPUT) ? EXP_F16_CONST_INF : poly.f16;

    return poly.f16;
}
// end of exp_f16
#endif

// Compute library of log_f64
static const float64_t LOG_F64_CONST_COE0 = -2.29561495781f;
static const float64_t LOG_F64_CONST_COE1 = -2.47071170807f;
static const float64_t LOG_F64_CONST_COE2 = -5.68692588806f;
static const float64_t LOG_F64_CONST_COE3 = -0.165253549814f;
static const float64_t LOG_F64_CONST_COE4 = 5.17591238022f;
static const float64_t LOG_F64_CONST_COE5 = 0.844007015228f;
static const float64_t LOG_F64_CONST_COE6 = 4.58445882797f;
static const float64_t LOG_F64_CONST_COE7 = 0.0141278216615f;
static const int   CONST_1023  = 1023;

static inline float64_t vec_taylor_poly_log_f64(float64_t x)
{
    float64_t A = LOG_F64_CONST_COE0 + LOG_F64_CONST_COE4 * x;
    float64_t B = LOG_F64_CONST_COE2 + LOG_F64_CONST_COE6 * x;
    float64_t C = LOG_F64_CONST_COE1 + LOG_F64_CONST_COE5 * x;
    float64_t D = LOG_F64_CONST_COE3 + LOG_F64_CONST_COE7 * x;
    float64_t x2 = x * x;
    float64_t x4 = x2 * x2;
    float64_t res = (A + B * x2) + (C + D * x2) * x4;
    return res;
}

static inline float64_t vec_log_f64(float64_t x)
{
#ifdef ENA_HIGHER_PERFORMANCE_VLOG_F32
    // Extract exponent
    union64_utils_t val, tmp;
    val.f64 = x;
    tmp.u64 = val.u64 >> 52;
    int64_t m = tmp.i64 - CONST_1023;
    int64_t m2 = m << 52;
    val.i64 = val.i64 - m2;

    // Polynomial Approximation
    float64_t out = vec_taylor_poly_log_f64(val.f64);

    // Reconstruct
    out = out + (float64_t)m * F64_CONST_LN2;

    return out;
#else
    return log(x);
#endif
}
// end of log_f64

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

static inline float vec_taylor_poly_log_f32(float x)
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

static inline float vec_log_f32(float x)
{
#ifdef ENA_HIGHER_PERFORMANCE_VLOG_F32
    // Extract exponent
    union32_utils_t val, tmp;
    val.f32 = x;
    tmp.u32 = val.u32 >> 23;
    int m = tmp.i32 - CONST_127;
    int m2 = m << 23;
    val.i32 = val.i32 - m2;

    // Polynomial Approximation
    float out = vec_taylor_poly_log_f32(val.f32);

    // Reconstruct
    out = out + (float)m * CONST_LN2;

    return out;
#else
    return logf(x);
#endif
}
// end of log_f32

#if defined (__riscv_zfh)
// Compute library of log_f16
static const float16_t LOG_F16_CONST_COE0 = -2.29561495781f;
static const float16_t LOG_F16_CONST_COE1 = -2.47071170807f;
static const float16_t LOG_F16_CONST_COE2 = -5.68692588806f;
static const float16_t LOG_F16_CONST_COE3 = -0.165253549814f;
static const float16_t LOG_F16_CONST_COE4 = 5.17591238022f;
static const float16_t LOG_F16_CONST_COE5 = 0.844007015228f;
static const float16_t LOG_F16_CONST_COE6 = 4.58445882797f;
static const float16_t LOG_F16_CONST_COE7 = 0.0141278216615f;
static const int   CONST_15  = 15;

static inline float16_t vec_taylor_poly_log_f16(float16_t x)
{
    float16_t A = LOG_F16_CONST_COE0 + LOG_F16_CONST_COE4 * x;
    float16_t B = LOG_F16_CONST_COE2 + LOG_F16_CONST_COE6 * x;
    float16_t C = LOG_F16_CONST_COE1 + LOG_F16_CONST_COE5 * x;
    float16_t D = LOG_F16_CONST_COE3 + LOG_F16_CONST_COE7 * x;
    float16_t x2 = x * x;
    float16_t x4 = x2 * x2;
    float16_t res = (A + B * x2) + (C + D * x2) * x4;
    return res;
}

static inline float16_t vec_log_f16(float16_t x)
{
    // Extract exponent
    union16_utils_t val, tmp;
    val.f16 = x;
    tmp.u16 = val.u16 >> 10;
    int m = tmp.i16 - CONST_15;
    int m2 = m << 10;
    val.i16 = val.i16 - m2;

    // Polynomial Approximation
    float16_t out = vec_taylor_poly_log_f16(val.f16);

    // Reconstruct
    out = out + (float16_t)m * F16_CONST_LN2;

    return out;
}
// end of log_f16
#endif

// log_q31
#define LOG_Q31_ACCURACY 31

/* Bit to represent the normalization factor
   It is Ceiling[Log2[LOG_Q31_ACCURACY]] of the previous value.
   The Log2 algorithm is assuming that the value x is
   1 <= x < 2.

   But input value could be as small a 2^-LOG_Q31_ACCURACY
   which would give an integer part of -31.
*/
#define LOG_Q31_INTEGER_PART 5

/* 2.0 in Q30 */
#define LOQ_Q31_THRESHOLD (1u << LOG_Q31_ACCURACY)

/* HALF */
#define LOQ_Q31_Q32_HALF LOQ_Q31_THRESHOLD
#define LOQ_Q31_Q30_HALF (LOQ_Q31_Q32_HALF >> 2)

/* 1.0 / Log2[Exp[1]] in Q31 */
#define LOG_Q31_INVLOG2EXP 0x58b90bfbuL

/* Clay Turner algorithm */
static inline q31_t vec_log_q31(q31_t src)
{
   int32_t i;

   int32_t c = NDS_ISA_CLZ(src);

   int32_t normalization=0;

   /* 0.5 in q26 */
   uint32_t inc = LOQ_Q31_Q32_HALF >> (LOG_Q31_INTEGER_PART + 1);

   /* Will compute y = log2(x) for 1 <= x < 2.0 */
   uint32_t x;

   /* q26 */
   uint32_t y=0;

   /* q26 */
   int32_t tmp;

   /* Normalize and convert to q30 format */
   x = src;
   if ((c-1) < 0)
   {
     x = x >> (1-c);
   }
   else
   {
     x = x << (c-1);
   }
   normalization = c;

   /* Compute the Log2. Result is in q26
      because we know 0 <= y < 1.0 but
      do not want to use q32 to allow
      following computation with less instructions.
   */

   for(i = 0; i < LOG_Q31_ACCURACY ; i++)
   {
      x = ((int64_t)x*x)  >> (LOG_Q31_ACCURACY - 1);
      if (x >= LOQ_Q31_THRESHOLD)
      {
         y += inc ;
         x = x >> 1;
      }
      inc = inc >> 1;
   }

   /*
      Convert the Log2 to Log and apply normalization.
      We compute (y - normalisation) * (1 / Log2[e]).

   */

   /* q26 */
   tmp = (int32_t)y - (normalization << (LOG_Q31_ACCURACY - LOG_Q31_INTEGER_PART));

   /* q5.26 */
   y = ((int64_t)tmp * LOG_Q31_INVLOG2EXP) >> 31;

   return(y);
}
// end of log_q31

// log_q15
#define LOG_Q15_ACCURACY 15

/* Bit to represent the normalization factor
   It is Ceiling[Log2[LOG_Q15_ACCURACY]] of the previous value.
   The Log2 algorithm is assuming that the value x is
   1 <= x < 2.

   But input value could be as small a 2^-LOG_Q15_ACCURACY
   which would give an integer part of -15.
*/
#define LOG_Q15_INTEGER_PART 4

/* 2.0 in q14 */
#define LOQ_Q15_THRESHOLD (1u << LOG_Q15_ACCURACY)

/* HALF */
#define LOQ_Q15_Q16_HALF LOQ_Q15_THRESHOLD
#define LOQ_Q15_Q14_HALF (LOQ_Q15_Q16_HALF >> 2)

/* 1.0 / Log2[Exp[1]] in q15 */
#define LOG_Q15_INVLOG2EXP 0x58b9u

/* Clay Turner algorithm */
static inline uint16_t vec_log_q15(uint16_t src)
{
   int i;

   int16_t c = NDS_ISA_CLZ(src) - 16;

   int16_t normalization = 0;

   /* 0.5 in q11 */
   uint16_t inc = LOQ_Q15_Q16_HALF >> (LOG_Q15_INTEGER_PART + 1);

   /* Will compute y = log2(x) for 1 <= x < 2.0 */
   uint16_t x;

   /* q11 */
   uint16_t y = 0;

   /* q11 */
   int16_t tmp;

   /* Normalize and convert to q14 format */
   x = src;
   if ((c - 1) < 0)
   {
     x = x >> (1 - c);
   }
   else
   {
     x = x << (c - 1);
   }
   normalization = c;

   /* Compute the Log2. Result is in q11 instead of q16
      because we know 0 <= y < 1.0 but
      we want a result allowing to do a
      product on int16 rather than having to go
      through int32
   */
   for(i = 0; i < LOG_Q15_ACCURACY ; i++)
   {
      x = (((int32_t) x * x)) >> (LOG_Q15_ACCURACY - 1);

      if (x >= LOQ_Q15_THRESHOLD)
      {
         y += inc ;
         x = x >> 1;
      }
      inc = inc >> 1;
   }

   /*
      Convert the Log2 to Log and apply normalization.
      We compute (y - normalisation) * (1 / Log2[e]).

   */

   /* q11 */
   tmp = (int16_t)y - (normalization << (LOG_Q15_ACCURACY - LOG_Q15_INTEGER_PART));

   /* q4.11 */
   y = ((int32_t)tmp * LOG_Q15_INVLOG2EXP) >> 15;

   return(y);
}
// end of log_q15

// Compute library of sigmoid_f32
static const float32_t CONST_1 = 1.f;
static const float32_t CONST_MIN_SIGMOID = -10.f;
static const float32_t CONST_MAX_SIGMOID = 10.f;

static inline float32_t vec_sigmoid_f32(float32_t x)
{
    x = (x < CONST_MIN_SIGMOID) ? CONST_MIN_SIGMOID : x;
    x = (x > CONST_MAX_SIGMOID) ? CONST_MAX_SIGMOID : x;
    float32_t num = vec_exp_f32(x);
    float32_t den = num + CONST_1;
    float32_t out = num / den;
    return out;
}
// end of sigmoid_f32

#if defined (__riscv_zfh)
// Compute library of sigmoid_f16
static const float16_t F16_CONST_1 = 1.f;
static const float16_t F16_CONST_MIN_SIGMOID = -10.f;
static const float16_t F16_CONST_MAX_SIGMOID = 10.f;

static inline float16_t vec_sigmoid_f16(float16_t x)
{
    x = (x < F16_CONST_MIN_SIGMOID) ? F16_CONST_MIN_SIGMOID : x;
    x = (x > F16_CONST_MAX_SIGMOID) ? F16_CONST_MAX_SIGMOID : x;
    float16_t num = vec_exp_f16(x);
    float16_t den = num + F16_CONST_1;
    float16_t out = num / den;
    return out;
}
// end of sigmoid_f16
#endif

// Compute library of sin_f32
#define CONST_PI (3.14159265358979323846)
static const float32_t pi_v     = CONST_PI;
static const float32_t pio2_v   = (CONST_PI / 2);
static const float32_t ipi_v    = (1 / CONST_PI);
static const float32_t te_sin_coeff2 = 0.166666666666f; // 1/(2*3)
static const float32_t te_sin_coeff3 = 0.05f;           // 1/(4*5)
static const float32_t te_sin_coeff4 = 0.023809523810f; // 1/(6*7)
static const float32_t te_sin_coeff5 = 0.013888888889f; // 1/(8*9)
#ifdef ENA_ACCURACY_ENHANCE_SIN
static const float32_t te_sin_coeff6 = 0.009090909091f; // 1/(10*11)
#endif
static const int INT_ZERO = 0;

static inline float vec_sin_f32(float x)
{
#ifdef ENA_HIGHER_PERFORMANCE_VSIN_F32
    // algorithm from compute library
    union32_utils_t c_v;

    //Find positive or negative
    c_v.i32 = NDS_ISA_ABS((int)x * ipi_v);
    uint32_t sign_v = (x < 0) ? 1 : 0;
    uint32_t odd_v  = c_v.u32 & 1;
    uint32_t neg_v = odd_v ^ sign_v;

    // Modulus a - (n * int(a*(1/n)))
    float32_t absx = fabsf(x);
    float32_t ma = absx - pi_v * (float)c_v.i32;
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

    //4th elem: x^7 / 7!
    elem = (elem * ma2) * te_sin_coeff4;
    res = res - elem;

    //5th elem: x^9 / 9!
    elem = (elem * ma2) * te_sin_coeff5;
    res = res + elem;

#ifdef ENA_ACCURACY_ENHANCE_SIN
    elem = (elem * ma2) * te_sin_coeff6;
    res = res - elem;
#endif

    //Change of sign
    neg_v = neg_v << 31;
    union32_utils_t ures, out;
    ures.f32 = res;
    out.u32 = (ures.u32 ^ neg_v);
    return (out.f32);
#else
    return sinf(x);
#endif
}

// Compute library of sin_f32
#define CONST_PI_F64 (3.1415926535897932384626433832795)
static const float64_t pi_v_f64     = CONST_PI_F64;
static const float64_t pio2_v_f64   = (CONST_PI_F64 / 2);
static const float64_t ipi_v_f64    = (1 / CONST_PI_F64);
static const float64_t sin_f64_coeff2 = 0.16666666666666666666666666666667; // 1/(2*3)
static const float64_t sin_f64_coeff3 = 0.05;           // 1/(4*5)
static const float64_t sin_f64_coeff4 = 0.02380952380952380952380952380952; // 1/(6*7)
static const float64_t sin_f64_coeff5 = 0.01388888888888888888888888888889; // 1/(8*9)
#ifdef ENA_ACCURACY_ENHANCE_SIN
static const float64_t sin_f64_coeff6 = 0.00909090909090909090909090909091; // 1/(10*11)
static const float64_t sin_f64_coeff7 = 0.00641025641025641025641025641026; // 1/(12*13)
static const float64_t sin_f64_coeff8 = 0.0047619047619047619047619047619; // 1/(14*15)
static const float64_t sin_f64_coeff9 = 0.00367647058823529411764705882353; // 1/(16*17)
static const float64_t sin_f64_coeff10 = 0.00292397660818713450292397660819; // 1/(18*19)
#endif

static const double CONST_0_F64            = 0.;


#define NDS_ISA_ABS_64(data) ((data >= 0) ? data : ((data == INT64_MIN) ? INT64_MAX : -data))
static inline float64_t vec_sin_f64(float64_t x)
{
    return sin(x);
}

static const q31_t fix_pi_q29 = 0x6487ED4E;        // PI in Q29
static const q31_t fix_one_q29 = 0x1FFFFFFF;       // 1.0 in Q29
static const q31_t fix_mone_q29 = 0xE0000000;      //-536870912, -1.0 in Q29
static const q31_t fix_scale_1 = 0x05555555;       // 1/3! in Q29
static const q31_t fix_scale_2 = 0x0199999A;       // 1/(4*5) in Q29
static const q31_t fix_scale_3 = 0x00C30C31;       // 1/(6*7) in Q29
static const q31_t fix_scale_4 = 0x0071C71C;        // 1/(8*9) in Q29
static const q31_t fix_scale_5 = 0x004A7905;        // 1/(10*11) in Q29
static inline q31_t vec_sin_q31(q31_t x)
{
    // input: -1.0 <= x < 1.0 in Q1.31
    q31_t input_x = 0;
    q63_t input_x_pi = 0;
    q63_t input_sq   = 0;
    q63_t acc        = 0;
    q63_t tmp_mul    = 0;
    q31_t result     = 0;
    if( x > (q31_t)(0x40000000))        // 1/2 < x < 1;
    {
        // input_x = 1.0 - x;
        input_x = (q31_t)0x7fffffff - x;
    }
    else if (x < (q31_t)(0xc0000000))   // -1 <= x < -1/2
    {
        // input_x = -1.0 - x;
        input_x = (q31_t)0x80000000 - x;
    }
    else
    {
        input_x = x;
    }

    input_x_pi = (((q63_t)input_x*(q63_t)fix_pi_q29)>>31) ;   //input_x_pi in Q29
    input_sq = (((q63_t)input_x_pi*(q63_t)input_x_pi)>>31) ;  //input_x_pi^2 in Q27

    acc = (q63_t)fix_one_q29;                                        // acc = 1.0 in Q29
    tmp_mul = (((q63_t)input_sq*(q63_t)fix_scale_1) >> 27) ;  // x^2 / 3! in Q29
    acc = acc- tmp_mul;

    tmp_mul = (((q63_t)input_sq*(q63_t)tmp_mul) >> 27) ;      // x^4 / 3! in Q29
    tmp_mul = (((q63_t)fix_scale_2*(q63_t)tmp_mul) >> 29) ;   // x^4 / 5! in Q29
    acc = acc + tmp_mul;

    tmp_mul = (((q63_t)input_sq*(q63_t)tmp_mul) >> 27) ;      // x^6 / 5! in Q29
    tmp_mul = (((q63_t)fix_scale_3*(q63_t)tmp_mul) >> 29) ;   // x^6 / 7! in Q29
    acc = acc - tmp_mul;

    tmp_mul = (((q63_t)input_sq*(q63_t)tmp_mul) >> 27) ;      // x^8 / 7! in Q29
    tmp_mul = (((q63_t)fix_scale_4*(q63_t)tmp_mul) >> 29) ;   // x^8 / 9! in Q29
    acc = acc + tmp_mul;

    tmp_mul = (((q63_t)input_sq*(q63_t)tmp_mul) >> 27) ;      // x^10 / 9! in Q29
    tmp_mul = (((q63_t)fix_scale_5*(q63_t)tmp_mul) >> 29) ;   // x^10 / 11! in Q29
    acc = acc - tmp_mul;

    acc = (((q63_t)acc*(q63_t)input_x_pi) >> (29));          // Q29
    result = (q31_t) acc;
    if(result >= fix_one_q29)
    {
        result = (fix_one_q29);
    }
    else if( result <= fix_mone_q29)
    {
        result = fix_mone_q29;
    }
    result = NDS_ISA_SATS(result*4, 32);                     // Q31

    return result;

}

// cos_q31 using the fact: sin(x+pi/2) = cos(x);
static inline q31_t vec_cos_q31(q31_t x)
{
    // input: -1.0 <= x < 1.0 in Q1.31
    q31_t input_offset = 0;
    q31_t result     = 0;
    // due to the range of input_offset is [-1 1];
    // input_offset = (x+pi/2) mod 2*pi;
    if( x>= (q31_t)(0x40000000))
    {
        input_offset = x - 0x7fffffff;
        input_offset = input_offset + 0x40000000;
        input_offset = input_offset - 0x7fffffff;
    }
    else
    {
        input_offset = x + 0x40000000;
    }
    result = vec_sin_q31(input_offset);
    return result;
}

static const q15_t fix_pi_q13 = 0x6487;        // PI in Q13
static const q15_t fix_one_q13 = 0x1FFF;       // 1.0 in Q13
static const q15_t fix_mone_q13 = 0xE000;      //-8192, -1.0 in Q13
static const q15_t fix_scale_1_q13 = 0x0555;       // 1/3! in Q13
static const q15_t fix_scale_2_q13 = 0x0199;       // 1/(4*5) in Q13
static const q15_t fix_scale_3_q13 = 0x00C3;       // 1/(6*7) in Q13
static const q15_t fix_scale_4_q13 = 0x0071;        // 1/(8*9) in Q13

static inline q31_t vec_sin_q15(q15_t x)
{
    // input: -1.0 <= x < 1.0 in Q1.31
    q15_t input_x = 0;
    q31_t input_x_pi = 0;
    q31_t input_sq   = 0;
    q31_t acc        = 0;
    q31_t tmp_mul    = 0;
    q15_t result     = 0;
    if( x > (q15_t)(0x4000))        // 1/2 < x < 1;
    {
        // input_x = 1.0 - x;
        input_x = (q15_t)0x7fff - x;
    }
    else if (x < (q15_t)(0xc000))   // -1 <= x < -1/2
    {
        // input_x = -1.0 - x;
        input_x = (q15_t)0x8000 - x;
    }
    else
    {
        input_x = x;
    }

    input_x_pi = (((q31_t)input_x*(q31_t)fix_pi_q13)>>15) ;   //input_x_pi in Q13
    input_sq = (((q31_t)input_x_pi*(q31_t)input_x_pi)>>15) ;  //input_x_pi^2 in Q11

    acc = (q31_t)fix_one_q13;                                        // acc = 1.0 in Q13
    tmp_mul = (((q31_t)input_sq*(q31_t)fix_scale_1_q13) >> 11) ;  // x^2 / 3! in Q13
    acc = acc- tmp_mul;

    tmp_mul = (((q31_t)input_sq*(q31_t)tmp_mul) >> 11) ;      // x^4 / 3! in Q13
    tmp_mul = (((q31_t)fix_scale_2_q13*(q31_t)tmp_mul) >> 13) ;   // x^4 / 5! in Q13
    acc = acc + tmp_mul;

    tmp_mul = (((q31_t)input_sq*(q31_t)tmp_mul) >> 11) ;      // x^6 / 5! in Q13
    tmp_mul = (((q31_t)fix_scale_3_q13*(q31_t)tmp_mul) >> 13) ;   // x^6 / 7! in Q13
    acc = acc - tmp_mul;

    tmp_mul = (((q31_t)input_sq*(q31_t)tmp_mul) >> 11) ;      // x^8 / 7! in Q13
    tmp_mul = (((q31_t)fix_scale_4_q13*(q31_t)tmp_mul) >> 13) ;   // x^8 / 9! in Q13
    acc = acc + tmp_mul;
    acc = (((q31_t)acc*(q31_t)input_x_pi) >> (13));          // Q13
    result = (q15_t) acc;
    if(result >= fix_one_q13)
    {
        result = (fix_one_q13);
    }
    else if( result <= fix_mone_q13)
    {
        result = fix_mone_q13;
    }
    result = NDS_ISA_SATS(result*4, 16);                     // Q15

    return result;
}

static inline q31_t vec_cos_q15(q15_t x)
{
    q15_t input_offset = 0;
    q15_t result     = 0;
    // due to the range of input_offset is [-1 1];
    // input_offset = (x+pi/2) mod 2*pi;
    if( x>= (q31_t)(0x4000))
    {
        input_offset = x + 0x8000;
        input_offset = input_offset + 0x4000;
        input_offset = input_offset + 0x8000;
    }
    else
    {
        input_offset = x + 0x4000;
    }
    result = vec_sin_q15(input_offset);
    return result;
}


#if defined (__riscv_zfh)
static const float16_t f16_pi_v     = CONST_PI;
static const float16_t f16_pio2_v   = (CONST_PI / 2);
static const float16_t f16_ipi_v    = (1 / CONST_PI);
static const float16_t f16_te_sin_coeff2 = 0.166666666666f; // 1/(2*3)
static const float16_t f16_te_sin_coeff3 = 0.05f;           // 1/(4*5)
static const float16_t f16_te_sin_coeff4 = 0.023809523810f; // 1/(6*7)
static const float16_t f16_te_sin_coeff5 = 0.013888888889f; // 1/(8*9)

static inline float16_t vec_sin_f16(float16_t x)
{
    // algorithm from compute library
    union16_utils_t c_v;

    //Find positive or negative
    c_v.i16 = NDS_ISA_ABS((int)x * f16_ipi_v);
    uint32_t sign_v = (x < 0) ? 1 : 0;
    uint32_t odd_v  = c_v.u16 & 1;
    uint32_t neg_v = odd_v ^ sign_v;

    // Modulus a - (n * int(a*(1/n)))
    float16_t absx = fabsf(x);
    float16_t ma = absx - f16_pi_v * (float16_t)c_v.i16;
    const uint32_t reb_v = (ma >= f16_pio2_v) ? 1 : 0;

    //Rebase a between 0 and pi/2
    ma = (reb_v != 0) ? (f16_pi_v - ma) : ma;

    // Taylor series
    const float16_t ma2 = ma * ma;

    //2nd elem: x^3 / 3!
    float16_t elem = (ma * ma2) * f16_te_sin_coeff2;
    float16_t res = ma - elem;

    //3rd elem: x^5 / 5!
    elem = (elem * ma2) * f16_te_sin_coeff3;
    res = res + elem;

    //4th elem: x^7 / 7!float32x2_t vsin_f32(float32x2_t val)
    elem = (elem * ma2) * f16_te_sin_coeff4;
    res = res - elem;

    //5th elem: x^9 / 9!
    elem = (elem * ma2) * f16_te_sin_coeff5;
    res = res + elem;

    //Change of sign
    neg_v = neg_v << 15;
    union16_utils_t ures, out;
    ures.f16 = res;
    out.u16 = (ures.u16 ^ neg_v);
    return (out.f16);
}
#endif

static inline float vec_cos_f32(float x)
{
#ifdef ENA_HIGHER_PERFORMANCE_VCOS_F32
    float32_t val = x + pio2_v;
    return (vec_sin_f32(val));
#else
    return cosf(x);
#endif
}

static inline float64_t vec_cos_f64(float64_t x)
{
    return cos(x);
}

#if defined (__riscv_zfh)
static inline float16_t vec_cos_f16(float16_t x)
{
    float16_t val = x + f16_pio2_v;
    return (vec_sin_f16(val));
}
#endif

#define F2Q(f) ((int)((f) * 0x8000))
#define MULQ(a, b) (((a) * (b)) >> 15)
#define SQRT2 1.4142135623730950488016887242097

static inline int vec_rsqrt(int x)
{
    int y;
    if (x & 0x8000)
    {
        y = MULQ(F2Q(3.81) - x, F2Q(0.37));
    }
    else
        y = MULQ(F2Q(3.81) - (x << 1), F2Q(0.37 * SQRT2));

    int x2 = x >> 1;
    y = MULQ(y, (F2Q(1.5) - MULQ(MULQ(x2, y), y)));
    y = MULQ(y, (F2Q(1.5) - MULQ(MULQ(x2, y), y)));

    return y;
}

static inline q15_t vec_sqrt_q15(q15_t val_in)
{
    int val = val_in;
    if (val > 0)
    {
        unsigned signbits = NDS_ISA_CLZ(val) - 16; // - 17 + 1
        signbits &= ~0x1;
        val = val << signbits;
        val = MULQ(val, vec_rsqrt(val));
        val = val >> (signbits >> 1);
        return val;
    }
    else
    {
        return (val != 0) ? -1 : 0;
    }
}

static inline q31_t vec_sqrt_q31(q31_t src)
{
        q31_t val, temp1, var1, signbit, halfval, out;

        val = src;
        if (val > 0)
        {
            signbit = NDS_ISA_CLZ(val) - 1;
            signbit &= ~0x1;
            val = val << signbit;
            halfval = val >> 1;
            temp1 = val;

            // The following formulas are derived from the above transformation.
            if (val <= 0x40000000) //input=16, cycle is 3,139,583
                var1 = (q31_t)(0x9BACEF80UL - (uint32_t) val);  // var1 + val = 2611801984 (0x9BACEF80)

            else if (val < 0x4AC00000)
                var1 = (q31_t)(0x9E4CEF80UL - (uint32_t) val);  // var1 + val ~= 2655842176 (0x9E4CEF80)

            else if (val < 0x56400001)
                var1 = (q31_t)(0xA40CEF80UL - (uint32_t) val);  // var1 + val ~= 2752311168 (0xA40CEF80)

            else if (val < 0x62400001)
                var1 = (q31_t)(0xAA0CEF80UL - (uint32_t) val);  // var1 + val ~= 2852974464 (0xAA0CEF80)

            else if (val < 0x6E400001)
                var1 = (q31_t)(0xB00CEF80UL - (uint32_t) val);  // var1 + val ~= 2953637760 (0xB00CEF80)

            else if (val < 0x79000000)
                var1 = (q31_t)(0xB5ECEF80UL - (uint32_t) val);  // var1 + val ~= 3052203904 (0xB5ECEF80)

            else
                var1 = halfval;    // var1 is the half of val. halfval = val >> 1;

            /* After three iterations:                     */
            /* Error: max is 68. (3.17 * 10^ -8, about 25 bits ) */
            /* The inst count is 75, The cycle count is 82.     */

            /* After four iterations:                     */
            /* Error: max is 7. (3.3 * 10^ -9, about 29 bits ) */
            /* The inst count is 89, The cycle count is 96.     */

            /* Iteration */
            var1 = ((q31_t)((q63_t)var1
                            * (0x30000000 - ((q31_t)((((q31_t)(((q63_t)var1
                                             * var1) >> 31)) * (q63_t)halfval) >> 31))) >> 31)) << 2;

            var1 = ((q31_t)((q63_t)var1
                            * (0x30000000 - ((q31_t)((((q31_t)(((q63_t)var1
                                             * var1) >> 31)) * (q63_t)halfval) >> 31))) >> 31)) << 2;

            var1 = ((q31_t)((q63_t)var1
                            * (0x30000000 - ((q31_t)((((q31_t)(((q63_t)var1
                                             * var1) >> 31)) * (q63_t)halfval) >> 31))) >> 31)) << 2;
            var1 = ((q31_t)((q63_t)var1
                            * (0x30000000 - ((q31_t)((((q31_t)(((q63_t)var1
                                             * var1) >> 31)) * (q63_t)halfval) >> 31))) >> 31)) << 2;
            var1 = ((q31_t)(((q63_t)temp1 * var1) >> 31)) << 1;

            var1 = var1 >> (signbit / 2);
            return var1;
        }
        else
        {
            out = (val == 0) ? 0 : -1;
            return out;
        }
}

static inline float32_t vec_int_exp_f32(float32_t base, int32_t expt)
{
    float32_t in = base;
    expt--;
    while(expt != 0)
    {
        in = in * base;
        expt--;
    }
    return in;
}

#if defined (__riscv_zfh)
static inline float16_t vec_int_exp_f16(float16_t base, int32_t expt)
{
    float16_t in = base;
    expt--;
    while(expt != 0)
    {
        in = in * base;
        expt--;
    }
    return in;
}
#endif

#define NORM_SH     (29)
#define CST_1       (1L << NORM_SH)
#define CST_2       (2L * CST_1)
#define D2QN(F, SH) ((int)((float)F * (float)(1L << SH)))
#define GHMUL(X, Y) ((int)(((q63_t)X * Y) >> 32))


static inline int vec_divide_q31(q31_t numerator, q31_t denominator, q31_t *quotient, int16_t *shift)
{
    int16_t sign=0;
    q63_t temp;
    int16_t shiftForNormalizing;

    *shift = 0;

    sign = (numerator>>31) ^ (denominator>>31);

    if (denominator == 0)
    {
       if (sign)
       {
          *quotient = 0x80000000;
       }
       else
       {
          *quotient = 0x7FFFFFFF;
       }
       return -1;
    }

    int dvd, dvs;
    dvd = NDS_ISA_ABS(numerator);
    dvs = NDS_ISA_ABS(denominator);

    //temp = ((q63_t)dvd << 31) / ((q63_t)dvs);
    q31_t xi;
    int sh;
    const q31_t cst_2 = CST_2;
    sh = (int32_t)NDS_ISA_CLZ(dvs) - (int32_t)(32 - NORM_SH);
    dvs = (sh >= 0) ? (dvs << sh) : (dvs >> (-sh));
    xi = GHMUL(dvs, D2QN(2.6192200905308, 29));
    xi = xi - D2QN(5.83543568753, 26);
    xi = GHMUL(dvs, xi);
    xi = xi + D2QN(4.2349928801493, 23);    /* q23 */
    temp = D2QN(2.0, 20) - GHMUL(xi, dvs);  /* q20 */
    temp <<= 9;                             /* q29 */
    xi = GHMUL(xi, temp);                   /* q20 */
    xi <<= 9;
    temp = GHMUL(xi, dvs);
    temp <<= 3;
    xi = GHMUL(xi, (cst_2 - temp));
    xi <<= 3;

    sh = NORM_SH - (31 - NORM_SH) - sh;
    int64_t ta = (q63_t)xi * dvd;
    temp = ta >> sh;

    shiftForNormalizing= 32 - NDS_ISA_CLZ(temp >> 31);

    if (shiftForNormalizing > 0)
    {
       *shift = shiftForNormalizing;
       temp = temp >> shiftForNormalizing;
    }
    if (sign)
    {
      temp = -temp;
    }

    *quotient = (q31_t)temp;

    return 0;
}

static inline int vec_divide_q15(q15_t numerator, q15_t denominator, q15_t *quotient, int16_t *shift)
{
    int16_t sign=0;
    q31_t temp;
    int16_t shiftForNormalizing;

    *shift = 0;

    sign = (numerator >> 15) ^ (denominator >> 15);

    if (denominator == 0)
    {
       if (sign)
       {
          *quotient = 0x8000;
       }
       else
       {
          *quotient = 0x7FFF;
       }
       return -1;
    }

    q15_t dvd, dvs;
    dvd = NDS_ABS_Q15(numerator);
    dvs = NDS_ABS_Q15(denominator);

    temp = ((q31_t)dvd << 15) / ((q31_t)dvs);

    shiftForNormalizing= 17 - NDS_ISA_CLZ(temp);

    if (shiftForNormalizing > 0)
    {
       *shift = shiftForNormalizing;
       temp = temp >> shiftForNormalizing;
    }
    if (sign)
    {
      temp = -temp;
    }

    *quotient = temp;

    return 0;
}

#endif  // __INTERNAL_VEC_UTILS_H__

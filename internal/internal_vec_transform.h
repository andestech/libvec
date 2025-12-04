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

#ifndef __INTERNAL_VEC_TRANSFORM_H__
#define __INTERNAL_VEC_TRANSFORM_H__

#include "riscv_vec_transform.h"
#include "math.h"
#include "stdlib.h"

///////////////////////////
// Internal macro define
///////////////////////////
#define NDSV_ALIGNMENT 8
// #define NE10_INLINE inline static

/*
 * FFT Factor Flags
 *
 * These are used within Ne10 to decide how an input FFT size should be factored into
 * stages (i.e. what radices should be used).
 *
 * - NDSV_FACTOR_DEFAULT factors into 2, 3, 4, 5.
 * - NDSV_FACTOR_EIGHT_FIRST_STAGE is NDSV_FACTOR_DEFAULT with the extended ability to
 *   have a radix-8 initial stage.
 * - NDSV_FACTOR_EIGHT factors into 2, 3, 4, 5, 8.
 */
#define NDSV_FACTOR_DEFAULT             0
#define NDSV_FACTOR_EIGHT_FIRST_STAGE   1
#define NDSV_FACTOR_EIGHT               2

#ifdef DEBUG_MODE
#define DBG_MSG         printf
#else
#define DBG_MSG(...)
#endif

///////////////////////////
// common varibles
///////////////////////////

/* Twiddles used in Radix-8 FFT */
const static float32_t TW_81_F32  =  0.70710678; // sqrt (2) / 2
const static float32_t TW_81N_F32 = -0.70710678; // - TW_81_F32

/* Twiddles used in Radix-5 FFT */
const static riscv_vec_cpx_f32_t TW_5A_F32 =
        {
             0.309016994374947, //   cos (2 * pi / 5)
            -0.951056516295154  // - sin (2 * pi / 5)
        };
const static riscv_vec_cpx_q31_t TW_5A_S32 =
        {
              663608942, // round (TW_5A_F32.r * 2^31)
            -2042378317  // round (TW_5A_F32.i * 2^31)
        };

const static riscv_vec_cpx_f32_t TW_5B_F32 =
        {
            -0.809016994374947, //   cos (4 * pi / 5)
            -0.587785252292473  // - sin (4 * pi / 5)
        };
const static riscv_vec_cpx_q31_t TW_5B_S32 =
        {
            -1737350766, // round (TW_5B_F32.r * 2^31)
            -1262259218  // round (TW_5B_F32.i * 2^31)
        };

/* Twiddles used in Radix-3 FFT */
const static float32_t TW_3I_F32  =   0.866025403784439; // sqrt (3) / 2
const static float32_t TW_3IN_F32 = - 0.866025403784439; // - TW_3IN_F32
const static q31_t TW_3I_S32 = 1859775393; // round (TW_3I_F32 * 2^31)
const static q31_t TW_3IN_S32 = -1859775393; // round (TW_3IN_F32 * 2^31)

#define NDSV_STOCKHAM_FACTOR_MAX    4
// factor element definition
#define OUT_INDEX                   0
#define SIZE_FIRST_STAGE            1
#define RFFT_EN                     2
#define IS_RADIX_8                  3
#define NDSV_FACTOR_MAX             32
/////////////////////////////////////////////////////////
// constant values that are used across the library
/////////////////////////////////////////////////////////

#define NDSV_PI (float32_t)(3.1415926535897932384626433832795)

/////////////////////////////////////////////////////////
// some external macro definitions to be exposed to the users
/////////////////////////////////////////////////////////

#define NDSV_MALLOC malloc
#define NDSV_FREE(p) \
    do { \
        free(p); \
        p = 0; \
    }while(0)

#define NDSV_MIN(a,b) ((a)>(b)?(b):(a))
#define NDSV_MAX(a,b) ((a)<(b)?(b):(a))

#define NDSV_ALIGN(address, alignment) \
    do { \
        (address) = (((address) + ((alignment) - 1)) & ~ ((alignment) - 1)); \
    }while (0)

/////////////////////////////////////////////////////////
// macro definitions for float to fixed point
/////////////////////////////////////////////////////////
#define NDSV_F2I16_MAX         32767
#define NDSV_F2I16_SHIFT       15
#define NDSV_F2I16_SAMPPROD    q31_t
#define NDSV_F2I16_OP(x)       (q15_t)((x)*NDSV_F2I16_MAX + 0.5f)
#define NDSV_F2I16_SROUND(x)   (q15_t)((((x)<<1)+(1<<NDSV_F2I16_SHIFT))>>16)
#define NDSV_F2I16_SMUL(a,b)   ((NDSV_F2I16_SAMPPROD)(a)*(b))
#define NDSV_F2I16_FIXDIV(c,div) \
    do {    ((c).r) = ( ( ((c).r)/div) );  \
        ((c).i) = ( ( ((c).i)/div) ); }while (0)

#define NDSV_F2I32_MAX         2147483647
#define NDSV_F2I32_SHIFT       31
#define NDSV_F2I32_SAMPPROD    q63_t
#define NDSV_F2I32_OP(x)       (q31_t)((x)*NDSV_F2I32_MAX + 0.5f)
#define NDSV_F2I32_SROUND(x)   (q31_t) ((x)>>NDSV_F2I32_SHIFT)
#define NDSV_F2I32_SMUL(a,b)    ((NDSV_F2I32_SAMPPROD)(a)*(b))
#define NDSV_F2I32_FIXDIV(c,div) \
    do {    ((c).r) = ( ( ((c).r)/div) );  \
        ((c).i) = ( ( ((c).i)/div) ); }while (0)

#endif

#include "internal_nds_types.h"
#ifdef ENA_HVM
#include "internal_hvm_ctrl.h"
#endif

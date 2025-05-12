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

#ifndef __INTERNAL_VEC_ISA_H__
#define __INTERNAL_VEC_ISA_H__

#define NDS_ASM     __asm__ __volatile__

//Macros for vector instructions

/* Switches for RVV version */

//NDS_VEC_RVV_VERSION format for RVV spec. version:
//Format: XXYYC, where XXYYC is a decimal number and
//  XX: Major version
//  YY: Minor version
//  C: RC number
//An Example for RVV spec. v20.33 RC1:
//  XX = 20, YY = 33, C = 1
//So NDS_VEC_RVV_VERSION = 20331
#ifndef NDS_VEC_RVV_VERSION
    #define NDS_VEC_RVV_VERSION     1000
#endif

#if NDS_VEC_RVV_VERSION == 70
    #define NDS_VEC_RVV_VERSION_0_7         //riscv-v-spec-0.7
#elif NDS_VEC_RVV_VERSION == 80
    #define NDS_VEC_RVV_VERSION_0_8         //riscv-v-spec-0.8
#elif NDS_VEC_RVV_VERSION == 90
    #define NDS_VEC_RVV_VERSION_0_9         //riscv-v-spec-0.9
#elif NDS_VEC_RVV_VERSION == 100
    #define NDS_VEC_RVV_VERSION_0_10        //riscv-v-spec-0.10
#elif NDS_VEC_RVV_VERSION == 1000
    #define NDS_VEC_RVV_VERSION_1_0         //riscv-v-spec-1.0
#else
    #error "[RVV spec. error] The version of RVV is not correct! Please use -DNDS_VEC_RVV_VERSION=<version number> to specify the version of RVV."
#endif

/* Switches for support strided and indexed loads/stores */
#define ENA_VEC_ISA_STRIDE_LS
#define ENA_VEC_ISA_INDEXED_LS

/* Switches for sub-extensions */

//Zvlsseg
//#define ENA_VEC_ISA_ZVLSSEG

//Zvamo
//#define ENA_VEC_ISA_ZVAMO

//Zvediv
//#define ENA_VEC_ISA_ZVEDIV

//Zvqmac is supported from v0.8 and later.
#ifdef NDS_VEC_RVV_VERSION_0_7
    #undef ENA_VEC_ISA_ZVQMAC
#else
    #define ENA_VEC_ISA_ZVQMAC
#endif

//AndeStar V5 Vector Dot Product Extension
#ifndef ENA_NDS_V5_VEC_DOT_PROD
    #define ENA_NDS_V5_VEC_DOT_PROD
#endif

//Vector registers (V0 to V31)
#define NDS_VEC_V0                  "v0"
#define NDS_VEC_V1                  "v1"
#define NDS_VEC_V2                  "v2"
#define NDS_VEC_V3                  "v3"
#define NDS_VEC_V4                  "v4"
#define NDS_VEC_V5                  "v5"
#define NDS_VEC_V6                  "v6"
#define NDS_VEC_V7                  "v7"
#define NDS_VEC_V8                  "v8"
#define NDS_VEC_V9                  "v9"
#define NDS_VEC_V10                 "v10"
#define NDS_VEC_V11                 "v11"
#define NDS_VEC_V12                 "v12"
#define NDS_VEC_V13                 "v13"
#define NDS_VEC_V14                 "v14"
#define NDS_VEC_V15                 "v15"
#define NDS_VEC_V16                 "v16"
#define NDS_VEC_V17                 "v17"
#define NDS_VEC_V18                 "v18"
#define NDS_VEC_V19                 "v19"
#define NDS_VEC_V20                 "v20"
#define NDS_VEC_V21                 "v21"
#define NDS_VEC_V22                 "v22"
#define NDS_VEC_V23                 "v23"
#define NDS_VEC_V24                 "v24"
#define NDS_VEC_V25                 "v25"
#define NDS_VEC_V26                 "v26"
#define NDS_VEC_V27                 "v27"
#define NDS_VEC_V28                 "v28"
#define NDS_VEC_V29                 "v29"
#define NDS_VEC_V30                 "v30"
#define NDS_VEC_V31                 "v31"

#define NDS_VEC_VTYPE_SEW_E8        "e8"
#define NDS_VEC_VTYPE_SEW_E16       "e16"
#define NDS_VEC_VTYPE_SEW_E32       "e32"
#define NDS_VEC_VTYPE_SEW_E64       "e64"
#define NDS_VEC_VTYPE_SEW_E128      "e128"
#define NDS_VEC_VTYPE_SEW_E256      "e256"
#define NDS_VEC_VTYPE_SEW_E512      "e512"
#define NDS_VEC_VTYPE_SEW_E1024     "e1024"

#define NDS_VEC_VTYPE_SEW_BIT       3       //The start bit of VSEW
#define NDS_VEC_VTYPE_SEW_VAL_E8   0b000UL
#define NDS_VEC_VTYPE_SEW_VAL_E16  0b001UL
#define NDS_VEC_VTYPE_SEW_VAL_E32  0b010UL
#define NDS_VEC_VTYPE_SEW_VAL_E64  0b011UL

#define NDS_VEC_VTYPE_LMUL_M1       "m1"
#define NDS_VEC_VTYPE_LMUL_M2       "m2"
#define NDS_VEC_VTYPE_LMUL_M4       "m4"
#define NDS_VEC_VTYPE_LMUL_M8       "m8"

#define NDS_VEC_VTYPE_LMUL_ID_M1    1
#define NDS_VEC_VTYPE_LMUL_ID_M2    2
#define NDS_VEC_VTYPE_LMUL_ID_M4    4
#define NDS_VEC_VTYPE_LMUL_ID_M8    8

#define NDS_VEC_VTYPE_LMUL_VAL_M1  0b000UL
#define NDS_VEC_VTYPE_LMUL_VAL_M2  0b001UL
#define NDS_VEC_VTYPE_LMUL_VAL_M4  0b010UL
#define NDS_VEC_VTYPE_LMUL_VAL_M8  0b011UL
#define NDS_VEC_VTYPE_LMUL_VAL_MF2 0b111UL
#define NDS_VEC_VTYPE_LMUL_VAL_MF4 0b110UL
#define NDS_VEC_VTYPE_LMUL_VAL_MF8 0b101UL

#define NDS_VEC_VTYPE_EDIV_D1       "d1"
#define NDS_VEC_VTYPE_EDIV_D2       "d2"
#define NDS_VEC_VTYPE_EDIV_D4       "d4"
#define NDS_VEC_VTYPE_EDIV_D8       "d8"

#define NDS_VEC_MAKE_VTYPE(VSEW, VLMUL)     ((VSEW << NDS_VEC_VTYPE_SEW_BIT) | VLMUL)

//Include Vector instruction Macros definitions:
#if defined(NDS_VEC_RVV_VERSION_0_7)
    //riscv-v-spec-0.7
    //#pragma message ("riscv-v-spec-0.7")
    #include "internal_vec_i.h"
    #include "internal_vec_im.h"
#elif defined(NDS_VEC_RVV_VERSION_0_8)
    //riscv-v-spec-0.8
    //#pragma message ("riscv-v-spec-0.8")
    #include "internal_vec_i_v0_8.h"
    #include "internal_vec_im_v0_8.h"
#elif defined(NDS_VEC_RVV_VERSION_0_9)
    //riscv-v-spec-0.9
    //#pragma message ("riscv-v-spec-0.9")
    #include "internal_vec_i_v0_9.h"
    #include "internal_vec_im_v0_9.h"
#elif defined(NDS_VEC_RVV_VERSION_1_0)
    //riscv-v-spec-1.0
    //#pragma message ("riscv-v-spec-1.0")
    #include "internal_vec_i_v1_0.h"
    #include "internal_vec_im_v1_0.h"
#else
    //defined(NDS_VEC_RVV_VERSION_0_10)
    //riscv-v-spec-0.10
    //#pragma message ("riscv-v-spec-0.10")
    #include "internal_vec_i_v0_10.h"
    #include "internal_vec_im_v0_10.h"
#endif

#if NDS_VEC_RVV_VERSION >= 100
    #include <nds_intrinsic.h>

    #define NDS_MVEC_CFG_DW_BIT         8       //The start bit of DW
    #define NDS_MVEC_CFG_DW_SIZE        3       //The bit size of DW
    #define NDS_MVEC_CFG_DW_VAL_1_1     0UL     //DLEN = VLEN/1
    #define NDS_MVEC_CFG_DW_VAL_1_2     1UL     //DLEN = VLEN/2
    #define NDS_MVEC_CFG_DW_VAL_1_4     2UL     //DLEN = VLEN/4
    #define NDS_MVEC_CFG_DW_VAL_1_8     3UL     //DLEN = VLEN/8
        
    #define NDS_VEC_CSR_GET_FIELD(VCSR_NAME, VCSR_FIELD_BIT_INDEX, VCSR_FIELD_WIDTH) \
        (((unsigned long)(NDS_VEC_CSR_GET(VCSR_NAME)) >> VCSR_FIELD_BIT_INDEX) & ((1UL << VCSR_FIELD_WIDTH) - 1UL))

    #define NDS_VEC_CSR_GET(VCSR_NAME)  __nds__read_csr((VCSR_NAME))
    #define NDS_VEC_CSR_GET_VSTART()    NDS_VEC_CSR_GET(NDS_VSTART)
    #define NDS_VEC_CSR_GET_VXSAT()     NDS_VEC_CSR_GET(NDS_VXSAT)
    #define NDS_VEC_CSR_GET_VXRM()      NDS_VEC_CSR_GET(NDS_VXRM)
    #define NDS_VEC_CSR_GET_VCSR()      NDS_VEC_CSR_GET(NDS_VCSR)
    #define NDS_VEC_CSR_GET_VL()        NDS_VEC_CSR_GET(NDS_VL)
    #define NDS_VEC_CSR_GET_VTYPE()     NDS_VEC_CSR_GET(NDS_VTYPE)
    #define NDS_VEC_CSR_GET_VLENB()     NDS_VEC_CSR_GET(NDS_VLENB)

    #ifdef __linux__
        #define NDS_VEC_CSR_GET_DLEN_RATIO()    (NDS_MVEC_CFG_DW_VAL_1_1)
    #else
        #define NDS_VEC_CSR_GET_DLEN_RATIO()    NDS_VEC_CSR_GET_FIELD(NDS_MVEC_CFG, NDS_MVEC_CFG_DW_BIT, NDS_MVEC_CFG_DW_SIZE)
    #endif

#endif

#include "internal_vec_def_vg.h"

// Support RVV intrinsic
#ifdef ENA_VEC_INTRINSIC
    #include <riscv_vector.h>
#endif

#endif // __INTERNAL_VEC_ISA_H__

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
#ifndef __INTERNAL_WINDOW_H__
#define __INTERNAL_WINDOW_H__

#include <math.h>
#include <riscv_vec_types.h>

#include "riscv_vec_utils.h"
#include "internal_vec_utils.h"   // _cos_f32

#define CST_F2 2.0
#define CST_F3 3.0
#define CST_F4 4.0
#define CST_F5 5.0
#define CST_F6 6.0
#define CST_F7 7.0
#define CST_F8 8.0
#define CST_F9 9.0
#define CST_F10 10.0
#define CST_PI (3.14159265358979323846)

#ifdef ENA_F32_PARAMETER
#define NDS_VG_VSETVLI NDS_VG_VSETVLI_E32
#define NDS_VSTORE NDS_VEC_VSE32_V
#define CST_PIO2V pio2_v
#define CST_IPI_V ipi_v
#define CST_FZERO CONST_0
#define CST_PI_V pi_v
#define CST_VEC_SIN_COEFF2 te_sin_coeff2
#define CST_VEC_SIN_COEFF3 te_sin_coeff3
#define CST_VEC_SIN_COEFF4 te_sin_coeff4
#define CST_VEC_SIN_COEFF5 te_sin_coeff5
#define CST_VEC_SIN_COEFF6 te_sin_coeff6
#define SIGN_BIT 0x80000000
#define MM_TYPE float32_t
#define COS_TYPE cosf
#define NDS_GET_COS_VAL GET_COS_VAL

#elif defined ENA_F64_PARAMETER
#define NDS_VG_VSETVLI NDS_VG_VSETVLI_E64
#define NDS_VSTORE NDS_VEC_VSE64_V
#define CST_PIO2V pio2_v_f64
#define CST_IPI_V ipi_v_f64
#define CST_FZERO CONST_0_F64
#define CST_PI_V pi_v_f64
#define CST_VEC_SIN_COEFF2 sin_f64_coeff2
#define CST_VEC_SIN_COEFF3 sin_f64_coeff3
#define CST_VEC_SIN_COEFF4 sin_f64_coeff4
#define CST_VEC_SIN_COEFF5 sin_f64_coeff5
#define CST_VEC_SIN_COEFF6 sin_f64_coeff6
#define CST_VEC_SIN_COEFF7 sin_f64_coeff7
#define CST_VEC_SIN_COEFF8 sin_f64_coeff8
#define CST_VEC_SIN_COEFF9 sin_f64_coeff9
#define CST_VEC_SIN_COEFF10 sin_f64_coeff10
#define SIGN_BIT 0x8000000000000000
#define MM_TYPE float64_t
#define COS_TYPE cos
#define NDS_GET_COS_VAL GET_COS_VAL_F64

#elif defined ENA_F16_PARAMETER
#define NDS_VG_VSETVLI NDS_VG_VSETVLI_E16
#define NDS_VSTORE NDS_VEC_VSE16_V
#define CST_PIO2V f16_pio2_v
#define CST_IPI_V f16_ipi_v
#define CST_FZERO F16_CONST_0
#define CST_PI_V f16_pi_v
#define CST_VEC_SIN_COEFF2 f16_te_sin_coeff2
#define CST_VEC_SIN_COEFF3 f16_te_sin_coeff3
#define CST_VEC_SIN_COEFF4 f16_te_sin_coeff4
#define CST_VEC_SIN_COEFF5 f16_te_sin_coeff5
#define SIGN_BIT 0x8000
#define MM_TYPE float16_t
#define COS_TYPE cosf
#define NDS_GET_COS_VAL GET_COS_VAL_F16
#endif

#define GET_COS_VAL(VOUT, VIN) \
        NDS_VEC_VFADD_VF(NDS_VEC_VG1, VIN, CST_PIO2V); \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG1, CST_IPI_V);  \
        NDS_VEC_VFCVT_RTZ_X_F_V(NDS_VEC_VG2, NDS_VEC_VG2);  \
        NDS_VEC_VRSUB_VX(NDS_VEC_VG3, NDS_VEC_VG2, INT_ZERO);  \
        NDS_VEC_VMAX_VV(NDS_VEC_VG2, NDS_VEC_VG3, NDS_VEC_VG2);  \
        NDS_VEC_VMFLT_VF(NDS_VEC_VG0, NDS_VEC_VG1, CST_FZERO);   \
        NDS_VEC_VAND_VI(NDS_VEC_VG3, NDS_VEC_VG3, 0x0);   \
        NDS_VEC_VADD_VI(NDS_VEC_VG4, NDS_VEC_VG3, 0x1);   \
        NDS_VEC_VMERGE_VVM(NDS_VEC_VG5, NDS_VEC_VG3, NDS_VEC_VG4);   \
        NDS_VEC_VAND_VV(NDS_VEC_VG6, NDS_VEC_VG2, NDS_VEC_VG4);  \
        NDS_VEC_VXOR_VV(NDS_VEC_VG7, NDS_VEC_VG5, NDS_VEC_VG6);  \
        NDS_VEC_VFSGNJ_VF(NDS_VEC_VG5, NDS_VEC_VG1, CST_FZERO);   \
        NDS_VEC_VFCVT_F_X_V(NDS_VEC_VG6, NDS_VEC_VG2);    \
        NDS_VEC_VFNMSAC_VF(NDS_VEC_VG5, CST_PI_V, NDS_VEC_VG6);   \
        NDS_VEC_VMFGE_VF(NDS_VEC_VG0, NDS_VEC_VG5, CST_PIO2V);   \
        NDS_VEC_VMERGE_VVM(NDS_VEC_VG6, NDS_VEC_VG3, NDS_VEC_VG4);   \
        NDS_VEC_VMSNE_VI(NDS_VEC_VG0, NDS_VEC_VG6, 0);      \
        NDS_VEC_VFRSUB_VF(NDS_VEC_VG4, NDS_VEC_VG5, CST_PI_V);  \
        NDS_VEC_VMERGE_VVM(NDS_VEC_VG3, NDS_VEC_VG5, NDS_VEC_VG4);  \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG1, NDS_VEC_VG3, NDS_VEC_VG3);  \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG3, NDS_VEC_VG1);  \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG2, CST_VEC_SIN_COEFF2);  \
        NDS_VEC_VFSUB_VV(NDS_VEC_VG4, NDS_VEC_VG3, NDS_VEC_VG2);  \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG2, NDS_VEC_VG1); \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG2, CST_VEC_SIN_COEFF3); \
        NDS_VEC_VFADD_VV(NDS_VEC_VG4, NDS_VEC_VG4, NDS_VEC_VG2);  \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG2, NDS_VEC_VG1); \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG2, CST_VEC_SIN_COEFF4);  \
        NDS_VEC_VFSUB_VV(NDS_VEC_VG4, NDS_VEC_VG4, NDS_VEC_VG2);  \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG2, NDS_VEC_VG1); \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG2, CST_VEC_SIN_COEFF5);  \
        NDS_VEC_VFADD_VV(NDS_VEC_VG4, NDS_VEC_VG4, NDS_VEC_VG2);   \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG2, NDS_VEC_VG1);  \
        NDS_VEC_VFNMSAC_VF(NDS_VEC_VG4, CST_VEC_SIN_COEFF6, NDS_VEC_VG2); \
        NDS_VEC_VMUL_VX(NDS_VEC_VG7, NDS_VEC_VG7, SIGN_BIT); \
        NDS_VEC_VXOR_VV(VOUT, NDS_VEC_VG4, NDS_VEC_VG7);

#define GET_COS_VAL_F64(VOUT, VIN) \
        NDS_VEC_VFADD_VF(NDS_VEC_VG1, VIN, CST_PIO2V); \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG1, CST_IPI_V);  \
        NDS_VEC_VFCVT_RTZ_X_F_V(NDS_VEC_VG2, NDS_VEC_VG2);  \
        NDS_VEC_VRSUB_VX(NDS_VEC_VG3, NDS_VEC_VG2, INT_ZERO);  \
        NDS_VEC_VMAX_VV(NDS_VEC_VG2, NDS_VEC_VG3, NDS_VEC_VG2);  \
        NDS_VEC_VMFLT_VF(NDS_VEC_VG0, NDS_VEC_VG1, CST_FZERO);   \
        NDS_VEC_VAND_VI(NDS_VEC_VG3, NDS_VEC_VG3, 0x0);   \
        NDS_VEC_VADD_VI(NDS_VEC_VG4, NDS_VEC_VG3, 0x1);   \
        NDS_VEC_VMERGE_VVM(NDS_VEC_VG5, NDS_VEC_VG3, NDS_VEC_VG4);   \
        NDS_VEC_VAND_VV(NDS_VEC_VG6, NDS_VEC_VG2, NDS_VEC_VG4);  \
        NDS_VEC_VXOR_VV(NDS_VEC_VG7, NDS_VEC_VG5, NDS_VEC_VG6);  \
        NDS_VEC_VFSGNJ_VF(NDS_VEC_VG5, NDS_VEC_VG1, CST_FZERO);   \
        NDS_VEC_VFCVT_F_X_V(NDS_VEC_VG6, NDS_VEC_VG2);    \
        NDS_VEC_VFNMSAC_VF(NDS_VEC_VG5, CST_PI_V, NDS_VEC_VG6);   \
        NDS_VEC_VMFGE_VF(NDS_VEC_VG0, NDS_VEC_VG5, CST_PIO2V);   \
        NDS_VEC_VMERGE_VVM(NDS_VEC_VG6, NDS_VEC_VG3, NDS_VEC_VG4);   \
        NDS_VEC_VMSNE_VI(NDS_VEC_VG0, NDS_VEC_VG6, 0);      \
        NDS_VEC_VFRSUB_VF(NDS_VEC_VG4, NDS_VEC_VG5, CST_PI_V);  \
        NDS_VEC_VMERGE_VVM(NDS_VEC_VG3, NDS_VEC_VG5, NDS_VEC_VG4);  \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG1, NDS_VEC_VG3, NDS_VEC_VG3);  \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG3, NDS_VEC_VG1);  \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG2, CST_VEC_SIN_COEFF2);  \
        NDS_VEC_VFSUB_VV(NDS_VEC_VG4, NDS_VEC_VG3, NDS_VEC_VG2);  \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG2, NDS_VEC_VG1); \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG2, CST_VEC_SIN_COEFF3); \
        NDS_VEC_VFADD_VV(NDS_VEC_VG4, NDS_VEC_VG4, NDS_VEC_VG2);  \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG2, NDS_VEC_VG1); \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG2, CST_VEC_SIN_COEFF4);  \
        NDS_VEC_VFSUB_VV(NDS_VEC_VG4, NDS_VEC_VG4, NDS_VEC_VG2);  \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG2, NDS_VEC_VG1); \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG2, CST_VEC_SIN_COEFF5);  \
        NDS_VEC_VFADD_VV(NDS_VEC_VG4, NDS_VEC_VG4, NDS_VEC_VG2);   \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG2, NDS_VEC_VG1);  \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG2, CST_VEC_SIN_COEFF6); \
        NDS_VEC_VFSUB_VV(NDS_VEC_VG4, NDS_VEC_VG4, NDS_VEC_VG2); \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG2, NDS_VEC_VG1); \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG2, CST_VEC_SIN_COEFF7); \
        NDS_VEC_VFADD_VV(NDS_VEC_VG4, NDS_VEC_VG4, NDS_VEC_VG2);    \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG2, NDS_VEC_VG1);  \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG2, CST_VEC_SIN_COEFF8); \
        NDS_VEC_VFSUB_VV(NDS_VEC_VG4, NDS_VEC_VG4, NDS_VEC_VG2);  \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG2, NDS_VEC_VG1); \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG2, CST_VEC_SIN_COEFF9); \
        NDS_VEC_VFADD_VV(NDS_VEC_VG4, NDS_VEC_VG4, NDS_VEC_VG2);    \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG2, NDS_VEC_VG1);  \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG2, CST_VEC_SIN_COEFF10); \
        NDS_VEC_VFSUB_VV(NDS_VEC_VG4, NDS_VEC_VG4, NDS_VEC_VG2);  \
        NDS_VEC_VMUL_VX(NDS_VEC_VG7, NDS_VEC_VG7, SIGN_BIT); \
        NDS_VEC_VXOR_VV(VOUT, NDS_VEC_VG4, NDS_VEC_VG7);

#define GET_COS_VAL_F16(VOUT, VIN) \
        NDS_VEC_VFADD_VF(NDS_VEC_VG1, VIN, CST_PIO2V);  \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG1, CST_IPI_V);  \
        NDS_VEC_VFCVT_RTZ_X_F_V(NDS_VEC_VG2, NDS_VEC_VG2);  \
        NDS_VEC_VRSUB_VX(NDS_VEC_VG3, NDS_VEC_VG2, INT_ZERO);  \
        NDS_VEC_VMAX_VV(NDS_VEC_VG2, NDS_VEC_VG3, NDS_VEC_VG2);  \
        NDS_VEC_VMFLT_VF(NDS_VEC_VG0, NDS_VEC_VG1, CST_FZERO);  \
        NDS_VEC_VAND_VI(NDS_VEC_VG3, NDS_VEC_VG3, 0x0);  \
        NDS_VEC_VADD_VI(NDS_VEC_VG4, NDS_VEC_VG3, 0x1);  \
        NDS_VEC_VMERGE_VVM(NDS_VEC_VG5, NDS_VEC_VG3, NDS_VEC_VG4);  \
        NDS_VEC_VAND_VV(NDS_VEC_VG6, NDS_VEC_VG2, NDS_VEC_VG4);  \
        NDS_VEC_VXOR_VV(NDS_VEC_VG7, NDS_VEC_VG5, NDS_VEC_VG6);  \
        NDS_VEC_VFSGNJ_VF(NDS_VEC_VG5, NDS_VEC_VG1, CST_FZERO);  \
        NDS_VEC_VFCVT_F_X_V(NDS_VEC_VG6, NDS_VEC_VG2);  \
        NDS_VEC_VFNMSAC_VF(NDS_VEC_VG5, CST_PI_V, NDS_VEC_VG6);  \
        NDS_VEC_VMFGE_VF(NDS_VEC_VG0, NDS_VEC_VG5, CST_PIO2V);  \
        NDS_VEC_VMERGE_VVM(NDS_VEC_VG6, NDS_VEC_VG3, NDS_VEC_VG4);  \
        NDS_VEC_VMSNE_VI(NDS_VEC_VG0, NDS_VEC_VG6, 0);  \
        NDS_VEC_VFRSUB_VF(NDS_VEC_VG4, NDS_VEC_VG5, CST_PI_V);  \
        NDS_VEC_VMERGE_VVM(NDS_VEC_VG3, NDS_VEC_VG5, NDS_VEC_VG4);  \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG1, NDS_VEC_VG3, NDS_VEC_VG3);  \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG3, NDS_VEC_VG1);  \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG2, CST_VEC_SIN_COEFF2);  \
        NDS_VEC_VFSUB_VV(NDS_VEC_VG4, NDS_VEC_VG3, NDS_VEC_VG2);  \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG2, NDS_VEC_VG1);  \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG2, CST_VEC_SIN_COEFF3);  \
        NDS_VEC_VFADD_VV(NDS_VEC_VG4, NDS_VEC_VG4, NDS_VEC_VG2);  \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG2, NDS_VEC_VG1);  \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG2, CST_VEC_SIN_COEFF4);  \
        NDS_VEC_VFSUB_VV(NDS_VEC_VG4, NDS_VEC_VG4, NDS_VEC_VG2);  \
        NDS_VEC_VFMUL_VV(NDS_VEC_VG2, NDS_VEC_VG2, NDS_VEC_VG1);  \
        NDS_VEC_VFMUL_VF(NDS_VEC_VG2, NDS_VEC_VG2, CST_VEC_SIN_COEFF5);  \
        NDS_VEC_VFADD_VV(NDS_VEC_VG4, NDS_VEC_VG4, NDS_VEC_VG2);  \
        NDS_VEC_VMUL_VX(NDS_VEC_VG7, NDS_VEC_VG7, SIGN_BIT); \
        NDS_VEC_VXOR_VV(VOUT, NDS_VEC_VG4, NDS_VEC_VG7);

static inline void vec_weighted_sum_of_cos_1(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1)
{
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = (MM_TYPE)CST_PI * i * k;
         w = c0 + c1 * COS_TYPE(w);
         out[i] = w;
    }
}

static inline void vec_weighted_sum_of_cos_2(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2)
{
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = CST_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w);
         out[i] = w;
    }
}

static inline void vec_weighted_sum_of_cos_3(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2,
                                          MM_TYPE c3)
{
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = CST_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w) + c3 * COS_TYPE((MM_TYPE)3.0 * w);
         out[i] = w;
    }
}

static inline void vec_weighted_sum_of_cos_4(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2,
                                          MM_TYPE c3,
                                          MM_TYPE c4)
{
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = CST_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w) + c3 * COS_TYPE((MM_TYPE)3.0 * w) + c4 * COS_TYPE((MM_TYPE)4.0 * w);
         out[i] = w;
    }
}

static inline void vec_weighted_sum_of_cos_5(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2,
                                          MM_TYPE c3,
                                          MM_TYPE c4,
                                          MM_TYPE c5)
{
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = CST_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w) + c3 * COS_TYPE((MM_TYPE)3.0 * w) + c4 * COS_TYPE((MM_TYPE)4.0 * w)
             + c5 * COS_TYPE((MM_TYPE)5.0 * w);
         out[i] = w;
    }
}

static inline void vec_weighted_sum_of_cos_6(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2,
                                          MM_TYPE c3,
                                          MM_TYPE c4,
                                          MM_TYPE c5,
                                          MM_TYPE c6)
{
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = CST_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w) + c3 * COS_TYPE((MM_TYPE)3.0 * w) + c4 * COS_TYPE((MM_TYPE)4.0 * w)
             + c5 * COS_TYPE((MM_TYPE)5.0 * w) + c6 * COS_TYPE((MM_TYPE)6.0 * w);
         out[i] = w;
    }
}

static inline void vec_weighted_sum_of_cos_7(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2,
                                          MM_TYPE c3,
                                          MM_TYPE c4,
                                          MM_TYPE c5,
                                          MM_TYPE c6,
                                          MM_TYPE c7)
{
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = CST_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w) + c3 * COS_TYPE((MM_TYPE)3.0 * w) + c4 * COS_TYPE((MM_TYPE)4.0 * w)
             + c5 * COS_TYPE((MM_TYPE)5.0 * w) + c6 * COS_TYPE((MM_TYPE)6.0 * w) + c7 * COS_TYPE((MM_TYPE)7.0 * w);
         out[i] = w;
    }
}

static inline void vec_weighted_sum_of_cos_8(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2,
                                          MM_TYPE c3,
                                          MM_TYPE c4,
                                          MM_TYPE c5,
                                          MM_TYPE c6,
                                          MM_TYPE c7,
                                          MM_TYPE c8)
{
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = CST_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w) + c3 * COS_TYPE((MM_TYPE)3.0 * w) + c4 * COS_TYPE((MM_TYPE)4.0 * w)
             + c5 * COS_TYPE((MM_TYPE)5.0 * w) + c6 * COS_TYPE((MM_TYPE)6.0 * w) + c7 * COS_TYPE((MM_TYPE)7.0 * w)
             + c8 * COS_TYPE((MM_TYPE)8.0 * w);
         out[i] = w;
    }
}
static inline void vec_weighted_sum_of_cos_9(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2,
                                          MM_TYPE c3,
                                          MM_TYPE c4,
                                          MM_TYPE c5,
                                          MM_TYPE c6,
                                          MM_TYPE c7,
                                          MM_TYPE c8,
                                          MM_TYPE c9)
{
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = CST_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w) + c3 * COS_TYPE((MM_TYPE)3.0 * w) + c4 * COS_TYPE((MM_TYPE)4.0 * w)
             + c5 * COS_TYPE((MM_TYPE)5.0 * w) + c6 * COS_TYPE((MM_TYPE)6.0 * w) + c7 * COS_TYPE((MM_TYPE)7.0 * w)
             + c8 * COS_TYPE((MM_TYPE)8.0 * w) + c9 * COS_TYPE((MM_TYPE)9.0 * w);
         out[i] = w;
    }
}
static inline void vec_weighted_sum_of_cos_10(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2,
                                          MM_TYPE c3,
                                          MM_TYPE c4,
                                          MM_TYPE c5,
                                          MM_TYPE c6,
                                          MM_TYPE c7,
                                          MM_TYPE c8,
                                          MM_TYPE c9,
                                          MM_TYPE c10)
{
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = CST_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w) + c3 * COS_TYPE((MM_TYPE)3.0 * w) + c4 * COS_TYPE((MM_TYPE)4.0 * w)
             + c5 * COS_TYPE((MM_TYPE)5.0 * w) + c6 * COS_TYPE((MM_TYPE)6.0 * w) + c7 * COS_TYPE((MM_TYPE)7.0 * w)
             + c8 * COS_TYPE((MM_TYPE)8.0 * w) + c9 * COS_TYPE((MM_TYPE)9.0 * w) + c10 * COS_TYPE((MM_TYPE)10.0 * w);
         out[i] = w;
    }
}
#endif // __INTERNAL_WINDOW_H__

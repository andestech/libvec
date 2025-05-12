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

 /**
  * @defgroup libvec config header file
  */

  /**
   * This header file will be included for each libvec's C file
   * with while compiling. So any configuration or common
   * definition can be written here.
   */

#ifndef __INTERNAL_CONFIG_H__
#define __INTERNAL_CONFIG_H__

#ifdef  __cplusplus
extern "C"
{
#endif

#include "internal_nds_config.h"
#include "riscv_dsp_math_types.h"

//For those algorithms which have worse performance when using stride load/store instructions.
//Enable this flag for stride load/store experiments. Otherwise, disable this
//flag to use the alternative algorithm(without stride ls) to get better performance.
#ifdef ENA_WORSE_PERF_STRIDE_LS
#define EXP_WORSE_PERF_STRIDE_LS
#endif


// Option for using the polynomial algorithm from ACL
#define ENA_HIGHER_PERFORMANCE_VEXP_F32
#define ENA_HIGHER_PERFORMANCE_VLOG_F32
// Option for using the polynomial algorithm from ACL
// w/ fpu: libm
// w/o fpu : ACL
#define ENA_ACCURACY_ENHANCE_SIN
// COS using the same algorithm with SIN



// Option for exp, log, sogmoid, cos, sin f16 functions
#define ENA_EXP_F16_HIGH_ACCURACY
#define ENA_LOG_F16_HIGH_ACCURACY
#define ENA_SIGMOID_F16_HIGH_ACCURACY
#define ENA_SIN_F16_HIGH_ACCURACY
#define ENA_COS_F16_HIGH_ACCURACY

// Option for the FFT/IFFT algorithm, when this option is enabled
// FFT/IFFT will use the FID sotckham algorithm, if this option is disabled
// then FFT/IFFT will use the origina libvec algorithm.
// The option is enabled by default for better performance.
#define ENA_STOCKHAM_FFT
#ifdef ENA_STOCKHAM_FFT
// Enable Stockham fft radix 8 calculation for the first butterfly
#define STOCKHAM_FFT_RADIX8
// Enable Stockham fft radix 8 calculation for the first/middle butterfly, now only works for type f64/f32/f16
#define STOCKHAM_FFT_MAIN_RADIX8
#endif


//Define the input pointers are restricted.
#define ENA_FUNC_RESTRICT

#ifdef ENA_FUNC_RESTRICT
#define FUNC_RESTRICT       __restrict
#else
#define FUNC_RESTRICT
#endif

// Default using ENA_MAT_MUL_PACKING in rmmat_mul algorithm
#define ENA_MAT_MUL_PACKING

#ifdef  __cplusplus
}
#endif


#endif      //__INTERNAL_CONFIG_H__

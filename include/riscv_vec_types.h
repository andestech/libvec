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
#ifndef __RISCV_VEC_TYPES_H__
#define __RISCV_VEC_TYPES_H__

/** @file*/

#ifdef  __cplusplus
extern "C"
{
#endif

#undef assert
#define assert(X)
#include "riscv_dsp_math_types.h"

typedef uint8_t    u8_t;
typedef uint16_t   u16_t;
typedef uint32_t   u32_t;
typedef uint64_t   u64_t;
typedef int8_t     s8_t;
typedef int16_t    s16_t;
typedef int32_t    s32_t;
typedef int64_t    s64_t;

//Vector structures:
/**
 * @brief A structure of two float32_t values.
 */
typedef struct
{
    float32_t x;    /**< x term of riscv_vec_f32x2_t */
    float32_t y;    /**< y term of riscv_vec_f32x2_t */
} riscv_vec_f32x2_t;
/**
 * @brief A structure of three float32_t values.
 */
typedef struct
{
    float32_t x;    /**< x term of riscv_vec_f32x3_t */
    float32_t y;    /**< y term of riscv_vec_f32x3_t */
    float32_t z;    /**< z term of riscv_vec_f32x3_t */
} riscv_vec_f32x3_t;

/**
 * @brief A structure of four float32_t values.
 */
typedef struct
{
    float32_t x;    /**< x term of riscv_vec_f32x4_t */
    float32_t y;    /**< y term of riscv_vec_f32x4_t */
    float32_t z;    /**< z term of riscv_vec_f32x4_t */
    float32_t w;    /**< w term of riscv_vec_f32x4_t */
} riscv_vec_f32x4_t;

#define RISCV_VEC_OK             ((int)0)
#define RISCV_VEC_FAIL           ((int)-1)

typedef enum
{
    RISCV_VEC_SUCCESS                 =  0,
    RISCV_VEC_MALLOC_FAIL             = -1,
    RISCV_VEC_INPUT_PARAM_ERROR       = -2,
} riscv_vec_status;


#ifdef  __cplusplus
}
#endif
#endif // __NDS_TYPES_H__

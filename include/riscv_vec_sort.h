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

#ifndef __RISCV_VEC_SORT_H__
#define __RISCV_VEC_SORT_H__

#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_math_types.h"
#include <riscv_vec_types.h>

void riscv_vec_sort_descend_f32(float32_t * src, float32_t * dst, uint32_t size);
void riscv_vec_sort_descend_q31(q31_t * src, q31_t * dst, uint32_t size);
void riscv_vec_sort_descend_q15(q15_t * src, q15_t * dst, uint32_t size);
void riscv_vec_sort_descend_q7(q7_t * src, q7_t * dst, uint32_t size);

void riscv_vec_sort_ascend_f32(float32_t * src, float32_t * dst, uint32_t size);
void riscv_vec_sort_ascend_q31(q31_t * src, q31_t * dst, uint32_t size);
void riscv_vec_sort_ascend_q15(q15_t * src, q15_t * dst, uint32_t size);
void riscv_vec_sort_ascend_q7(q7_t * src, q7_t * dst, uint32_t size);

/**
 * @brief Struct for specifying sorting algorithm
 */
typedef enum
{
  RISCV_VEC_SORT_DESCENDING = 0,      /**< Descending order */
  RISCV_VEC_SORT_ASCENDING = 1        /**< Ascending order  */
} riscv_vec_sort_order;

int riscv_vec_sort_q31(q31_t * src, q31_t * dst, uint32_t size, riscv_vec_sort_order order);
int riscv_vec_sort_f32(float32_t * src, float32_t * dst, uint32_t size, riscv_vec_sort_order order);
int riscv_vec_sort_q15(q15_t * src, q15_t * dst, uint32_t size, riscv_vec_sort_order order);
int riscv_vec_sort_q7(q7_t * src, q7_t * dst, uint32_t size, riscv_vec_sort_order order);

#endif // __RISCV_VEC_SORT_H__

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

#include "internal_nds_types.h"
#include "riscv_vec_filtering.h"

/* function description */
void riscv_vec_conv_q15(q15_t * FUNC_RESTRICT src1, uint32_t len1, q15_t * FUNC_RESTRICT src2, uint32_t len2, q15_t * FUNC_RESTRICT dst)
{
    uint32_t output_size = len1 + len2 - 1;
    riscv_vec_conv_partial_q15((q15_t *) src1, len1, (q15_t *) src2, len2, (q15_t *) dst, 0, output_size);
}

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

#define ENA_F32_PARAMETER
#include "internal_window.h"

/**
  @par Parameters of the window
  | Parameter                             | Value              |
  | ------------------------------------: | -----------------: |
  | Peak sidelobe level                   |           98.1 dB  |
  | Normalized equivalent noise bandwidth |       1.9761 bins  |
  | 3 dB bandwidth                        |       1.8687 bins  |
  | Flatness                              |        -0.8506 dB  |
  | Recommended overlap                   |            65.6 %  |
  | ------------------------------------: | -----------------: |
 */

/* function description */
void riscv_vec_win_nuttall4c_f32(float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    vec_weighted_sum_of_cos_3(dst, size, 0.3635819f, -0.4891775f, 0.1365995f, -0.0106411f);
}

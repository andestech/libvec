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

#define ENA_PUREC

#define ENA_F64_PARAMETER
#include "internal_window.h"

/**
  @par Parameters of the window
  | Parameter                             | Value              |
  | ------------------------------------: | -----------------: |
  | Peak sidelobe level                   |           95.0 dB  |
  | Normalized equivalent noise bandwidth |       3.8112 bins  |
  | 3 dB bandwidth                        |       3.7590 bins  |
  | Flatness                              |         0.0044 dB  |
  | Recommended overlap                   |            75.6 %  |
  | ------------------------------------: | -----------------: |
 */

/* function description */
void riscv_vec_win_hft95_f64(float64_t * FUNC_RESTRICT dst, uint32_t size)
{
    vec_weighted_sum_of_cos_4(dst, size, 1.0, -1.9383379, 1.3045202, -0.4028270, 0.0350665);
}

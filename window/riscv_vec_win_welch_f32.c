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

/**
  @par Parameters of the window
  | Parameter                             | Value              |
  | ------------------------------------: | -----------------: |
  | Peak sidelobe level                   |           21.3 dB  |
  | Normalized equivalent noise bandwidth |          1.2 bins  |
  | 3 dB bandwidth                        |       1.1535 bins  |
  | Flatness                              |        -2.2248 dB  |
  | Recommended overlap                   |            29.3 %  |
 */

/* function description */
void riscv_vec_win_welch_f32(float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    float32_t k = 2.0f / ((float32_t) size);
    float32_t w;

    for(uint32_t i = 0; i < size; i++)
    {
        w = i * k - 1.0f;
        w = 1.0f - w * w;
        dst[i] = w;
    }
}

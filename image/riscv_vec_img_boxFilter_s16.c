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
#include <math.h>
#include "riscv_vec_image.h"

void riscv_vec_img_boxFilter_s16(riscv_vec_img_s16_t img_src,
                                 riscv_vec_img_s16_t img_dst,
                                 riscv_vec_point_t ksize,
                                 riscv_vec_point_t anchor,
                                 uint8_t normalized,
                                 riscv_vec_img_border type)
{
    riscv_vec_img_f32_t img_kernel;
    img_kernel.channels = 1;
    img_kernel.width = ksize.x;
    img_kernel.height = ksize.y;

    // for OpenCV v4.8.x, the anchor can pass to boxFilter, 
    // but in kernel, only default case will be used.
    anchor.x = -1;
    anchor.y = -1;
    float kernel_data[ksize.x * ksize.y];
    float kernel_val = 1.0f;
    if(normalized)
    {
        kernel_val = (float)1.0/(float)(ksize.x*ksize.y); 
    }

    for(int32_t tmp_i = 0 ; tmp_i < (uint32_t)ksize.x*ksize.y ; tmp_i++)
    {
        kernel_data[tmp_i] = kernel_val ;
    }
    img_kernel.data = kernel_data;

    riscv_vec_img_filter2D_s16_f32_s16(img_src, img_kernel, img_dst, anchor, 0.0, type);
    return;
}

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
#include "internal_vec_image_gaussianBlur.h"

void riscv_vec_img_gaussianBlur_f32(riscv_vec_img_f32_t img_src,
                                    riscv_vec_img_f32_t img_dst,
                                    riscv_vec_point_t ksize,
                                    double sigmaX,
                                    double sigmaY,
                                    riscv_vec_img_border type)
{
    riscv_vec_img_f32_t img_kernel;
    img_kernel.channels = 1;
    img_kernel.width = ksize.x;
    img_kernel.height = ksize.y;

    float kernel_data[ksize.x * ksize.y];
    vec_generateGaussianKernel(kernel_data, ksize, sigmaX, sigmaY);
    img_kernel.data = kernel_data;

    riscv_vec_point_t anchor;
    anchor.x = -1, anchor.y = -1;
    riscv_vec_img_filter2D_f32(img_src, img_kernel, img_dst, anchor, 0.0, type);
    return;
}

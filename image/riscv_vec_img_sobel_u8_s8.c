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

#include "riscv_vec_image.h"

#include "internal_nds_types.h"
#include <math.h>

void riscv_vec_img_sobel_u8_s8(riscv_vec_img_u8_t img_src,
                               riscv_vec_img_s8_t img_dst,
                               int dx,
                               int dy,
                               int kernel_size,
                               double scale,
                               double delta,
                               riscv_vec_img_border type)
{
    riscv_vec_point_t anchor;
    anchor.x = kernel_size / 2;
    anchor.y = anchor.x;
#define SRC_TYPE u8_t
#define KERNEL_TYPE s8_t
#define DST_TYPE s8_t
#define TEMP_TYPE int32_t

    float scalef = scale;
    float deltaf = delta;
#define SAT_CAST_SCALE_DELTA(input) \
        ({\
        input = input * scalef + deltaf; \
        ( (DST_TYPE)( (input < -128) ? -128 : (input > 127) ? 127 : input ) ); \
        })
#include "internal_vec_image_sobel_template_purec.h"

    s8_t sobel_kernel = 1;
    riscv_vec_img_s8_t img_kernel;
    img_kernel.channels = 1;
    img_kernel.height = kernel_size;
    img_kernel.width = kernel_size;
    img_kernel.data = &sobel_kernel;

    if(kernel_size == 1 || kernel_size == 3 || kernel_size == 5 || kernel_size == 7)
    {
        if(kernel_size == 3)
        {
            if(dx && dy)
                img_kernel.data = sobel_kernel3x3_xy;
            else if(dx)
                img_kernel.data = sobel_kernel3x3_x;
            else if(dy)
                img_kernel.data = sobel_kernel3x3_y;
        }
        else if(kernel_size == 5)
        {
            if(dx && dy)
                img_kernel.data = sobel_kernel5x5_xy;
            else if(dx)
                img_kernel.data = sobel_kernel5x5_x;
            else if(dy)
                img_kernel.data = sobel_kernel5x5_y;
        }
        else if(kernel_size == 7)
        {
            if(dx && dy)
                img_kernel.data = sobel_kernel7x7_xy;
            else if(dx)
                img_kernel.data = sobel_kernel7x7_x;
            else if(dy)
                img_kernel.data = sobel_kernel7x7_y;
        }

        SOBEL_INIT;
        SOBEL_BORDER_L_C;
        SOBEL_BORDER_R_C;
        SOBEL_BORDER_T_C;
        SOBEL_BORDER_B_C;
        SOBEL_MIDDLE_C;
    }

    return;

}

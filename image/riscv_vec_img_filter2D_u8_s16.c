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
//#include <string.h>
#include <math.h>
#include "riscv_vec_image.h"


void riscv_vec_img_filter2D_u8_s16(riscv_vec_img_u8_t img_src,
                        riscv_vec_img_s16_t img_kernel,
                        riscv_vec_img_s16_t img_dst,
                        riscv_vec_point_t anchor,
                        double delta,
                        riscv_vec_img_border type)
{
    delta = round(delta);
    anchor.x = (anchor.x == -1) ? img_kernel.width / 2 : anchor.x;
    anchor.y = (anchor.y == -1) ? img_kernel.height / 2 : anchor.y;
    #define SRC_TYPE u8_t
    #define KERNEL_TYPE s16_t
    #define TEMP_TYPE int32_t
    #define DST_TYPE s16_t
    #define SAT_CAST(input) ( (DST_TYPE)( (input < -32768) ? -32768 : (input > 32767) ? 32767 : input ) )

    #include "internal_vec_image_filter2D_template_pureC.h"

    FILTER2D_INIT;
    FILTER2D_BORDER_L_C;
    FILTER2D_BORDER_R_C;
    FILTER2D_BORDER_T_C;
    FILTER2D_BORDER_B_C;
    FILTER2D_MIDDLE_C;

    return;
}
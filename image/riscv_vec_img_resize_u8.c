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
#include "riscv_vec_image.h"
#include <string.h>

#ifdef ENA_HVM
#include "internal_hvm_ctrl.h"
#endif

#include "internal_vec_image_resize_template_purec_u8.h"

riscv_vec_status riscv_vec_img_resize_u8(
    riscv_vec_img_u8_t img_src,
    riscv_vec_img_u8_t img_dst,
    riscv_vec_img_interp_mode interp_mode)
{
    if(img_src.channels != img_dst.channels)
    {
        return RISCV_VEC_INPUT_PARAM_ERROR;
    }

    if (img_src.height == img_dst.height && img_src.width == img_dst.width)
    {
        // Source and destination are of same size. Use simple copy.
        memcpy(img_dst.data, img_src.data, img_src.width * img_src.height * img_src.channels * sizeof(u8_t));
        return RISCV_VEC_SUCCESS;
    }

    switch(interp_mode)
    {
        case RISCV_VEC_INTER_NEAREST:
        {
            vec_img_resize_nearest_u8(img_dst.data, img_dst.width, img_dst.height,
                                      img_src.data, img_src.width, img_src.height, img_src.width * img_src.channels, img_src.channels);
            break;
        };
        case RISCV_VEC_INTER_LINEAR:
        {
            vec_img_resize_bilinear_u8(img_dst.data, img_dst.width, img_dst.height,
                                       img_src.data, img_src.width, img_src.height, img_src.width * img_src.channels, img_src.channels);
            break;
        }
        case RISCV_VEC_INTER_CUBIC:
        {
            vec_img_resize_bicubic_u8(img_dst.data, img_dst.width, img_dst.height,
                                      img_src.data, img_src.width, img_src.height, img_src.width * img_src.channels, img_src.channels);
            break;
        }
        case RISCV_VEC_INTER_NEAREST_EXACT:
        {
            vec_img_resize_nearest_exact_u8(img_dst.data, img_dst.width, img_dst.height,
                                            img_src.data, img_src.width, img_src.height, img_src.width * img_src.channels, img_src.channels);
            break;
        }
        default:
        {
            // error case
            return NDSV_FAIL;
        }
    }
    return RISCV_VEC_SUCCESS;
}

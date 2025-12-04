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
#include  <float.h>
#include "riscv_vec_image.h"

#define SRC_TYPE uint8_t
#define KERNEL_TYPE float32_t
#define DST_TYPE uint8_t

#include "internal_vec_image_bilateral_template_purec_u8.h"

#ifdef ENA_HVM
#include "internal_hvm_ctrl.h"
#endif

riscv_vec_status riscv_vec_img_bilateral_u8(riscv_vec_img_u8_t img_src,
        riscv_vec_img_u8_t img_dst,
        int32_t d,
        double sigma_color,
        double sigma_space,
        riscv_vec_img_border type)
{
    uint32_t cn = img_src.channels;
    int32_t i, j, k, maxk, radius;
    const int exp_bin_per_cn = 256;
    int32_t exp_bins = exp_bin_per_cn * cn;
    KERNEL_TYPE *p_sp_tbl = NULL;
    KERNEL_TYPE *p_color_tbl = NULL;
    int32_t   *p_sp_off   = NULL; // for middle
    int32_t   *p_sp_off_h = NULL; // for border
    int32_t   *p_sp_off_w = NULL; // for border
    KERNEL_TYPE *p_sum_w = NULL;
    KERNEL_TYPE *p_sum = NULL;

    if( sigma_color <= 0 )
    {
        sigma_color = 1;
    }
    if( sigma_space <= 0 )
    {
        sigma_space = 1;
    }
    double gauss_color_coeff = -0.5 / (sigma_color * sigma_color);
    double gauss_space_coeff = -0.5 / (sigma_space * sigma_space);

    {
        radius = d / 2;
    }
    if(radius >= BILATERAL_MIN(img_src.height, img_src.width))
    {
        // current algorithm can not handle these case.
        return RISCV_VEC_INPUT_PARAM_ERROR;
    }
    radius = BILATERAL_MAX(radius, 1);

    d = radius * 2 + 1;

    p_sp_tbl = (KERNEL_TYPE*) NDSV_MALLOC( d * d * sizeof (KERNEL_TYPE));
    p_sp_off = (int32_t *) NDSV_MALLOC( d * d * sizeof (int32_t));
    p_sp_off_h = (int32_t *) NDSV_MALLOC( d * d * sizeof (int32_t));
    p_sp_off_w = (int32_t *) NDSV_MALLOC( d * d * sizeof (int32_t));
    p_sum_w  = (KERNEL_TYPE *) NDSV_MALLOC( img_src.width * sizeof (KERNEL_TYPE));
    p_sum    = (KERNEL_TYPE *) NDSV_MALLOC( img_src.width * cn * sizeof (KERNEL_TYPE));
    p_color_tbl = (KERNEL_TYPE *) NDSV_MALLOC((exp_bins) * sizeof (KERNEL_TYPE));

    if( (p_sp_tbl == NULL) || (p_sp_off == NULL) || (p_sp_off_h == NULL) || (p_sp_off_w == NULL) || (p_sum_w == NULL) || (p_sum == NULL) || (p_color_tbl == NULL) )
    {
        NDSV_FREE(p_sp_tbl);
        NDSV_FREE(p_sp_off);
        NDSV_FREE(p_sp_off_h);
        NDSV_FREE(p_sp_off_w);
        NDSV_FREE(p_sum_w);
        NDSV_FREE(p_sum);
        NDSV_FREE(p_color_tbl);
        return RISCV_VEC_MALLOC_FAIL;
    }

    for(i = 0; i < exp_bins ; i++)
    {
        KERNEL_TYPE tmp_i = (KERNEL_TYPE)i;
        p_color_tbl[i] = expf(tmp_i * tmp_i * gauss_color_coeff);
    }

    // init space table
    maxk = 0;
    for( i = -radius, maxk = 0; i <= radius; i++ )
    {
        for( j = -radius; j <= radius; j++ )
        {
            float r = sqrtf((float)(i * i + j * j));
            if( r > radius )
            {
                continue;
            }
            p_sp_tbl[maxk] = expf(r * r * gauss_space_coeff);
            p_sp_off_h[maxk] = i;
            p_sp_off_w[maxk] = j;
            p_sp_off[maxk++] = i * (img_src.width) * cn + j * cn; // offset pixel index
        }
    }

    // filter operator

    /*
        According to the radius, the src image is divided into 5 parts:
        1. border left
        2. border right
        3. border top
        4. border down
        5. middle stage
        the data of border is config by the input "type"

               src_width
        ------------------------
        |                      |
        |         3            |  radius
        |                      |
        ------------------------
        |   |              |   |
        | 1 |     5        | 2 |   src_height
        |   |              |   |
        |   |              |   |
        ------------------------
        |radius                |
        |                      |
        |         4            |
        ------------------------


    */

    BILATERAL_INIT_C;
    if(cn == 1)
    {
        BILATERAL_BORDER_L_CN1_C;
        BILATERAL_BORDER_R_CN1_C;
        BILATERAL_BORDER_T_CN1_C;
        BILATERAL_BORDER_D_CN1_C;
        BILATERAL_MIDDLE_CN1_C;
    }
    else //(cn==3)
    {
        BILATERAL_BORDER_L_CN3_C;
        BILATERAL_BORDER_R_CN3_C;
        BILATERAL_BORDER_T_CN3_C;
        BILATERAL_BORDER_D_CN3_C;
        BILATERAL_MIDDLE_CN3_C;
    }
    NDSV_FREE(p_sp_tbl);
    NDSV_FREE(p_sp_off);
    NDSV_FREE(p_sp_off_h);
    NDSV_FREE(p_sp_off_w);
    NDSV_FREE(p_sum_w);
    NDSV_FREE(p_sum);
    NDSV_FREE(p_color_tbl);

    return RISCV_VEC_SUCCESS;
}

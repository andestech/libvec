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
#include "stdio.h"
#include "internal_nds_types.h"
#include <math.h>
#include  <float.h>
#include "riscv_vec_image.h"

// for max / min value
#include "riscv_vec_statistics.h"


#define SRC_TYPE f32_t
#define TEMP_TYPE f32_t
#define KERNEL_TYPE f32_t
#define DST_TYPE f32_t

#include "internal_vec_image_bilateral_template_purec_f32.h"

#ifdef ENA_HVM
#include "internal_hvm_ctrl.h"
#endif


riscv_vec_status riscv_vec_img_bilateral_f32(riscv_vec_img_f32_t img_src,
        riscv_vec_img_f32_t img_dst,
        int32_t d,
        double sigma_color,
        double sigma_space,
        riscv_vec_img_border type)
{
    uint32_t cn = img_src.channels;
    int32_t i, j, k, maxk, radius, data_len;
    float min_src = -1, max_src = 1;
    const int exp_bin_per_cn = 1 << 12;
    int32_t exp_bins = exp_bin_per_cn * cn;
    float last_exp_val = 1.f;
    float len, scale_index;
    uint32_t out_min_index, out_max_index;
    float32_t *p_in = img_src.data;
    float32_t *p_out = img_dst.data;
    float32_t *p_sp_tbl = NULL;
    float32_t *p_color_tbl = NULL;
    int32_t   *p_sp_off   = NULL; // for middle
    int32_t   *p_sp_off_h = NULL; // for border
    int32_t   *p_sp_off_w = NULL; // for border
    float32_t *p_sum_w = NULL;

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

    data_len = img_src.height * img_src.width * img_src.channels;
    min_src = riscv_vec_min_f32(p_in, data_len, &out_min_index);
    max_src = riscv_vec_max_f32(p_in, data_len, &out_max_index);
    if( fabsf(max_src - min_src) < FLT_EPSILON)
    {
        // input a const image, output = input
        for(i = 0; i < data_len ; i++)
        {
            p_out[i] = p_in[i];
        }
        return RISCV_VEC_SUCCESS;
    }
    p_sp_tbl = (float32_t *) NDSV_MALLOC( d * d * sizeof (float32_t));
    p_sp_off = (int32_t *) NDSV_MALLOC( d * d * sizeof (int32_t));
    p_sp_off_h = (int32_t *) NDSV_MALLOC( d * d * sizeof (int32_t));
    p_sp_off_w = (int32_t *) NDSV_MALLOC( d * d * sizeof (int32_t));
    p_sum_w  = (float32_t *) NDSV_MALLOC( img_src.width * sizeof (float32_t));
    p_color_tbl = (float32_t *) NDSV_MALLOC((exp_bins + 2) * sizeof (float32_t));

    if( (p_sp_tbl == NULL) || (p_sp_off == NULL) || (p_sp_off_h == NULL) || (p_sp_off_w == NULL) || (p_sum_w == NULL) || (p_color_tbl == NULL) )
    {
        NDSV_FREE(p_sp_tbl);
        NDSV_FREE(p_sp_off);
        NDSV_FREE(p_sp_off_h);
        NDSV_FREE(p_sp_off_w);
        NDSV_FREE(p_sum_w);
        NDSV_FREE(p_color_tbl);
        return RISCV_VEC_MALLOC_FAIL;
    }

    len = (max_src - min_src) * cn;
    scale_index = (float32_t)exp_bins / len;
    for(i = 0; i < exp_bins + 2 ; i++)
    {
        if( last_exp_val > 0.f )
        {
            double val = (float)i / scale_index;
            p_color_tbl[i] = expf(val * val * gauss_color_coeff);
            last_exp_val = p_color_tbl[i];
        }
        else
        {
            p_color_tbl[i] = 0.0f;
        }
    }

    // init space table
    maxk = 0;
    for( i = -radius, maxk = 0; i <= radius; i++ )
    {
        for( j = -radius; j <= radius; j++ )
        {
            float r = sqrtf((float)(i * i + j * j));
            if( r > radius || ( i == 0 && j == 0 ) )
            {
                continue;
            }
            p_sp_tbl[maxk] = expf(r * r * gauss_space_coeff);
            p_sp_off_h[maxk] = i;
            p_sp_off_w[maxk] = j;
            p_sp_off[maxk++] = i * (img_src.width) * cn + j * cn; // offset pixel index

        }
    }

    for( i = 0 ; i <  img_src.height * img_src.width * cn ; i++)
    {
        img_dst.data[i] = 0.0f;
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
    NDSV_FREE(p_color_tbl);

    return RISCV_VEC_SUCCESS;
}

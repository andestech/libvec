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

#define SRC_TYPE u16_t
#define DST_TYPE u16_t

#include "internal_vec_image_medianblur_template_purec_u16.h"

#ifdef ENA_HVM
#include "internal_hvm_ctrl.h"
#endif


riscv_vec_status riscv_vec_img_medianBlur_u16(riscv_vec_img_u16_t img_src,
        riscv_vec_img_u16_t img_dst,
        uint32_t ksize)
{
    int32_t i = 0, j = 0, kw = 0, kh = 0 ;
    int32_t k = 0;
    int32_t radius = 0 ;
    DST_TYPE *TMP = NULL;
    int32_t m_val = 0 ;
    if(ksize > 5 || ((ksize % 2) != 1))
    {
        // u16 only support the case ksize = 3 or 5.
        // ksize must be an odd number
        return RISCV_VEC_INPUT_PARAM_ERROR;
    }
    if( (ksize >= img_src.height) || (ksize >= img_src.width) )
    {
        return RISCV_VEC_INPUT_PARAM_ERROR;
    }
    TMP = (DST_TYPE*)NDSV_MALLOC(25 * sizeof(DST_TYPE));
    if(TMP == NULL)
    {
        NDSV_FREE(TMP);
        return RISCV_VEC_MALLOC_FAIL;
    }

    radius = (int32_t)ksize / 2 ; 
    m_val = (int32_t)(ksize*ksize >> 1) ;
    // filter operator

    /*
        According to the radius, the src image is divided into 3 parts:
        1. border left
        2. border right
        3. middle stage
        the data of bordertype is for BORDER_REPLICATE only

               src_width
        ------------------------
        |   |              |   |
        |   |              |   |  
        |   |              |   |
        |   |              |   |
        | 1 |      3       | 2 |   src_height
        |   |              |   |
        |   |              |   |
        |rad|ius           |   |
        |   |              |   |
        |   |              |   |
        ------------------------


    */

    MEDIANBLUR_INIT_C;
    MEDIANBLUR_S_K_BORDER_L_C;
    MEDIANBLUR_S_K_BORDER_R_C;
    MEDIANBLUR_S_K_MIDDLE_C;
    NDSV_FREE(TMP);

    return RISCV_VEC_SUCCESS;
}

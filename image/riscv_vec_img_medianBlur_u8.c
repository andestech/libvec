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

#define SRC_TYPE u8_t
#define DST_TYPE u8_t

#include "internal_vec_image_medianblur_template_purec_u8.h"

#ifdef ENA_HVM
#include "internal_hvm_ctrl.h"
#endif


riscv_vec_status riscv_vec_img_medianBlur_u8(riscv_vec_img_u8_t img_src,
        riscv_vec_img_u8_t img_dst,
        uint32_t ksize)
{
    int32_t i = 0, j = 0, kw = 0, kh = 0 ;
    int32_t k = 0;
    int32_t radius = 0 ;
    //DST_TYPE TMP[25] = {0};
    DST_TYPE *TMP = NULL;
    int32_t m_val = 0 ;
    uint32_t *p_zone0 = NULL;
    uint32_t *p_zone1 = NULL; 
    if(((ksize % 2) != 1) || (ksize==1))
    {
        // ksize must be an odd number and >1
        return RISCV_VEC_INPUT_PARAM_ERROR;
    }
    if( (ksize >= img_src.height) || (ksize >= img_src.width) )
    {
        return RISCV_VEC_INPUT_PARAM_ERROR;
    }

    TMP = (DST_TYPE*) NDSV_MALLOC(25* sizeof(DST_TYPE));
    if( TMP==NULL )
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
    if( (ksize == 3) || (ksize == 5) )
    {
        MEDIANBLUR_S_K_BORDER_L_C;
        MEDIANBLUR_S_K_BORDER_R_C;
        MEDIANBLUR_S_K_MIDDLE_C;
    }
    else
    {
        p_zone0 = (uint32_t*)NDSV_MALLOC(16 * sizeof(uint32_t));
        p_zone1 = (uint32_t*)NDSV_MALLOC(256 * sizeof(uint32_t));
        if( (p_zone0 == NULL) || (p_zone1 == NULL) )
        {
            NDSV_FREE(p_zone0);
            NDSV_FREE(p_zone1);
            return RISCV_VEC_MALLOC_FAIL;
        }
        m_val = m_val +1 ; 
        MEDIANBLUR_L_K_BORDER_R_C;
        MEDIANBLUR_L_K_BORDER_L_C;
        MEDIANBLUR_L_K_MIDDLE_C ;
        NDSV_FREE(p_zone0);
        NDSV_FREE(p_zone1);
    }
    NDSV_FREE(TMP);
    NDSV_FREE(p_zone0);
    NDSV_FREE(p_zone1);
    return RISCV_VEC_SUCCESS;
}

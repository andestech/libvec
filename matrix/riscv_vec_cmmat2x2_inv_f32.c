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
#include "internal_vec_matrix.h"

int riscv_vec_cmmat2x2_inv_f32(riscv_vec_cmmat2x2_f32_t * dst, riscv_vec_cmmat2x2_f32_t * src, uint32_t count)
{
    float32_t det = 0.0f;
    NDSV_CHECK_DstSrc1Src2;

    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        // float32_t out = ((mat->c1.x * mat->c2.y) - (mat->c2.x * mat->c1.y));
        det = vec_cmmat2x2_det_f32(&src[ itr ]);

        if (1 == IS_FLOAT_NEAR_ZERO (det))
        {
            det = 1.0f;
        }
        det = 1.0f / det;
        dst[ itr ].c1.x =         det * src[ itr ].c2.y;
        dst[ itr ].c1.y = -1.0f * det * src[ itr ].c1.y;

        dst[ itr ].c2.x = -1.0f * det * src[ itr ].c2.x;
        dst[ itr ].c2.y =         det * src[ itr ].c1.x;
    }
    return NDSV_OK;

}

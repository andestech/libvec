/******************************************************************************
 * Copyright (C) 2010-2023 Arm Limited or its affiliates. All rights reserved.*
 * Copyright (C) 2018-2023 Andes Technology Corporation. All rights reserved. *
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

int riscv_vec_cmmat2x2_add_f32(riscv_vec_cmmat2x2_f32_t * dst, riscv_vec_cmmat2x2_f32_t * src1, riscv_vec_cmmat2x2_f32_t * src2, uint32_t count)
{
    NDSV_CHECK_DstSrc1Src2;
    uint32_t itr;
    for ( itr = 0; itr < count; itr++ )
    {
        dst[ itr ].c1.x = src1[ itr ].c1.x + src2[ itr ].c1.x;
        dst[ itr ].c1.y = src1[ itr ].c1.y + src2[ itr ].c1.y;

        dst[ itr ].c2.x = src1[ itr ].c2.x + src2[ itr ].c2.x;
        dst[ itr ].c2.y = src1[ itr ].c2.y + src2[ itr ].c2.y;
    }
    return NDSV_OK;
}

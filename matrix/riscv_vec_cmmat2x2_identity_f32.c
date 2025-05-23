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

int riscv_vec_cmmat2x2_identity_f32(riscv_vec_cmmat2x2_f32_t * dst, uint32_t count)
{
    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        dst[ itr ].c1.x =  1.0f;
        dst[ itr ].c1.y =  0.0f;

        dst[ itr ].c2.x =  0.0f;
        dst[ itr ].c2.y =  1.0f;
    }
    return NDSV_OK;
}

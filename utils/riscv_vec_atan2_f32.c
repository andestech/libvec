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

#include "internal_vec_utils.h"

#define PI          3.14159265358979f
#define HALF_PI     1.57079632679490f

/* function description */
void riscv_vec_atan2_f32(float32_t * FUNC_RESTRICT srcy, float32_t * FUNC_RESTRICT srcx, float32_t * FUNC_RESTRICT dst, uint32_t size)
{

    //second-order approximation
    for(int i = 0 ; i < size ; i++)
    {
        if ( srcx[i] == 0.0f )
        {
            if ( srcy[i] > 0.0f )
                dst[i] = HALF_PI;
            else if ( srcy[i] == 0.0f )
                dst[i] = 0.0f;
            else
                dst[i] = -HALF_PI;
        }
        else
        {
            float32_t atan;
            float32_t z = srcy[i] / srcx[i];

            if ( fabsf( z ) < 1.0f )
            {
                atan = z / (1.0f + 0.28f * z * z);
                if ( srcx[i] < 0.0f )
                {
                    if ( srcy[i] < 0.0f )
                        atan = atan - PI;
                    else
                        atan = atan + PI;
                }
            }
            else
            {
                atan = HALF_PI - z / (z * z + 0.28f);
                if ( srcy[i] < 0.0f )
                    atan = atan - PI;
            }
            dst[i] = atan;
        }
    }
    return;
}

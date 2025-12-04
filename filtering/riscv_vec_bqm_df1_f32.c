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

#include <string.h> //memset
#include "riscv_vec_filtering.h"
#include "internal_nds_types.h"

/* function description */
void riscv_vec_bqm_df1_f32(const riscv_vec_bqm_df1_f32_t * FUNC_RESTRICT instance,
                           float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    float32_t *in = src;
    float32_t *out = dst;
    float32_t *state = instance->state;
    float32_t *coeff = instance->coeff;
    uint32_t ch = instance->nchannel;
    uint32_t stage = instance->nstage;
    uint32_t i;

    float32_t acc;
    float32_t b0, b1, b2, a1, a2;
    float32_t xn, xn1, xn2, yn1, yn2;

    for(int t = 0 ; t < ch ; t++)
    {
        float32_t *in_t = in + t;
        float32_t *out_t = out + t;

        float32_t *coeffTmp = coeff + t;
        float32_t *stateTmp = state + t;

        for(int s = 0 ; s < stage ; s++)
        {
            b0 = *coeffTmp; coeffTmp+=ch;
            b1 = *coeffTmp; coeffTmp+=ch;
            b2 = *coeffTmp; coeffTmp+=ch;
            a1 = *coeffTmp; coeffTmp+=ch;
            a2 = *coeffTmp; coeffTmp+=ch;

            xn1 = stateTmp[0];
            xn2 = stateTmp[ch];
            yn1 = stateTmp[ch*2];
            yn2 = stateTmp[ch*3];

            /* for one stage */
            i = size;
            while (i != 0u)
            {
                xn = *in_t;
                acc = (b0 * xn) + (b1 * xn1) + (b2 * xn2) + (a1 * yn1) + (a2 * yn2);
                xn2 = xn1;
                xn1 = xn;
                yn2 = yn1;
                yn1 = acc;
                *out_t = acc;

                in_t+=ch;
                out_t+=ch;
                i--;
            }

            *stateTmp = xn1; stateTmp+=ch;
            *stateTmp = xn2; stateTmp+=ch;
            *stateTmp = yn1; stateTmp+=ch;
            *stateTmp = yn2; stateTmp+=ch;

            in_t = dst + t;
            out_t = dst + t;
        }
    }
}

void riscv_vec_bqm_df1_init_f32(riscv_vec_bqm_df1_f32_t *instance, uint32_t nstage, float32_t *state, float32_t *coeff, uint32_t nch)
{
    const uint32_t state_memneeded = 4 * nstage * nch;
    const uint32_t coeff_memneeded = 5 * nstage * nch;

    instance->state = state; //(float32_t *) NDSV_MALLOC (state_memneeded * sizeof(float32_t));
    memset(instance->state, 0, state_memneeded * sizeof(float32_t));

    instance->coeff = (float32_t *) NDSV_MALLOC (coeff_memneeded * sizeof(float32_t));

    instance->nstage = nstage;
    instance->nchannel = nch;

    // rearrange coeff
    // stage = 2, batch = 2
    // intput           stage0                stage1
    // batch0  {{ b1,  b2,  b3,  b4,  b5}, { b6,  b7,  b8,  b9, b10}}
    // batch1  {{b11, b12, b13, b14, b15}, {b16, b17, b18, b19, b20}}
    // output  b1 b11 b2 b12 b3 b13 b4 b14 b5 b15 b6 b16 b7 b17 b8 b18 b9 b19 b10 b20

    float32_t *coeff_tmp = instance->coeff;
    for(int i = 0 ; i < 5 * nstage; i++)
    {
        for(int b = 0 ; b < nch ; b++)
        {
            *coeff_tmp++ = *(coeff + (b * 5 * nstage) + i);
        }
    }

    return;
}

void riscv_vec_bqm_df1_free_f32(riscv_vec_bqm_df1_f32_t *instance)
{
    // NDSV_FREE(instance->state);
    NDSV_FREE(instance->coeff);
    return;
}

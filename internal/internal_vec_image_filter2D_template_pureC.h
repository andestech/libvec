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

#define MAX(a, b) ((a < b) ? b : a)
#define MIN(a, b) ((a < b) ? a : b)

#define FILTER2D_INIT \
    SRC_TYPE * src = img_src.data;\
    KERNEL_TYPE * kernel = img_kernel.data;\
    DST_TYPE * dst = img_dst.data;\
    int channel = img_src.channels;\
    int ker_w = img_kernel.width;\
    int ker_h = img_kernel.height;\
    int src_w = img_src.width;\
    int src_h = img_src.height;\
    int anchor_x = anchor.x, anchor_y = anchor.y;\
    TEMP_TYPE _delta = SAT_CAST(delta);\
    src_w *= channel;\
    src_h *= src_w;\
    int j, i;\
    int s_j, s_i, e_j, e_i;\
\
    s_j = anchor_y * src_w;\
    e_j = src_h - (ker_h-1 - anchor_y) * src_w;\
    s_i = anchor_x * channel;\
    e_i = src_w - (ker_w-1 - anchor_x) * channel;

#define FILTER2D_BORDER_L_C \
    for( i = 0 ; i < s_i ; i+=channel)\
    {\
        for( j = 0 ; j < src_h ; j += src_w)\
        {\
            int y = j - anchor_y * src_w;\
            int x = i - anchor_x * channel;\
            for(int cn = 0 ; cn < channel ; cn++)\
            {\
                TEMP_TYPE tmp = _delta;\
                for(int kh = 0, ty = y ; kh < ker_h ; kh++, ty+=src_w)\
                {\
                    for(int kw = 0, tx = x ; kw < ker_w ; kw++, tx+=channel)\
                    {\
                        TEMP_TYPE k_parameter = kernel[(kh  * ker_w) + kw];\
                        if(type == RISCV_VEC_IMG_BORDER_CONSTANT)\
                        {\
                            if(!(ty < 0 || ty >= src_h || tx < 0))\
                            {\
                                tmp += k_parameter * src[ty + tx + cn];\
                            }\
                        }\
                        else if(type == RISCV_VEC_IMG_BORDER_REFLECT101)\
                        {\
                            int ttx = MAX(tx, -tx);\
                            if(ty >= 0 && ty < src_h)\
                            {\
                                tmp += k_parameter * src[ty + ttx + cn];\
                            }\
                            else if(ty < 0)\
                            {\
                                tmp += k_parameter * src[-ty + ttx + cn];\
                            }\
                            else if(ty >= src_h)\
                            {\
                                int tty = (src_h-src_w) - (ty - (src_h-src_w));\
                                tmp += k_parameter * src[tty + ttx + cn];\
                            }\
                        }\
                    }\
                }\
                dst[j + i + cn] = SAT_CAST(tmp);\
            }\
        }\
    }

#define FILTER2D_BORDER_R_C \
    for( i = e_i ; i < src_w ; i+=channel)\
    {\
        for( j = 0 ; j < src_h ; j += src_w)\
        {\
            int y = j - anchor_y * src_w;\
            int x = i - anchor_x * channel;\
            for(int cn = 0 ; cn < channel ; cn++)\
            {\
                TEMP_TYPE tmp = _delta;\
                for(int kh = 0, ty = y ; kh < ker_h ; kh++, ty+=src_w)\
                {\
                    for(int kw = 0, tx = x ; kw < ker_w ; kw++, tx+=channel)\
                    {\
                        TEMP_TYPE k_parameter = kernel[(kh  * ker_w) + kw];\
    \
                        if(type == RISCV_VEC_IMG_BORDER_CONSTANT)\
                        {\
                            if(!(ty < 0 || ty >= src_h || tx >= src_w))\
                            {\
                                tmp += k_parameter * src[ty + tx + cn];\
                            }\
                        }\
                        else if(type == RISCV_VEC_IMG_BORDER_REFLECT101)\
                        {\
                            int ttx = MIN(tx, (src_w-channel) - (tx - (src_w-channel)));\
                            if(ty >= 0 && ty < src_h)\
                            {\
                                tmp += k_parameter * src[ty + ttx + cn];\
                            }\
                            else if(ty < 0)\
                            {\
                                tmp += k_parameter * src[-ty + ttx + cn];\
                            }\
                            else if(ty >= src_h)\
                            {\
                                int tty = (src_h-src_w) - (ty - (src_h-src_w));\
                                tmp += k_parameter * src[tty + ttx + cn];\
                            }\
                        }\
                    }\
                }\
                dst[j + i + cn] = SAT_CAST(tmp);\
            }\
        }\
    }

#define FILTER2D_BORDER_T_C \
    for( j = 0 ; j < s_j ; j += src_w)\
    {\
        for( i = s_i ; i < e_i ; i++)\
        {\
            int y = j - anchor_y * src_w;\
            int x = i - anchor_x * channel;\
            TEMP_TYPE tmp = _delta;\
            for(int kh = 0, ty = y ; kh < ker_h ; kh++, ty+=src_w)\
            {\
                for(int kw = 0, tx = x; kw < ker_w ; kw++, tx+=channel)\
                {\
                    TEMP_TYPE k_parameter = kernel[(kh  * ker_w) + kw];\
                    \
                    if(type == RISCV_VEC_IMG_BORDER_CONSTANT)\
                    {\
                        if(!(ty < 0))\
                            tmp += k_parameter * src[ty + tx];\
                    }\
                    else if(type == RISCV_VEC_IMG_BORDER_REFLECT101)\
                    {\
                        int tty = MAX(ty, -ty);\
                        tmp += k_parameter * src[tty + tx];\
                    }\
                }\
            }\
            dst[j + i] = SAT_CAST(tmp);\
        }\
    }

#define FILTER2D_BORDER_B_C \
    for( j = e_j ; j < src_h ; j += src_w)\
    {\
        for( i = s_i ; i < e_i ; i++)\
        {\
            int y = j - anchor_y * src_w;\
            int x = i - anchor_x * channel;\
            TEMP_TYPE tmp = _delta;\
            for(int kh = 0, ty = y ; kh < ker_h ; kh++, ty+=src_w)\
            {\
                for(int kw = 0, tx = x; kw < ker_w ; kw++, tx+=channel)\
                {\
                    TEMP_TYPE k_parameter = kernel[(kh  * ker_w) + kw];\
                    \
                    if(type == RISCV_VEC_IMG_BORDER_CONSTANT)\
                    {\
                        if(!(ty >= src_h))\
                            tmp += k_parameter * src[ty + tx];\
                    }\
                    else if(type == RISCV_VEC_IMG_BORDER_REFLECT101)\
                    {\
                        int tty = MIN(ty, (src_h-src_w) - (ty - (src_h-src_w)));\
                        tmp += k_parameter * src[tty + tx];\
                    }\
                }\
            }\
            dst[j + i] = SAT_CAST(tmp);\
        }\
    }

#define FILTER2D_MIDDLE_C \
    for( j = s_j ; j < e_j; j += src_w) \
    {\
        for( i = s_i ; i < e_i ; i++)\
        {\
            int y = j - anchor_y * src_w;\
            int x = i - anchor_x * channel;\
            TEMP_TYPE tmp = _delta;\
            for(int kh = 0, ty = y ; kh < ker_h ; kh++, ty+=src_w)\
            {\
                for(int kw = 0, tx = x; kw < ker_w ; kw++, tx+=channel)\
                {\
                    TEMP_TYPE k_parameter = kernel[(kh  * ker_w) + kw];\
                    tmp += k_parameter * src[ty + tx];\
                }\
            }\
            dst[j + i] = SAT_CAST(tmp);\
        }\
    }

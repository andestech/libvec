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

#ifndef __INTERNAL_VEC_IMAGE_BILATERAL_TEMPLATE_PUREC_U8_H__
#define __INTERNAL_VEC_IMAGE_BILATERAL_TEMPLATE_PUREC_U8_H__

#define BILATERAL_MAX(a, b) ((a < b) ? b : a)
#define BILATERAL_MIN(a, b) ((a < b) ? a : b)

#define BILATERAL_INIT_C \
    SRC_TYPE * p_src = img_src.data;\
    SRC_TYPE * p_cal = NULL;\
    DST_TYPE * p_dst = img_dst.data;\
    DST_TYPE * p_result = NULL;\
    float32_t range_val = 0.0f;\
    SRC_TYPE const_border_val = 0; \
    int32_t proc_height = img_dst.height; \
    int32_t proc_width  = img_dst.width; \
    int32_t n_ch       = img_dst.channels; \
    int32_t l_border = 0, l_border_num = 0, r_border = 0, r_border_num = 0; \
    int32_t t_border_num = 0, d_border_num = 0; \
    int32_t w_idx = 0, q_shift = 0; \


#define BILATERAL_BORDER_L_CN1_C \
    for(i = radius ; i < proc_height - radius ; i++) \
    { \
        p_result = &p_dst[i*proc_width]; \
        SRC_TYPE * p_color_p = NULL; \
        for(j = 0 ; j < radius ; j++) \
        { \
            p_sum_w[j] = 0.0f; \
            p_sum[j] = 0.0f; \
        } \
        p_cal = &p_src[i*proc_width]; \
        for(k = 0 ; k < maxk ; k++) \
        { \
            range_val = p_sp_tbl[k]; \
            SRC_TYPE * p_color_q = NULL; \
            l_border_num = 0; \
            if(p_sp_off_w[k] < 0) \
            { \
                l_border = (p_sp_off_h[k])*proc_width; \
                l_border_num = -p_sp_off_w[k]; \
                p_color_q = &p_cal[l_border]; \
            } \
            else \
            { \
                p_color_q = &p_cal[p_sp_off[k]]; \
            } \
            p_color_p = &p_cal[0]; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < l_border_num ; j++) \
                { \
                    SRC_TYPE val = const_border_val; \
                    SRC_TYPE rval = p_color_p[j]; \
                    int idx = abs(val - rval); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[j] += w; \
                    p_sum[j] += val * w; \
                } \
            } \
            else if(type == RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < l_border_num ; j++) \
                { \
                    SRC_TYPE val = p_color_q[l_border_num-j]; \
                    SRC_TYPE rval = p_color_p[j]; \
                    int idx = abs(val-rval); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[j] += w; \
                    p_sum[j] += val * w; \
                } \
            } \
            p_color_q = &p_cal[p_sp_off[k]]; \
            for(j = l_border_num ; j < radius ; j++) \
            { \
                SRC_TYPE val = p_color_q[j]; \
                SRC_TYPE rval = p_color_p[j]; \
                int idx = abs(val-rval); \
                KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                p_sum_w[j] += w; \
                p_sum[j] += val * w; \
            } \
        } \
        p_color_p = &p_cal[0]; \
        for(j = 0 ; j < radius ; j++) \
        { \
            p_result[j] = (uint8_t)(lrintf(p_sum[j]/p_sum_w[j])); \
        } \
    }

#define BILATERAL_BORDER_R_CN1_C \
    for(i = radius ; i < proc_height - radius ; i++) \
    { \
        p_result = &p_dst[i*proc_width]; \
        SRC_TYPE * p_color_p = NULL; \
        for(j =  proc_width - radius; j < proc_width  ; j++) \
        { \
            p_sum_w[j] = 0.0f; \
            p_sum[j] = 0.0f; \
        } \
        p_cal = &p_src[i*proc_width]; \
        for(k = 0 ; k < maxk ; k++) \
        { \
            range_val = p_sp_tbl[k]; \
            SRC_TYPE * p_color_q = NULL; \
            r_border_num = 0; \
            if(p_sp_off_w[k] > 0) \
            { \
                r_border = (p_sp_off_h[k])*proc_width; \
                r_border_num = p_sp_off_w[k]; \
            } \
            p_color_p = &p_cal[0]; \
            p_color_q = &p_cal[p_sp_off[k]]; \
            for(j = proc_width-radius ; j < proc_width - r_border_num ; j++) \
            { \
                SRC_TYPE val = p_color_q[j]; \
                SRC_TYPE rval = p_color_p[j]; \
                int idx = abs(rval-val); \
                KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                p_sum_w[j] += w; \
                p_sum[j] += val * w; \
            } \
            p_color_q = &p_cal[r_border]; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < r_border_num ; j ++) \
                { \
                    SRC_TYPE val = const_border_val; \
                    SRC_TYPE rval = p_color_p[proc_width -  r_border_num + j]; \
                    int idx = abs(rval-val); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[proc_width -  r_border_num + j] += w; \
                    p_sum[proc_width -  r_border_num + j] += val * w; \
                } \
            } \
            else if(type == RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < r_border_num ; j ++) \
                { \
                    SRC_TYPE val = p_color_q[proc_width-2-j]; \
                    SRC_TYPE rval = p_color_p[proc_width -  r_border_num + j]; \
                    int idx = abs(rval-val); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[proc_width -  r_border_num + j] += w; \
                    p_sum[proc_width -  r_border_num + j] += val * w; \
                } \
            } \
        } \
        p_color_p = &p_cal[0]; \
        for(j =  proc_width - radius; j < proc_width  ; j++) \
        { \
            p_result[j] = (uint8_t)(lrintf(p_sum[j]/p_sum_w[j])); \
        } \
    }

#define BILATERAL_BORDER_T_CN1_C \
    for(i = 0 ; i < radius ; i++) \
    { \
        p_result = &p_dst[i*proc_width]; \
        SRC_TYPE * p_color_p = NULL; \
        SRC_TYPE * p_color_q_t_off = NULL; \
        for(j = 0; j < proc_width  ; j++) \
        { \
            p_sum_w[j] = 0.0f; \
            p_sum[j] = 0.0f; \
        } \
        p_cal = &p_src[i*proc_width]; \
        for(k = 0 ; k < maxk ; k++) \
        { \
            range_val = p_sp_tbl[k]; \
            SRC_TYPE * p_color_q = NULL; \
            r_border_num = 0; \
            l_border_num = 0; \
            t_border_num = 0; \
            if(p_sp_off_h[k]+i < 0) \
            { \
                t_border_num = -(p_sp_off_h[k]+i); \
                p_color_q_t_off = &p_src[t_border_num*proc_width]; \
            } \
            else \
            { \
                p_color_q_t_off = &p_src[(p_sp_off_h[k]+i)*proc_width];; \
            } \
            if(p_sp_off_w[k] > 0) \
            { \
                r_border_num = p_sp_off_w[k]; \
            } \
            else if (p_sp_off_w[k] < 0) \
            { \
                l_border_num = -p_sp_off_w[k]; \
            } \
            p_color_p = &p_cal[0]; \
            p_color_q = &p_color_q_t_off[0]; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < l_border_num ; j++) \
                { \
                    SRC_TYPE val = const_border_val; \
                    SRC_TYPE rval = p_color_p[j]; \
                    int idx = abs(rval-val); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[j] += w; \
                    p_sum[j] += val * w; \
                } \
            } \
            else if(type == RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < l_border_num ; j++) \
                { \
                    SRC_TYPE val = p_color_q[l_border_num-j]; \
                    SRC_TYPE rval = p_color_p[j]; \
                    int idx = abs(rval-val); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[j] += w; \
                    p_sum[j] += val * w; \
                } \
            } \
            p_color_q = &p_color_q_t_off[p_sp_off_w[k]]; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                if(t_border_num == 0) \
                { \
                    for( j = l_border_num ; j < proc_width - r_border_num ; j++)\
                    { \
                        SRC_TYPE val = p_color_q[j]; \
                        SRC_TYPE rval = p_color_p[j]; \
                        int idx = abs(rval-val); \
                        KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                        p_sum_w[j] += w; \
                        p_sum[j] += val * w; \
                    } \
                } \
                else \
                { \
                    for( j = l_border_num ; j < proc_width - r_border_num ; j++)\
                    { \
                        SRC_TYPE val = const_border_val; \
                        SRC_TYPE rval = p_color_p[j]; \
                        int idx = abs(rval-val); \
                        KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                        p_sum_w[j] += w; \
                        p_sum[j] += val * w; \
                    } \
                } \
            } \
            else if (type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for( j = l_border_num ; j < proc_width - r_border_num ; j++)\
                { \
                    SRC_TYPE val = p_color_q[j]; \
                    SRC_TYPE rval = p_color_p[j]; \
                    int idx = abs(rval-val); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[j] += w; \
                    p_sum[j] += val * w; \
                } \
            } \
            p_color_q = &p_color_q_t_off[0]; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < r_border_num ; j ++) \
                { \
                    SRC_TYPE val = const_border_val; \
                    SRC_TYPE rval = p_color_p[proc_width -  r_border_num + j]; \
                    int idx = abs(rval-val); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[proc_width -  r_border_num + j] += w; \
                    p_sum[proc_width -  r_border_num + j] += val * w; \
                } \
            } \
            else if(type == RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < r_border_num ; j ++) \
                { \
                    SRC_TYPE val = p_color_q[proc_width-2-j]; \
                    SRC_TYPE rval = p_color_p[proc_width -  r_border_num + j]; \
                    int idx = abs(rval-val); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[proc_width -  r_border_num + j] += w; \
                    p_sum[proc_width -  r_border_num + j] += val * w; \
                } \
            } \
        } \
        p_color_p = &p_cal[0]; \
        for(j = 0 ; j < proc_width ; j++) \
        { \
            p_result[j] = (uint8_t)(lrintf(p_sum[j]/p_sum_w[j])); \
        } \
    }


#define BILATERAL_BORDER_D_CN1_C \
    for(i = proc_height - radius  ; i < proc_height ; i++) \
    { \
        p_result = &p_dst[i*proc_width]; \
        SRC_TYPE * p_color_p = NULL; \
        SRC_TYPE * p_color_q_d_off = NULL; \
        for(j = 0; j < proc_width  ; j++) \
        { \
            p_sum_w[j] = 0.0f; \
            p_sum[j] = 0.0f; \
        } \
        p_cal = &p_src[i*proc_width]; \
        for(k = 0 ; k < maxk ; k++) \
        { \
            range_val = p_sp_tbl[k]; \
            SRC_TYPE * p_color_q = NULL; \
            r_border_num = 0; \
            l_border_num = 0; \
            d_border_num = 0; \
            if( (p_sp_off_h[k]+i) > (proc_height-1)) \
            { \
                d_border_num = (proc_height-1) - (p_sp_off_h[k]+i - proc_height+1); \
                p_color_q_d_off = &p_src[d_border_num*proc_width]; \
            } \
            else \
            { \
                p_color_q_d_off = &p_src[(p_sp_off_h[k]+i)*proc_width];; \
            } \
            if(p_sp_off_w[k] > 0) \
            { \
                r_border_num = p_sp_off_w[k]; \
            } \
            else if (p_sp_off_w[k] < 0) \
            { \
                l_border_num = -p_sp_off_w[k]; \
            } \
            p_color_p = &p_cal[0]; \
            p_color_q = &p_color_q_d_off[0]; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < l_border_num ; j++) \
                { \
                    SRC_TYPE val = const_border_val; \
                    SRC_TYPE rval = p_color_p[j]; \
                    int idx = abs(rval-val); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[j] += w; \
                    p_sum[j] += val * w; \
                } \
            } \
            else if(type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < l_border_num ; j++) \
                { \
                    SRC_TYPE val = p_color_q[l_border_num-j]; \
                    SRC_TYPE rval = p_color_p[j]; \
                    int idx = abs(rval-val); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[j] += w; \
                    p_sum[j] += val * w; \
                } \
            } \
            p_color_q = &p_color_q_d_off[p_sp_off_w[k]]; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                if(d_border_num == 0) \
                { \
                    for( j = l_border_num ; j < proc_width - r_border_num ; j++)\
                    { \
                        SRC_TYPE val = p_color_q[j]; \
                        SRC_TYPE rval = p_color_p[j]; \
                        int idx = abs(rval-val); \
                        KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                        p_sum_w[j] += w; \
                        p_sum[j] += val * w; \
                    } \
                } \
                else \
                { \
                    for( j = l_border_num ; j < proc_width - r_border_num ; j++)\
                    { \
                        SRC_TYPE val = const_border_val; \
                        SRC_TYPE rval = p_color_p[j]; \
                        int idx = abs(rval-val); \
                        KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                        p_sum_w[j] += w; \
                        p_sum[j] += val * w; \
                    } \
                }\
            } \
            else if(type == RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for( j = l_border_num ; j < proc_width - r_border_num ; j++)\
                { \
                    SRC_TYPE val = p_color_q[j]; \
                    SRC_TYPE rval = p_color_p[j]; \
                    int idx = abs(rval-val); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[j] += w; \
                    p_sum[j] += val * w; \
                } \
            } \
            p_color_q = &p_color_q_d_off[0]; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < r_border_num ; j ++) \
                { \
                    SRC_TYPE val = const_border_val; \
                    SRC_TYPE rval = p_color_p[proc_width -  r_border_num + j]; \
                    int idx = abs(rval-val); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[proc_width -  r_border_num + j] += w; \
                    p_sum[proc_width -  r_border_num + j] += val * w; \
                } \
            } \
            else if(type == RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < r_border_num ; j ++) \
                { \
                    SRC_TYPE val = p_color_q[proc_width-2-j]; \
                    SRC_TYPE rval = p_color_p[proc_width -  r_border_num + j]; \
                    int idx = abs(rval-val); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[proc_width -  r_border_num + j] += w; \
                    p_sum[proc_width -  r_border_num + j] += val * w; \
                } \
            } \
        } \
        p_color_p = &p_cal[0]; \
        for(j = 0 ; j < proc_width ; j++) \
        { \
            p_result[j] = (uint8_t)(lrintf(p_sum[j]/p_sum_w[j])); \
        } \
    }

#define BILATERAL_MIDDLE_CN1_C \
    for(i = radius  ; i < proc_height - radius ; i++) \
    { \
        SRC_TYPE * p_color_p = NULL; \
        SRC_TYPE * p_color_q = NULL; \
        p_result = &p_dst[i*proc_width]; \
        for(j = radius ; j < proc_width - radius ; j++) \
        { \
            p_sum_w[j] = 0.0f; \
            p_sum[j] = 0.0f; \
        } \
        p_cal = &p_src[i*proc_width]; \
        for(k = 0 ; k < maxk ; k++) \
        { \
            range_val = p_sp_tbl[k]; \
            p_color_q = &p_cal[p_sp_off[k]]; \
            p_color_p = &p_cal[0]; \
            for(j = radius ; j < proc_width - radius ; j++) \
            { \
                SRC_TYPE val = p_color_q[j]; \
                SRC_TYPE rval = p_color_p[j]; \
                int idx = abs(rval-val); \
                KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                p_sum_w[j] += w; \
                p_sum[j] += val * w; \
            } \
        } \
        p_color_p = &p_cal[0]; \
        for(j = radius ; j < proc_width - radius ; j++) \
        { \
            p_result[j] = (uint8_t)(lrintf(p_sum[j]/p_sum_w[j])); \
        } \
    }

#define BILATERAL_BORDER_L_CN3_C \
    for(i = radius ; i < proc_height - radius ; i++) \
    { \
        SRC_TYPE * p_color_p = NULL; \
        SRC_TYPE * p_color_q = NULL; \
        p_result = &p_dst[i*proc_width*n_ch]; \
        for(j = 0 ; j < radius ; j++) \
        { \
            p_sum_w[j] = 0.0f; \
            p_sum[n_ch*j+0] = 0.0f; \
            p_sum[n_ch*j+1] = 0.0f; \
            p_sum[n_ch*j+2] = 0.0f; \
        } \
        p_cal = &p_src[i*proc_width*n_ch]; \
        for(k = 0 ; k < maxk ; k++) \
        { \
            range_val = p_sp_tbl[k]; \
            p_color_q = &p_cal[p_sp_off[k]]; \
            p_color_p = &p_cal[0]; \
            w_idx = 0; \
            l_border_num = 0; \
            if(p_sp_off_w[k] < 0) \
            { \
                l_border = (p_sp_off_h[k])*proc_width*n_ch; \
                l_border_num = -p_sp_off_w[k]; \
                p_color_q = &p_cal[l_border]; \
            } \
            else \
            { \
                p_color_q = &p_cal[p_sp_off[k]]; \
            } \
            p_color_p = &p_cal[0]; \
            q_shift = 0 ;\
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < l_border_num*n_ch ; j+=n_ch, w_idx++, q_shift++) \
                { \
                    SRC_TYPE b = const_border_val, g = const_border_val, r = const_border_val; \
                    SRC_TYPE rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                    int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[w_idx] += w; \
                    p_sum[j] += b * w; \
                    p_sum[j+1] += g * w; \
                    p_sum[j+2] += r * w; \
                } \
            } \
            else if(type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < l_border_num*n_ch ; j+=n_ch, w_idx++, q_shift++) \
                { \
                    SRC_TYPE b = p_color_q[(l_border_num-q_shift)*n_ch], g = p_color_q[(l_border_num-q_shift)*n_ch+1], r = p_color_q[(l_border_num-q_shift)*n_ch+2]; \
                    SRC_TYPE rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                    int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[w_idx] += w; \
                    p_sum[j] += b * w; \
                    p_sum[j+1] += g * w; \
                    p_sum[j+2] += r * w; \
                } \
            } \
            p_color_q = &p_cal[p_sp_off[k]]; \
            for(j = l_border_num*n_ch ; j < radius*n_ch ; j+=n_ch, w_idx++) \
            { \
                SRC_TYPE b = p_color_q[j], g = p_color_q[j+1], r = p_color_q[j+2]; \
                SRC_TYPE rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                p_sum_w[w_idx] += w; \
                p_sum[j] +=  b * w; \
                p_sum[j+1] += g * w; \
                p_sum[j+2] += r * w; \
            } \
        } \
        p_color_p = &p_cal[0]; \
        w_idx = 0; \
        for(j = 0 ; j < radius*n_ch ; j+= n_ch, w_idx++) \
        { \
            KERNEL_TYPE w_inv = 1/p_sum_w[w_idx]; \
            p_result[j]   = (uint8_t)lrintf(p_sum[j+0]*w_inv); \
            p_result[j+1] = (uint8_t)lrintf(p_sum[j+1]*w_inv); \
            p_result[j+2] = (uint8_t)lrintf(p_sum[j+2]*w_inv); \
        } \
    }

#define BILATERAL_BORDER_R_CN3_C \
    for(i = radius ; i < proc_height - radius ; i++) \
    { \
        p_result = &p_dst[i*proc_width*n_ch]; \
        SRC_TYPE * p_color_p = NULL; \
        SRC_TYPE * p_color_q = NULL; \
        w_idx = 0; \
        for(j = proc_width - radius; j < proc_width  ; j++) \
        { \
            p_sum_w[j] = 0.0f; \
            p_sum[n_ch*j+0] = 0.0f; \
            p_sum[n_ch*j+1] = 0.0f; \
            p_sum[n_ch*j+2] = 0.0f; \
        } \
        p_cal = &p_src[i*proc_width*n_ch]; \
        for(k = 0 ; k < maxk ; k++) \
        { \
            range_val = p_sp_tbl[k]; \
            p_color_q = &p_cal[p_sp_off[k]]; \
            p_color_p = &p_cal[0]; \
            w_idx = proc_width - radius; \
            r_border_num = 0 ;\
            if(p_sp_off_w[k] > 0) \
            { \
                r_border = (p_sp_off_h[k])*proc_width*n_ch; \
                r_border_num = p_sp_off_w[k]; \
            } \
            p_color_p = &p_cal[0]; \
            for(j = (proc_width-radius)*n_ch ; j < (proc_width - r_border_num)*n_ch ; j+=n_ch, w_idx++) \
            { \
                SRC_TYPE b = p_color_q[j], g = p_color_q[j+1], r = p_color_q[j+2]; \
                SRC_TYPE rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                p_sum_w[w_idx] += w; \
                p_sum[j] += b * w; \
                p_sum[j+1] += g * w; \
                p_sum[j+2] += r * w; \
            } \
            p_color_q = &p_cal[r_border]; \
            q_shift = 0 ; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < r_border_num*n_ch ; j+=n_ch, w_idx++, q_shift++) \
                { \
                    SRC_TYPE b = const_border_val; \
                    SRC_TYPE g = const_border_val; \
                    SRC_TYPE r = const_border_val; \
                    SRC_TYPE rb = p_color_p[(proc_width - r_border_num)*n_ch + j ] ;\
                    SRC_TYPE rg = p_color_p[(proc_width - r_border_num)*n_ch + j + 1]; \
                    SRC_TYPE rr = p_color_p[(proc_width - r_border_num)*n_ch + j + 2]; \
                    int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[w_idx] += w; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j] += b*w ; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j+1] += g*w ; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j+2] += r*w ; \
                } \
            } \
            else if (type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < r_border_num*n_ch ; j+=n_ch, w_idx++, q_shift++) \
                { \
                    SRC_TYPE b = p_color_q[(proc_width-2-q_shift)*n_ch+0]; \
                    SRC_TYPE g = p_color_q[(proc_width-2-q_shift)*n_ch+1]; \
                    SRC_TYPE r = p_color_q[(proc_width-2-q_shift)*n_ch+2]; \
                    SRC_TYPE rb = p_color_p[(proc_width - r_border_num)*n_ch + j ] ;\
                    SRC_TYPE rg = p_color_p[(proc_width - r_border_num)*n_ch + j + 1]; \
                    SRC_TYPE rr = p_color_p[(proc_width - r_border_num)*n_ch + j + 2]; \
                    int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[w_idx] += w; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j] += b*w ; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j+1] += g*w ; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j+2] += r*w ; \
                } \
            } \
        } \
        w_idx = proc_width - radius; \
        for(j = (proc_width - radius)*n_ch; j < proc_width*n_ch ; j += n_ch, w_idx++) \
        { \
            KERNEL_TYPE w_inv = 1/p_sum_w[w_idx]; \
            p_result[j]   = (uint8_t)lrintf(p_sum[j+0]*w_inv); \
            p_result[j+1] = (uint8_t)lrintf(p_sum[j+1]*w_inv); \
            p_result[j+2] = (uint8_t)lrintf(p_sum[j+2]*w_inv); \
        } \
    }

#define BILATERAL_BORDER_T_CN3_C \
    for(i = 0 ; i < radius ; i++) \
    { \
        SRC_TYPE * p_color_p = NULL; \
        SRC_TYPE * p_color_q = NULL; \
        p_result = &p_dst[i*proc_width*n_ch]; \
        for(j = 0; j < proc_width  ; j++) \
        { \
            p_sum_w[j] = 0.0f; \
            p_sum[n_ch*j+0] = 0.0f; \
            p_sum[n_ch*j+1] = 0.0f; \
            p_sum[n_ch*j+2] = 0.0f; \
        } \
        p_cal = &p_src[i*proc_width*n_ch]; \
        for(k = 0 ; k < maxk ; k++) \
        { \
            range_val = p_sp_tbl[k]; \
            SRC_TYPE * p_color_q_t_off = NULL; \
            r_border_num = 0; \
            l_border_num = 0; \
            t_border_num = 0; \
            if(p_sp_off_h[k]+i < 0) \
            { \
                t_border_num = -(p_sp_off_h[k]+i); \
                p_color_q_t_off = &p_src[t_border_num*proc_width*n_ch]; \
            } \
            else \
            { \
                p_color_q_t_off = &p_src[(p_sp_off_h[k]+i)*proc_width*n_ch];; \
            } \
            if(p_sp_off_w[k] > 0) \
            { \
                r_border_num = p_sp_off_w[k]; \
            } \
            else if (p_sp_off_w[k] < 0) \
            { \
                l_border_num = -p_sp_off_w[k]; \
            } \
            p_color_p = &p_cal[0]; \
            p_color_q = &p_color_q_t_off[0]; \
            q_shift = 0 ;\
            w_idx = 0; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < l_border_num*n_ch ; j+=n_ch, q_shift++) \
                { \
                    SRC_TYPE b = const_border_val; \
                    SRC_TYPE g = const_border_val; \
                    SRC_TYPE r = const_border_val; \
                    SRC_TYPE rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                    int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[w_idx++] += w; \
                    p_sum[j] += b*w ; \
                    p_sum[j+1] += g*w ; \
                    p_sum[j+2] += r*w ; \
                } \
            } \
            else if (type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < l_border_num*n_ch ; j+=n_ch, q_shift++) \
                { \
                    SRC_TYPE b = p_color_q[(l_border_num-q_shift)*n_ch]; \
                    SRC_TYPE g = p_color_q[(l_border_num-q_shift)*n_ch+1]; \
                    SRC_TYPE r = p_color_q[(l_border_num-q_shift)*n_ch+2]; \
                    SRC_TYPE rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                    int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[w_idx++] += w; \
                    p_sum[j] += b*w ; \
                    p_sum[j+1] += g*w ; \
                    p_sum[j+2] += r*w ; \
                } \
            } \
            p_color_q = &p_color_q_t_off[p_sp_off_w[k]*n_ch]; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                if(t_border_num==0) \
                { \
                    for(j = l_border_num*n_ch ; j < (proc_width - r_border_num)*n_ch ; j+=n_ch) \
                    { \
                        SRC_TYPE b = p_color_q[j], g = p_color_q[j+1], r = p_color_q[j+2]; \
                        SRC_TYPE rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                        int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                        KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                        p_sum_w[w_idx++] += w; \
                        p_sum[j] += b*w ; \
                        p_sum[j+1] += g*w ; \
                        p_sum[j+2] += r*w ; \
                    } \
                } \
                else \
                { \
                    for(j = l_border_num*n_ch ; j < (proc_width - r_border_num)*n_ch ; j+=n_ch) \
                    { \
                        SRC_TYPE b = const_border_val, g = const_border_val, r = const_border_val; \
                        SRC_TYPE rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                        int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                        KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                        p_sum_w[w_idx++] += w; \
                        p_sum[j] += b*w ; \
                        p_sum[j+1] += g*w ; \
                        p_sum[j+2] += r*w ; \
                    } \
                } \
            } \
            else if(type == RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = l_border_num*n_ch ; j < (proc_width - r_border_num)*n_ch ; j+=n_ch) \
                { \
                    SRC_TYPE b = p_color_q[j], g = p_color_q[j+1], r = p_color_q[j+2]; \
                    SRC_TYPE rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                    int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[w_idx++] += w; \
                    p_sum[j] += b*w ; \
                    p_sum[j+1] += g*w ; \
                    p_sum[j+2] += r*w ; \
                } \
            } \
            p_color_q = &p_color_q_t_off[0]; \
            q_shift = 0 ; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < r_border_num*n_ch ; j+=n_ch) \
                { \
                    SRC_TYPE b = const_border_val; \
                    SRC_TYPE g = const_border_val; \
                    SRC_TYPE r = const_border_val; \
                    SRC_TYPE rb = p_color_p[(proc_width - r_border_num)*n_ch + j ] ;\
                    SRC_TYPE rg = p_color_p[(proc_width - r_border_num)*n_ch + j + 1]; \
                    SRC_TYPE rr = p_color_p[(proc_width - r_border_num)*n_ch + j + 2]; \
                    int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[w_idx++] += w; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j] += b*w ; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j+1] += g*w ; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j+2] += r*w ; \
                } \
            } \
            else if (type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < r_border_num*n_ch ; j+=n_ch, q_shift++) \
                { \
                    SRC_TYPE b = p_color_q[(proc_width-2-q_shift)*n_ch+0]; \
                    SRC_TYPE g = p_color_q[(proc_width-2-q_shift)*n_ch+1]; \
                    SRC_TYPE r = p_color_q[(proc_width-2-q_shift)*n_ch+2]; \
                    SRC_TYPE rb = p_color_p[(proc_width - r_border_num)*n_ch + j ] ;\
                    SRC_TYPE rg = p_color_p[(proc_width - r_border_num)*n_ch + j + 1]; \
                    SRC_TYPE rr = p_color_p[(proc_width - r_border_num)*n_ch + j + 2]; \
                    int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[w_idx++] += w; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j] += b*w ; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j+1] += g*w ; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j+2] += r*w ; \
                } \
            } \
        } \
        p_color_p = &p_cal[0]; \
        w_idx = 0; \
        for(j = 0; j < proc_width*n_ch ; j += n_ch, w_idx++) \
        { \
            KERNEL_TYPE w_inv = 1/p_sum_w[w_idx]; \
            p_result[j]   = (uint8_t)lrintf(p_sum[j+0]*w_inv); \
            p_result[j+1] = (uint8_t)lrintf(p_sum[j+1]*w_inv); \
            p_result[j+2] = (uint8_t)lrintf(p_sum[j+2]*w_inv); \
        } \
    }


#define BILATERAL_BORDER_D_CN3_C \
    for(i = proc_height - radius  ; i < proc_height ; i++) \
    { \
        SRC_TYPE * p_color_p = NULL; \
        SRC_TYPE * p_color_q = NULL; \
        p_result = &p_dst[i*proc_width*n_ch]; \
        for(j = 0; j < proc_width  ; j++) \
        { \
            p_sum_w[j] = 0.0f; \
            p_sum[n_ch*j+0] = 0.0f; \
            p_sum[n_ch*j+1] = 0.0f; \
            p_sum[n_ch*j+2] = 0.0f; \
        } \
        p_cal = &p_src[i*proc_width*n_ch]; \
        for(k = 0 ; k < maxk ; k++) \
        { \
            range_val = p_sp_tbl[k]; \
            SRC_TYPE * p_color_q_t_off = NULL; \
            r_border_num = 0; \
            l_border_num = 0; \
            d_border_num = 0; \
            if((p_sp_off_h[k]+i) > (proc_height-1)) \
            { \
                d_border_num = (proc_height-1) - (p_sp_off_h[k]+i - proc_height+1); \
                p_color_q_t_off = &p_src[d_border_num*proc_width*n_ch]; \
            } \
            else \
            { \
                p_color_q_t_off = &p_src[(p_sp_off_h[k]+i)*proc_width*n_ch];; \
            } \
            if(p_sp_off_w[k] > 0) \
            { \
                r_border_num = p_sp_off_w[k]; \
            } \
            else if (p_sp_off_w[k] < 0) \
            { \
                l_border_num = -p_sp_off_w[k]; \
            } \
            p_color_p = &p_cal[0]; \
            p_color_q = &p_color_q_t_off[0]; \
            q_shift = 0 ;\
            w_idx = 0; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < l_border_num*n_ch ; j+=n_ch, q_shift++) \
                { \
                    SRC_TYPE b = const_border_val; \
                    SRC_TYPE g = const_border_val; \
                    SRC_TYPE r = const_border_val; \
                    SRC_TYPE rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                    int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[w_idx++] += w; \
                    p_sum[j] += b*w ; \
                    p_sum[j+1] += g*w ; \
                    p_sum[j+2] += r*w ; \
                } \
            } \
            else if (type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < l_border_num*n_ch ; j+=n_ch, q_shift++) \
                { \
                    SRC_TYPE b = p_color_q[(l_border_num-q_shift)*n_ch]; \
                    SRC_TYPE g = p_color_q[(l_border_num-q_shift)*n_ch+1]; \
                    SRC_TYPE r = p_color_q[(l_border_num-q_shift)*n_ch+2]; \
                    SRC_TYPE rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                    int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[w_idx++] += w; \
                    p_sum[j] += b*w ; \
                    p_sum[j+1] += g*w ; \
                    p_sum[j+2] += r*w ; \
                } \
            } \
            p_color_q = &p_color_q_t_off[p_sp_off_w[k]*n_ch]; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                if(d_border_num==0) \
                { \
                    for(j = l_border_num*n_ch ; j < (proc_width - r_border_num)*n_ch ; j+=n_ch) \
                    { \
                        SRC_TYPE b = p_color_q[j], g = p_color_q[j+1], r = p_color_q[j+2]; \
                        SRC_TYPE rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                        int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                        KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                        p_sum_w[w_idx++] += w; \
                        p_sum[j] += b*w ; \
                        p_sum[j+1] += g*w ; \
                        p_sum[j+2] += r*w ; \
                    } \
                } \
                else \
                { \
                    for(j = l_border_num*n_ch ; j < (proc_width - r_border_num)*n_ch ; j+=n_ch) \
                    { \
                        SRC_TYPE b = const_border_val, g = const_border_val, r = const_border_val; \
                        SRC_TYPE rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                        int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                        KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                        p_sum_w[w_idx++] += w; \
                        p_sum[j] += b*w ; \
                        p_sum[j+1] += g*w ; \
                        p_sum[j+2] += r*w ; \
                    } \
                } \
            } \
            else if(type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = l_border_num*n_ch ; j < (proc_width - r_border_num)*n_ch ; j+=n_ch) \
                { \
                    SRC_TYPE b = p_color_q[j], g = p_color_q[j+1], r = p_color_q[j+2]; \
                    SRC_TYPE rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                    int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[w_idx++] += w; \
                    p_sum[j] += b*w ; \
                    p_sum[j+1] += g*w ; \
                    p_sum[j+2] += r*w ; \
                } \
            } \
            p_color_q = &p_color_q_t_off[0]; \
            q_shift = 0 ; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < r_border_num*n_ch ; j+=n_ch, q_shift++) \
                { \
                    SRC_TYPE b = const_border_val; \
                    SRC_TYPE g = const_border_val; \
                    SRC_TYPE r = const_border_val; \
                    SRC_TYPE rb = p_color_p[(proc_width - r_border_num)*n_ch + j ] ;\
                    SRC_TYPE rg = p_color_p[(proc_width - r_border_num)*n_ch + j + 1]; \
                    SRC_TYPE rr = p_color_p[(proc_width - r_border_num)*n_ch + j + 2]; \
                    int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[w_idx++] += w; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j] += b*w ; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j+1] += g*w ; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j+2] += r*w ; \
                } \
            } \
            else if (type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < r_border_num*n_ch ; j+=n_ch, q_shift++) \
                { \
                    SRC_TYPE b = p_color_q[(proc_width-2-q_shift)*n_ch+0]; \
                    SRC_TYPE g = p_color_q[(proc_width-2-q_shift)*n_ch+1]; \
                    SRC_TYPE r = p_color_q[(proc_width-2-q_shift)*n_ch+2]; \
                    SRC_TYPE rb = p_color_p[(proc_width - r_border_num)*n_ch + j ] ;\
                    SRC_TYPE rg = p_color_p[(proc_width - r_border_num)*n_ch + j + 1]; \
                    SRC_TYPE rr = p_color_p[(proc_width - r_border_num)*n_ch + j + 2]; \
                    int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                    KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                    p_sum_w[w_idx++] += w; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j] += b*w ; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j+1] += g*w ; \
                    p_sum[(proc_width - r_border_num)*n_ch+ j+2] += r*w ; \
                } \
            } \
        } \
        p_color_p = &p_cal[0]; \
        w_idx = 0; \
        for(j = 0; j < proc_width*n_ch ; j += n_ch, w_idx++) \
        { \
            KERNEL_TYPE w_inv = 1/p_sum_w[w_idx]; \
            p_result[j]   = (uint8_t)lrintf(p_sum[j+0]*w_inv); \
            p_result[j+1] = (uint8_t)lrintf(p_sum[j+1]*w_inv); \
            p_result[j+2] = (uint8_t)lrintf(p_sum[j+2]*w_inv); \
        } \
    }

#define BILATERAL_MIDDLE_CN3_C \
    for(i = radius  ; i < proc_height - radius ; i++) \
    { \
        SRC_TYPE * p_color_p = NULL; \
        SRC_TYPE * p_color_q = NULL; \
        p_result = &p_dst[i*proc_width*n_ch]; \
        for(j = radius ; j < (proc_width - radius) ; j++) \
        { \
            p_sum_w[j] = 0.0f; \
            p_sum[n_ch*j+0] = 0.0f; \
            p_sum[n_ch*j+1] = 0.0f; \
            p_sum[n_ch*j+2] = 0.0f; \
        } \
        p_cal = &p_src[i*proc_width*n_ch]; \
        for(k = 0 ; k < maxk ; k++) \
        { \
            range_val = p_sp_tbl[k]; \
            p_color_q = &p_cal[p_sp_off[k]]; \
            p_color_p = &p_cal[0]; \
            w_idx = radius; \
            for(j = radius*n_ch ; j < (proc_width - radius)*n_ch ; j+=n_ch, w_idx++) \
            { \
                SRC_TYPE b = p_color_q[j], g = p_color_q[j+1], r = p_color_q[j+2]; \
                SRC_TYPE rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                int idx = abs(b-rb)+abs(g-rg)+abs(r-rr); \
                KERNEL_TYPE w = range_val * (p_color_tbl[idx]); \
                p_sum_w[w_idx] += w; \
                p_sum[j] += b*w ; \
                p_sum[j+1] += g*w ; \
                p_sum[j+2] += r*w ; \
            } \
        } \
        p_color_p = &p_cal[0]; \
        w_idx = radius; \
        for (j = radius*n_ch; j < (proc_width - radius)*n_ch; j+=n_ch, w_idx++) \
        { \
            KERNEL_TYPE w_inv = 1/p_sum_w[w_idx]; \
            p_result[j]   = (uint8_t)lrintf(p_sum[j+0]*w_inv); \
            p_result[j+1] = (uint8_t)lrintf(p_sum[j+1]*w_inv); \
            p_result[j+2] = (uint8_t)lrintf(p_sum[j+2]*w_inv); \
        } \
    }

#endif

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

#ifndef __INTERNAL_VEC_IMAGE_BILATERAL_TEMPLATE_PUREC_F32_H__
#define __INTERNAL_VEC_IMAGE_BILATERAL_TEMPLATE_PUREC_F32_H__

#define BILATERAL_MAX(a, b) ((a < b) ? b : a)
#define BILATERAL_MIN(a, b) ((a < b) ? a : b)
typedef union f32_suf
{
    int32_t i;
    uint32_t u;
    float f;
}
f32_suf;

inline int32_t IsNaN( float32_t value )
{

    f32_suf ieee754;
    ieee754.f = value;
    return ((ieee754.u & 0x7fffffff) > 0x7f800000);
}

inline int32_t get_floor( float32_t value)
{
    int32_t i = (int32_t)value;
    return i - (i > value);
}


#define BILATERAL_INIT_C \
    SRC_TYPE * p_src = img_src.data;\
    SRC_TYPE * p_cal = NULL;\
    DST_TYPE * p_dst = img_dst.data;\
    DST_TYPE * p_result = NULL;\
    float32_t range_val = 0.0f;\
    float32_t const_border_val = 0.0f; \
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
                    float val = const_border_val; \
                    float rval = p_color_p[j]; \
                    float alpha = fabsf(val - rval) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if (!IsNaN(val)) \
                    { \
                        float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx    ]))); \
                        p_sum_w[j] += w; \
                        p_result[j] += val * w; \
                    } \
                } \
            } \
            else if(type == RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < l_border_num ; j++) \
                { \
                    float val = p_color_q[l_border_num-j]; \
                    float rval = p_color_p[j]; \
                    float alpha = fabsf(val - rval) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if (!IsNaN(val)) \
                    { \
                        float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx    ]))); \
                        p_sum_w[j] += w; \
                        p_result[j] += val * w; \
                    } \
                } \
            } \
            p_color_q = &p_cal[p_sp_off[k]]; \
            for(j = l_border_num ; j < radius ; j++) \
            { \
                float val = p_color_q[j]; \
                float rval = p_color_p[j]; \
                float alpha = fabsf(val - rval) * scale_index; \
                int idx = get_floor(alpha); \
                alpha -= (float)idx; \
                if (!IsNaN(val)) \
                { \
                    float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx    ]))); \
                    p_sum_w[j] += w; \
                    p_result[j] += val * w; \
                } \
            } \
        } \
        p_color_p = &p_cal[0]; \
        for(j = 0 ; j < radius ; j++) \
        { \
            p_result[j] = IsNaN(p_color_p[j])? p_result[j] / p_sum_w[j] : (p_result[j] + p_color_p[j]) / (p_sum_w[j] + 1.f); \
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
                float val = p_color_q[j]; \
                float rval = p_color_p[j]; \
                float alpha = fabsf(val - rval) * scale_index; \
                int idx = get_floor(alpha); \
                alpha -= (float)idx; \
                if (!IsNaN(val)) \
                { \
                    float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx    ]))); \
                    p_sum_w[j] += w; \
                    p_result[j] += val * w; \
                } \
            } \
            p_color_q = &p_cal[r_border]; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < r_border_num ; j ++) \
                { \
                    float val = const_border_val; \
                    float rval = p_color_p[proc_width -  r_border_num + j]; \
                    float alpha = fabsf(val - rval) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if (!IsNaN(val)) \
                    { \
                        float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[proc_width -  r_border_num + j] += w; \
                        p_result[proc_width -  r_border_num + j] += val * w; \
                    } \
                } \
            } \
            else if(type == RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < r_border_num ; j ++) \
                { \
                    float val = p_color_q[proc_width-2-j]; \
                    float rval = p_color_p[proc_width -  r_border_num + j]; \
                    float alpha = fabsf(val - rval) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if (!IsNaN(val)) \
                    { \
                        float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[proc_width -  r_border_num + j] += w; \
                        p_result[proc_width -  r_border_num + j] += val * w; \
                    } \
                } \
            } \
        } \
        p_color_p = &p_cal[0]; \
        for(j =  proc_width - radius; j < proc_width  ; j++) \
        { \
            p_result[j] = IsNaN(p_color_p[j])? p_result[j] / p_sum_w[j] : (p_result[j] + p_color_p[j]) / (p_sum_w[j] + 1.f);\
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
                    float val = const_border_val; \
                    float rval = p_color_p[j]; \
                    float alpha = fabsf(val - rval) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if (!IsNaN(val)) \
                    { \
                        float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[j] += w; \
                        p_result[j] += val * w; \
                    } \
                } \
            } \
            else if(type == RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < l_border_num ; j++) \
                { \
                    float val = p_color_q[l_border_num-j]; \
                    float rval = p_color_p[j]; \
                    float alpha = fabsf(val - rval) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if (!IsNaN(val)) \
                    { \
                        float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[j] += w; \
                        p_result[j] += val * w; \
                    } \
                } \
            } \
            p_color_q = &p_color_q_t_off[p_sp_off_w[k]]; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                if(t_border_num == 0) \
                { \
                    for( j = l_border_num ; j < proc_width - r_border_num ; j++)\
                    { \
                        float val = p_color_q[j]; \
                        float rval = p_color_p[j]; \
                        float alpha = fabsf(val - rval) * scale_index; \
                        int idx = get_floor(alpha); \
                        alpha -= (float)idx; \
                        if (!IsNaN(val)) \
                        { \
                            float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                            p_sum_w[j] += w; \
                            p_result[j] += val * w; \
                        } \
                    } \
                } \
                else \
                { \
                    for( j = l_border_num ; j < proc_width - r_border_num ; j++)\
                    { \
                        float val = const_border_val; \
                        float rval = p_color_p[j]; \
                        float alpha = fabsf(val - rval) * scale_index; \
                        int idx = get_floor(alpha); \
                        alpha -= (float)idx; \
                        if (!IsNaN(val)) \
                        { \
                            float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                            p_sum_w[j] += w; \
                            p_result[j] += val * w; \
                        } \
                    } \
                } \
            } \
            else if (type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for( j = l_border_num ; j < proc_width - r_border_num ; j++)\
                { \
                    float val = p_color_q[j]; \
                    float rval = p_color_p[j]; \
                    float alpha = fabsf(val - rval) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if (!IsNaN(val)) \
                    { \
                        float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[j] += w; \
                        p_result[j] += val * w; \
                    } \
                } \
            } \
            p_color_q = &p_color_q_t_off[0]; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < r_border_num ; j ++) \
                { \
                    float val = const_border_val; \
                    float rval = p_color_p[proc_width -  r_border_num + j]; \
                    float alpha = fabsf(val - rval) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if (!IsNaN(val)) \
                    { \
                        float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[proc_width -  r_border_num + j] += w; \
                        p_result[proc_width -  r_border_num + j] += val * w; \
                    } \
                } \
            } \
            else if(type == RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < r_border_num ; j ++) \
                { \
                    float val = p_color_q[proc_width-2-j]; \
                    float rval = p_color_p[proc_width -  r_border_num + j]; \
                    float alpha = fabsf(val - rval) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if (!IsNaN(val)) \
                    { \
                        float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[proc_width -  r_border_num + j] += w; \
                        p_result[proc_width -  r_border_num + j] += val * w; \
                    } \
                } \
            } \
        } \
        p_color_p = &p_cal[0]; \
        for(j = 0 ; j < proc_width ; j++) \
        { \
            p_result[j] = IsNaN(p_color_p[j])? p_result[j] / p_sum_w[j] : (p_result[j] + p_color_p[j]) / (p_sum_w[j] + 1.f); \
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
                    float val = const_border_val; \
                    float rval = p_color_p[j]; \
                    float alpha = fabsf(val - rval) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if (!IsNaN(val)) \
                    { \
                        float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[j] += w; \
                        p_result[j] += val * w; \
                    } \
                } \
            } \
            else if(type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < l_border_num ; j++) \
                { \
                    float val = p_color_q[l_border_num-j]; \
                    float rval = p_color_p[j]; \
                    float alpha = fabsf(val - rval) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if (!IsNaN(val)) \
                    { \
                        float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[j] += w; \
                        p_result[j] += val * w; \
                    } \
                } \
            } \
            p_color_q = &p_color_q_d_off[p_sp_off_w[k]]; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                if(d_border_num == 0) \
                { \
                    for( j = l_border_num ; j < proc_width - r_border_num ; j++)\
                    { \
                        float val = p_color_q[j]; \
                        float rval = p_color_p[j]; \
                        float alpha = fabsf(val - rval) * scale_index; \
                        int idx = get_floor(alpha); \
                        alpha -= (float)idx; \
                        if (!IsNaN(val)) \
                        { \
                            float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] -      p_color_tbl[idx]))); \
                            p_sum_w[j] += w; \
                            p_result[j] += val * w; \
                        } \
                    } \
                } \
                else \
                { \
                    for( j = l_border_num ; j < proc_width - r_border_num ; j++)\
                    { \
                        float val = const_border_val; \
                        float rval = p_color_p[j]; \
                        float alpha = fabsf(val - rval) * scale_index; \
                        int idx = get_floor(alpha); \
                        alpha -= (float)idx; \
                        if (!IsNaN(val)) \
                        { \
                            float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] -      p_color_tbl[idx]))); \
                            p_sum_w[j] += w; \
                            p_result[j] += val * w; \
                        } \
                    } \
                }\
            } \
            else if(type == RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for( j = l_border_num ; j < proc_width - r_border_num ; j++)\
                { \
                    float val = p_color_q[j]; \
                    float rval = p_color_p[j]; \
                    float alpha = fabsf(val - rval) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if (!IsNaN(val)) \
                    { \
                        float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[j] += w; \
                        p_result[j] += val * w; \
                    } \
                } \
            } \
            p_color_q = &p_color_q_d_off[0]; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < r_border_num ; j ++) \
                { \
                    float val = const_border_val; \
                    float rval = p_color_p[proc_width -  r_border_num + j]; \
                    float alpha = fabsf(val - rval) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if (!IsNaN(val)) \
                    { \
                        float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[proc_width -  r_border_num + j] += w; \
                        p_result[proc_width -  r_border_num + j] += val * w; \
                    } \
                } \
            } \
            else if(type == RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < r_border_num ; j ++) \
                { \
                    float val = p_color_q[proc_width-2-j]; \
                    float rval = p_color_p[proc_width -  r_border_num + j]; \
                    float alpha = fabsf(val - rval) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if (!IsNaN(val)) \
                    { \
                        float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[proc_width -  r_border_num + j] += w; \
                        p_result[proc_width -  r_border_num + j] += val * w; \
                    } \
                } \
            } \
        } \
        p_color_p = &p_cal[0]; \
        for(j = 0 ; j < proc_width ; j++) \
        { \
            p_result[j] = IsNaN(p_color_p[j])? p_result[j] / p_sum_w[j] : (p_result[j] + p_color_p[j]) / (p_sum_w[j] + 1.f); \
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
                float alpha = fabsf(val - rval) * scale_index; \
                int idx = get_floor(alpha); \
                alpha -= (float)idx; \
                if (!IsNaN(val)) \
                { \
                    float w = range_val * (IsNaN(rval) ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                    p_sum_w[j] += w; \
                    p_result[j] += val * w; \
                } \
            } \
        } \
        p_color_p = &p_cal[0]; \
        for(j = radius ; j < proc_width - radius ; j++) \
        { \
            p_result[j] = IsNaN(p_color_p[j])? p_result[j] / p_sum_w[j] : (p_result[j] + p_color_p[j]) / (p_sum_w[j] + 1.f); \
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
                    float b = const_border_val, g = const_border_val, r = const_border_val; \
                    int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                    float rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                    int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                    float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if(!is_nan) \
                    { \
                        float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[w_idx] += w; \
                        p_result[j] += b*w ; \
                        p_result[j+1] += g*w ; \
                        p_result[j+2] += r*w ; \
                    } \
                } \
            } \
            else if(type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < l_border_num*n_ch ; j+=n_ch, w_idx++, q_shift++) \
                { \
                    float b = p_color_q[(l_border_num-q_shift)*n_ch], g = p_color_q[(l_border_num-q_shift)*n_ch+1], r = p_color_q[(l_border_num-q_shift)*n_ch+2]; \
                    int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                    float rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                    int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                    float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if(!is_nan) \
                    { \
                        float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[w_idx] += w; \
                        p_result[j] += b*w ; \
                        p_result[j+1] += g*w ; \
                        p_result[j+2] += r*w ; \
                    } \
                } \
            } \
            p_color_q = &p_cal[p_sp_off[k]]; \
            for(j = l_border_num*n_ch ; j < radius*n_ch ; j+=n_ch, w_idx++) \
            { \
                float b = p_color_q[j], g = p_color_q[j+1], r = p_color_q[j+2]; \
                int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                float rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                int idx = get_floor(alpha); \
                alpha -= (float)idx; \
                if(!is_nan) \
                { \
                    float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[    idx]))); \
                    p_sum_w[w_idx] += w; \
                    p_result[j] += b*w ; \
                    p_result[j+1] += g*w ; \
                    p_result[j+2] += r*w ; \
                } \
            } \
        } \
        p_color_p = &p_cal[0]; \
        w_idx = 0; \
        for(j = 0 ; j < radius*n_ch ; j+= n_ch, w_idx++) \
        { \
            float b = p_color_p[j], g = p_color_p[j+1], r = p_color_p[j+2]; \
            if(IsNaN(b) || IsNaN(g) || IsNaN(r)) \
            {   \
                p_sum_w[w_idx] = 1.f / p_sum_w[w_idx]; \
                p_result[j] = p_result[j]*p_sum_w[w_idx]; \
                p_result[j+1] = p_result[j+1]*p_sum_w[w_idx]; \
                p_result[j+2] = p_result[j+2]*p_sum_w[w_idx]; \
            } \
            else \
            { \
                p_sum_w[w_idx] = 1.f / (1.f + p_sum_w[w_idx]); \
                p_result[j] = (p_result[j] + b)*p_sum_w[w_idx]; \
                p_result[j+1] = (p_result[j+1] + g)*p_sum_w[w_idx]; \
                p_result[j+2] = (p_result[j+2] + r)*p_sum_w[w_idx]; \
            } \
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
                float b = p_color_q[j], g = p_color_q[j+1], r = p_color_q[j+2]; \
                int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                float rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                int idx = get_floor(alpha); \
                alpha -= (float)idx; \
                if(!is_nan) \
                { \
                    float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                    p_sum_w[w_idx] += w; \
                    p_result[j] += b*w ; \
                    p_result[j+1] += g*w ; \
                    p_result[j+2] += r*w ; \
                } \
            } \
            p_color_q = &p_cal[r_border]; \
            q_shift = 0 ; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < r_border_num*n_ch ; j+=n_ch, w_idx++, q_shift++) \
                { \
                    float b = const_border_val; \
                    float g = const_border_val; \
                    float r = const_border_val; \
                    int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                    float rb = p_color_p[(proc_width - r_border_num)*n_ch + j ] ;\
                    float rg = p_color_p[(proc_width - r_border_num)*n_ch + j + 1]; \
                    float rr = p_color_p[(proc_width - r_border_num)*n_ch + j + 2]; \
                    int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                    float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if(!is_nan) \
                    { \
                        float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[w_idx] += w; \
                        p_result[(proc_width - r_border_num)*n_ch+ j] += b*w ; \
                        p_result[(proc_width - r_border_num)*n_ch+ j+1] += g*w ; \
                        p_result[(proc_width - r_border_num)*n_ch+ j+2] += r*w ; \
                    } \
                } \
            } \
            else if (type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < r_border_num*n_ch ; j+=n_ch, w_idx++, q_shift++) \
                { \
                    float b = p_color_q[(proc_width-2-q_shift)*n_ch+0]; \
                    float g = p_color_q[(proc_width-2-q_shift)*n_ch+1]; \
                    float r = p_color_q[(proc_width-2-q_shift)*n_ch+2]; \
                    int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                    float rb = p_color_p[(proc_width - r_border_num)*n_ch + j ] ;\
                    float rg = p_color_p[(proc_width - r_border_num)*n_ch + j + 1]; \
                    float rr = p_color_p[(proc_width - r_border_num)*n_ch + j + 2]; \
                    int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                    float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if(!is_nan) \
                    { \
                        float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[w_idx] += w; \
                        p_result[(proc_width - r_border_num)*n_ch+ j] += b*w ; \
                        p_result[(proc_width - r_border_num)*n_ch+ j+1] += g*w ; \
                        p_result[(proc_width - r_border_num)*n_ch+ j+2] += r*w ; \
                    } \
                } \
            } \
        } \
        p_color_p = &p_cal[0]; \
        w_idx = proc_width - radius; \
        for(j = (proc_width - radius)*n_ch; j < proc_width*n_ch ; j += n_ch, w_idx++) \
        { \
            float b = p_color_p[j], g = p_color_p[j+1], r = p_color_p[j+2]; \
            if(IsNaN(b) || IsNaN(g) || IsNaN(r)) \
            {   \
                p_sum_w[w_idx] = 1.f / p_sum_w[w_idx]; \
                p_result[j] = p_result[j]*p_sum_w[w_idx]; \
                p_result[j+1] = p_result[j+1]*p_sum_w[w_idx]; \
                p_result[j+2] = p_result[j+2]*p_sum_w[w_idx]; \
            } \
            else \
            { \
                p_sum_w[w_idx] = 1.f / (1.f + p_sum_w[w_idx]); \
                p_result[j] = (p_result[j] + b)*p_sum_w[w_idx]; \
                p_result[j+1] = (p_result[j+1] + g)*p_sum_w[w_idx]; \
                p_result[j+2] = (p_result[j+2] + r)*p_sum_w[w_idx]; \
            } \
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
                    float b = const_border_val; \
                    float g = const_border_val; \
                    float r = const_border_val; \
                    int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                    float rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                    int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                    float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if(!is_nan) \
                    { \
                        float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[w_idx++] += w; \
                        p_result[j] += b*w ; \
                        p_result[j+1] += g*w ; \
                        p_result[j+2] += r*w ; \
                    } \
                } \
            } \
            else if (type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < l_border_num*n_ch ; j+=n_ch, q_shift++) \
                { \
                    float b = p_color_q[(l_border_num-q_shift)*n_ch]; \
                    float g = p_color_q[(l_border_num-q_shift)*n_ch+1]; \
                    float r = p_color_q[(l_border_num-q_shift)*n_ch+2]; \
                    int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                    float rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                    int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                    float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if(!is_nan) \
                    { \
                        float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[w_idx++] += w; \
                        p_result[j] += b*w ; \
                        p_result[j+1] += g*w ; \
                        p_result[j+2] += r*w ; \
                    } \
                } \
            } \
            p_color_q = &p_color_q_t_off[p_sp_off_w[k]*n_ch]; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                if(t_border_num==0) \
                { \
                    for(j = l_border_num*n_ch ; j < (proc_width - r_border_num)*n_ch ; j+=n_ch) \
                    { \
                        float b = p_color_q[j], g = p_color_q[j+1], r = p_color_q[j+2]; \
                        int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                        float rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                        int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                        float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                        int idx = get_floor(alpha); \
                        alpha -= (float)idx; \
                        if(!is_nan) \
                        { \
                            float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[    idx]))); \
                            p_sum_w[w_idx++] += w; \
                            p_result[j] += b*w ; \
                            p_result[j+1] += g*w ; \
                            p_result[j+2] += r*w ; \
                        } \
                    } \
                } \
                else \
                { \
                    for(j = l_border_num*n_ch ; j < (proc_width - r_border_num)*n_ch ; j+=n_ch) \
                    { \
                        float b = const_border_val, g = const_border_val, r = const_border_val; \
                        int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                        float rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                        int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                        float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                        int idx = get_floor(alpha); \
                        alpha -= (float)idx; \
                        if(!is_nan) \
                        { \
                            float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[    idx]))); \
                            p_sum_w[w_idx++] += w; \
                            p_result[j] += b*w ; \
                            p_result[j+1] += g*w ; \
                            p_result[j+2] += r*w ; \
                        } \
                    } \
                } \
            } \
            else if(type == RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = l_border_num*n_ch ; j < (proc_width - r_border_num)*n_ch ; j+=n_ch) \
                { \
                    float b = p_color_q[j], g = p_color_q[j+1], r = p_color_q[j+2]; \
                    int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                    float rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                    int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                    float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if(!is_nan) \
                    { \
                        float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[    idx]))); \
                        p_sum_w[w_idx++] += w; \
                        p_result[j] += b*w ; \
                        p_result[j+1] += g*w ; \
                        p_result[j+2] += r*w ; \
                    } \
                } \
            } \
            p_color_q = &p_color_q_t_off[0]; \
            q_shift = 0 ; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < r_border_num*n_ch ; j+=n_ch) \
                { \
                    float b = const_border_val; \
                    float g = const_border_val; \
                    float r = const_border_val; \
                    int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                    float rb = p_color_p[(proc_width - r_border_num)*n_ch + j ] ;\
                    float rg = p_color_p[(proc_width - r_border_num)*n_ch + j + 1]; \
                    float rr = p_color_p[(proc_width - r_border_num)*n_ch + j + 2]; \
                    int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                    float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if(!is_nan) \
                    { \
                        float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[w_idx++] += w; \
                        p_result[(proc_width - r_border_num)*n_ch+ j] += b*w ; \
                        p_result[(proc_width - r_border_num)*n_ch+ j+1] += g*w ; \
                        p_result[(proc_width - r_border_num)*n_ch+ j+2] += r*w ; \
                    } \
                } \
            } \
            else if (type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < r_border_num*n_ch ; j+=n_ch, q_shift++) \
                { \
                    float b = p_color_q[(proc_width-2-q_shift)*n_ch+0]; \
                    float g = p_color_q[(proc_width-2-q_shift)*n_ch+1]; \
                    float r = p_color_q[(proc_width-2-q_shift)*n_ch+2]; \
                    int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                    float rb = p_color_p[(proc_width - r_border_num)*n_ch + j ] ;\
                    float rg = p_color_p[(proc_width - r_border_num)*n_ch + j + 1]; \
                    float rr = p_color_p[(proc_width - r_border_num)*n_ch + j + 2]; \
                    int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                    float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if(!is_nan) \
                    { \
                        float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[w_idx++] += w; \
                        p_result[(proc_width - r_border_num)*n_ch+ j] += b*w ; \
                        p_result[(proc_width - r_border_num)*n_ch+ j+1] += g*w ; \
                        p_result[(proc_width - r_border_num)*n_ch+ j+2] += r*w ; \
                    } \
                } \
            } \
        } \
        p_color_p = &p_cal[0]; \
        w_idx = 0; \
        for(j = 0; j < proc_width*n_ch ; j += n_ch, w_idx++) \
        { \
            float b = p_color_p[j], g = p_color_p[j+1], r = p_color_p[j+2]; \
            if(IsNaN(b) || IsNaN(g) || IsNaN(r)) \
            {   \
                p_sum_w[w_idx] = 1.f / p_sum_w[w_idx]; \
                p_result[j] = p_result[j]*p_sum_w[w_idx]; \
                p_result[j+1] = p_result[j+1]*p_sum_w[w_idx]; \
                p_result[j+2] = p_result[j+2]*p_sum_w[w_idx]; \
            } \
            else \
            { \
                p_sum_w[w_idx] = 1.f / (1.f + p_sum_w[w_idx]); \
                p_result[j] = (p_result[j] + b)*p_sum_w[w_idx]; \
                p_result[j+1] = (p_result[j+1] + g)*p_sum_w[w_idx]; \
                p_result[j+2] = (p_result[j+2] + r)*p_sum_w[w_idx]; \
            } \
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
                    float b = const_border_val; \
                    float g = const_border_val; \
                    float r = const_border_val; \
                    int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                    float rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                    int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                    float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if(!is_nan) \
                    { \
                        float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[w_idx++] += w; \
                        p_result[j] += b*w ; \
                        p_result[j+1] += g*w ; \
                        p_result[j+2] += r*w ; \
                    } \
                } \
            } \
            else if (type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < l_border_num*n_ch ; j+=n_ch, q_shift++) \
                { \
                    float b = p_color_q[(l_border_num-q_shift)*n_ch]; \
                    float g = p_color_q[(l_border_num-q_shift)*n_ch+1]; \
                    float r = p_color_q[(l_border_num-q_shift)*n_ch+2]; \
                    int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                    float rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                    int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                    float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if(!is_nan) \
                    { \
                        float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[w_idx++] += w; \
                        p_result[j] += b*w ; \
                        p_result[j+1] += g*w ; \
                        p_result[j+2] += r*w ; \
                    } \
                } \
            } \
            p_color_q = &p_color_q_t_off[p_sp_off_w[k]*n_ch]; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                if(d_border_num==0) \
                { \
                    for(j = l_border_num*n_ch ; j < (proc_width - r_border_num)*n_ch ; j+=n_ch) \
                    { \
                        float b = p_color_q[j], g = p_color_q[j+1], r = p_color_q[j+2]; \
                        int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                        float rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                        int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                        float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                        int idx = get_floor(alpha); \
                        alpha -= (float)idx; \
                        if(!is_nan) \
                        { \
                            float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[    idx]))); \
                            p_sum_w[w_idx++] += w; \
                            p_result[j] += b*w ; \
                            p_result[j+1] += g*w ; \
                            p_result[j+2] += r*w ; \
                        } \
                    } \
                } \
                else \
                { \
                    for(j = l_border_num*n_ch ; j < (proc_width - r_border_num)*n_ch ; j+=n_ch) \
                    { \
                        float b = const_border_val, g = const_border_val, r = const_border_val; \
                        int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                        float rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                        int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                        float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                        int idx = get_floor(alpha); \
                        alpha -= (float)idx; \
                        if(!is_nan) \
                        { \
                            float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[    idx]))); \
                            p_sum_w[w_idx++] += w; \
                            p_result[j] += b*w ; \
                            p_result[j+1] += g*w ; \
                            p_result[j+2] += r*w ; \
                        } \
                    } \
                } \
            } \
            else if(type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = l_border_num*n_ch ; j < (proc_width - r_border_num)*n_ch ; j+=n_ch) \
                { \
                    float b = p_color_q[j], g = p_color_q[j+1], r = p_color_q[j+2]; \
                    int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                    float rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                    int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                    float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if(!is_nan) \
                    { \
                        float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[    idx]))); \
                        p_sum_w[w_idx++] += w; \
                        p_result[j] += b*w ; \
                        p_result[j+1] += g*w ; \
                        p_result[j+2] += r*w ; \
                    } \
                } \
            } \
            p_color_q = &p_color_q_t_off[0]; \
            q_shift = 0 ; \
            if(type == RISCV_VEC_IMG_BORDER_CONSTANT) \
            { \
                for(j = 0 ; j < r_border_num*n_ch ; j+=n_ch, q_shift++) \
                { \
                    float b = const_border_val; \
                    float g = const_border_val; \
                    float r = const_border_val; \
                    int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                    float rb = p_color_p[(proc_width - r_border_num)*n_ch + j ] ;\
                    float rg = p_color_p[(proc_width - r_border_num)*n_ch + j + 1]; \
                    float rr = p_color_p[(proc_width - r_border_num)*n_ch + j + 2]; \
                    int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                    float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if(!is_nan) \
                    { \
                        float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[w_idx++] += w; \
                        p_result[(proc_width - r_border_num)*n_ch+ j] += b*w ; \
                        p_result[(proc_width - r_border_num)*n_ch+ j+1] += g*w ; \
                        p_result[(proc_width - r_border_num)*n_ch+ j+2] += r*w ; \
                    } \
                } \
            } \
            else if (type ==RISCV_VEC_IMG_BORDER_REFLECT101) \
            { \
                for(j = 0 ; j < r_border_num*n_ch ; j+=n_ch, q_shift++) \
                { \
                    float b = p_color_q[(proc_width-2-q_shift)*n_ch+0]; \
                    float g = p_color_q[(proc_width-2-q_shift)*n_ch+1]; \
                    float r = p_color_q[(proc_width-2-q_shift)*n_ch+2]; \
                    int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                    float rb = p_color_p[(proc_width - r_border_num)*n_ch + j ] ;\
                    float rg = p_color_p[(proc_width - r_border_num)*n_ch + j + 1]; \
                    float rr = p_color_p[(proc_width - r_border_num)*n_ch + j + 2]; \
                    int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                    float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                    int idx = get_floor(alpha); \
                    alpha -= (float)idx; \
                    if(!is_nan) \
                    { \
                        float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                        p_sum_w[w_idx++] += w; \
                        p_result[(proc_width - r_border_num)*n_ch+ j] += b*w ; \
                        p_result[(proc_width - r_border_num)*n_ch+ j+1] += g*w ; \
                        p_result[(proc_width - r_border_num)*n_ch+ j+2] += r*w ; \
                    } \
                } \
            } \
        } \
        p_color_p = &p_cal[0]; \
        w_idx = 0; \
        for(j = 0; j < proc_width*n_ch ; j += n_ch, w_idx++) \
        { \
            float b = p_color_p[j], g = p_color_p[j+1], r = p_color_p[j+2]; \
            if(IsNaN(b) || IsNaN(g) || IsNaN(r)) \
            {   \
                p_sum_w[w_idx] = 1.f / p_sum_w[w_idx]; \
                p_result[j] = p_result[j]*p_sum_w[w_idx]; \
                p_result[j+1] = p_result[j+1]*p_sum_w[w_idx]; \
                p_result[j+2] = p_result[j+2]*p_sum_w[w_idx]; \
            } \
            else \
            { \
                p_sum_w[w_idx] = 1.f / (1.f + p_sum_w[w_idx]); \
                p_result[j] = (p_result[j] + b)*p_sum_w[w_idx]; \
                p_result[j+1] = (p_result[j+1] + g)*p_sum_w[w_idx]; \
                p_result[j+2] = (p_result[j+2] + r)*p_sum_w[w_idx]; \
            } \
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
                float b = p_color_q[j], g = p_color_q[j+1], r = p_color_q[j+2]; \
                int32_t is_nan = IsNaN(b) || IsNaN(g) || IsNaN(r); \
                float rb = p_color_p[j], rg = p_color_p[j+1], rr= p_color_p[j+2]; \
                int32_t r_is_nan = IsNaN(rb) || IsNaN(rg) || IsNaN(rr); \
                float alpha = (fabsf(b-rb) + fabsf(g-rg) + fabsf(r-rr)) * scale_index; \
                int idx = get_floor(alpha); \
                alpha -= (float)idx; \
                if(!is_nan) \
                { \
                    float w = range_val * (r_is_nan ? 1.f : (p_color_tbl[idx] + alpha*(p_color_tbl[idx + 1] - p_color_tbl[idx]))); \
                    p_sum_w[w_idx] += w; \
                    p_result[j] += b*w ; \
                    p_result[j+1] += g*w ; \
                    p_result[j+2] += r*w ; \
                } \
            } \
        } \
        p_color_p = &p_cal[0]; \
        w_idx = radius; \
        for (j = radius*n_ch; j < (proc_width - radius)*n_ch; j+=n_ch, w_idx++) \
        { \
            float b = p_color_p[j], g = p_color_p[j+1], r = p_color_p[j+2]; \
            if(IsNaN(b) || IsNaN(g) || IsNaN(r)) \
            {   \
                p_sum_w[w_idx] = 1.f / p_sum_w[w_idx]; \
                p_result[j] = p_result[j]*p_sum_w[w_idx]; \
                p_result[j+1] = p_result[j+1]*p_sum_w[w_idx]; \
                p_result[j+2] = p_result[j+2]*p_sum_w[w_idx]; \
            } \
            else \
            { \
                p_sum_w[w_idx] = 1.f / (1.f + p_sum_w[w_idx]); \
                p_result[j] = (p_result[j] + b)*p_sum_w[w_idx]; \
                p_result[j+1] = (p_result[j+1] + g)*p_sum_w[w_idx]; \
                p_result[j+2] = (p_result[j+2] + r)*p_sum_w[w_idx]; \
            } \
        } \
    }

#endif //__INTERNAL_VEC_IMAGE_BILATERAL_TEMPLATE_PUREC_F32_H__

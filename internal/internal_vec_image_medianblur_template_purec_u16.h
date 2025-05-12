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

static inline void vec_min_max_u16(SRC_TYPE *x, SRC_TYPE *y)
{
    SRC_TYPE tmp = *x;
    *x = ((*x <= *y) ? *x : *y);
    *y = ((*y < tmp) ? tmp : *y);
}

static inline void vec_sortnet_3(SRC_TYPE *p_cal)
{
    vec_min_max_u16(&p_cal[1], &p_cal[2]);
    vec_min_max_u16(&p_cal[4], &p_cal[5]);
    vec_min_max_u16(&p_cal[7], &p_cal[8]);
    vec_min_max_u16(&p_cal[0], &p_cal[1]);
    vec_min_max_u16(&p_cal[3], &p_cal[4]);
    vec_min_max_u16(&p_cal[6], &p_cal[7]);
    vec_min_max_u16(&p_cal[1], &p_cal[2]);
    vec_min_max_u16(&p_cal[4], &p_cal[5]);
    vec_min_max_u16(&p_cal[7], &p_cal[8]);
    vec_min_max_u16(&p_cal[0], &p_cal[3]);
    vec_min_max_u16(&p_cal[5], &p_cal[8]);
    vec_min_max_u16(&p_cal[4], &p_cal[7]);
    vec_min_max_u16(&p_cal[3], &p_cal[6]);
    vec_min_max_u16(&p_cal[1], &p_cal[4]);
    vec_min_max_u16(&p_cal[2], &p_cal[5]);
    vec_min_max_u16(&p_cal[4], &p_cal[7]);
    vec_min_max_u16(&p_cal[4], &p_cal[2]);
    vec_min_max_u16(&p_cal[6], &p_cal[4]);
    vec_min_max_u16(&p_cal[4], &p_cal[2]);
}

static inline void vec_sortnet_5(SRC_TYPE *p_cal)
{
    vec_min_max_u16(&p_cal[1], &p_cal[2]);
    vec_min_max_u16(&p_cal[0], &p_cal[1]);
    vec_min_max_u16(&p_cal[1], &p_cal[2]);
    vec_min_max_u16(&p_cal[4], &p_cal[5]);
    vec_min_max_u16(&p_cal[3], &p_cal[4]);
    vec_min_max_u16(&p_cal[4], &p_cal[5]);
    vec_min_max_u16(&p_cal[0], &p_cal[3]);
    vec_min_max_u16(&p_cal[2], &p_cal[5]);
    vec_min_max_u16(&p_cal[2], &p_cal[3]);
    vec_min_max_u16(&p_cal[1], &p_cal[4]);
    vec_min_max_u16(&p_cal[1], &p_cal[2]);
    vec_min_max_u16(&p_cal[3], &p_cal[4]);
    vec_min_max_u16(&p_cal[7], &p_cal[8]);
    vec_min_max_u16(&p_cal[6], &p_cal[7]);
    vec_min_max_u16(&p_cal[7], &p_cal[8]);
    vec_min_max_u16(&p_cal[10], &p_cal[11]);
    vec_min_max_u16(&p_cal[9], &p_cal[10]);
    vec_min_max_u16(&p_cal[10], &p_cal[11]);
    vec_min_max_u16(&p_cal[6], &p_cal[9]);
    vec_min_max_u16(&p_cal[8], &p_cal[11]);
    vec_min_max_u16(&p_cal[8], &p_cal[9]);
    vec_min_max_u16(&p_cal[7], &p_cal[10]);
    vec_min_max_u16(&p_cal[7], &p_cal[8]);
    vec_min_max_u16(&p_cal[9], &p_cal[10]);
    vec_min_max_u16(&p_cal[0], &p_cal[6]);
    vec_min_max_u16(&p_cal[4], &p_cal[10]);
    vec_min_max_u16(&p_cal[4], &p_cal[6]);
    vec_min_max_u16(&p_cal[2], &p_cal[8]);
    vec_min_max_u16(&p_cal[2], &p_cal[4]);
    vec_min_max_u16(&p_cal[6], &p_cal[8]);
    vec_min_max_u16(&p_cal[1], &p_cal[7]);
    vec_min_max_u16(&p_cal[5], &p_cal[11]);
    vec_min_max_u16(&p_cal[5], &p_cal[7]);
    vec_min_max_u16(&p_cal[3], &p_cal[9]);
    vec_min_max_u16(&p_cal[3], &p_cal[5]);
    vec_min_max_u16(&p_cal[7], &p_cal[9]);
    vec_min_max_u16(&p_cal[1], &p_cal[2]);
    vec_min_max_u16(&p_cal[3], &p_cal[4]);
    vec_min_max_u16(&p_cal[5], &p_cal[6]);
    vec_min_max_u16(&p_cal[7], &p_cal[8]);
    vec_min_max_u16(&p_cal[9], &p_cal[10]);
    vec_min_max_u16(&p_cal[13], &p_cal[14]);
    vec_min_max_u16(&p_cal[12], &p_cal[13]);
    vec_min_max_u16(&p_cal[13], &p_cal[14]);
    vec_min_max_u16(&p_cal[16], &p_cal[17]);
    vec_min_max_u16(&p_cal[15], &p_cal[16]);
    vec_min_max_u16(&p_cal[16], &p_cal[17]);
    vec_min_max_u16(&p_cal[12], &p_cal[15]);
    vec_min_max_u16(&p_cal[14], &p_cal[17]);
    vec_min_max_u16(&p_cal[14], &p_cal[15]);
    vec_min_max_u16(&p_cal[13], &p_cal[16]);
    vec_min_max_u16(&p_cal[13], &p_cal[14]);
    vec_min_max_u16(&p_cal[15], &p_cal[16]);
    vec_min_max_u16(&p_cal[19], &p_cal[20]);
    vec_min_max_u16(&p_cal[18], &p_cal[19]);
    vec_min_max_u16(&p_cal[19], &p_cal[20]);
    vec_min_max_u16(&p_cal[21], &p_cal[22]);
    vec_min_max_u16(&p_cal[23], &p_cal[24]);
    vec_min_max_u16(&p_cal[21], &p_cal[23]);
    vec_min_max_u16(&p_cal[22], &p_cal[24]);
    vec_min_max_u16(&p_cal[22], &p_cal[23]);
    vec_min_max_u16(&p_cal[18], &p_cal[21]);
    vec_min_max_u16(&p_cal[20], &p_cal[23]);
    vec_min_max_u16(&p_cal[20], &p_cal[21]);
    vec_min_max_u16(&p_cal[19], &p_cal[22]);
    vec_min_max_u16(&p_cal[22], &p_cal[24]);
    vec_min_max_u16(&p_cal[19], &p_cal[20]);
    vec_min_max_u16(&p_cal[21], &p_cal[22]);
    vec_min_max_u16(&p_cal[23], &p_cal[24]);
    vec_min_max_u16(&p_cal[12], &p_cal[18]);
    vec_min_max_u16(&p_cal[16], &p_cal[22]);
    vec_min_max_u16(&p_cal[16], &p_cal[18]);
    vec_min_max_u16(&p_cal[14], &p_cal[20]);
    vec_min_max_u16(&p_cal[20], &p_cal[24]);
    vec_min_max_u16(&p_cal[14], &p_cal[16]);
    vec_min_max_u16(&p_cal[18], &p_cal[20]);
    vec_min_max_u16(&p_cal[22], &p_cal[24]);
    vec_min_max_u16(&p_cal[13], &p_cal[19]);
    vec_min_max_u16(&p_cal[17], &p_cal[23]);
    vec_min_max_u16(&p_cal[17], &p_cal[19]);
    vec_min_max_u16(&p_cal[15], &p_cal[21]);
    vec_min_max_u16(&p_cal[15], &p_cal[17]);
    vec_min_max_u16(&p_cal[19], &p_cal[21]);
    vec_min_max_u16(&p_cal[13], &p_cal[14]);
    vec_min_max_u16(&p_cal[15], &p_cal[16]);
    vec_min_max_u16(&p_cal[17], &p_cal[18]);
    vec_min_max_u16(&p_cal[19], &p_cal[20]);
    vec_min_max_u16(&p_cal[21], &p_cal[22]);
    vec_min_max_u16(&p_cal[23], &p_cal[24]);
    vec_min_max_u16(&p_cal[0], &p_cal[12]);
    vec_min_max_u16(&p_cal[8], &p_cal[20]);
    vec_min_max_u16(&p_cal[8], &p_cal[12]);
    vec_min_max_u16(&p_cal[4], &p_cal[16]);
    vec_min_max_u16(&p_cal[16], &p_cal[24]);
    vec_min_max_u16(&p_cal[12], &p_cal[16]);
    vec_min_max_u16(&p_cal[2], &p_cal[14]);
    vec_min_max_u16(&p_cal[10], &p_cal[22]);
    vec_min_max_u16(&p_cal[10], &p_cal[14]);
    vec_min_max_u16(&p_cal[6], &p_cal[18]);
    vec_min_max_u16(&p_cal[6], &p_cal[10]);
    vec_min_max_u16(&p_cal[10], &p_cal[12]);
    vec_min_max_u16(&p_cal[1], &p_cal[13]);
    vec_min_max_u16(&p_cal[9], &p_cal[21]);
    vec_min_max_u16(&p_cal[9], &p_cal[13]);
    vec_min_max_u16(&p_cal[5], &p_cal[17]);
    vec_min_max_u16(&p_cal[13], &p_cal[17]);
    vec_min_max_u16(&p_cal[3], &p_cal[15]);
    vec_min_max_u16(&p_cal[11], &p_cal[23]);
    vec_min_max_u16(&p_cal[11], &p_cal[15]);
    vec_min_max_u16(&p_cal[7], &p_cal[19]);
    vec_min_max_u16(&p_cal[7], &p_cal[11]);
    vec_min_max_u16(&p_cal[11], &p_cal[13]);
    vec_min_max_u16(&p_cal[11], &p_cal[12]);
}

#define MEDIANBLUR_INIT_C \
    SRC_TYPE * p_src = img_src.data;\
    SRC_TYPE * p_cal = NULL;\
    SRC_TYPE * p_in = NULL; \
    DST_TYPE * p_result = NULL;\
    DST_TYPE * p_out = img_dst.data; \
    int32_t proc_height = img_src.height; \
    int32_t proc_width  = img_src.width; \
    int32_t n_ch       = img_src.channels; \
    int32_t t_border_num = 0, d_border_num = 0, l_border_num = 0, r_border_num = 0; \
    int32_t h_idx = 0, w_idx= 0, ch_idx = 0;


#define MEDIANBLUR_S_K_BORDER_R_C \
    if(ksize == 3) \
    { \
        for(i = 0  ; i < radius  ; i++) \
        { \
            t_border_num = (radius - i) ; \
            for( j = proc_width-radius ; j <  proc_width ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                r_border_num = (j+radius)-proc_width+1 ; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    k = 0; \
                    w_idx = j-radius; \
                    for(kh = 0 ; kh < t_border_num ; kh++) \
                    { \
                        p_in = &p_src[(w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < ksize - (r_border_num) ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch] ; \
                            k++; \
                        } \
                        for(kw = ksize - r_border_num ; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[(ksize-r_border_num-1)*n_ch]; \
                            k++; \
                        } \
                    } \
                    h_idx = 0 ; \
                    w_idx = j-radius; \
                    for(kh = (t_border_num) ; kh < ksize ; kh++) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < ksize - (r_border_num) ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        for(kw = ksize - r_border_num ; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[(ksize-r_border_num-1)*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    vec_sortnet_3(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
        for( i = radius ; i < proc_height - radius ; i++) \
        { \
            for( j = proc_width-radius ; j <  proc_width ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                r_border_num = (j+radius)-proc_width+1; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    h_idx = i-radius; \
                    w_idx = j-radius; \
                    k = 0 ; \
                    for(kh = 0 ; kh < ksize ; kh++) \
                    { \
                        p_in = &p_src[(h_idx*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < ksize - (r_border_num) ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        for(kw = ksize - r_border_num ; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[(ksize-r_border_num-1)*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    vec_sortnet_3(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
        for( i = proc_height - radius ; i < proc_height ; i++) \
        { \
            d_border_num = (i+radius)-proc_height+1 ; \
            for( j = proc_width-radius ; j <  proc_width ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                r_border_num = (j+radius)-proc_width+1; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    k = 0; \
                    h_idx = i-radius; \
                    w_idx = j-radius; \
                    for(kh = 0 ; kh < ksize - d_border_num ; kh++ ) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0 ; kw < ksize - r_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        for(kw = ksize - r_border_num ; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[(ksize-r_border_num-1)*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    h_idx = proc_height - 1 ; \
                    w_idx = j-radius; \
                    for(kh =  ksize - d_border_num ; kh<ksize ; kh++) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0 ; kw < ksize - r_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        for(kw = ksize - r_border_num ; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[(ksize-r_border_num-1)*n_ch]; \
                            k++; \
                        } \
                    } \
                    vec_sortnet_3(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
    } \
    else if(ksize == 5) \
    { \
        for(i = 0  ; i < radius  ; i++) \
        { \
            t_border_num = (radius - i) ; \
            for( j = proc_width-radius ; j <  proc_width ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                r_border_num = (j+radius)-proc_width+1 ; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    k = 0; \
                    w_idx = j-radius; \
                    for(kh = 0 ; kh < t_border_num ; kh++) \
                    { \
                        p_in = &p_src[(w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < ksize - (r_border_num) ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch] ; \
                            k++; \
                        } \
                        for(kw = ksize - r_border_num ; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[(ksize-r_border_num-1)*n_ch]; \
                            k++; \
                        } \
                    } \
                    h_idx = 0 ; \
                    w_idx = j-radius; \
                    for(kh = (t_border_num) ; kh < ksize ; kh++) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < ksize - (r_border_num) ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        for(kw = ksize - r_border_num ; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[(ksize-r_border_num-1)*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    vec_sortnet_5(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
        for( i = radius ; i < proc_height - radius ; i++) \
        { \
            for( j = proc_width-radius ; j <  proc_width ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                r_border_num = (j+radius)-proc_width+1; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    h_idx = i-radius; \
                    w_idx = j-radius; \
                    k = 0 ; \
                    for(kh = 0 ; kh < ksize ; kh++) \
                    { \
                        p_in = &p_src[(h_idx*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < ksize - (r_border_num) ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        for(kw = ksize - r_border_num ; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[(ksize-r_border_num-1)*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    vec_sortnet_5(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
        for( i = proc_height - radius ; i < proc_height ; i++) \
        { \
            d_border_num = (i+radius)-proc_height+1 ; \
            for( j = proc_width-radius ; j <  proc_width ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                r_border_num = (j+radius)-proc_width+1; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    k = 0; \
                    h_idx = i-radius; \
                    w_idx = j-radius; \
                    for(kh = 0 ; kh < ksize - d_border_num ; kh++ ) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0 ; kw < ksize - r_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        for(kw = ksize - r_border_num ; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[(ksize-r_border_num-1)*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    h_idx = proc_height - 1 ; \
                    w_idx = j-radius; \
                    for(kh =  ksize - d_border_num ; kh<ksize ; kh++) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0 ; kw < ksize - r_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        for(kw = ksize - r_border_num ; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[(ksize-r_border_num-1)*n_ch]; \
                            k++; \
                        } \
                    } \
                    vec_sortnet_5(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
    }


#define MEDIANBLUR_S_K_BORDER_L_C \
    if(ksize == 3) \
    { \
        for(i = 0  ; i < radius  ; i++) \
        { \
            t_border_num = (radius - i) ; \
            for( j = 0 ; j <  radius ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                l_border_num = radius -j ; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    k = 0; \
                    w_idx = 0; \
                    for(kh = 0 ; kh < t_border_num ; kh++) \
                    { \
                        p_in = &p_src[(w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < l_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[0]; \
                            k++;\
                        } \
                        for(kw = 0; kw < (ksize-l_border_num) ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch] ; \
                            k++; \
                        } \
                    } \
                    h_idx = 0 ; \
                    w_idx = 0; \
                    for(kh = (t_border_num) ; kh < ksize ; kh++) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < l_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[0]; \
                            k++; \
                        } \
                        for(kw = 0; kw < (ksize-l_border_num) ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    vec_sortnet_3(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
        for( i = radius ; i < proc_height - radius ; i++) \
        { \
            for( j = 0 ; j <  radius ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                l_border_num = radius -j ; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    h_idx = i-radius; \
                    w_idx = 0; \
                    k = 0 ; \
                    for(kh = 0 ; kh < ksize ; kh++) \
                    { \
                        p_in = &p_src[(h_idx*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0 ; kw < l_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[0]; \
                            k++;\
                        } \
                        for(kw = 0 ; kw < ksize - l_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    vec_sortnet_3(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
        for( i = proc_height - radius ; i < proc_height ; i++) \
        { \
            d_border_num = (i+radius)-proc_height+1 ; \
            for( j = 0 ; j <  radius ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                l_border_num = radius-j; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    k = 0; \
                    h_idx = i-radius; \
                    w_idx = 0; \
                    for(kh = 0 ; kh < ksize - d_border_num ; kh++ ) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0 ; kw < l_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[0]; \
                            k++; \
                        } \
                        for(kw = 0 ; kw < ksize - l_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    h_idx = proc_height - 1 ; \
                    w_idx = 0; \
                    for(kh =  ksize - d_border_num ; kh<ksize ; kh++) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0 ; kw < l_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[0]; \
                            k++; \
                        } \
                        for(kw = 0; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                    } \
                    vec_sortnet_3(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
    } \
    else if(ksize == 5) \
    { \
        for(i = 0  ; i < radius  ; i++) \
        { \
            t_border_num = (radius - i) ; \
            for( j = 0 ; j <  radius ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                l_border_num = radius -j ; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    k = 0; \
                    w_idx = 0; \
                    for(kh = 0 ; kh < t_border_num ; kh++) \
                    { \
                        p_in = &p_src[(w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < l_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[0]; \
                            k++;\
                        } \
                        for(kw = 0; kw < (ksize-l_border_num) ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch] ; \
                            k++; \
                        } \
                    } \
                    h_idx = 0 ; \
                    w_idx = 0; \
                    for(kh = (t_border_num) ; kh < ksize ; kh++) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < l_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[0]; \
                            k++; \
                        } \
                        for(kw = 0; kw < (ksize-l_border_num) ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    vec_sortnet_5(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
        for( i = radius ; i < proc_height - radius ; i++) \
        { \
            for( j = 0 ; j <  radius ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                l_border_num = radius -j ; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    h_idx = i-radius; \
                    w_idx = 0; \
                    k = 0 ; \
                    for(kh = 0 ; kh < ksize ; kh++) \
                    { \
                        p_in = &p_src[(h_idx*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0 ; kw < l_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[0]; \
                            k++;\
                        } \
                        for(kw = 0 ; kw < ksize - l_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    vec_sortnet_5(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
        for( i = proc_height - radius ; i < proc_height ; i++) \
        { \
            d_border_num = (i+radius)-proc_height+1 ; \
            for( j = 0 ; j <  radius ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                l_border_num = radius-j; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    k = 0; \
                    h_idx = i-radius; \
                    w_idx = 0; \
                    for(kh = 0 ; kh < ksize - d_border_num ; kh++ ) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0 ; kw < l_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[0]; \
                            k++; \
                        } \
                        for(kw = 0 ; kw < ksize - l_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    h_idx = proc_height - 1 ; \
                    w_idx = 0; \
                    for(kh =  ksize - d_border_num ; kh<ksize ; kh++) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0 ; kw < l_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[0]; \
                            k++; \
                        } \
                        for(kw = 0 ; kw < ksize - l_border_num ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                    } \
                    vec_sortnet_5(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
    }

#define MEDIANBLUR_S_K_MIDDLE_C \
    if(ksize == 3) \
    { \
        for(i = 0  ; i < radius  ; i++) \
        { \
            t_border_num = (radius - i) ; \
            for( j = radius ; j < proc_width - radius ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    k = 0; \
                    w_idx = j-radius; \
                    for(kh = 0 ; kh < t_border_num ; kh++) \
                    { \
                        p_in = &p_src[(w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch] ; \
                            k++ ; \
                        } \
                    } \
                    h_idx = 0 ; \
                    w_idx = j-radius; \
                    for(kh = (t_border_num) ; kh < ksize ; kh++) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    vec_sortnet_3(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
        for( i = radius ; i < proc_height - radius ; i++) \
        { \
            for( j = radius ; j < proc_width - radius ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    h_idx = i-radius; \
                    w_idx = j-radius; \
                    k = 0 ; \
                    for(kh = 0 ; kh < ksize ; kh++) \
                    { \
                        p_in = &p_src[(h_idx*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    vec_sortnet_3(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
        for( i = proc_height - radius ; i < proc_height ; i++) \
        { \
            d_border_num = (i+radius)-proc_height+1 ; \
            for( j = radius ; j < proc_width - radius ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    k = 0; \
                    h_idx = i-radius; \
                    w_idx = j-radius; \
                    for(kh = 0 ; kh < ksize - d_border_num ; kh++ ) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    h_idx = proc_height - 1 ; \
                    w_idx = j-radius; \
                    for(kh =  ksize - d_border_num ; kh<ksize ; kh++) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++ ;\
                        } \
                    } \
                    vec_sortnet_3(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
    } \
    else if (ksize == 5) \
    { \
        for(i = 0  ; i < radius  ; i++) \
        { \
            t_border_num = (radius - i) ; \
            for( j = radius ; j < proc_width - radius ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    k = 0; \
                    w_idx = j-radius; \
                    for(kh = 0 ; kh < t_border_num ; kh++) \
                    { \
                        p_in = &p_src[(w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch] ; \
                            k++ ; \
                        } \
                    } \
                    h_idx = 0 ; \
                    w_idx = j-radius; \
                    for(kh = (t_border_num) ; kh < ksize ; kh++) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    vec_sortnet_5(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
        for( i = radius ; i < proc_height - radius ; i++) \
        { \
            for( j = radius ; j < proc_width - radius ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    h_idx = i-radius; \
                    w_idx = j-radius; \
                    k = 0 ; \
                    for(kh = 0 ; kh < ksize ; kh++) \
                    { \
                        p_in = &p_src[(h_idx*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    vec_sortnet_5(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
        for( i = proc_height - radius ; i < proc_height ; i++) \
        { \
            d_border_num = (i+radius)-proc_height+1 ; \
            for( j = radius ; j < proc_width - radius ; j++) \
            { \
                p_result = &p_out[(i*proc_width+j)*n_ch]; \
                for( ch_idx = 0 ; ch_idx < n_ch ; ch_idx++) \
                { \
                    p_cal = &TMP[0]; \
                    k = 0; \
                    h_idx = i-radius; \
                    w_idx = j-radius; \
                    for(kh = 0 ; kh < ksize - d_border_num ; kh++ ) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++; \
                        } \
                        h_idx++; \
                    } \
                    h_idx = proc_height - 1 ; \
                    w_idx = j-radius; \
                    for(kh =  ksize - d_border_num ; kh<ksize ; kh++) \
                    { \
                        p_in = &p_src[((h_idx)*proc_width+w_idx)*n_ch + ch_idx]; \
                        for(kw = 0; kw < ksize ; kw++) \
                        { \
                            p_cal[k] = p_in[kw*n_ch]; \
                            k++ ;\
                        } \
                    } \
                    vec_sortnet_5(&p_cal[0]); \
                    p_result[ch_idx] = p_cal[m_val]; \
                } \
            } \
        } \
    }

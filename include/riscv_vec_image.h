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
#ifndef __RISCV_VEC_IMAGE_H__
#define __RISCV_VEC_IMAGE_H__

/** @file*/

#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_math_types.h"
#include <riscv_vec_types.h>

/**
 * @defgroup groupImage Image Functions
 * @brief image process Functions
 *
 * This section includes the following image functions and their corresponding data structures.
 */
/**
 * @addtogroup groupImage
 * @{
 */

/**
 * @defgroup img_struct Data Structures in Image Functions
 */
/**
 * @addtogroup img_struct
 * @{
 */
/**
 * @brief The structure of F32 image vectors
 */
typedef struct
{
        float32_t *data;   /**<Points to the image data*/
        u32_t height;      /**<Height of the image*/
        u32_t width;       /**<Width of the image*/
        u32_t channels;    /**<Channels of the image*/
} riscv_vec_img_f32_t;

/**
 * @brief The structure of U8 image vectors
 */
typedef struct
{
        u8_t *data;      /**<Points to the image data*/
        u32_t height;    /**<Height of the image*/
        u32_t width;     /**<Width of the image*/
        u32_t channels;  /**<Channels of the image*/
} riscv_vec_img_u8_t;

/**
 * @brief The structure of S8 image vectors
 */
typedef struct
{
        s8_t *data;      /**<Points to the image data*/
        u32_t height;    /**<Height of the image*/
        u32_t width;     /**<Width of the image*/
        u32_t channels;  /**<Channels of the image*/
} riscv_vec_img_s8_t;

/**
 * @brief The structure of U16 image vectors
 */
typedef struct
{
        u16_t *data;     /**<Points to the image data*/
        u32_t height;    /**<Height of the image*/
        u32_t width;     /**<Width of the image*/
        u32_t channels;  /**<Channels of the image*/
} riscv_vec_img_u16_t;

/**
 * @brief The structure of S16 image vectors
 */
typedef struct
{
        s16_t *data;     /**<Points to the image data*/
        u32_t height;    /**<Height of the image*/
        u32_t width;     /**<Width of the image*/
        u32_t channels; /**<Channels of the image*/
} riscv_vec_img_s16_t;

/**
 * @brief The enum of the image boarder type
 */
typedef enum
{
        RISCV_VEC_IMG_BORDER_REFLECT101 = 0,    /**< fedcba|abcdefgh|hgfedc */
        RISCV_VEC_IMG_BORDER_CONSTANT   = 1,    /**< 000000|abcdefgh|000000 */
} riscv_vec_img_border;

/**
 * @brief The interpolation type of the image resize
 */
typedef enum
{
    RISCV_VEC_INTER_NEAREST         = 0,
    RISCV_VEC_INTER_LINEAR          = 1,
    RISCV_VEC_INTER_CUBIC           = 2,
    RISCV_VEC_INTER_NEAREST_EXACT   = 3
} riscv_vec_img_interp_mode;
/** @} img_struct */

/**
 * @defgroup img_resize Image Resize Functions
 *
 * @brief The function resizes images using the bilinear interpolation algorithm.
 *
 * The function resizes images using the bilinear interpolation algorithm. The input pixel is arranged in RGBA format such as [R[0], G[0], B[0], A[0], ...].
 */

 /**
  * @addtogroup img_resize
  * @{
  */

/**
 * @brief Image resize U8 function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] dst_width number of width in the output vector
 * @param[in] dst_height number of height in the output vector
 * @param[in] src pointer of the input vector
 * @param[in] src_width number of width in the input vector
 * @param[in] src_height number of height in the input vector
 * @param[in] src_stride stride of the input vector
 *
 * @b Note:
 *
 * This function will allocate two temporary buffers. One is to store the index for the calculated resize dimension and its size is “(4 * dst_width + dst_height) * 12”; the other is to reserve indices of the rows that may be reused and its size is “ 8 * dst_width_align16” in which dst_width_align16 is 16 byte aligned of dst_width.
 *
 * @b Example
 *     <pre>
 *      \#define src_width   2
 *      \#define src_height  2
 *      \#define dst_width   4
 *      \#define dst_height  4
 *      \#define src_stride  (4 * src_width)
 *
 *      u8_t src[4 * src_width * src_height] = {…};
 *      u8_t dst[4 * dst_width * dst_height];
 *      riscv_vec_img_resize_bilinear_rgba_u8 (dst, dst_width, dst_height, src, src_width, src_height, src_stride);
 *     </pre>
 */
void riscv_vec_img_resize_bilinear_rgba_u8(u8_t* dst,
                                      u32_t dst_width,
                                      u32_t dst_height,
                                      u8_t* src,
                                      u32_t src_width,
                                      u32_t src_height,
                                      u32_t src_stride);

/**
 * @brief Image resize U8 function
 *
 * @param[in] img_src     Points to the input image structure
 * @param[out] img_dst    Points to the output image structure
 * @param[in] interp_mode interpolation type
 */
riscv_vec_status riscv_vec_img_resize_u8(riscv_vec_img_u8_t img_src,
                                         riscv_vec_img_u8_t img_dst,
                                         riscv_vec_img_interp_mode interp_mode);
/** @} img_resize */

/**
 * @defgroup img_filter2d Image Filter2D function
 *
 * @brief Image Filter2D function
 *
 * These functions apply a 2D filter to images using the input image data and kernel data
 */

 /**
  * @addtogroup img_filter2d
  * @{
  */

/**
 * @brief Image filter2D function for F32.
 *
 * @param[in] img_src           Points to the input image structure
 * @param[in] img_kernel        Points to the input kernel structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] anchor            Anchor of the kernel. Each element has a data type of riscv_vec_point_t
 * @param[in] delta             Added value of the output image
 * @param[in] type              Extrapolation method for image border
 * @return none
 */
void riscv_vec_img_filter2D_f32(riscv_vec_img_f32_t img_src,
                        riscv_vec_img_f32_t img_kernel,
                        riscv_vec_img_f32_t img_dst,
                        riscv_vec_point_t anchor,
                        double delta,
                        riscv_vec_img_border type);

/**
 * @brief Image filter2D function with S16 input kernel for U8.
 *
 * @param[in] img_src           Points to the input image structure
 * @param[in] img_kernel        Points to the input kernel structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] anchor            Anchor of the kernel. Each element has a data type of riscv_vec_point_t
 * @param[in] delta             Added value of the output image
 * @param[in] type              Extrapolation method for image border
 * @return none
 */
void riscv_vec_img_filter2D_u8_s16(riscv_vec_img_u8_t img_src,
                        riscv_vec_img_s16_t img_kernel,
                        riscv_vec_img_s16_t img_dst,
                        riscv_vec_point_t anchor,
                        double delta,
                        riscv_vec_img_border type);

/**
 * @brief Image filter2D function with S8 input kernel for U8.
 *
 * @param[in] img_src           Points to the input image structure
 * @param[in] img_kernel        Points to the input kernel structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] anchor            Anchor of the kernel. Each element has a data type of riscv_vec_point_t
 * @param[in] delta             Added value of the output image
 * @param[in] type              Extrapolation method for image border
 * @return none
 */
void riscv_vec_img_filter2D_u8_s8(riscv_vec_img_u8_t img_src,
                        riscv_vec_img_s8_t img_kernel,
                        riscv_vec_img_s8_t img_dst,
                        riscv_vec_point_t anchor,
                        double delta,
                        riscv_vec_img_border type);

/**
 * @brief Image filter2D function for S16.
 *
 * @param[in] img_src           Points to the input image structure
 * @param[in] img_kernel        Points to the input kernel structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] anchor            Anchor of the kernel. Each element has a data type of riscv_vec_point_t
 * @param[in] delta             Added value of the output image
 * @param[in] type              Extrapolation method for image border
 * @return none
 */
void riscv_vec_img_filter2D_s16(riscv_vec_img_s16_t img_src,
                        riscv_vec_img_s16_t img_kernel,
                        riscv_vec_img_s16_t img_dst,
                        riscv_vec_point_t anchor,
                        double delta,
                        riscv_vec_img_border type);

/**
 * @brief Image filter2D function with F32 input kernel for S16.
 *
 * @param[in] img_src           Points to the input image structure
 * @param[in] img_kernel        Points to the input kernel structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] anchor            Anchor of the kernel. Each element has a data type of riscv_vec_point_t
 * @param[in] delta             Added value of the output image
 * @param[in] type              Extrapolation method for image border
 * @return none
 */
void riscv_vec_img_filter2D_s16_f32_s16(riscv_vec_img_s16_t img_src,
                        riscv_vec_img_f32_t img_kernel,
                        riscv_vec_img_s16_t img_dst,
                        riscv_vec_point_t anchor,
                        double delta,
                        riscv_vec_img_border type);

/**
 * @brief Image filter2D function with F32 input kernel for U8.
 *
 * @param[in] img_src           Points to the input image structure
 * @param[in] img_kernel        Points to the input kernel structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] anchor            Anchor of the kernel. Each element has a data type of riscv_vec_point_t
 * @param[in] delta             Added value of the output image
 * @param[in] type              Extrapolation method for image border
 * @return none
 */
void riscv_vec_img_filter2D_u8_f32_u8(riscv_vec_img_u8_t img_src,
                        riscv_vec_img_f32_t img_kernel,
                        riscv_vec_img_u8_t img_dst,
                        riscv_vec_point_t anchor,
                        double delta,
                        riscv_vec_img_border type);

/** @} img_filter2d */

/**
 * @defgroup img_boxfilter Image Box Filter Functions
 *
 * @brief Image Box Filter Functions
 *
 * These functions apply a normalized box filter to blur images.
 */

/**
 * @addtogroup img_boxfilter
 * @{
 */

/**
 * @brief Image box filter function for F32.
 *
 * @param[in] img_src           Points to the input image structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] ksize             Blurring kernel size. Each element has a data type of riscv_vec_point_t
 * @param[in] anchor            Anchor of the kernel. Each element has a data type of riscv_vec_point_t
 * @param[in] normalized        Specifies whether the kernel is normalized by its area (1 = yes, 0 = no)
 * @param[in] type              Extrapolation method for image border
 * @return none
 */
void riscv_vec_img_boxFilter_f32(riscv_vec_img_f32_t img_src,
                                 riscv_vec_img_f32_t img_dst,
                                 riscv_vec_point_t ksize,
                                 riscv_vec_point_t anchor,
                                 uint8_t normalized,
                                 riscv_vec_img_border type);

/**
 * @brief Image box filter function for S16.
 *
 * @param[in] img_src           Points to the input image structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] ksize             Blurring kernel size. Each element has a data type of riscv_vec_point_t
 * @param[in] anchor            Anchor of the kernel. Each element has a data type of riscv_vec_point_t
 * @param[in] normalized        Specifies whether the kernel is normalized by its area (1 = yes, 0 = no)
 * @param[in] type              Extrapolation method for image border
 * @return none
 */
void riscv_vec_img_boxFilter_s16(riscv_vec_img_s16_t img_src,
                                 riscv_vec_img_s16_t img_dst,
                                 riscv_vec_point_t ksize,
                                 riscv_vec_point_t anchor,
                                 uint8_t normalized,
                                 riscv_vec_img_border type);

/**
 * @brief Image box filter function for U8.
 *
 * @param[in] img_src           Points to the input image structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] ksize             Blurring kernel size. Each element has a data type of riscv_vec_point_t
 * @param[in] anchor            Anchor of the kernel. Each element has a data type of riscv_vec_point_t
 * @param[in] normalized        Specifies whether the kernel is normalized by its area (1 = yes, 0 = no)
 * @param[in] type              Extrapolation method for image border
 * @return none
 */
void riscv_vec_img_boxFilter_u8(riscv_vec_img_u8_t img_src,
                                 riscv_vec_img_u8_t img_dst,
                                 riscv_vec_point_t ksize,
                                 riscv_vec_point_t anchor,
                                 uint8_t normalized,
                                 riscv_vec_img_border type);
/** @} img_boxfilter */

/**
 * @defgroup img_sobel Image Sobel Functions
 *
 * @brief Image Sobel Functions
 *
 * These functions calculate the image Sobel of the input image data.
 */

/**
 * @addtogroup img_sobel
 * @{
 */

/**
 * @brief Image Sobel function with S8 output for U8.
 *
 * @param[in] img_src 	        Points to the input image structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] dx                Order of the derivative X
 * @param[in] dy                Order of the derivative Y.
 * @param[in] kernel_size       Size of the extended Sobel kernel; it must be 1, 3, 5, or 7.
 * @param[in] scale             Scale factor for the computed derivative values
 * @param[in] delta             Added value of the output image
 * @param[in] type              Extrapolation method for image border
 * @return none
 */
void riscv_vec_img_sobel_u8_s8(riscv_vec_img_u8_t img_src,
                        riscv_vec_img_s8_t img_dst,
                        int dx,
                        int dy,
                        int kernel_size,
                        double scale,
                        double delta,
                        riscv_vec_img_border type);

/**
 * @brief Image Sobel function for S16.
 *
 * @param[in] img_src 	        Points to the input image structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] dx                Order of the derivative X
 * @param[in] dy                Order of the derivative Y.
 * @param[in] kernel_size       Size of the extended Sobel kernel; it must be 1, 3, 5, or 7.
 * @param[in] scale             Scale factor for the computed derivative values
 * @param[in] delta             Added value of the output image
 * @param[in] type              Extrapolation method for image border
 * @return none
 */
void riscv_vec_img_sobel_s16(riscv_vec_img_s16_t img_src,
                        riscv_vec_img_s16_t img_dst,
                        int dx,
                        int dy,
                        int kernel_size,
                        double scale,
                        double delta,
                        riscv_vec_img_border type);

/**
 * @brief Image Sobel function with S16 output for U8.
 *
 * @param[in] img_src 	        Points to the input image structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] dx                Order of the derivative X
 * @param[in] dy                Order of the derivative Y.
 * @param[in] kernel_size       Size of the extended Sobel kernel; it must be 1, 3, 5, or 7.
 * @param[in] scale             Scale factor for the computed derivative values
 * @param[in] delta             Added value of the output image
 * @param[in] type              Extrapolation method for image border
 * @return none
 */
void riscv_vec_img_sobel_u8_s16(riscv_vec_img_u8_t img_src,
                        riscv_vec_img_s16_t img_dst,
                        int dx,
                        int dy,
                        int kernel_size,
                        double scale,
                        double delta,
                        riscv_vec_img_border type);

/**
 * @brief Image Sobel function for F32.
 *
 * @param[in] img_src 	        Points to the input image structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] dx                Order of the derivative X
 * @param[in] dy                Order of the derivative Y.
 * @param[in] kernel_size       Size of the extended Sobel kernel; it must be 1, 3, 5, or 7.
 * @param[in] scale             Scale factor for the computed derivative values
 * @param[in] delta             Added value of the output image
 * @param[in] type              Extrapolation method for image border
 * @return none
 */
void riscv_vec_img_sobel_f32(riscv_vec_img_f32_t img_src,
                        riscv_vec_img_f32_t img_dst,
                        int dx,
                        int dy,
                        int kernel_size,
                        double scale,
                        double delta,
                        riscv_vec_img_border type);
/** @} img_sobel */

/**
 * @defgroup img_gauss Image Gaussian Blur Functions
 *
 * @brief Image Gaussian Blur Functions
 *
 * These functions apply a Gaussian filter to blur images.
 */

/**
 * @addtogroup img_gauss
 * @{
 */

/**
 * @brief Image Gaussian blur function for F32.
 *
 * @param[in] img_src 	        Points to the input image structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] ksize             Gaussian kernel size. Each element has a data type of riscv_vec_point_t ksize.width and ksize.height can differ
but they both must be positive and odd.
 * @param[in] sigmaX            Gaussian kernel standard deviation in X direction
 * @param[in] sigmaY            Gaussian kernel standard deviation in Y direction
 * @param[in] type              Image extrapolation border method
 * @return none
 */
void riscv_vec_img_gaussianBlur_f32(riscv_vec_img_f32_t img_src,
                        riscv_vec_img_f32_t img_dst,
                        riscv_vec_point_t ksize,
                        double sigmaX,
                        double sigmaY,
                        riscv_vec_img_border type);

/**
 * @brief Image Gaussian blur function for S16.
 *
 * @param[in] img_src 	        Points to the input image structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] ksize             Gaussian kernel size. Each element has a data type of riscv_vec_point_t ksize.width and ksize.height can differ
but they both must be positive and odd.
 * @param[in] sigmaX            Gaussian kernel standard deviation in X direction
 * @param[in] sigmaY            Gaussian kernel standard deviation in Y direction
 * @param[in] type              Image extrapolation border method
 * @return none
 */
void riscv_vec_img_gaussianBlur_s16(riscv_vec_img_s16_t img_src,
                        riscv_vec_img_s16_t img_dst,
                        riscv_vec_point_t ksize,
                        double sigmaX,
                        double sigmaY,
                        riscv_vec_img_border type);

/**
 * @brief Image Gaussian blur function for U8.
 *
 * @param[in] img_src 	        Points to the input image structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] ksize             Gaussian kernel size. Each element has a data type of riscv_vec_point_t ksize.width and ksize.height can differ
but they both must be positive and odd.
 * @param[in] sigmaX            Gaussian kernel standard deviation in X direction
 * @param[in] sigmaY            Gaussian kernel standard deviation in Y direction
 * @param[in] type              Image extrapolation border method
 * @return none
 */
void riscv_vec_img_gaussianBlur_u8(riscv_vec_img_u8_t img_src,
                        riscv_vec_img_u8_t img_dst,
                        riscv_vec_point_t ksize,
                        double sigmaX,
                        double sigmaY,
                        riscv_vec_img_border type);
/** @} img_gauss */

/**
 * @defgroup img_bilateral Image Bilateral Functions
 *
 * @brief Image Bilateral Functions
 *
 * These functions apply a bilateral filter to images.
 */

 /**
  * @addtogroup img_bilateral
  * @{
  */
/**
 * @brief Image bilateral function for F32.
 *
 * @param[in] img_src           Points to the input image structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] d                 Diameter of each pixel neighborhood that is used during filtering
 * @param[in] sigma_color       Filter sigma in the color space
 * @param[in] sigma_space       Filter sigma in the coordinate space
 * @param[in] type              Extrapolation method for image border
 * @return RISCV_VEC_INPUT_PARAM_ERROR if the input ksize is not equal to 3 or 5, or the ksize is greater than and equal to img_src.height and img_src.width.
 *        RISCV_VEC_MALLOC_FAIL if allocation fails
 *        RISCV_VEC_SUCCESS if successful
 */

riscv_vec_status riscv_vec_img_bilateral_f32(riscv_vec_img_f32_t img_src,
                        riscv_vec_img_f32_t img_dst,
                        int32_t d,
                        double sigma_color,
                        double sigma_space,
                        riscv_vec_img_border type);

/**
 * @brief Image bilateral function for U8.
 *
 * @param[in] img_src           Points to the input image structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] d                 Diameter of each pixel neighborhood that is used during filtering
 * @param[in] sigma_color       Filter sigma in the color space
 * @param[in] sigma_space       Filter sigma in the coordinate space
 * @param[in] type              Extrapolation method for image border
 * @return RISCV_VEC_INPUT_PARAM_ERROR if the input ksize is not equal to 3 or 5, or the ksize is greater than and equal to img_src.height and img_src.width.
 *        RISCV_VEC_MALLOC_FAIL if allocation fails
 *        RISCV_VEC_SUCCESS if successful
 */
riscv_vec_status riscv_vec_img_bilateral_u8(riscv_vec_img_u8_t img_src,
                        riscv_vec_img_u8_t img_dst,
                        int32_t d,
                        double sigma_color,
                        double sigma_space,
                        riscv_vec_img_border type);
/** @} img_bilateral */

/**
 * @defgroup img_median Image Median Blur Functions
 *
 * @brief Image Median Blur Functions
 *
 * These functions apply a median filter to blur images.
 */

 /**
  * @addtogroup img_median
  * @{
  */
/**
 * @brief Image median blur function for F32.
 *
 * @param[in] img_src           Points to the input image structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] ksize             Size of the median kernel, it only supports for 3 and 5
 * @return RISCV_VEC_INPUT_PARAM_ERROR if the input ksize is not equal to 3 or 5, or the ksize is greater than and equal to img_src.height and img_src.width.
 *        RISCV_VEC_MALLOC_FAIL if allocation fails
 *        RISCV_VEC_SUCCESS if successful
 */

riscv_vec_status riscv_vec_img_medianBlur_f32(riscv_vec_img_f32_t img_src,
                       riscv_vec_img_f32_t img_dst,
                       uint32_t ksize);

/**
 * @brief Image median blur function for U16.
 *
 * @param[in] img_src           Points to the input image structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] ksize             Size of the median kernel, it only supports for 3 and 5
 * @return RISCV_VEC_INPUT_PARAM_ERROR if the input ksize is not equal to 3 or 5, or the ksize is greater than and equal to img_src.height and img_src.width.
 *        RISCV_VEC_MALLOC_FAIL if allocation fails
 *        RISCV_VEC_SUCCESS if successful
 */
riscv_vec_status riscv_vec_img_medianBlur_u16(riscv_vec_img_u16_t img_src,
                       riscv_vec_img_u16_t img_dst,
                       uint32_t ksize);


/**
 * @brief Image median blur function for U8.
 *
 * @param[in] img_src           Points to the input image structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] ksize             Size of the median kernel, it only supports for 3 and 5
 * @return RISCV_VEC_INPUT_PARAM_ERROR if the input ksize is not equal to 3 or 5, or the ksize is greater than and equal to img_src.height and img_src.width.
 *        RISCV_VEC_MALLOC_FAIL if allocation fails
 *        RISCV_VEC_SUCCESS if successful
 */
riscv_vec_status riscv_vec_img_medianBlur_u8(riscv_vec_img_u8_t img_src,
                       riscv_vec_img_u8_t img_dst,
                       uint32_t ksize);

/** @} img_median */

/**
 * @defgroup image_cvt Image Convert Functions
 *
 * @brief Image Convert Functions
 *
 * This function converts an image from one color space to another. Andes vector library supports the following conversion code as riscv_vec_img_color_cvt of enum type.
 *
 * typedef enum {
 *    RISCV_VEC_COLOR_YUV2RGB_NV12 = 90,
 *    RISCV_VEC_COLOR_YUV2BGR_NV12 = 91,
 *    RISCV_VEC_COLOR_YUV2RGB_NV21  = 92,
 *    RISCV_VEC_COLOR_YUV2BGR_NV21  = 93,
 *    RISCV_VEC_COLOR_YUV2RGBA_NV12 = 94,
 *    RISCV_VEC_COLOR_YUV2BGRA_NV12 = 95,
 *    RISCV_VEC_COLOR_YUV2RGBA_NV21 = 96,
 *    RISCV_VEC_COLOR_YUV2BGRA_NV21 = 97,
 *    RISCV_VEC_COLOR_YUV2RGB_YV12 = 98,
 *    RISCV_VEC_COLOR_YUV2BGR_YV12 = 99,
 *    RISCV_VEC_COLOR_YUV2RGB_IYUV = 100,
 *    RISCV_VEC_COLOR_YUV2BGR_IYUV = 101,
 *    RISCV_VEC_COLOR_YUV2RGBA_YV12 = 102,
 *    RISCV_VEC_COLOR_YUV2BGRA_YV12 = 103,
 *    RISCV_VEC_COLOR_YUV2RGBA_IYUV = 104,
 *    RISCV_VEC_COLOR_YUV2BGRA_IYUV = 105
 * } riscv_vec_img_color_cvt;
 */

/**
 * @addtogroup image_cvt
 * @{
 */

/**
 * @brief Conversion code of riscv_vec_img_color_cvt
 */
typedef enum {
       RISCV_VEC_COLOR_YUV2RGB_NV12  = 90,
       RISCV_VEC_COLOR_YUV2BGR_NV12  = 91,
       RISCV_VEC_COLOR_YUV2RGB_NV21  = 92,
       RISCV_VEC_COLOR_YUV2BGR_NV21  = 93,
       RISCV_VEC_COLOR_YUV2RGBA_NV12 = 94,
       RISCV_VEC_COLOR_YUV2BGRA_NV12 = 95,
       RISCV_VEC_COLOR_YUV2RGBA_NV21 = 96,
       RISCV_VEC_COLOR_YUV2BGRA_NV21 = 97,
       RISCV_VEC_COLOR_YUV2RGB_YV12 = 98,
       RISCV_VEC_COLOR_YUV2BGR_YV12 = 99,
       RISCV_VEC_COLOR_YUV2RGB_IYUV = 100,
       RISCV_VEC_COLOR_YUV2BGR_IYUV = 101,
       RISCV_VEC_COLOR_YUV2RGBA_YV12 = 102,
       RISCV_VEC_COLOR_YUV2BGRA_YV12 = 103,
       RISCV_VEC_COLOR_YUV2RGBA_IYUV = 104,
       RISCV_VEC_COLOR_YUV2BGRA_IYUV = 105
} riscv_vec_img_color_cvt;

/**
 * @brief Image color conversion function for U8.
 *
 * @param[in] img_src           Points to the input image structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] type              Image color conversion code
 * @return RISCV_VEC_FAIL if conversion fails
 *         RISCV_VEC_OK if successful
 */
q31_t riscv_vec_img_cvtColor_u8(riscv_vec_img_u8_t img_src, riscv_vec_img_u8_t img_dst, riscv_vec_img_color_cvt type);
/** @} image_cvt */


/** @} groupImage */

#ifdef  __cplusplus
}
#endif
#endif

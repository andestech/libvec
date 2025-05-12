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
#ifndef __RISCV_VEC_DISTANCE_H__
#define __RISCV_VEC_DISTANCE_H__


/** @file*/

#ifdef __cplusplus
extern "C" {
#endif

#include "riscv_vec_types.h"

/**
 * @defgroup dist Distance Functions
 * @brief Distance Functions
 *
 * These functions compute the distance between two vectors.
 *
 * Andes vector library supports distinct distance functions for the following data types: single- and half-precision floating-point. These functions are described in the subsections below.
 */
/**
 * @addtogroup dist
 * @{
 */

/**
 * @defgroup fdist Float Distance Functions
 * @brief Float Distance Functions
 *
 * These functions compute the distance between two vectors of float values: F32 and F16.
Andes vector library supports algorithms for calculating the following distances:
 *     - Bray-Curtis distance
 *     - Canberra distance
 *     - Chebyshev distance
 *     - Cityblock (Manhattan) distance
 *     - Correlation distance
 *     - Cosine distance
 *     - Euclidean distance
 *     - Jensen-Shannon distance
 *     - Minkowski distance
 */
/**
 * @addtogroup fdist
 * @{
 */

/**
 * @brief        Jensen-Shannon distance between two vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 */
float32_t riscv_vec_dist_jensen_shannon_f32(const float32_t *src1, const float32_t *src2, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief        Jensen-Shannon distance between two vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 */
float16_t riscv_vec_dist_jensen_shannon_f16(const float16_t *src1, const float16_t *src2, uint32_t size);
#endif

/**
 * @brief        Bray-Curtis distance between two f32 vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 */
float32_t riscv_vec_dist_bray_curtis_f32(const float32_t *src1, const float32_t *src2, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief        Bray-Curtis distance between two f16 vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 */
float16_t riscv_vec_dist_bray_curtis_f16(const float16_t *src1, const float16_t *src2, uint32_t size);
#endif

/**
 * @brief        Canberra distance between two f32 vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 */
float32_t riscv_vec_dist_canberra_f32(const float32_t *src1, const float32_t *src2, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief        Canberra distance between two f16 vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 */
float16_t riscv_vec_dist_canberra_f16(const float16_t *src1, const float16_t *src2, uint32_t size);
#endif

/**
 * @brief        Chebyshev distance between two f32 vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 */
float32_t riscv_vec_dist_chebyshev_f32(const float32_t *src1, const float32_t *src2, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief        Chebyshev distance between two f16 vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 */
float16_t riscv_vec_dist_chebyshev_f16(const float16_t *src1, const float16_t *src2, uint32_t size);
#endif

/**
 * @brief        Cityblock (Manhattan) distance between two f32 vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 */
float32_t riscv_vec_dist_city_block_f32(const float32_t *src1, const float32_t *src2, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief        Cityblock (Manhattan) distance between two f16 vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 */
float16_t riscv_vec_dist_city_block_f16(const float16_t *src1, const float16_t *src2, uint32_t size);
#endif

/**
 * @brief        Correlation distance between two f32 vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 */
float32_t riscv_vec_dist_corr_f32(const float32_t *src1, const float32_t *src2, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief        Correlation distance between two f16 vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 */
float16_t riscv_vec_dist_corr_f16(const float16_t *src1, const float16_t *src2, uint32_t size);
#endif

/**
 * @brief        Cosine distance between two f32 vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 */
float32_t riscv_vec_dist_cos_f32(const float32_t *src1, const float32_t *src2, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief        Cosine distance between two f16 vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 */
float16_t riscv_vec_dist_cos_f16(const float16_t *src1, const float16_t *src2, uint32_t size);
#endif


/**
 * @brief        Euclidean distance between two f32 vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 */
float32_t riscv_vec_dist_euclidean_f32(const float32_t *src1, const float32_t *src2, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief        Euclidean distance between two f16 vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 */
float16_t riscv_vec_dist_euclidean_f16(const float16_t *src1, const float16_t *src2, uint32_t size);
#endif

/**
 * @brief        Minkowski distance between two vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    order        distance order
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 *
 * @b Example
 *     <pre>
 *      \#define size 4
 *      \#define order 3
 *      float32_t src1[size] = {1.0, 0.5, 0.4, -0.1};
 *      float32_t src2[size] = {0.40, -0.2, 0.24, 0.4};
 *      float32_t dist_out;
 *      dist_out = riscv_vec_dist_minkowski_f32(src1, src2, order, size);
 *     </pre>
 */
float32_t riscv_vec_dist_minkowski_f32(const float32_t *src1, const float32_t *src2, int32_t order, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief        Minkowski distance between two vectors
 * @param[in]    src1         pointer of the first input vector
 * @param[in]    src2         pointer of the second input vector
 * @param[in]    order        distance order
 * @param[in]    size         size of elements in a vector
 * @return Distance value
 */
float16_t riscv_vec_dist_minkowski_f16(const float16_t *src1, const float16_t *src2, int32_t order, uint32_t size);
#endif

/** @} fdist */

/**
 * @defgroup dtw_dist Dynamic Time Warping (DTW) Distance Functions
 * @brief Dynamic Time Warping (DTW) Distance Functions
 *
 * These functions compute the DTW distance between the input F32 distance matrix and input Q7 window matrix, which can be NULL (representing a full-range computation) or a result computed from riscv_vec_dtw_init_window_q7. Additionally, it can compute the DTW path using riscv_vec_dtw_path_f32.
 *
 */
/**
 * @addtogroup dtw_dist
 * @{
 */

/**
 * @brief         DTW distance function.
 * @param[in]     dtw_src        Points to the input distance matrix
 * @param[in]     dtw_window     Points to the input window matrix
 * @param[out]    dtw_out        Points to the output DTW cost matrix
 * @param[out]    dtw_distance   Points to the output DTW distance
 * @param[in]     row            Number of rows in a matrix
 * @param[in]     col            Number of columns in a matrix
 * @return 0 if successful
 *         -1 if no path can be found due to window constraints
 *
 */
int riscv_vec_dtw_distance_f32(const float32_t *dtw_src,
                               const q7_t *dtw_window,
                               float32_t *dtw_out,
                               float32_t *dtw_distance,
                               uint32_t row,
                               uint32_t col);

/**
 * @brief        DTW path compute function
 * @param[in]    dtw              Points to the input DTW cost matrix
 * @param[out]   dtw_path         Points to the output DTW path vector
 * @param[out]   dtw_path_length  Points to the output Number of path length
 * @param[in]    row              Number of rows in a matrix
 * @param[in]    col              Number of columns in a matrix
 * @return none.
 */
void riscv_vec_dtw_path_f32(const float32_t *dtw,
                            int16_t *dtw_path,
                            uint32_t *dtw_path_length,
                            uint32_t row,
                            uint32_t col);

/**
 * @brief window type of riscv_vec_dtw_window_type
 */
typedef enum
{
    RISCV_VEC_DTW_SAKOE_CHIBA_WINDOW = 1,
    RISCV_VEC_DTW_SLANTED_BAND_WINDOW = 2
} riscv_vec_dtw_window_type;

/**
 * @brief       DTW window compute function.
 * @param[in]     window_type  Type of window
 * @param[in]     window_size  Size of window
 * @param[in,out] dtw_window   Points to the output window matrix
 * @param[in]     row          Number of rows in a matrix
 * @param[in]     col          Number of columns in a matrix
 * @return 0 if successful
 *         -1 if the type is not supported
 *
 * @b Note:
 * The supported window type of riscv_vec_dtw_window_type includes:
 *     - RISCV_VEC_DTW_SAKOE_CHIBA_WINDOW SAKOE_CHIBA window type
 *     - RISCV_VEC_DTW_SLANTED_BAND_WINDOW SLANTED_BAND window type
 */
int32_t riscv_vec_dtw_init_window_q7(const riscv_vec_dtw_window_type window_type,
                                     const int32_t window_size,
                                     q7_t *dtw_window,
                                     uint32_t row,
                                     uint32_t col);

/** @} dtw_dist */

/** @} dist */
#ifdef  __cplusplus
}
#endif
#endif

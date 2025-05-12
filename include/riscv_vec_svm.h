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
#ifndef __RISCV_VEC_SVM_H__
#define __RISCV_VEC_SVM_H__


/** @file*/

#ifdef __cplusplus
extern "C" {
#endif

#include "riscv_vec_types.h"

/**
 * @defgroup svm Support Vector Machine (SVM) Estimation Functions
 * @brief SVM Estimation Functions
 *
 * These functions implement two-class SVM classification based on the provided instance structure.
 */

/**
 * @addtogroup svm
 * @{
 */

/**
 * @defgroup svm_linear SVM Linear Estimation
 * @brief SVM Linear Estimation
 */

/**
 * @addtogroup svm_linear
 * @{
 */

/**
 * @brief instance structure of the floating-point SVM linear estimator
 */
typedef struct
{
    uint32_t        numofvec;     /**< number of support vectors */
    uint32_t        dimofvec;     /**< dimension of vector space */
    float32_t       intercept;    /**< Intercept */
    const float32_t *dualcoe;     /**< dual coefficients */
    const float32_t *vec;         /**< support vectors */
    const int32_t   *classes;     /**< the 2 classes of SVM */
} riscv_vec_svm_linear_f32_t;

/**
 * @brief SVM linear estimation function for F32.
 * @param[in]    instance    pointer of the instance structure
 * @param[in]    src         pointer of the input vector
 * @param[out]   result      decision value
 * @return none.
 *
 * @b Example
 *     <pre>
 *      \#define numofvec 3
 *      \#define dimofvec 4
 *      \#define intercept 0.1
 *      float32_t src[dimofvec] = {1.0, 0.5, 0.4, -0.1};
 *      float32_t dualcoe[numofvec] = {0.40, 0.10, 0.24};
 *      float32_t vec[dimofvec * numofvec] = {0.40, 0.10, 0.24, -0.40, -0.34, 0.20, 0.06, 0.28, -0.04, -0.20, 0.08, 0.40};
 *      int32_t classes[2] = {3, 7};
 *      int32_t *result;
 *      riscv_vec_svm_linear_f32_t instance = {numofvec, dimofvec, intercept, dualcoe, vec, classes};
 *      riscv_vec_svm_linear_est_f32(&instance, src, result);
 *     </pre>
 */

void riscv_vec_svm_linear_est_f32(const riscv_vec_svm_linear_f32_t *instance, const float32_t *src, int32_t *result);

#if defined (__riscv_zfh)
/**
 * @brief instance structure of the floating-point SVM linear estimator
 */
typedef struct
{
    uint32_t        numofvec;     /**< number of support vectors */
    uint32_t        dimofvec;     /**< dimension of vector space */
    float16_t       intercept;    /**< Intercept */
    const float16_t *dualcoe;     /**< dual coefficients */
    const float16_t *vec;         /**< support vectors */
    const int32_t   *classes;     /**< the 2 classes of SVM */
} riscv_vec_svm_linear_f16_t;

/**
 * @brief SVM linear estimation function for F16.
 * @param[in]    instance    pointer of the instance structure
 * @param[in]    src         pointer of the input vector
 * @param[out]   result      decision value
 * @return none.
 *
 * @b Note:
This function is supported only by toolchains with HW FPU support. Additionally, you must apply the -mzfh option in both the compiler and linker flags for applications that use this function. For details about the -mzfh linking option, see Section 1.1 or 1.2.
 */
void riscv_vec_svm_linear_est_f16(const riscv_vec_svm_linear_f16_t *instance, const float16_t *src, int32_t *result);
#endif
/** @} svm_linear */

/**
 * @defgroup svm_poly SVM Polynomial Estimation
 * @brief SVM Polynomial Estimation
 *
 * This section describes the SVM polynomial estimation functions.
 */

/**
 * @addtogroup svm_poly
 * @{
 */

/**
 * @brief instance structure of the F32 SVM polynomial estimator
 */
typedef struct
{
    uint32_t        numofvec;     /**< Number of support vectors */
    uint32_t        dimofvec;     /**< Dimension of vector space */
    float32_t       intercept;    /**< Intercept                 */
    const float32_t *dualcoe;     /**< Dual coefficients         */
    const float32_t *vec;         /**< Support vectors           */
    const int32_t   *classes;     /**< The 2 SVM classes         */
    int32_t         exponent;     /**< Polynomial exponent       */
    float32_t       coef0;        /**< Independent constant      */
    float32_t       gamma;        /**< Gamma factor              */
} riscv_vec_svm_poly_f32_t;

/**
 * @brief SVM polynomial estimation function for F32.
 * @param[in]    instance    pointer of the instance structure
 * @param[in]    src         pointer of the input vector
 * @param[out]   result      decision value
 * @return none.
 *
 * @b Example
 *     <pre>
 *      \#define numofvec 3
 *      \#define dimofvec 4
 *      \#define intercept 0.1
 *      \#define gamma 0.2
 *      \#define exponent 3
 *      \#define coef0 0.3
 *
 *      float32_t src[dimofvec] = {1.0, 0.5, 0.4, -0.1};
 *      float32_t dualcoe[numofvec] = {0.40, 0.10, 0.24};
 *      float32_t vec[dimofvec * numofvec] = {0.40, 0.10, 0.24, -0.40, -0.34, 0.20, 0.06, 0.28, -0.04, -0.20, 0.08, 0.40};
 *      int32_t classes[2] = {-1, 4};
 *      int32_t *result;
 *      riscv_vec_svm_poly_f32_t instance = {numofvec, dimofvec, intercept, dualcoe, vec, classes, exponent, coef0, gamma};
 *      riscv_vec_svm_poly_est_f32(&instance, src, result);
 *     </pre>
 */
void riscv_vec_svm_poly_est_f32(const riscv_vec_svm_poly_f32_t *instance, const float32_t *src, int32_t *result);

#if defined (__riscv_zfh)
/**
 * @brief Instance structure of the F16 SVM polynomial estimator
 */
typedef struct
{
    uint32_t        numofvec;     /**< Number of support vectors */
    uint32_t        dimofvec;     /**< Dimension of vector space */
    float16_t       intercept;    /**< Intercept                 */
    const float16_t *dualcoe;     /**< Dual coefficients         */
    const float16_t *vec;         /**< Support vectors           */
    const int32_t   *classes;     /**< The 2 SVM classes         */
    int32_t         exponent;     /**< Polynomial exponent       */
    float16_t       coef0;        /**< Independent constant      */
    float16_t       gamma;        /**< Gamma factor              */
} riscv_vec_svm_poly_f16_t;

/**
 * @brief SVM polynomial estimation function for F16.
 * @param[in]    instance    pointer of the instance structure
 * @param[in]    src         pointer of the input vector
 * @param[out]   result      decision value
 * @return none.
 *
 * @b Note:
This function is supported only by toolchains with HW FPU support. Additionally, you must apply the -mzfh option in both the compiler and linker flags for applications that use this function. For details about the -mzfh linking option, see Section 1.1 or 1.2.
 */
void riscv_vec_svm_poly_est_f16(const riscv_vec_svm_poly_f16_t *instance, const float16_t *src, int32_t *result);
#endif
/** @} svm_poly */

/**
 * @defgroup svm_rbf SVM Radial Basis Function (RBF) Estimation
 * @brief SVM RBF Estimation
 *
 * This section describes the SVM radial basis function estimation functions.
 */
/**
 * @addtogroup svm_rbf
 * @{
 */
/**
 * @brief Instance structure of the F32 SVM RBF estimator
 */
typedef struct
{
    uint32_t        numofvec;     /**< Number of support vectors */
    uint32_t        dimofvec;     /**< Dimension of vector space */
    float32_t       intercept;    /**< Intercept                 */
    const float32_t *dualcoe;     /**< Dual coefficients         */
    const float32_t *vec;         /**< Support vectors           */
    const int32_t   *classes;       /**< The 2 SVM classes         */
    float32_t       gamma;        /**< Gamma factor              */
} riscv_vec_svm_rbf_f32_t;

/**
 * @brief SVM RBF estimation function for F32.
 * @param[in]    instance    pointer of the instance structure
 * @param[in]    src         pointer of the input vector
 * @param[out]   result      decision value
 * @return none.
 *
 * @b Example
 *     <pre>
 *      \#define numofvec 3
 *      \#define dimofvec 4
 *      \#define intercept 0.1
 *      \#define gamma 0.2
 *
 *      float32_t src[dimofvec] = {1.0, 0.5, 0.4, -0.1};
 *      float32_t dualcoe[numofvec] = {0.40, 0.10, 0.24};
 *      float32_t vec[dimofvec * numofvec] = {0.40, 0.10, 0.24, -0.40, -0.34, 0.20, 0.06, 0.28, -0.04, -0.20, 0.08, 0.40};
 *      int32_t classes[2] = {2, 5};
 *      int32_t *result;
 *      riscv_vec_svm_rbf_f32_t instance = {numofvec, dimofvec, intercept, dualcoe, vec, classes, gamma};
 *      riscv_vec_svm_rbf_est_f32(&instance, src, result);
 *     </pre>
 */
void riscv_vec_svm_rbf_est_f32(const riscv_vec_svm_rbf_f32_t *instance, const float32_t *src, int32_t *result);

#if defined (__riscv_zfh)
/**
 * @brief Instance structure of the F16 SVM RBF estimator
 */
typedef struct
{
    uint32_t        numofvec;     /**< Number of support vectors */
    uint32_t        dimofvec;     /**< Dimension of vector space */
    float16_t       intercept;    /**< Intercept                 */
    const float16_t *dualcoe;     /**< Dual coefficients         */
    const float16_t *vec;         /**< Support vectors           */
    const int32_t   *classes;       /**< The 2 SVM classes         */
    float16_t       gamma;        /**< Gamma factor              */
} riscv_vec_svm_rbf_f16_t;

/**
 * @brief SVM RBF estimation function for F16.
 * @param[in]    instance    pointer of the instance structure
 * @param[in]    src         pointer of the input vector
 * @param[out]   result      decision value
 * @return none.
 *
 * @b Note:
This function is supported only by toolchains with HW FPU support. Additionally, you must apply the -mzfh option in both the compiler and linker flags for applications that use this function. For details about the -mzfh linking option, see Section 1.1 or 1.2.
 */
void riscv_vec_svm_rbf_est_f16(const riscv_vec_svm_rbf_f16_t *instance, const float16_t *src, int32_t *result);
#endif
/** @} svm_rbf */

/**
 * @defgroup svm_sigmoid SVM Sigmoid Estimation
 * @brief SVM Sigmoid Estimation
 *
 * This section describes the SVM sigmoid estimation functions.
 */
/**
 * @addtogroup svm_sigmoid
 * @{
 */
/**
 * @brief Instance structure of the F32 SVM sigmoid estimator
 */
typedef struct
{
    uint32_t        numofvec;     /**< Number of support vectors */
    uint32_t        dimofvec;     /**< Dimension of vector space */
    float32_t       intercept;    /**< Intercept                 */
    const float32_t *dualcoe;     /**< Dual coefficients         */
    const float32_t *vec;         /**< Support vectors           */
    const int32_t   *classes;       /**< the 2 classes of SVM      */
    float32_t       coef0;        /**< Independent constant      */
    float32_t       gamma;        /**< Gamma factor              */
} riscv_vec_svm_sigmoid_f32_t;

/**
 * @brief SVM sigmoid estimation function for F32 .
 * @param[in]    instance    pointer of the instance structure
 * @param[in]    src         pointer of the input vector
 * @param[out]   result      decision value
 * @return none.
 *
 * @b Example
 *     <pre>
 *      \#define numofvec 3
 *      \#define dimofvec 4
 *      \#define intercept 0.1
 *      \#define gamma 0.2
 *
 *      float32_t src[dimofvec] = {1.0, 0.5, 0.4, -0.1};
 *      float32_t dualcoe[numofvec] = {0.40, 0.10, 0.24};
 *      float32_t vec[dimofvec * numofvec] = {0.40, 0.10, 0.24, -0.40, -0.34, 0.20, 0.06, 0.28, -0.04, -0.20, 0.08, 0.40};
 *      int32_t classes[2] = {2, 5};
 *      int32_t *result;
 *      riscv_vec_svm_rbf_f32_t instance = {numofvec, dimofvec, intercept, dualcoe, vec, classes, gamma};
 *      riscv_vec_svm_rbf_est_f32(&instance, src, result);
 *     </pre>
 */
void riscv_vec_svm_sigmoid_est_f32(const riscv_vec_svm_sigmoid_f32_t *instance, const float32_t *src, int32_t *result);

#if defined (__riscv_zfh)
/**
 * @brief Instance structure of the F16 SVM sigmoid estimator
 */
typedef struct
{
    uint32_t        numofvec;     /**< Number of support vectors */
    uint32_t        dimofvec;     /**< Dimension of vector space */
    float16_t       intercept;    /**< Intercept                 */
    const float16_t *dualcoe;     /**< Dual coefficients         */
    const float16_t *vec;         /**< Support vectors           */
    const int32_t   *classes;       /**< the 2 classes of SVM      */
    float16_t       coef0;        /**< Independent constant      */
    float16_t       gamma;        /**< Gamma factor              */
} riscv_vec_svm_sigmoid_f16_t;

/**
 * @brief SVM sigmoid estimation function for F16 .
 * @param[in]    instance    pointer of the instance structure
 * @param[in]    src         pointer of the input vector
 * @param[out]   result      decision value
 * @return none.
 *
 * @b Note:
This function is supported only by toolchains with HW FPU support. Additionally, you must apply the -mzfh option in both the compiler and linker flags for applications that use this function. For details about the -mzfh linking option, see Section 1.1 or 1.2.
 */
void riscv_vec_svm_sigmoid_est_f16(const riscv_vec_svm_sigmoid_f16_t *instance, const float16_t *src, int32_t *result);
#endif
/** @} svm_sigmoid */
/** @} svm */

#ifdef __cplusplus
}
#endif

#endif

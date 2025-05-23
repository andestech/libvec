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

/**
* @brief Solve LT * X = A where LT is a lower triangular matrix
* @param[in]  lt  The lower triangular matrix
* @param[in]  A  The matrix A
* @param[out] dst The solution X of LT * X = A.
* @return The function returns NDSV_FAIL, if the system can't be solved.
*/

/* function description */
int riscv_vec_rmmat_solve_lt_f32(float32_t* FUNC_RESTRICT lt, float32_t* FUNC_RESTRICT A, float32_t* FUNC_RESTRICT dst, uint32_t row, uint32_t col)
{
    //check if the input row or col is zero, and should be square matrix.
    if ((row <= 0) || (col <= 0) || (row != col))
    {
        return NDSV_FAIL;
    }
    int i, j, k;
    float32_t* pX = dst;
    float32_t* pLT = lt;
    float32_t* pA = A;

    float32_t* lt_row;
    float32_t* a_col;
    for (j = 0; j < col; j++)
    {
        a_col = &pA[j];

        for (i = 0; i < row; i++)
        {
            float32_t tmp = a_col[i * col];

            lt_row = &pLT[row * i];

            for (k = 0; k < i; k++)
            {
                tmp -= lt_row[k] * pX[col * k + j];
            }

            if (lt_row[i] == 0.0f)
            {
                return(NDSV_FAIL); //The matrix is singular
            }
            tmp = tmp / lt_row[i];
            pX[i * col + j] = tmp;
        }
    }


    return NDSV_OK;
}

/**
 * @} end of matrixtrans
 */

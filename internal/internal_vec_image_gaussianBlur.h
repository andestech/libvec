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


// special case for n=1,3,5,7,9 && sigma <=0,
// reference to OpenCV function: getGaussianKernel
static float size_1_minus_sigma[1]={1.0f};
static float size_3_minus_sigma[3]={0.25f, 0.5f, 0.25f};
static float size_5_minus_sigma[5]={0.0625f, 0.25f, 0.375f, 0.25, 0.0625};
static float size_7_minus_sigma[7]={0.03125f, 0.109375f, 0.21875f, 0.28125f, 0.21875f, 0.109375f, 0.03125f};
static float size_9_minus_sigma[9]={0.015625f, 0.05078125f, 0.1171875f, 0.19921875f, 0.234375f, 0.19921875f, 0.1171875f, 0.05078125f, 0.015625f};

static inline void vec_generateGaussianKernel(float *kernel, riscv_vec_point_t ksize, float sigmaX, float sigmaY)
{
    int kw = ksize.x, kh = ksize.y;
    int sigmaX_flag = 0;
    int sigmaY_flag = 0;

    if(sigmaY <= 0.0f)
    {
    // reference: GaussianBlur, OpenCV v4.8.x
        sigmaY = sigmaX;
    }

    if(sigmaX <= 0.0)
    {
        if((kw==1)||(kw==3)||(kw==5)||(kw==7)||(kw==9))
        {
            sigmaX_flag = 1;
        }
        sigmaX = 0.3 * ((kw - 1) * 0.5 - 1) + 0.8;
    }

    if(sigmaY <= 0.0)
    {   if((kh==1)||(kh==3)||(kh==5)||(kh==7)||(kh==9))
        {
            sigmaY_flag = 1;
        }
        sigmaY = 0.3 * ((kh - 1) * 0.5 - 1) + 0.8;
    }
    float sigma2X = 2.0 * sigmaX * sigmaX;
    float sigma2Y = 2.0 * sigmaY * sigmaY;
    float sum = 0.0;

    // compute value
    if ((sigmaX_flag == 1) && (sigmaY_flag == 1))
    {
        float *ptr_x = NULL;
        float *ptr_y = NULL;
        int   x_idx = 0, y_idx = 0;
        // get the table of direction-x
        if(kw == 1)
        {
            ptr_x = &size_1_minus_sigma[0];
        }
        else if(kw == 3)
        {
            ptr_x = &size_3_minus_sigma[0];
        }
        else if(kw == 5)
        {
            ptr_x = &size_5_minus_sigma[0];
        }
        else if(kw == 7)
        {
            ptr_x = &size_7_minus_sigma[0];
        }
        else
        {
            ptr_x = &size_9_minus_sigma[0];
        }
        // get the table of direction-y
        if(kh == 1)
        {
            ptr_y = &size_1_minus_sigma[0];
        }
        else if(kh == 3)
        {
            ptr_y = &size_3_minus_sigma[0];
        }
        else if(kh == 5)
        {
            ptr_y = &size_5_minus_sigma[0];
        }
        else if(kh == 7)
        {
            ptr_y = &size_7_minus_sigma[0];
        }
        else
        {
            ptr_y = &size_9_minus_sigma[0];
        }
        for (int x = -kw / 2; x <= kw / 2; x++)
        {
            y_idx = 0;
            for (int y = -kh / 2; y <= kh / 2; y++)
            {
                //float value = tmp_x*ptr_y[y_idx];
                float value = ptr_x[x_idx]*ptr_y[y_idx];
                kernel[(y + kh / 2) * kw + x + kw / 2] = value;
                sum += kernel[(y + kh / 2) * kw + x + kw / 2];
                y_idx += 1;
            }
            x_idx +=1 ;
        }
    } // (sigmaX_flag == True) && (sigmaY_flag==True)
    else if(sigmaX_flag == 1)
    {
        float *ptr_x = NULL;
        int   x_idx = 0;
        // get the table of direction-x
        if(kw == 1)
        {
            ptr_x = &size_1_minus_sigma[0];
        }
        else if(kw == 3)
        {
            ptr_x = &size_3_minus_sigma[0];
        }
        else if(kw == 5)
        {
            ptr_x = &size_5_minus_sigma[0];
        }
        else if(kw == 7)
        {
            ptr_x = &size_7_minus_sigma[0];
        }
        else
        {
            ptr_x = &size_9_minus_sigma[0];
        }
        for (int x = -kw / 2; x <= kw / 2; x++)
        {
            for (int y = -kh / 2; y <= kh / 2; y++)
            {
                float value = ptr_x[x_idx]*expf(-(y * y / sigma2Y));
                kernel[(y + kh / 2) * kw + x + kw / 2] = value;
                sum += kernel[(y + kh / 2) * kw + x + kw / 2];
            }
            x_idx +=1 ;
        }


    } // (sigmaX_flag == True)
    else
    {
        for (int x = -kw / 2; x <= kw / 2; x++)
        {
            for (int y = -kh / 2; y <= kh / 2; y++)
            {
                float value = expf(-(x * x / sigma2X + y * y / sigma2Y));
                kernel[(y + kh / 2) * kw + x + kw / 2] = value;
                sum += kernel[(y + kh / 2) * kw + x + kw / 2];
            }
        }
    } // (sigmaX_flag == False) && (sigmaY_flag == False)
    // normalization
    for (int i = 0; i < kh; i++)
    {
        for (int j = 0; j < kw; j++)
        {
            kernel[i * kw + j] /= sum;
        }
    }
}

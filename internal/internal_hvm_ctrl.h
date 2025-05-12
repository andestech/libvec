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

#ifndef __INTERNAL_NDS_HVMCTRL_H__
#define __INTERNAL_NDS_HVMCTRL_H__

#ifdef ENA_DBG_MSG
    #include "stdio.h"
    #define DBGMSG printf
#else //ENA_DBG_MSG
    #define DBGMSG(...)
#endif  //ENA_DBG_MSG

#define HVM_SIZE 512*1024

void* vec_test_malloc(uint32_t size);
void vec_test_free(void *ptr);

#undef NDSV_MALLOC
#define NDSV_MALLOC vec_test_malloc

#undef NDSV_FREE
#define NDSV_FREE vec_test_free

#endif // __INTERNAL_NDS_HVMCTRL_H__

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

#include "nds_hvm_lib.h"

#ifdef ENA_DBG_MSG
    #include "stdio.h"
    #define DBGMSG printf
#else //ENA_DBG_MSG
    #define DBGMSG(...)
#endif  //ENA_DBG_MSG

static int64_t hvm_type = 0;
static inline void* vec_libhvm_malloc(uint32_t size)
{
    void *st = NULL;
    st = nds_hvm_aligned_alloc(64, size, 0);
    if (st == NULL)
    {
        const long alignBytes = 64;
        uint32_t allocBytes = (size + (alignBytes - 1)) & ~(alignBytes - 1);
        st = malloc(allocBytes);
        hvm_type = 1;
    }
    return st;
}
static inline void vec_libhvm_free(void *ptr)
{
    if (hvm_type == 0)
    {
        nds_hvm_free(ptr);
    }
    else
    {
        free(ptr);
    }
}


#undef NDSV_MALLOC
#define NDSV_MALLOC(bSize) vec_libhvm_malloc(bSize)

#undef NDSV_FREE
#define NDSV_FREE(PTR) vec_libhvm_free(PTR)

#endif // __INTERNAL_NDS_HVMCTRL_H__

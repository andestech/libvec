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

#ifdef ENA_HVM
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "internal_hvm_ctrl.h"
//#undef DBGMSG
//#include "stdio.h"
//#define DBGMSG printf
extern long __HVM_SECTION_BEGIN__;
extern long __HVM_SECTION_INIT_END__;
static char *cur_hvm_end = (char *)&__HVM_SECTION_INIT_END__;
static char *hvm_section_begin = (char*) &__HVM_SECTION_BEGIN__;
int64_t hvm_rmder = 0;
void* vec_test_malloc(uint32_t size)
{
    const long alignBytes = 64;
    void *ptrAligned = NULL;
    void **ptrOrigin = NULL;
    uint32_t allocBytes = (size + (alignBytes - 1)) & ~(alignBytes - 1);
    if(hvm_rmder == 0)
    {
        hvm_rmder = HVM_SIZE - ((int64_t)cur_hvm_end - (int64_t)hvm_section_begin);
    }
    //DBGMSG("size = %d, allocBytes = %d, hvm_rmder = %d\n", size, allocBytes, (int32_t)hvm_rmder);
    if(((int64_t)allocBytes < hvm_rmder) && (hvm_rmder > 0))
    {
        *ptrOrigin = cur_hvm_end;   // in manual allocation mode, the ptrOrigin and ptrAligned will point to the same ad    dress
        cur_hvm_end += allocBytes;
        ptrAligned = (void*)*ptrOrigin;
        hvm_rmder = hvm_rmder - (int64_t) allocBytes;
    }
    else
    {
        DBGMSG("Error on test_malloc, remainder hvm size = %d, malloc_hvm = %d, using stdlib malloc\n", (int32_t)hvm_rmder, size);
        ptrAligned = malloc(size);
    }
    return ptrAligned;
}

void vec_test_free(void *ptr)
{
    if(hvm_rmder >= 0)
    {
        cur_hvm_end = (char *)&__HVM_SECTION_INIT_END__;
        hvm_rmder = 0 ;
        ptr = NULL;
    }
    else
    {
        free(ptr);
    }
}
#endif //ENA_HVM

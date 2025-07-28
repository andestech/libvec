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

#ifndef __INTERNAL_NDS_BF16_H__
#define __INTERNAL_NDS_BF16_H__

#if defined (__riscv_zfbfmin)
static inline long backup_mode_bit_set_bf16()
{
    long umic_ctl_bkp = __nds__read_csr(NDS_UMISC_CTL);
    long umic_ctl_bf16 = umic_ctl_bkp | (1L << 0);
    __nds__write_csr(umic_ctl_bf16, NDS_UMISC_CTL);
    return umic_ctl_bkp;
}

#define BACKUP_MODE_BIT_SET_BF16 backup_mode_bit_set_bf16();

#define RECOVER_MODE_BIT(umic_ctl_bkp)  __nds__write_csr(umic_ctl_bkp, NDS_UMISC_CTL);
#endif

#endif  // __INTERNAL_NDS_BF16_H__

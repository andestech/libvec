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
#ifndef __INTERNAL_VEC_TRANSPOSE_H__
#define __INTERNAL_VEC_TRANSPOSE_H__
#ifdef ENA_DYNAMIC_CALC_CACHE_CONFIG
#include "nds_intrinsic.h"
#include <math.h>
#endif

#ifdef ENA_DYNAMIC_CALC_CACHE_CONFIG
//----- difinitions for L2C_begin -----
#define __I                     volatile const	/* 'read only' permissions      */
#define __O                     volatile        /* 'write only' permissions     */
#define __IO                    volatile        /* 'read / write' permissions   */

/*****************************************************************************
 * SMU - AE350
 ****************************************************************************/
typedef struct {
	__I  unsigned int SYSTEMVER;            /* 0x00 SYSTEM ID and Revision Register */
	     unsigned int RESERVED0[1];         /* 0x04 Reserved */
	__I  unsigned int SYSTEMCFG;            /* 0x08 SYSTEM configuration register */
	__I  unsigned int SMUVER;               /* 0x0C SMU version register */
	__IO unsigned int WRSR;                 /* 0x10 Wakeup and Reset Status Register */
	__IO unsigned int SMUCR;                /* 0x14 SMU Command Register */
	     unsigned int RESERVED1[1];         /* 0x18 Reserved */
	__IO unsigned int WRMASK;               /* 0x1C Wake up Mask Register */
	__IO unsigned int CER;                  /* 0x20 Clock Enable Register */
	__IO unsigned int CRR;                  /* 0x24 Clock Ratio Register */
	     unsigned int RESERVED2[6];         /* 0x28 ~ 0x3C Reserved Register */
	__IO unsigned int SCRATCH;              /* 0x40 Scratch Register */
	     unsigned int RESERVED3[3];         /* 0x44 ~ 0x4C Reserved */
	__IO unsigned int RESET_VECTOR;         /* 0x50 CPU Reset Vector Register */
} SMU_RegDef;

/*****************************************************************************
 * L2CACHE - AE350
 ****************************************************************************/
typedef struct {
	__I  unsigned long long CFG;            /* 0x00 Configuration Register */
	__IO unsigned long long CTL;            /* 0x08 Control Register */
	__IO unsigned long long HPMCTL0;        /* 0x10 HPM Control Register0 */
	__IO unsigned long long HPMCTL1;        /* 0x18 HPM Control Register1 */
	__IO unsigned long long HPMCTL2;        /* 0x20 HPM Control Register2 */
	__IO unsigned long long HPMCTL3;        /* 0x28 HPM Control Register3 */
	__IO unsigned long long ASYNERR;        /* 0x30 Asynchronous Error Register */
	__IO unsigned long long ERR;            /* 0x38 Error Register */
	union {
		struct {
			__IO unsigned long long CORE0CCTLCMD;   /* 0x40 Core 0 CCTL Command Register */
			__IO unsigned long long CORE0CCTLACC;   /* 0x48 Core 0 CCTL Access Line Register */
			__IO unsigned long long CORE1CCTLCMD;   /* 0x50 Core 1 CCTL Command Register */
			__IO unsigned long long CORE1CCTLACC;   /* 0x58 Core 1 CCTL Access Line Register */
			__IO unsigned long long CORE2CCTLCMD;   /* 0x60 Core 2 CCTL Command Register */
			__IO unsigned long long CORE2CCTLACC;   /* 0x68 Core 2 CCTL Access Line Register */
			__IO unsigned long long CORE3CCTLCMD;   /* 0x70 Core 3 CCTL Command Register */
			__IO unsigned long long CORE3CCTLACC;   /* 0x78 Core 3 CCTL Access Line Register */
			__I  unsigned long long CCTLSTATUS;     /* 0x80 CCTL Status Register */
			     unsigned long long RESERVED0;      /* 0x88 Reserved */
			__IO unsigned long long TGTWDATA[4];    /* 0x90 ~ 0xAF TGT Write Data 0 to 3 */
			__I  unsigned long long TGTRDATA[4];    /* 0xB0 ~ 0xCF TGT Read Data 0 to 3 */
			__IO unsigned long long TGTWECC;        /* 0xD0 TGT Write ECC Code Register */
			__I  unsigned long long TGTRECC;        /* 0xD8 TGT Read ECC Code Register */
			     unsigned long long RESERVED1[36];  /* 0xE0 ~ 0x1FF Reserved */
			__IO unsigned long long HPMCNT[32];     /* 0x200 ~ 0x2F8 HPM Counter Register 0 to 31 */
			__I  unsigned long long WAYMASK[16];    /* 0x300 ~ 0x378 Way Allocation Mask Register 0 to 15 */
		} REG;
		struct {
			__IO unsigned long long CCTLCMD;        /* 0x40 Core 0 CCTL Command Register */
			__IO unsigned long long CCTLACC;        /* 0x48 Core 0 CCTL Access Line Register */
			     unsigned long long RESERVED0[6];   /* 0x50 ~ 0x7F Reserved */
			__I  unsigned long long CCTLSTATUS;     /* 0x80 Core 0 CCTL Status Register */
			     unsigned long long RESERVED[503];  /* 0x88 Reserved */
		} CORECCTL[8];
	};
} L2C_RegDef;

// #define SMU_BASE                _IO_(0xF0100000)
#define SMU_BASE                (0xF0100000)
#define L2C_BASE                (0xE0500000)
#define AE350_SMU               ((SMU_RegDef *)  SMU_BASE)
#define AE350_L2C               ((L2C_RegDef *)  L2C_BASE)
#define DEV_SMU              AE350_SMU
#define DEV_L2C              AE350_L2C

/* CSR NDS_MCACHE_CTL */
#define DC_WARND_MSK                            (0x3 << 13)
#define DC_COHEN_MSK                            (0x1 << 19)
#define DC_COHSTA_MSK                           (0x1 << 20)

/* CSR NDS_DCM_CFG */
#define DSET_MSK                                ((1ULL << 2) | (1ULL << 1) | (1ULL << 0))
#define DWAY_MSK                                ((1ULL << 5) | (1ULL << 4) | (1ULL << 3))
#define DSIZE_MSK                               ((1ULL << 8) | (1ULL << 7) | (1ULL << 6))

/* SMU.SYSTEMCFG Configuration Register */
#define L2C_CTL_OFF                             8
#define L2C_CTL_MSK                             (0x1 << L2C_CTL_OFF)

/* Configuration Register */
#define L2C_SIZE_OFF                            7
#define L2C_SIZE_MSK                            (0x1F << L2C_SIZE_OFF)
#define L2C_SIZE_0KB                            (0x00 << L2C_SIZE_OFF)
#define L2C_SIZE_128KB                          (0x01 << L2C_SIZE_OFF)
#define L2C_SIZE_256KB                          (0x02 << L2C_SIZE_OFF)
#define L2C_SIZE_512KB                          (0x04 << L2C_SIZE_OFF)
#define L2C_SIZE_1024KB                         (0x08 << L2C_SIZE_OFF)
#define L2C_SIZE_2048KB                         (0x10 << L2C_SIZE_OFF)
#define L2C_LINE_SIZE                           32

/* L2 cache version */
#define L2C_VERSION_OFF                         24
#define L2C_VERSION_MSK                         (0xFF << L2C_VERSION_OFF)

/* Control Register */
#define L2C_ENABLE                              (0x1 << 0)
#define L2C_IPFDPT                              (0x3 << 3)
#define L2C_DPFDPT                              (0x3 << 5)

static inline void vec_calc_tiling_size_and_cache_config(uint32_t *cache_size_byte, uint32_t *tiling_size)
{
    /* NOTE: */
    /* Simulator cannot verify these code because of the CSRs below. */

    if(DEV_SMU->SYSTEMCFG & L2C_CTL_MSK) // CPU supports L2C cache
    {
        if ((DEV_L2C->CFG & L2C_SIZE_MSK) >> L2C_SIZE_OFF)
        {
            *cache_size_byte = (unsigned int)(((DEV_L2C->CFG & L2C_SIZE_MSK) >> L2C_SIZE_OFF) * 128) << 10;
        }
    }
    else if((__nds__mfsr(NDS_MDCM_CFG) >> 6) & 7) // if L1 cache exits.
    {
        // calculate cache_size
        unsigned int seth, set, way, line;
        seth = (__nds__mfsr(NDS_MDCM_CFG) >> 24) & 1;
        set = __nds__mfsr(NDS_MDCM_CFG) & 7;
        way = (__nds__mfsr(NDS_MDCM_CFG) >> 3) & 7;
        line = (__nds__mfsr(NDS_MDCM_CFG) >> 6) & 7;
        // calculate set value
        if (seth == 0)
        {
            set = 64 << set;
        }
        else if (seth == 1)
        {
            set = 32 >> set;
        }
        // calculate way value
        way++;
        // calculate line value
        if (line != 0)
        {
            line = 4 << line;
        }
        *cache_size_byte = set * way * line;
    }
    else
    {
        *cache_size_byte = 0;
    }

    //decide bsize aka. tiling_size
    int a;
    int tiling_size_arr[] = { 1,8,16,32,64,128,256,512,1024 };
    int max_tile_size_in_cache = sqrt(((*cache_size_byte) >> 1) / sizeof(MM_TYPE));
    for (a = 1; a < sizeof(tiling_size_arr) / sizeof(tiling_size_arr[0]); a++)
    {
        if (tiling_size_arr[a] > max_tile_size_in_cache)
        {
            *tiling_size = tiling_size_arr[a - 1];
            break;
        }
    }
}
#endif //ENA_DYNAMIC_CALC_CACHE_CONFIG
#endif //__INTERNAL_VEC_TRANSPOSE_H__

/*
    This file is part of IngenicHAL.
    Copyright (C) 2022 Reimu NotMoe <reimu@sudomaker.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

typedef struct {
	// 0x00
	volatile const uint32_t INL;
	volatile const uint32_t _rsvd_04[3];
	// 0x10
	volatile uint32_t INT;
	volatile uint32_t INTS;
	volatile uint32_t INTC;
	volatile const uint32_t _rsvd_1c[1];
	// 0x20
	volatile uint32_t MSK;
	volatile uint32_t MSKS;
	volatile uint32_t MSKC;
	volatile const uint32_t _rsvd_2c[1];
	// 0x30
	volatile uint32_t PAT1;
	volatile uint32_t PAT1S;
	volatile uint32_t PAT1C;
	volatile const uint32_t _rsvd_3c[1];
	// 0x40
	volatile uint32_t PAT0;
	volatile uint32_t PAT0S;
	volatile uint32_t PAT0C;
	volatile const uint32_t _rsvd_4c[1];
	// 0x50
	volatile const uint32_t FLG;
	volatile const uint32_t _rsvd_54[1];
	volatile const uint32_t FLGC;
	volatile const uint32_t _rsvd_5c[1];
	// 0x60
	volatile const uint32_t _rsvd_60[4];
	// 0x70
#if defined(XHAL_CHIP_X2000)
	volatile const uint32_t EDG;
	volatile const uint32_t EDGS;
	volatile const uint32_t EDGC;
	volatile const uint32_t _rsvd_7c[1];
#else
	volatile uint32_t PEN;
	volatile uint32_t PENS;
	volatile uint32_t PENC;
	volatile const uint32_t _rsvd_7c[1];
#endif
	// 0x80
#if defined(XHAL_CHIP_X2000)
	volatile uint32_t PU;
	volatile uint32_t PUS;
	volatile uint32_t PUC;
	volatile const uint32_t _rsvd_8c[1];
#else
	volatile const uint32_t _rsvd_80[4];
#endif
	// 0x90
#if defined(XHAL_CHIP_X2000)
	volatile uint32_t PD;
	volatile uint32_t PDS;
	volatile uint32_t PDC;
	volatile const uint32_t _rsvd_9c[1];
#else
	volatile const uint32_t _rsvd_90[4];
#endif
	// 0xA0
#if defined(XHAL_CHIP_X2000)
	volatile uint32_t DS0;
	volatile uint32_t DS0S;
	volatile uint32_t DS0C;
	volatile const uint32_t _rsvd_ac[1];
#else
	volatile const uint32_t _rsvd_a0[4];
#endif
	// 0xB0
#if defined(XHAL_CHIP_X2000)
	volatile uint32_t DS1;
	volatile uint32_t DS1S;
	volatile uint32_t DS1C;
	volatile const uint32_t _rsvd_bc[1];
#else
	volatile const uint32_t _rsvd_b0[4];
#endif
	// 0xC0
#if defined(XHAL_CHIP_X2000)
	volatile uint32_t DS2;
	volatile uint32_t DS2S;
	volatile uint32_t DS2C;
	volatile const uint32_t _rsvd_cc[1];
#else
	volatile const uint32_t _rsvd_c0[4];
#endif
	// 0xD0
#if defined(XHAL_CHIP_X2000)
	volatile uint32_t SR;
	volatile uint32_t SRS;
	volatile uint32_t SRC;
	volatile const uint32_t _rsvd_dc[1];
#else
	volatile const uint32_t _rsvd_d0[4];
#endif
	// 0xE0
#if defined(XHAL_CHIP_X2000)
	volatile uint32_t SMT;
	volatile uint32_t SMTS;
	volatile uint32_t SMTC;
	volatile const uint32_t _rsvd_ec[1];
#else
	volatile const uint32_t _rsvd_e0[4];
#endif
	// 0xF0
	volatile uint32_t GID2LD;
	volatile const uint32_t _rsvd_f4[3];
} XHAL_GPIO_TypeDef;

#define XHAL_PHYSADDR_GPIO			0x10010000
#define XHAL_REGWIDTH_GPIO_PORT			0x100

#if defined(XHAL_CHIP_X2000)
#define XHAL_GPIO_NR_PORTS			5
#elif defined(XHAL_CHIP_X1000)
#define XHAL_GPIO_NR_PORTS			4
#else
#define XHAL_GPIO_NR_PORTS			7
#endif

typedef char __size_assert_XHAL_GPIO_TypeDef[(sizeof(XHAL_GPIO_TypeDef) == XHAL_REGWIDTH_GPIO_PORT) * 2 - 1];

#ifdef __cplusplus
}
#endif

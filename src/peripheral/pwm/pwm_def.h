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
	volatile uint32_t CCFG0;
	volatile uint32_t CCFG1;
	volatile const uint32_t _rsvd_08[2];
	// 0x10
	volatile uint32_t EN;
	volatile uint32_t ENS;
	volatile uint32_t ENC;
	volatile const uint32_t _rsvd_1c[1];
	// 0x20
	volatile uint32_t UPDATE;
	volatile uint32_t BUSY;
	volatile const uint32_t _rsvd_28[2];
	// 0x30
	volatile uint32_t LVL;
	volatile const uint32_t _rsvd_34[3];
	// 0x40 - 0xAF
	volatile const uint8_t _rsvd_40[0x70];
	// 0xB0
	volatile const uint32_t WCFG[16];
	// 0xF0
	volatile const uint32_t _rsvd_f0[4];
	// 0x100
	volatile uint32_t DES;
	volatile uint32_t DEC;
	volatile uint32_t DE;
	volatile const uint32_t _rsvd_10c[1];
	// 0x110
	volatile uint32_t DCR0;
	volatile uint32_t DCR1;
	volatile const uint32_t _rsvd_118[2];
	// 0x120
	volatile uint32_t DTRIG;
	volatile uint32_t DFER;
	volatile uint32_t DFSM;
	volatile const uint32_t _rsvd_12c[1];
	// 0x130
	volatile uint32_t DSR;
	volatile uint32_t DSCR;
	volatile uint32_t DINTC;
	volatile const uint32_t _rsvd_13c[1];
	// 0x140
	volatile uint32_t DMADDR[16];
	// 0x180
	volatile const uint32_t _rsvd_180[4];
	// 0x190
	volatile uint32_t DTLR[16];
	// 0x1D0 - 0x2FF
	volatile const uint8_t _rsvd_1d0[0x130];
	// 0x300
	volatile uint32_t OEN;
	volatile const uint32_t _rsvd_304[3];
} XHAL_PWM_TypeDef;

#define XHAL_PHYSADDR_PWM			0x134C0000
#define XHAL_REGWIDTH_PWM			0x310

typedef char __size_assert_XHAL_PWM_TypeDef[(sizeof(XHAL_PWM_TypeDef) == XHAL_REGWIDTH_PWM) * 2 - 1];

#ifdef __cplusplus
}
#endif

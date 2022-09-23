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

#include "pwm.h"

void XHAL_PWM_SetChannelPrescaler(volatile XHAL_PWM_TypeDef *xpwm, unsigned int channel_num, unsigned int prescaler) {
	volatile uint32_t *reg = (channel_num < 8) ? &xpwm->CCFG0 : &xpwm->CCFG1;

	channel_num &= 0x7;
	uint32_t reg_copy = *reg;

	reg_copy &= ~((0xf) << (channel_num * 4));
	reg_copy |= (prescaler & 0xf) << (channel_num * 4);

	*reg = reg_copy;
}

void XHAL_PWM_SetChannelDMAData(volatile XHAL_PWM_TypeDef *xpwm, unsigned int channel_num, uint32_t buf_physaddr, uint32_t buf_len) {
	xpwm->DMADDR[channel_num] = buf_physaddr;
	xpwm->DTLR[channel_num] = buf_len;
}
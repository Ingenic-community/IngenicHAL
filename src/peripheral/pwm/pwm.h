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

#include "pwm_def.h"
#include "../../common/bitops.h"

#define XHAL_PWM_SetChannelEnabled(xpwm, channel_num, enabled) if (enabled) {(xpwm)->ENS = XHAL_BIT(channel_num);} else {(xpwm)->ENC = XHAL_BIT(channel_num);}
#define XHAL_PWM_SetChannelMode(xpwm, channel_num, use_dma) if (use_dma) {XHAL_BIT_SET((xpwm)->DCR0, channel_num);} else {XHAL_BIT_CLR((xpwm)->DCR0, channel_num);}
#define XHAL_PWM_SetChannelDMAEnabled(xpwm, channel_num, enabled) if (enabled) {(xpwm)->DES = XHAL_BIT(channel_num);} else {(xpwm)->DEC = XHAL_BIT(channel_num);}
#define XHAL_PWM_SetChannelDMARepeatEnabled(xpwm, channel_num, enabled) if (enabled) {XHAL_BIT_SET((xpwm)->DCR1, channel_num);} else {XHAL_BIT_CLR((xpwm)->DCR1, channel_num);}
#define XHAL_PWM_StartChannelDMA(xpwm, channel_num) (xpwm)->DTRIG = XHAL_BIT(channel_num)

extern void XHAL_PWM_SetChannelPrescaler(volatile XHAL_PWM_TypeDef *xpwm, unsigned int channel_num, unsigned int prescaler);
extern void XHAL_PWM_SetChannelDMAData(volatile XHAL_PWM_TypeDef *xpwm, unsigned int channel_num, uint32_t buf_physaddr, uint32_t buf_len);

#ifdef __cplusplus
}
#endif

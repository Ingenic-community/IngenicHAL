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

#include "gpio_def.h"
#include "../../common/bitops.h"

#define XHAL_GPIO_GetINL(port_addr, pin_num)		XHAL_BIT_GET(((volatile XHAL_GPIO_TypeDef *)(port_addr))->INL, pin_num)

#define XHAL_GPIO_GetINT(port_addr, pin_num)		XHAL_BIT_GET(((volatile XHAL_GPIO_TypeDef *)(port_addr))->INT, pin_num)
#define XHAL_GPIO_SetINT(port_addr, pin_num, value)	if (value) ((volatile XHAL_GPIO_TypeDef *)(port_addr))->INTS = XHAL_BIT(pin_num); else \
							((volatile XHAL_GPIO_TypeDef *)(port_addr))->INTC = XHAL_BIT(pin_num)

#define XHAL_GPIO_GetMSK(port_addr, pin_num)		XHAL_BIT_GET(((volatile XHAL_GPIO_TypeDef *)(port_addr))->MSK, pin_num)
#define XHAL_GPIO_SetMSK(port_addr, pin_num, value)	if (value) ((volatile XHAL_GPIO_TypeDef *)(port_addr))->MSKS = XHAL_BIT(pin_num); else \
							((volatile XHAL_GPIO_TypeDef *)(port_addr))->MSKC = XHAL_BIT(pin_num)

#define XHAL_GPIO_GetPAT0(port_addr, pin_num)		XHAL_BIT_GET(((volatile XHAL_GPIO_TypeDef *)(port_addr))->PAT0, pin_num)
#define XHAL_GPIO_SetPAT0(port_addr, pin_num, value)	if (value) ((volatile XHAL_GPIO_TypeDef *)(port_addr))->PAT0S = XHAL_BIT(pin_num); else \
							((volatile XHAL_GPIO_TypeDef *)(port_addr))->PAT0C = XHAL_BIT(pin_num)

#define XHAL_GPIO_GetPAT1(port_addr, pin_num)		XHAL_BIT_GET(((volatile XHAL_GPIO_TypeDef *)(port_addr))->PAT1, pin_num)
#define XHAL_GPIO_SetPAT1(port_addr, pin_num, value)	if (value) ((volatile XHAL_GPIO_TypeDef *)(port_addr))->PAT1S = XHAL_BIT(pin_num); else \
							((volatile XHAL_GPIO_TypeDef *)(port_addr))->PAT1C = XHAL_BIT(pin_num)

#define XHAL_GPIO_GetFLG(port_addr, pin_num)		XHAL_BIT_GET(((volatile XHAL_GPIO_TypeDef *)(port_addr))->FLG, pin_num)
#define XHAL_GPIO_ClearFLG(port_addr, pin_num)		((volatile XHAL_GPIO_TypeDef *)(port_addr))->FLGC = XHAL_BIT(pin_num)

#define XHAL_GPIO_GetPEN(port_addr, pin_num)		XHAL_BIT_GET(((volatile XHAL_GPIO_TypeDef *)(port_addr))->PEN, pin_num)
#define XHAL_GPIO_SetPEN(port_addr, pin_num)		if (value) ((volatile XHAL_GPIO_TypeDef *)(port_addr))->PENS = XHAL_BIT(pin_num); else \
							((volatile XHAL_GPIO_TypeDef *)(port_addr))->PENC = XHAL_BIT(pin_num)

#define XHAL_GPIO_TogglePin(port_addr, pin_num)		if (XHAL_GPIO_GetPAT0(port_addr, pin_num)) \
							((volatile XHAL_GPIO_TypeDef *)(port_addr))->PAT0C = XHAL_BIT(pin_num); else                                         \
							((volatile XHAL_GPIO_TypeDef *)(port_addr))->PAT0S = XHAL_BIT(pin_num)

#define XHAL_GPIO_WritePin				XHAL_GPIO_SetPAT0
#define XHAL_GPIO_ReadPin				XHAL_GPIO_GetINL

#if defined(XHAL_CHIP_X2000)

#else

#endif

extern void XHAL_GPIO_SetAsFunction(volatile XHAL_GPIO_TypeDef *xgpio, uint8_t pin_number, uint8_t function);
extern void XHAL_GPIO_SetAsGPIO(volatile XHAL_GPIO_TypeDef *xgpio, uint8_t pin_number, uint8_t is_input);

#ifdef __cplusplus
}
#endif

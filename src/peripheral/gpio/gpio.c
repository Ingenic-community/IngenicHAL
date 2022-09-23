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

#include "gpio.h"

void XHAL_GPIO_SetAsFunction(volatile XHAL_GPIO_TypeDef *xgpio, uint8_t pin_number, uint8_t function) {
	XHAL_GPIO_SetINT(xgpio, pin_number, 0);
	XHAL_GPIO_SetMSK(xgpio, pin_number, 0);
	XHAL_GPIO_SetPAT0(xgpio, pin_number, XHAL_BIT_GET(function, 0));
	XHAL_GPIO_SetPAT1(xgpio, pin_number, XHAL_BIT_GET(function, 1));
}

void XHAL_GPIO_SetAsGPIO(volatile XHAL_GPIO_TypeDef *xgpio, uint8_t pin_number, uint8_t is_input) {
	xgpio->INTC = XHAL_BIT(pin_number);
	xgpio->MSKS = XHAL_BIT(pin_number);
	if (is_input) {
		xgpio->PAT1S = XHAL_BIT(pin_number);
	} else {
		xgpio->PAT1C = XHAL_BIT(pin_number);
	}
}

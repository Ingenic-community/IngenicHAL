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

#include <ingenic-hal.h>


int main() {
	volatile XHAL_GPIO_TypeDef *xgpiod = (volatile void *)(XHAL_PHYSADDR_GPIO + XHAL_REGWIDTH_GPIO_PORT * 3);

	XHAL_GPIO_SetAsGPIO(xgpiod, 0, 0);

	while (1) {
		xgpiod->PAT0S = XHAL_BIT(0);
		xgpiod->PAT0C = XHAL_BIT(0);
	}
}
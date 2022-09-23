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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/cachectl.h>

#include <ingenic-hal.h>

#if defined(XHAL_CHIP_X2000)
#define CHIP_NAME "X2000"
#else
#error This chip does NOT have the PWM peripheral
#endif

static volatile XHAL_PWM_TypeDef *xpwm = NULL;


#define DMA_SPACE_PHYSADDR	0x07ff0000

int main(int argc, char **argv) {
	int fd = open("/dev/mem", O_RDWR|O_SYNC);

	if (fd < 0) {
		perror("error: failed to open /dev/mem");
		return 2;
	}

	void *phys_mem = mmap(NULL, XHAL_REGWIDTH_PWM, PROT_READ|PROT_WRITE, MAP_SHARED, fd, XHAL_PHYSADDR_PWM);

	if (phys_mem == MAP_FAILED) {
		perror("error: mmap failed");
		return 2;
	}

	xpwm = phys_mem;

	volatile uint32_t *pwm_values = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, DMA_SPACE_PHYSADDR);

	if (pwm_values == MAP_FAILED) {
		perror("error: failed to map dma space");
		abort();
	}

	printf("PWM DMA buffer at %p\n", (void *)DMA_SPACE_PHYSADDR);

	for (size_t i=0; i<16; i+=2) {
		pwm_values[i] = 0x00100010;
		pwm_values[i+1] = 0x00080018;
	}

	XHAL_PWM_SetChannelEnabled(xpwm, 1, 0);
	XHAL_PWM_SetChannelDMAEnabled(xpwm, 1, 0);
	XHAL_PWM_SetChannelMode(xpwm, 1, 1);

	XHAL_PWM_SetChannelDMAData(xpwm, 1, DMA_SPACE_PHYSADDR, 16);

	while (xpwm->DFSM & XHAL_BIT(1));

	XHAL_PWM_SetChannelDMARepeatEnabled(xpwm, 1, 1);
	XHAL_PWM_SetChannelDMAEnabled(xpwm, 1, 1);
	XHAL_PWM_StartChannelDMA(xpwm, 1);

	printf("Done.\n");

	return 0;
}

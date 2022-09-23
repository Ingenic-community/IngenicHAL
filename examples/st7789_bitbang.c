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
#warning This is designed to run on X2000. Your mileage may vary.
#endif

static volatile XHAL_GPIO_TypeDef *xgpiob = NULL;

static void GPIO_Init() {
	for (size_t i=0; i<8; i++) {
		XHAL_GPIO_SetAsGPIO(xgpiob, i, 0);
	}

	// RESET
	XHAL_GPIO_SetAsGPIO(xgpiob, 23, 0);
	// RD
	XHAL_GPIO_SetAsGPIO(xgpiob, 16, 0);
	// WR
	XHAL_GPIO_SetAsGPIO(xgpiob, 26, 0);
	// CS
	XHAL_GPIO_SetAsGPIO(xgpiob, 24, 0);
	// RS
	XHAL_GPIO_SetAsGPIO(xgpiob, 25, 0);
}

#define delay()

static inline void PIN_RES(uint8_t v) {
	XHAL_GPIO_WritePin(xgpiob, 23, v);
	delay();
}

static inline void PIN_RD(uint8_t v) {
	XHAL_GPIO_WritePin(xgpiob, 16, v);
	delay();
}

static inline void PIN_WR(uint8_t v) {
	XHAL_GPIO_WritePin(xgpiob, 26, v);
	delay();
}

static inline void PIN_CS(uint8_t v) {
	XHAL_GPIO_WritePin(xgpiob, 24, v);
	delay();
}

static inline void PIN_RS(uint8_t v) {
	XHAL_GPIO_WritePin(xgpiob, 25, v);
	delay();
}

static inline void PIN_DATA8(uint8_t v) {
	xgpiob->PAT0C = 0x0000ff;
	xgpiob->PAT0S = v;
	delay();
}

void HD_reset() {
	PIN_RES(0);
	usleep(1000*150);
	PIN_RES(1);
	usleep(1000*100);
}

void LLCD_WRITE_CMD(uint8_t cmd) {
	PIN_RS(0);
	PIN_RD(1);
	PIN_WR(1);
	PIN_CS(0);
	PIN_DATA8(cmd);
	PIN_WR(0);
	PIN_WR(1);
	PIN_CS(1);
}

void LLCD_WRITE_DATA(uint8_t data) {
	PIN_RS(1);
	PIN_RD(1);
	PIN_WR(1);
	PIN_CS(0);
	PIN_DATA8(data);
	PIN_WR(0);
	PIN_WR(1);
	PIN_CS(1);
}

void LCD_init()
{
	HD_reset();
//---------------------------------------------------------------------------------------------------//
	LLCD_WRITE_CMD (0x11);
	usleep(1000*120); //Delay 120ms
//------------------------------display and color format setting--------------------------------//
	LLCD_WRITE_CMD (0x36);
	LLCD_WRITE_DATA (0x00);
	LLCD_WRITE_CMD (0x3a);
//	LLCD_WRITE_DATA (0x05); // 565
	LLCD_WRITE_DATA (0x06); // 888

//--------------------------------ST7789V Frame rate setting----------------------------------//
	LLCD_WRITE_CMD (0xb2);
	LLCD_WRITE_DATA (0x0c);
	LLCD_WRITE_DATA (0x0c);
	LLCD_WRITE_DATA (0x00);
	LLCD_WRITE_DATA (0x33);
	LLCD_WRITE_DATA (0x33);
	LLCD_WRITE_CMD (0xb7);
	LLCD_WRITE_DATA (0x35);
//---------------------------------ST7789V Power setting--------------------------------------//
	LLCD_WRITE_CMD (0xbb);
	LLCD_WRITE_DATA (0x28);
	LLCD_WRITE_CMD (0xc0);
	LLCD_WRITE_DATA (0x2c);
	LLCD_WRITE_CMD (0xc2);
	LLCD_WRITE_DATA (0x01);
	LLCD_WRITE_CMD (0xc3);
	LLCD_WRITE_DATA (0x0b);
	LLCD_WRITE_CMD (0xc4);
	LLCD_WRITE_DATA (0x20);
	LLCD_WRITE_CMD (0xc6);
	LLCD_WRITE_DATA (0x0f);
	LLCD_WRITE_CMD (0xd0);
	LLCD_WRITE_DATA (0xa4);
	LLCD_WRITE_DATA (0xa1);
//--------------------------------ST7789V gamma setting---------------------------------------//
	LLCD_WRITE_CMD (0xe0);
	LLCD_WRITE_DATA (0xd0);
	LLCD_WRITE_DATA (0x01);
	LLCD_WRITE_DATA (0x08);
	LLCD_WRITE_DATA (0x0f);
	LLCD_WRITE_DATA (0x11);
	LLCD_WRITE_DATA (0x2a);
	LLCD_WRITE_DATA (0x36);
	LLCD_WRITE_DATA (0x55);
	LLCD_WRITE_DATA (0x44);
	LLCD_WRITE_DATA (0x3a);
	LLCD_WRITE_DATA (0x0b);
	LLCD_WRITE_DATA (0x06);
	LLCD_WRITE_DATA (0x11);
	LLCD_WRITE_DATA (0x20);
	LLCD_WRITE_CMD (0xe1);
	LLCD_WRITE_DATA (0xd0);
	LLCD_WRITE_DATA (0x02);
	LLCD_WRITE_DATA (0x07);
	LLCD_WRITE_DATA (0x0a);
	LLCD_WRITE_DATA (0x0b);
	LLCD_WRITE_DATA (0x18);
	LLCD_WRITE_DATA (0x34);
	LLCD_WRITE_DATA (0x43);
	LLCD_WRITE_DATA (0x4a);
	LLCD_WRITE_DATA (0x2b);
	LLCD_WRITE_DATA (0x1b);
	LLCD_WRITE_DATA (0x1c);
	LLCD_WRITE_DATA (0x22);
	LLCD_WRITE_DATA (0x1f);
	LLCD_WRITE_CMD (0x29);
}

void LCD_setwindow(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1)
{
	LLCD_WRITE_CMD(0x2a);
	LLCD_WRITE_DATA(x0>>8);
	LLCD_WRITE_DATA(x0&0xff);
	LLCD_WRITE_DATA(x1>>8);
	LLCD_WRITE_DATA(x1&0xff);

	LLCD_WRITE_CMD(0x2b);
	LLCD_WRITE_DATA(y0>>8);
	LLCD_WRITE_DATA(y0&0xff);
	LLCD_WRITE_DATA(y1>>8);
	LLCD_WRITE_DATA(y1&0xff);

	LLCD_WRITE_CMD(0x2C);
}


void LCD_TEST_SingleColor(uint8_t r, uint8_t g, uint8_t b)
{

	unsigned int i,j;
	LCD_setwindow(0,0,239,319);

	PIN_CS(0);

	PIN_RS(1);
	PIN_RD(1);

	for(i=0;i<320;i++)
	{
		for(j=0;j<240;j++)
		{
			PIN_DATA8(r);
			PIN_WR(0);
			PIN_WR(1);
			PIN_DATA8(g);
			PIN_WR(0);
			PIN_WR(1);
			PIN_DATA8(b);
			PIN_WR(0);
			PIN_WR(1);
		}
	}

	PIN_CS(1);
}

void LCD_TEST_ColorBar()
{

	unsigned int i,j;
	LCD_setwindow(0,0,239,319);

	PIN_CS(0);

	PIN_RS(1);
	PIN_RD(1);

	for(i=0;i<320;i++)
	{
		for(j=0;j<80;j++)
		{
			PIN_DATA8(255);
			PIN_WR(0);
			PIN_WR(1);
			PIN_DATA8(0);
			PIN_WR(0);
			PIN_WR(1);
			PIN_DATA8(0);
			PIN_WR(0);
			PIN_WR(1);
		}

		for(j=0;j<80;j++)
		{
			PIN_DATA8(0);
			PIN_WR(0);
			PIN_WR(1);
			PIN_DATA8(255);
			PIN_WR(0);
			PIN_WR(1);
			PIN_DATA8(0);
			PIN_WR(0);
			PIN_WR(1);
		}

		for(j=0;j<80;j++)
		{
			PIN_DATA8(0);
			PIN_WR(0);
			PIN_WR(1);
			PIN_DATA8(0);
			PIN_WR(0);
			PIN_WR(1);
			PIN_DATA8(255);
			PIN_WR(0);
			PIN_WR(1);
		}
	}

	PIN_CS(1);
}

void LCD_TEST_ColorBar2()
{

	unsigned int i,j;
	LCD_setwindow(0,0,239,319);

	PIN_CS(0);

	PIN_RS(1);
	PIN_RD(1);

	for(i=0;i<320;i++)
	{
		for(j=0;j<80;j++)
		{
			PIN_DATA8(0);
			PIN_WR(0);
			PIN_WR(1);
			PIN_DATA8(0);
			PIN_WR(0);
			PIN_WR(1);
			PIN_DATA8(255);
			PIN_WR(0);
			PIN_WR(1);
		}

		for(j=0;j<80;j++)
		{
			PIN_DATA8(0);
			PIN_WR(0);
			PIN_WR(1);
			PIN_DATA8(255);
			PIN_WR(0);
			PIN_WR(1);
			PIN_DATA8(0);
			PIN_WR(0);
			PIN_WR(1);
		}

		for(j=0;j<80;j++)
		{
			PIN_DATA8(255);
			PIN_WR(0);
			PIN_WR(1);
			PIN_DATA8(0);
			PIN_WR(0);
			PIN_WR(1);
			PIN_DATA8(0);
			PIN_WR(0);
			PIN_WR(1);
		}
	}

	PIN_CS(1);
}

int main(int argc, char **argv) {
	int fd = open("/dev/mem", O_RDWR|O_SYNC);

	if (fd < 0) {
		perror("error: failed to open /dev/mem");
		return 2;
	}

	void *phys_mem = mmap(NULL, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, XHAL_PHYSADDR_GPIO);

	if (phys_mem == MAP_FAILED) {
		perror("error: mmap failed");
		return 2;
	}

	xgpiob = phys_mem + (XHAL_REGWIDTH_GPIO_PORT * 1);

	GPIO_Init();
	printf("GPIO init done.\n");

	LCD_init();

	printf("LCD init done.\n");

	while (1) {
		LCD_TEST_ColorBar();
		LCD_TEST_ColorBar2();
	}

	printf("done.\n");


	return 0;
}

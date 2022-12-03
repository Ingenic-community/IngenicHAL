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

volatile XHAL_GPIO_TypeDef *xgpioa, *xgpiob, *xgpioc, *xgpiod;

// Use this if you have a 16-bit RGB LCD is connected to PB<15:0>
// ffmpeg -vcodec png -i logo.png -vcodec rawvideo -f rawvideo -pix_fmt bgr565le logo.raw
#define LOGO_BGR565LE

// Use this if you have a 24-bit RGB LCD is connected to PB<24:0>, or a 16-bit RGB LCD with each color aligned to MSB
// ffmpeg -vcodec png -i logo.png -vcodec rawvideo -f rawvideo -pix_fmt bgra logo.raw
//#define LOGO_BGRA8888

// Snatch GPIO back from Linux
//#define ENABLE_GPIO_SNATCH

#if defined(LOGO_BGR565LE)
#define logo_pixel_t uint16_t
#define lcd_DATA lcd_DATA16
#elif defined(LOGO_BGRA8888)
#define logo_pixel_t uint32_t
#define lcd_DATA lcd_DATA24
#endif

// The logo file should be placed at 127MiB of DRAM
// The mini core has no MMU, so all addresses are in physical
volatile logo_pixel_t *logo = (void *)0x07F00000;
uint32_t logo_pos = 0;

logo_pixel_t lcd_Color() {
	logo_pixel_t ret = logo[logo_pos];
	logo_pos++;

	return ret;
}

int lcd_gpio_changed() {
	if (xgpiob->PAT1 & (1 << 27)) {
		return 1;
	}

	return 0;
}

void lcd_gpio_init(void) {
	int i;
	for (i=0; i<=27; i++) {
		XHAL_GPIO_SetAsGPIO(xgpiob, i, 0);
	}
}

void lcd_DATA16(uint16_t v) {
	xgpiob->PAT0C = 0xffff;
	xgpiob->PAT0S = v;
}

void lcd_DATA24(uint32_t v) {
	xgpiob->PAT0C = 0xffffff;
	xgpiob->PAT0S = v & 0xffffff;
}

void lcd_DE(int v) {
		XHAL_GPIO_WritePin(xgpiob, 27, v);
}

void lcd_PCLK(int v) {
		XHAL_GPIO_WritePin(xgpiob, 24, v);
}

void lcd_VSYNC(int v) {
		XHAL_GPIO_WritePin(xgpiob, 25, v);
}

void lcd_HSYNC(int v) {
		XHAL_GPIO_WritePin(xgpiob, 26, v);
}

void lcd_init() {
	lcd_gpio_init();

	lcd_VSYNC(1);
	lcd_HSYNC(1);
	lcd_DE(0);
	lcd_PCLK(0);
}

uint32_t resX = 480, resY = 272;

uint32_t Tvpw = 1;	/* VSYNC Width */
uint32_t Tvbp = 11;	/* VSYNC Back Porch */
uint32_t Tvfp = 13;	/* VSYNC Front Porch */

uint32_t Thpw = 8;	/* HSYNC Width */
uint32_t Thbp = 35;	/* HSYNC Back Porch */
uint32_t Thfp = 40;	/* HSYNC Front Porch */

//uint32_t resX = 800, resY = 480;
//
//uint32_t Tvpw = 3;	/* VSYNC Width */
//uint32_t Tvbp = 32;	/* VSYNC Back Porch */
//uint32_t Tvfp = 13;	/* VSYNC Front Porch */
//
//uint32_t Thpw = 48;	/* HSYNC Width */
//uint32_t Thbp = 88;	/* HSYNC Back Porch */
//uint32_t Thfp = 40;	/* HSYNC Front Porch */

void lcd_flip_PCLK() {
	lcd_PCLK(1);
	lcd_PCLK(0);
}

void lcd_BlastLine(int DE_needs_change) {
	uint32_t i;

	lcd_HSYNC(0);
	for (i=0; i<Thpw; i++) {
		lcd_flip_PCLK();
	}

	lcd_HSYNC(1);
	for (i=0; i<Thbp; i++) {
		lcd_flip_PCLK();
	}

	if (DE_needs_change)
		lcd_DE(1);

	for (i=0; i<resX; i++) {
		if (DE_needs_change) {
			lcd_DATA(lcd_Color());
		}
		lcd_flip_PCLK();
	}

	if (DE_needs_change)
		lcd_DE(0);

	for (i=0; i<Thfp; i++) {
		lcd_flip_PCLK();
	}

};

void lcd_BlastScreen() {
	uint32_t i;


	lcd_VSYNC(0);
	for (i=0; i<Tvpw; i++) {
		lcd_BlastLine(0);
	}

	lcd_VSYNC(1);
	for (i=0; i<Tvbp; i++) {
		lcd_BlastLine(0);
	}

	for (i=0; i<resY; i++) {
		lcd_BlastLine(1);
	}

	for (i=0; i<Tvfp; i++) {
		lcd_BlastLine(0);
	}

}

int main() {
	xgpioa = (volatile void *)(XHAL_PHYSADDR_GPIO + XHAL_REGWIDTH_GPIO_PORT * 0);
	xgpiob = (volatile void *)(XHAL_PHYSADDR_GPIO + XHAL_REGWIDTH_GPIO_PORT * 1);
	xgpioc = (volatile void *)(XHAL_PHYSADDR_GPIO + XHAL_REGWIDTH_GPIO_PORT * 2);
	xgpiod = (volatile void *)(XHAL_PHYSADDR_GPIO + XHAL_REGWIDTH_GPIO_PORT * 3);

	lcd_init();

	while (1) {
		lcd_BlastScreen();

#ifdef ENABLE_GPIO_SNATCH
		if (lcd_gpio_changed()) {
			lcd_gpio_init();
		}
#endif

		logo_pos = 0;
	}
}
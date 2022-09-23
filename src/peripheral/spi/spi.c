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

#include <stdio.h>
#include "spi.h"

int XHAL_SPI_Init(XHAL_SPI_HandleTypeDef *xhspi) {
	volatile XHAL_SPI_InitTypeDef *init = &xhspi->init;
	volatile XHAL_SPI_TypeDef *periph = xhspi->periph;

	periph->GR = init->brg;
	periph->CR0 = XHAL_REG_SPI_CR0_EACLRUN;
	periph->CR1 = 0x87830;
	periph->SR = 0;
	periph->CR0 |= XHAL_REG_SPI_CR0_SSIE;

	return 0;
}

int XHAL_SPI_CalculateBRG(uint32_t parent_clk, uint32_t desired_sclk, uint32_t *real_sclk) {
	// TODO

	return 0;
}

int XHAL_SPI_Transmit(XHAL_SPI_HandleTypeDef *xhspi, const uint8_t *buf, unsigned int len) {
	volatile XHAL_SPI_TypeDef *periph = xhspi->periph;

	const unsigned int fifo_half_len = XHAL_SPI_FIFO_SIZE / 2;
	unsigned int fifo_available = XHAL_SPI_FIFO_SIZE;

	periph->CR1bits.TTRG = fifo_half_len / 8;
	periph->CR0bits.DISREV = 1;

	uint8_t transfer_mode = 0;

	// Try to speed up when there are large amount of data
//	if (len >= fifo_half_len) {
//		if ((((uintptr_t) buf & 0b11) == 0) && (len % 4 == 0)) { // 32bit aligned
//			transfer_mode = 2;
//		} else if ((((uintptr_t) buf & 0b1) == 0) && (len % 2 == 0)) { // 16bit aligned
//			transfer_mode = 1;
//		}
//	}

	if (transfer_mode == 2) { // 32bit accelerated mode
		periph->CR1bits.FLEN = 32 - 2;
		periph->CR0bits.TENDIAN = 3;

		volatile uint32_t *dr32 = (volatile uint32_t *) &periph->DR;
		uint32_t *buf32 = (uint32_t *) buf;

		for (unsigned int i = 0; i < len / 4; i++) {
			while (!fifo_available) {
				if (periph->SR & XHAL_REG_SPI_SR_TFHE) {
					fifo_available = fifo_half_len;
				} else {
					if (xhspi->wait_callback) {
						int wrc = xhspi->wait_callback(xhspi);
						if (wrc != XHAL_WAIT_OK) {
							return wrc;
						}
					}

				}
			}

			*dr32 = buf32[i];
			fifo_available--;
		}
	} else if (transfer_mode == 1) { // 16bit accelerated mode
		periph->CR1bits.FLEN = 16 - 2;
		periph->CR0bits.TENDIAN = 3;

		volatile uint16_t *dr16 = (volatile uint16_t *) &periph->DR;
		uint16_t *buf16 = (uint16_t *) buf;

		for (unsigned int i = 0; i < len / 2; i++) {
			while (!fifo_available) {
				if (periph->SR & XHAL_REG_SPI_SR_TFHE) {
					fifo_available = fifo_half_len;
				} else {
					if (xhspi->wait_callback) {
						int wrc = xhspi->wait_callback(xhspi);
						if (wrc != XHAL_WAIT_OK) {
							return wrc;
						}
					}

				}
			}

			*dr16 = buf16[i];
			fifo_available--;
		}

	} else { // 8bit mode
		periph->CR1bits.FLEN = 8 - 2;
		periph->CR0bits.TENDIAN = 0;

		volatile uint8_t *dr = (volatile uint8_t *) &periph->DR;

		for (unsigned int i = 0; i < len; i++) {
			while (!fifo_available) {
				if (periph->SR & XHAL_REG_SPI_SR_TFHE) {
					fifo_available = fifo_half_len;
				} else {
					if (xhspi->wait_callback) {
						int wrc = xhspi->wait_callback(xhspi);
						if (wrc != XHAL_WAIT_OK) {
							return wrc;
						}
					}

				}
			}

			*dr = buf[i];
			fifo_available--;
		}
	}

	while (periph->SR & XHAL_REG_SPI_SR_BUSY) {
		if (xhspi->wait_callback) {
			int wrc = xhspi->wait_callback(xhspi);
			if (wrc != XHAL_WAIT_OK) {
				return wrc;
			}
		}
	}

	return 0;
}
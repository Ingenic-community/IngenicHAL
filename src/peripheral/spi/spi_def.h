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
#include "../../common/bitops.h"
#include "../../common/wait.h"

#define XHAL_REG_SPI_CR0_TENDIAN_LSB          XHAL_BIT(19)
#define XHAL_REG_SPI_CR0_RENDIAN_LSB          XHAL_BIT(17)
#define XHAL_REG_SPI_CR0_SSIE                 XHAL_BIT(15)
#define XHAL_REG_SPI_CR0_LOOP                 XHAL_BIT(10)
#define XHAL_REG_SPI_CR0_EACLRUN              XHAL_BIT(7)
#define XHAL_REG_SPI_CR0_FSEL                 XHAL_BIT(6)
#define XHAL_REG_SPI_CR0_TFLUSH               XHAL_BIT(2)
#define XHAL_REG_SPI_CR0_RFLUSH               XHAL_BIT(1)
#define XHAL_REG_SPI_CR0_DISREV               XHAL_BIT(0)

#define XHAL_REG_SPI_CR1_FRMHL_MASK           (XHAL_BIT(31) | XHAL_BIT(30))
#define XHAL_REG_SPI_CR1_FRMHL                XHAL_BIT(30)
#define XHAL_REG_SPI_CR1_LFST                 XHAL_BIT(25)
#define XHAL_REG_SPI_CR1_UNFIN                XHAL_BIT(23)
#define XHAL_REG_SPI_CR1_PHA                  XHAL_BIT(1)
#define XHAL_REG_SPI_CR1_POL                  XHAL_BIT(0)

#define XHAL_REG_SPI_SR_END                   XHAL_BIT(7)
#define XHAL_REG_SPI_SR_BUSY                  XHAL_BIT(6)
#define XHAL_REG_SPI_SR_TFF                   XHAL_BIT(5)
#define XHAL_REG_SPI_SR_RFE                   XHAL_BIT(4)
#define XHAL_REG_SPI_SR_TFHE                  XHAL_BIT(3)
#define XHAL_REG_SPI_SR_RFHF                  XHAL_BIT(2)
#define XHAL_REG_SPI_SR_UNDR                  XHAL_BIT(1)
#define XHAL_REG_SPI_SR_OVER                  XHAL_BIT(0)

typedef struct {
	volatile uint32_t DR;				/*!< SSI Data Register */
	volatile union {
		volatile uint32_t CR0;			/*!< SSI Control Register 0 */
		volatile struct {
			uint32_t DISREV : 1;		/*!< Receive disable */
			uint32_t RFLUSH : 1;		/*!< Flush RX FIFO */
			uint32_t TFLUSH : 1;		/*!< Flush TX FIFO */
			uint32_t TFMODE : 1;		/*!< FIFO drain mode */
			uint32_t VRCNT : 1;		/*!< Enable RCNT */
			uint32_t : 1;			/*!< Reserved */
			uint32_t FSEL : 1;		/*!< CE selection */
			uint32_t EACLRUN : 1;		/*!< Automatically clear TX FIFO underrun flag */
			uint32_t RFINC : 1;		/*!<  */
			uint32_t RFINE : 1;		/*!<  */
			uint32_t LOOP : 1;		/*!< Enable loopback mode */
			uint32_t REIE : 1;		/*!<  */
			uint32_t TEIE : 1;		/*!<  */
			uint32_t RIE : 1;		/*!<  */
			uint32_t TIE : 1;		/*!<  */
			uint32_t SSIE : 1;		/*!< SSI module enable */
			uint32_t RENDIAN : 2;		/*!< RX endian */
			uint32_t TENDIAN : 2;		/*!< TX endian */
			uint32_t : 12;			/*!< Reserved */
		} CR0bits;
	};
	volatile union {
		volatile uint32_t CR1;			/*!< SSI Control Register1 */
		volatile struct {
			uint32_t POL : 1;		/*!< SPI CPOL value */
			uint32_t PHA : 1;		/*!< SPI CPHA value */
			uint32_t : 1;			/*!< Reserved */
			uint32_t FLEN : 5;		/*!< Frame length & FIFO entry size (2~33) */
			uint32_t RTRG : 4;		/*!< RX FIFO half-full threshold (multiply with 8) */
			uint32_t MCOM : 4;		/*!< Length of command in Microwire format */
			uint32_t TTRG : 4;		/*!< TX FIFO half-full threshold (multiply with 8) */
			uint32_t FMAT : 2;		/*!< Transfer format */
			uint32_t : 1;			/*!< Reserved */
			uint32_t UNFIN : 1;		/*!< FIFO xrun behavior */
			uint32_t ITFRM : 1;		/*!< Assert CEs during interval time */
			uint32_t : 1;			/*!< Reserved */
			uint32_t TCKFI : 2;		/*!< Time from clock stop to frame invalid */
			uint32_t TFVCK : 2;		/*!< Time from frame valid to clock start */
			uint32_t FRMHL : 2;		/*!< Frame valid level select */
		} CR1bits;
	};
	volatile union {
		volatile uint32_t SR;			/*!< SSI Status Register */
		volatile struct {
			uint32_t OVER : 1;		/*!< RX FIFO overrun */
			uint32_t UNDR : 1;		/*!< TX FIFO underrun */
			uint32_t RFHF : 1;		/*!< RX FIFO half full */
			uint32_t TFHE : 1;		/*!< TX FIFO half empty */
			uint32_t RFE : 1;		/*!< RX FIFO empty */
			uint32_t TFF : 1;		/*!< TX FIFO full */
			uint32_t BUSY : 1;		/*!< TX/RX in process */
			uint32_t END : 1;		/*!< (Interval) Transfer finished */
			uint32_t RFIFO_NUM : 8;		/*!< RX FIFO usage */
			uint32_t TFIFO_NUM : 8;		/*!< TX FIFO usage */
			uint32_t : 8;			/*!< Reserved */
		} SRbits;
	};
	volatile uint32_t ITR;		/*!< SSI Interval Time Control Register */
	volatile uint32_t ICR;		/*!< SSI Interval Character-per-frame Control Register */
	volatile uint32_t GR;		/*!< SSI Clock Generator Register */
	volatile uint32_t RCNT;		/*!< SSI Receive Counter Register */
} XHAL_SPI_TypeDef;

typedef struct {
	uint8_t spi_mode;
	uint8_t brg;
} XHAL_SPI_InitTypeDef;

typedef struct {
	volatile XHAL_SPI_TypeDef *periph;
	XHAL_SPI_InitTypeDef init;
	XHAL_Wait_Callback *wait_callback;
	void *user_data;
} XHAL_SPI_HandleTypeDef;

#define XHAL_PHYSADDR_SPI		0x10043000
#define XHAL_SPI_FIFO_SIZE		128

#ifdef __cplusplus
}
#endif

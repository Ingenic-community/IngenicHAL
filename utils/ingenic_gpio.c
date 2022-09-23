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
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>

#include <ingenic-hal.h>

#if defined(XHAL_CHIP_X1000)
#define CHIP_NAME "X1000"
#elif defined(XHAL_CHIP_X2000)
#define CHIP_NAME "X2000"
#else
#define CHIP_NAME "unknown"
#endif

static void show_help() {
	puts(
		"Usage: ingenic_gpio <show|[GPIO_DEF [COMMAND VALUE]]>\n"
		"GPIO diagnostic tool for Ingenic " CHIP_NAME " SoCs.\n"
		"\n"
		"Commands:\n"
		"  inl                        Read input level\n"
		"  int                        Set interrupt\n"
		"  msk                        Set mask\n"
		"  pat0                       Set pattern 0 (data)\n"
		"  pat1                       Set pattern 1 (direction)\n"
		"  gpio_input                 Shortcut of `int 0', `msk 1', `pat1 1'\n"
		"  gpio_output                Shortcut of `int 0', `msk 1', `pat1 0'\n"
		"  read                       Shortcut of `inl'\n"
		"  write                      Shortcut of `pat0'\n"
		"  func                       Shortcut of `int 0', `msk 0', `pat1 <1>', `pat0 <0>'\n"
		"\n"
		"Examples:\n"
		"  jz_gpio show\n"
		"  jz_gpio pc23 gpio_input\n"
		"  jz_gpio pc23 read\n"
		"  jz_gpio pa00 gpio_output\n"
		"  jz_gpio pa00 write 1\n"
		"  jz_gpio pb27 func 0  # Set PB27 as 24MHz clock output on X1000\n"
	);
}

static void *phys_mem = NULL;

static void show_gpios() {
	for (int i=0; i<XHAL_GPIO_NR_PORTS; i++) {
		volatile XHAL_GPIO_TypeDef *port = phys_mem + i * XHAL_REGWIDTH_GPIO_PORT;

		printf("Port %c\n", 'A' + i);
		printf("================\n");

		for (int j=0; j<32; j++) {
			printf("P%c%02u: ", 'A' + i, j);

			bool b_int = XHAL_BIT_GET(port->INT, j);
			bool b_msk = XHAL_BIT_GET(port->MSK, j);
			bool b_pat1 = XHAL_BIT_GET(port->PAT1, j);
			bool b_pat0 = XHAL_BIT_GET(port->PAT0, j);

			if (b_int) {
				printf("INTERRUPT ");

				if (b_pat1) {
					if (b_pat0) {
						printf("RISING");
					} else {
						printf("FALLING");
					}
					printf("_EDGE ");
				} else {
					if (b_pat0) {
						printf("HIGH");
					} else {
						printf("LOW");
					}
					printf("_LEVEL ");
				}

				if (b_msk) {
					printf("DISABLED\n");
				} else {
					printf("ENABLED\n");
				}
			} else {
				if (b_msk) {
					printf("GPIO ");

					if (b_pat1) {
						bool b_inl = XHAL_BIT_GET(port->INL, j);
						printf("INPUT %u\n", b_inl);

					} else {
						printf("OUTPUT %u\n", b_pat0);
					}

				} else {
					printf("FUNCTION %d\n", b_pat1 << 1 | b_pat0);
				}
			}
		}

		printf("\n");
	}
}

static bool str2portoff(const char *str, void **port, uint8_t *offset) {
	if (strlen(str) != 4) {
		return false;
	}

	uint8_t portchar = toupper(str[1]);
	if (portchar < 'A' || portchar > 'G') {
		return false;
	}

	uint8_t off = strtol(str+2, NULL, 10);
	if (off > 31) {
		return false;
	}

	*port = phys_mem + (portchar - 'A') * XHAL_REGWIDTH_GPIO_PORT;
	*offset = off;

	return true;
}

static void gpio_read_inl(void *port_addr, uint8_t offset) {
	volatile XHAL_GPIO_TypeDef *port = port_addr;

	printf("%u\n", XHAL_BIT_GET(port->INL, offset));
}

static long check_val(const char *val) {
	if (!val) {
		printf("error: value not specified");
		exit(2);
	}

	return strtol(val, NULL, 10);
}

int main(int argc, char **argv) {
	if (argc < 2) {
		show_help();
		return 1;
	}

	int fd = open("/dev/mem", O_RDWR|O_SYNC);

	if (fd < 0) {
		perror("error: failed to open /dev/mem");
		return 2;
	}

	phys_mem = mmap(NULL, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, XHAL_PHYSADDR_GPIO);

	if (phys_mem == MAP_FAILED) {
		perror("error: mmap failed");
		return 2;
	}

	if (0 == strcmp(argv[1], "show")) {
		show_gpios();
	} else {
		volatile XHAL_GPIO_TypeDef *port;
		uint8_t offset;
		const char *val = argv[3];

		if (str2portoff(argv[1], (void **) &port, &offset)) {
			if (!argv[2]) {
				printf("error: no command specified\n");
				return 2;
			}

			if (0 == strcmp(argv[2], "inl") || 0 == strcmp(argv[2], "read")) {
				printf("%u\n", XHAL_BIT_GET(port->INL, offset));
			} else if (0 == strcmp(argv[2], "int")) {
				uint8_t v = check_val(val);
				if (v) {
					XHAL_BIT_SET(port->INTS, offset);
				} else {
					XHAL_BIT_SET(port->INTC, offset);
				}
			} else if (0 == strcmp(argv[2], "pat0") || 0 == strcmp(argv[2], "write")) {
				uint8_t v = check_val(val);
				if (v) {
					port->PAT0S = XHAL_BIT(offset);
				} else {
					port->PAT0C = XHAL_BIT(offset);
				}
			} else if (0 == strcmp(argv[2], "flip")) {
				printf("Flipping... Ctrl-C to exit.\n");
				while (1) {
					port->PAT0S = XHAL_BIT(offset);
					port->PAT0C = XHAL_BIT(offset);
				}
			} else if (0 == strcmp(argv[2], "pat1")) {
				uint8_t v = check_val(val);
				if (v) {
					port->PAT1S = XHAL_BIT(offset);
				} else {
					port->PAT1C = XHAL_BIT(offset);
				}
			} else if (0 == strcmp(argv[2], "gpio_input")) {
				XHAL_BIT_SET(port->INTC, offset);
				XHAL_BIT_SET(port->MSKS, offset);
				XHAL_BIT_SET(port->PAT1S, offset);
			} else if (0 == strcmp(argv[2], "gpio_output")) {
				XHAL_BIT_SET(port->INTC, offset);
				XHAL_BIT_SET(port->MSKS, offset);
				port->PAT1C = XHAL_BIT(offset);
			} else if (0 == strcmp(argv[2], "func")) {
				uint8_t v = check_val(val);

				XHAL_BIT_SET(port->INTC, offset);
				XHAL_BIT_SET(port->MSKC, offset);

				XHAL_BIT_GET(v, 1) ? (port->PAT1S = XHAL_BIT(offset)) : (port->PAT1C = XHAL_BIT(offset));
				XHAL_BIT_GET(v, 0) ? (port->PAT0S = XHAL_BIT(offset)) : (port->PAT0C = XHAL_BIT(offset));
			} else {
				printf("error: Bad command `%s'\n", argv[2]);
			}
		} else {
			printf("error: Bad pin specification `%s'\n", argv[1]);
			return 2;
		}
	}

	return 0;
}

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
#include <sys/stat.h>

#if defined(XHAL_CHIP_X1000)
#define CHIP_NAME "X1000"
#elif defined(XHAL_CHIP_X2000)
#define CHIP_NAME "X2000"
#else
#define CHIP_NAME "unknown"
#endif

static void show_help() {
	puts(
		"Usage: minicore_loader <firmware file>\n"
	);
}

int main(int argc, char **argv) {
	if (argc < 2) {
		show_help();
		return 1;
	}

	int fd_mem = open("/dev/mem", O_RDWR | O_SYNC);

	if (fd_mem < 0) {
		perror("error: failed to open /dev/mem");
		return 2;
	}

	void *phys_mem = mmap(NULL, 0x10000, PROT_READ | PROT_WRITE, MAP_SHARED, fd_mem, 0x13422000);

	if (phys_mem == MAP_FAILED) {
		perror("error: mmap failed");
		return 2;
	}

	int fd_img = open(argv[1], O_RDWR);
	if (fd_img < 0) {
		perror("error: failed to open image");
		return 2;
	}

	struct stat sbuf;
	stat(argv[1], &sbuf);

	void *image = mmap(NULL, sbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd_img, 0);

	memcpy(phys_mem, image, sbuf.st_size);

	munmap(image, sbuf.st_size);
	munmap(phys_mem, 0x10000);

	return 0;
}
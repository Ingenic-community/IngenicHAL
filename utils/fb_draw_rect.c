//
// Created by root on 10/20/22.
//

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/mman.h>

#include <linux/fb.h>
#include <sys/ioctl.h>
#include <assert.h>


void draw_rect_rgb565(uint16_t *vmem, uint16_t pix_value, uint32_t screenW, uint32_t screenH, uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
	for (uint32_t currentY = y; currentY < y + h; currentY += 1) {
		uint16_t * offset = vmem + screenW * currentY;
		for (uint32_t currentX = x;currentX < x + w; currentX += 1) {
			*(offset + currentX) = pix_value;
		}
	}
}

int main(int argc, char **argv) {
	if (argc < 7) {
		puts("Usage: fb_draw_rect: </dev/fbX> <pixel_value> <x> <y> <w> <h>\n"
		     "    e.g. fb_draw_rect /dev/fb0 aaaa 0 0 240 320 // RGB565\n"
		     "         fb_draw_rect /dev/fb0 55 0 0 240 320 // Grey8\n"
		     "         fb_draw_rect /dev/fb0 00ff00 0 0 240 320 // RGB888");
		exit(1);
	}

	uint32_t x, y, w, h, pix;

	pix = strtol(argv[2], NULL, 16);
	x = strtol(argv[3], NULL, 10);
	y = strtol(argv[4], NULL, 10);
	w = strtol(argv[5], NULL, 10);
	h = strtol(argv[6], NULL, 10);

	int fd_fb = open(argv[1], O_RDWR);

	assert(fd_fb > 0);

	struct fb_var_screeninfo vinfo;

	ioctl(fd_fb, FBIOGET_VSCREENINFO, &vinfo);

	uint32_t fb_width = vinfo.xres;
	uint32_t fb_height = vinfo.yres;
	uint32_t fb_bpp = vinfo.bits_per_pixel;
	uint32_t fb_bytepp = fb_bpp / 8;

	uint32_t fb_data_size = fb_width * fb_height * fb_bytepp;

	void *fbdata = mmap(0, fb_data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, (off_t)0);

	draw_rect_rgb565(fbdata, pix, fb_width, fb_height, x, y, w, h);

	munmap(fbdata, fb_data_size);
	close(fd_fb);
	return 0;

}
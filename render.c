
#include <render.h>

void place_pixelBW(unsigned char* framebuffer, int xpos, int ypos, unsigned char value) {
	if(xpos < 0) return;
	if(ypos < 0) return;
	if(xpos > IMAGE_WIDTH-1) return;
	if(ypos > IMAGE_HEIGHT-1) return;
	*(framebuffer + xpos + (ypos * IMAGE_WIDTH)) = value;
}

void fill_circleBW(unsigned char* framebuffer, int xpos, int ypos, int radius ,unsigned char value) {
	for(int i = -1*radius; i < radius; i++) {
		for(int j = -1*radius; j < radius; j++) {
			if((pow(i, 2) + pow(j, 2)) < pow(radius, 2)) place_pixelBW(framebuffer, i+xpos, j+ypos, value);
		}
	}
}

void render_imageBW(particle* particles, int partNum, unsigned char *framebuffer) {

	#pragma omp parallel for
	for(int i = 0;i < partNum; i++) {
		if((particles + i)->ignore) continue;

		int xpos = (((particles + i)->xpos + 10) / 20) * IMAGE_WIDTH;
		int ypos = (((particles + i)->ypos + 10) / 20) * IMAGE_HEIGHT;

		/*
		place_pixelBW(framebuffer, xpos, ypos, 0xff);
		place_pixelBW(framebuffer, xpos, ypos+1, 0xff);
		place_pixelBW(framebuffer, xpos, ypos-1, 0xff);
		place_pixelBW(framebuffer, xpos+1, ypos, 0xff);
		place_pixelBW(framebuffer, xpos+1, ypos+1, 0xff);
		place_pixelBW(framebuffer, xpos+1, ypos-1, 0xff);
		place_pixelBW(framebuffer, xpos-1, ypos, 0xff);
		place_pixelBW(framebuffer, xpos-1, ypos+1, 0xff);
		place_pixelBW(framebuffer, xpos-1, ypos-1, 0xff);
		*/

		int radius = ceil((particles + i)->size * IMAGE_HEIGHT/20);
		fill_circleBW(framebuffer, xpos, ypos, radius, 0xff);

	}

	//stbi_write_bmp(text, IMAGE_WIDTH, IMAGE_HEIGHT, 1, framebuffer);
	//stbi_write_jpg(text, IMAGE_WIDTH, IMAGE_HEIGHT, 1, framebuffer, 90);
}

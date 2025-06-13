
#ifndef _RENDER_H_
#define _RENDER_H_

#define IMAGE_HEIGHT 1000UL
#define IMAGE_WIDTH 1000UL

#define IMAGE_PIXELS (IMAGE_WIDTH * IMAGE_HEIGHT)

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdlib.h>
#include <omp.h>
#include "particle.h"

void place_pixelBW(unsigned char* framebuffer, int xpos, int ypos, unsigned char value);
void fill_circleBW(unsigned char* framebuffer, int xpos, int ypos, int radius ,unsigned char value);
void render_imageBW(particle* particles, int partNum, unsigned char *framebuffer);






#endif //_RENDER_H_

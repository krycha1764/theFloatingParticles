/*
 * main.c
 *
 *  Created on: 20 kwi 2025
 *      Author: mikolaj
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include <unistd.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "particle.h"
#include "render.h"
#include "stb_image_write.h"

#define COLORS 2
#define PARTICLES 10
#define FRAMES (3600*1)
#define TIMESTEP 0.01

float randf(void);
unsigned int randr(unsigned int range);

void update_particles(particle* table, int partNum, float* coeff, unsigned int colors, float timestep);

particle* partTable;

unsigned char* frames[FRAMES] = {0};

int main() {
	float coeffs[COLORS * COLORS] = {-1, -1, -1, -1};

	//unsigned char *framebuffer = malloc((IMAGE_PIXELS) * (FRAMES));
	for(size_t i = 0; i < FRAMES; i++) {
		frames[i] = malloc(IMAGE_PIXELS);
	}

	printf("Particle init...   ");
	srand(time(NULL));

	partTable = (particle*)malloc(sizeof(particle) * PARTICLES);
	for(int i = 0; i < PARTICLES; i++) {
		(partTable + i)->xpos = randf();
		(partTable + i)->ypos = randf();
		(partTable + i)->xvel = randf();
		(partTable + i)->yvel = randf();
		(partTable + i)->color = randr(COLORS);
		(partTable + i)->mass = 1.0;
		(partTable + i)->ignore = 0;
		//(partTable + i)->size = 0.03;
		(partTable + i)->size = 0.03;
	}

	(partTable + 0)->xpos = 0.0;
	(partTable + 0)->ypos = 0.0;
	(partTable + 0)->xvel = 0.0;
	(partTable + 0)->yvel = 0.0;
	(partTable + 0)->mass = 100.0;
	(partTable + 0)->size = 0.3;

	printf("DONE\n");
	//save_particlesCSV(partTable, PARTICLES, -1);
	//printf("Saved initial positions.\n");

	printf("Starting generating %i frames:\n", FRAMES);
	for(uint64_t i = 0; i < FRAMES; i++) {
		//printf("Generating %i frame:\n", i);
		update_particles(partTable, PARTICLES, coeffs, COLORS, TIMESTEP);

		int remain = 0;
		for(int j = 0; j < PARTICLES; j++) {
			if((partTable + j)->ignore == 0) remain++;
		}

		printf("Frame %lu/%i FINISHED RENDERING %.2f%% REMAINING PARTICLES: %i \n", i+1, FRAMES, 100.0*(i+1)/(FRAMES), remain);
		//save_particlesCSV(partTable, PARTICLES, i);

		memset(frames[i], 0x00, IMAGE_PIXELS);
		render_imageBW(partTable, PARTICLES, frames[i]);
		//sprintf(text, "FRAMES/FRAME-%i.JPG", i);
		//stbi_write_jpg(text, IMAGE_WIDTH, IMAGE_HEIGHT, 1, framebuffer, 90);
		//printf("Saved current frame.\n");
	}
	printf("ALL FRAMES RENDERED\n\n");
	//sleep(5);

#pragma omp parallel for
	for(size_t i = 0; i < FRAMES; i++) {
		char text[100] = {0};
		printf("FRAME %i/%i SAVED\n", (int)i+1, FRAMES);
		sprintf(text, "FRAMES/FRAME-%li.JPG", i);
		stbi_write_jpg(text, IMAGE_WIDTH, IMAGE_HEIGHT, 1, frames[i], 90);
		free(frames[i]);
	}
	//sleep(1);

	free(partTable);

	for(size_t i = 0; i < FRAMES; i++) {
		//free(frames[i]);
	}
	//free(framebuffer);
	return 0;
}

float randf(void) {
	return ((float)rand() / (float)RAND_MAX) * 20.0 - 10.0;
}

unsigned int randr(unsigned int range) {
	return rand() % range;
}

/*
 * particle.h
 *
 *  Created on: 20 kwi 2025
 *      Author: mikolaj
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_


#include <math.h>
#include <stdio.h>

#include <omp.h>

typedef struct {
	float xpos;
	float ypos;
	float xvel;
	float yvel;
	unsigned int color;
	unsigned char ignore;
	float mass;
	float size;
} particle;

float particle_calc_dist(particle* part1, particle* part2);
float particle_calc_angle(particle* part1, particle* part2);
void particle_calc_vel(particle* part1, particle* part2,float* coeff, unsigned int colors, float timestep);
void particle_calc_pos(particle* part, float timestep);
void particle_detect_collisions(particle* part1, particle* part2);
void save_particlesCSV(particle* table, int partNum, int fr);
void update_particles(particle* table, int partNum, float* coeff, unsigned int colors, float timestep);

#endif /* PARTICLE_H_ */

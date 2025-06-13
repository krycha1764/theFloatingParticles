/*
 * particle.c
 *
 *  Created on: 20 kwi 2025
 *      Author: mikolaj
 */

#include "particle.h"

float particle_calc_dist(particle* part1, particle* part2) {
	float dx = part1->xpos - part2->xpos;
	float dy = part1->ypos - part2->ypos;
	return sqrt(pow(dx, 2) + pow(dy, 2));
}

float particle_calc_angle(particle* part1, particle* part2) {
	float dx = part1->xpos - part2->xpos;
	float dy = part1->ypos - part2->ypos;
	return atan2(dy, dx);
}

void particle_calc_vel(particle* part1, particle* part2,float* coeff, unsigned int colors, float timestep) {
	float dist = particle_calc_dist(part1, part2);
	float force = (coeff[part1->color + (part2->color * colors)]) * (part1->mass * part2->mass) / (dist * dist);
	float angle = particle_calc_angle(part1, part2);
	part1->xvel += timestep * (force * cos(angle)) / part1->mass;
	part1->yvel += timestep * (force * sin(angle)) / part1->mass;
	return;
}

void particle_calc_pos(particle* part, float timestep) {
	part->xpos += part->xvel * timestep;
	part->ypos += part->yvel * timestep;

	while(part->xpos > 10.0) part->xpos -= 20.0;
	while(part->ypos > 10.0) part->ypos -= 20.0;
	while(part->xpos < -10.0) part->xpos += 20.0;
	while(part->ypos < -10.0) part->ypos += 20.0;

	return;
}

void particle_detect_collisions(particle* part1, particle* part2) {
	float dist = particle_calc_dist(part1, part2);
	if(dist > (part1->size + part2->size)/2) return;
	float moment1 = part1->mass * sqrt(part1->xvel*part1->xvel * part1->yvel*part1->yvel);
	float moment2 = part2->mass * sqrt(part2->xvel*part2->xvel * part2->yvel*part2->yvel);
	float newmass = part1->mass + part2->mass;
	float newxvel = ((part1->xvel * part1->mass)+(part2->xvel * part2->mass))/newmass;
	float newyvel = ((part1->yvel * part1->mass)+(part2->yvel * part2->mass))/newmass;
	float newsize = 2 * sqrt((M_PI * pow((part1->size / 2), 2) + M_PI * pow((part2->size / 2), 2))/M_PI);

	if(moment1 > moment2) {
		part2->ignore = 1;
		part1->size = newsize;
		part1->mass = newmass;
		part1->xvel = newxvel;
		part1->yvel = newyvel;
	}else {
		part1->ignore = 1;
		part2->size = newsize;
		part2->mass = newmass;
		part2->xvel = newxvel;
		part2->yvel = newyvel;
	}

	return;
}

void save_particlesCSV(particle* table, int partNum, int fr) {
	char tmp[100];
	int size;
	size = sprintf(tmp, "FRAMES/FRAME-%i.CSV", fr);
	FILE* f = fopen(tmp, "w");
	for(int i = 0; i < partNum; i++) {
		if((table + i)->ignore) continue;
		size = sprintf(tmp, "%f, %f\n", (table + i)->xpos, (table + i)->ypos);
		fwrite(tmp, size, 1, f);
	}
	fclose(f);
}

void update_particles(particle* table, int partNum, float* coeff, unsigned int colors, float timestep) {
	//printf("\tCalculating velocity vectors...   ");
#pragma omp parallel for
	for(int i = 0; i < partNum; i++) {
		if((table + i)->ignore) continue;
		for(int j = 0; j < partNum; j++) {
			if((table + j)->ignore) continue;
			if(i == j) continue;
			particle_calc_vel((table + i), (table + j), coeff, colors, timestep);
		}
	}
	//printf("DONE\n");
	//printf("\tCalculating positions...  ");
#pragma omp parallel for
	for(int i = 0; i < partNum; i++) {
		if((table + i)->ignore) continue;
		particle_calc_pos((table + i), timestep);
	}

#pragma omp parallel for
	for(int i = 0; i < partNum; i++) {
		if((table + i)->ignore) continue;
		for(int j = i; j < partNum; j++) {
			if((table + j)->ignore) continue;
			if(i == j) continue;
			particle_detect_collisions((table + i), (table + j));
		}
	}

	//printf("DONE\n");

	return;
}

#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

#define NUM_ITERATIONS 1
#define NUM_PARTICLES 100


#define NUM_CONSTRAINTS 10

struct Constraint
{
	int particleA;
	int particleB;
	float restLength;
};

struct ParticleData
{
	glm::vec3 current[NUM_PARTICLES];
	glm::vec3 old[NUM_PARTICLES];
	glm::vec3 acceleration[NUM_PARTICLES];
	glm::vec3 gravity;
	float timeStep;
};

void accumulateForces(ParticleData *particles);
void verlet(ParticleData *particles);
void satisfyConstraints(ParticleData *particles);
void satisfyConstraints(ParticleData *particles, Constraint *constraints);
void timeStep(ParticleData *particles);

#endif

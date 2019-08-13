#ifndef PHYSICS_H
#define PHYSICS_H

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

void accumulateForces(ParticleData *particles)
{
	for (int i = 0; i < NUM_PARTICLES; ++i)
	{
		particles->acceleration[i] = particles->gravity;
	}
}

void verlet(ParticleData *particles)
{
	float timeStepSq = particles->timeStep * particles->timeStep;
	for (int i = 0; i < NUM_PARTICLES; ++i)
	{
		glm::vec3 *current = &particles->current[i];
		glm::vec3 temp = *current;

		glm::vec3 *old = &particles->old[i];

		glm::vec3 *acceleration = &particles->acceleration[i];

		(*current) += (*current) - (*old) + (*acceleration) * timeStepSq;
		(*old) = temp;
	}
}

void satisfyConstraints(ParticleData *particles)
{
	float restLength = 100.0f;
	
	for (int j = 0; j < NUM_ITERATIONS; ++j)
	{
		for (int i = 0; i < NUM_PARTICLES; ++i)
		{
			glm::vec3 *current = &particles->current[i];
			(*current) = glm::min(glm::max((*current), glm::vec3(0, 0, 0)), glm::vec3(1000, 1000, 1000));
		}

		glm::vec3 *current1 = &particles->current[0];
		glm::vec3 *current2 = &particles->current[1];

		glm::vec3 delta = (*current2) - (*current1);
		float deltaLength = sqrt(glm::dot(delta, delta));
		float diff = (deltaLength - restLength) / deltaLength;
		(*current1) -= delta * 0.5f * diff;
		(*current2) += delta * 0.5f * diff;
	}

}

void satisfyConstraints(ParticleData *particles, Constraint *constraints)
{
	for (int j = 0; j < NUM_ITERATIONS; ++j)
	{
		for (int i = 0; i < NUM_CONSTRAINTS; ++i)
		{
			Constraint *c = &constraints[i];
			glm::vec3 *current1 = &particles->current[c->particleA];
			glm::vec3 *current2 = &particles->current[c->particleB];

			float restLengthSq = c->restLength * c->restLength;
			glm::vec3 delta = (*current2) - (*current1);
			delta *= restLengthSq / (delta * delta + restLengthSq) - 0.5f;
			(*current1) -= delta;
			(*current2) += delta;

			/*
			// Pseudo-code to satisfy (C2)
			delta = x2-x1;
			deltalength = sqrt(delta*delta);
			diff = (deltalength-restlength)/(deltalength*(invmass1+invmass2));
			x1 -= invmass1*delta*diff;
			x2 += invmass2*delta*diff;
			*/
		}

		particles->current[0] = glm::vec3(0.0f);
	}

}

void timeStep(ParticleData *particles)
{
	accumulateForces(particles);
	verlet(particles);
	satisfyConstraints(particles);
}

#endif

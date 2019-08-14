#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>

struct PhysicsData
{
	glm::vec3 position;
	float heights[1000000];
};

void physicsUpdate(glm::vec3 *player, PhysicsData *data);

#endif

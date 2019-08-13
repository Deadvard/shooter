#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>

struct PhysicsData
{
	glm::vec3 position;
	float heights[10000];
};

void physicsUpdate(glm::vec3 *player, PhysicsData *data);

#endif

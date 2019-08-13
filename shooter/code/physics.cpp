#include "physics.h"
#include <stdio.h>

void physicsUpdate(glm::vec3 *player, PhysicsData *data)
{
	glm::ivec3 pos = (*player) - data->position;
	float y = 0.0f;

	y = data->heights[pos.x * pos.z];

	//printf("(%i, %i)\n", pos.x, pos.z);
	player->x = glm::clamp(player->x, -50.0f, 50.0f);
	player->z = glm::clamp(player->z, -50.0f, 50.0f);
	player->y = y;
}
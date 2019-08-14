#include "physics.h"
#include <stdio.h>

static void collision(glm::vec3 *player, glm::vec3 coll, float radius)
{
	glm::vec2 playerPos = glm::vec2(player->x, player->z);
	glm::vec2 collPos = glm::vec2(coll.x, coll.z);
	
	glm::vec2 dir = playerPos - collPos;
	float dist = glm::length(dir);
	if (dist < radius)
	{
		dir = glm::normalize(dir);
		player->x += dir.x * (radius - dist);
		player->z += dir.y * (radius - dist);
	}
}

void physicsUpdate(glm::vec3 *player, PhysicsData *data)
{
	float radius = 2.0f;
	glm::ivec3 pos = (*player) - data->position;
	float y = 0.0f;

	float ratioX = player->x - (int)player->x;
	float ratioZ = player->z - (int)player->z;

	float *h = data->heights;
	y = (h[(pos.x - 1) * pos.z] * (1.0f - ratioX) +
		 h[(pos.x + 1) * pos.z] * ratioX +
		 h[pos.x * (pos.z - 1)] * (1.0f - ratioZ) +
		 h[pos.x * (pos.z + 1)] * ratioZ);

	collision(player, glm::vec3(0, 0, -10), 2.0f);
	collision(player, glm::vec3(5, 0, 0), 2.0f);

	//printf("(%i, %i)\n", pos.x, pos.z);
	player->x = glm::clamp(player->x, -50.0f, 50.0f);
	player->z = glm::clamp(player->z, -50.0f, 50.0f);
	player->y = y;
}
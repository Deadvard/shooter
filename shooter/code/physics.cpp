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
	glm::ivec3 pos = ((*player) - data->position) * 10.0f;
	float y = 0.0f;

	float *h = data->heights;
	/*float avarage = (
		h[pos.x * pos.z] +
		h[pos.x * (pos.z - 1)] +
		h[pos.x * (pos.z + 1)] +

		h[(pos.x - 1) * pos.z] +
		h[(pos.x - 1) * (pos.z - 1)] +
		h[(pos.x - 1) * (pos.z + 1)] +

		h[(pos.x + 1) * pos.z] +
		h[(pos.x + 1) * (pos.z - 1)] +
		h[(pos.x + 1) * (pos.z + 1)]
		) / 9.0f;*/

	y = data->heights[pos.x * pos.z];

	collision(player, glm::vec3(0, 0, -10), 2.0f);
	collision(player, glm::vec3(5, 0, 0), 2.0f);

	//printf("(%i, %i)\n", pos.x, pos.z);
	player->x = glm::clamp(player->x, -49.0f, 49.0f);
	player->z = glm::clamp(player->z, -49.0f, 49.0f);
	player->y = y;
}
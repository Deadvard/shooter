#include "physics.h"
#include <stdio.h>

bool point_in_aabb(AABB* aabb, glm::vec3* point)
{
	glm::vec3 min = get_min(aabb);
	glm::vec3 max = get_max(aabb);

	if (point->x < min.x || point->y < min.y || point->z < min.z) return false;
	if (point->x > max.x || point->y > max.y || point->z > max.z) return false;
	return true;
}

bool aabb_aabb(AABB* left, AABB* right)
{
	glm::vec3 left_min = get_min(left);
	glm::vec3 left_max = get_max(left);
	glm::vec3 right_min = get_min(right);
	glm::vec3 right_max = get_max(right);

	return
		left_min.x <= right_max.x && left_max.x >= right_min.x &&
		left_min.y <= right_max.y && left_max.y >= right_min.y &&
		left_min.z <= right_max.z && left_max.z >= right_min.z;
}

glm::vec3 closest_point(AABB* aabb, glm::vec3* point)
{
	glm::vec3 result = *point;
	glm::vec3 min = get_min(aabb);
	glm::vec3 max = get_max(aabb);

	result.x = result.x < min.x ? min.x : result.x;
	result.y = result.y < min.y ? min.y : result.y;
	result.z = result.z < min.z ? min.z : result.z;

	result.x = result.x > max.x ? max.x : result.x;
	result.y = result.y > max.y ? max.y : result.y;
	result.z = result.z > max.z ? max.z : result.z;

	return result;
}

glm::vec3 get_min(AABB* aabb)
{
	glm::vec3 p1 = aabb->position + aabb->size;
	glm::vec3 p2 = aabb->position - aabb->size;

	return glm::vec3(
		glm::min(p1.x, p2.x),
		glm::min(p1.y, p2.y),
		glm::min(p1.z, p2.z));
}

glm::vec3 get_max(AABB* aabb)
{
	glm::vec3 p1 = aabb->position + aabb->size;
	glm::vec3 p2 = aabb->position - aabb->size;

	return glm::vec3(
		glm::max(p1.x, p2.x),
		glm::max(p1.y, p2.y),
		glm::max(p1.z, p2.z));
}



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
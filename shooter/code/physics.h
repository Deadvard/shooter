#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>

typedef struct
{
	glm::vec3 origin;
	glm::vec3 direction;
} Ray;

typedef struct
{
	glm::vec3 position;
	glm::vec3 size;
} AABB;

bool point_in_aabb(AABB* aabb, glm::vec3* point);
bool aabb_aabb(AABB* left, AABB* right);
glm::vec3 closest_point(AABB* aabb, glm::vec3* point);
glm::vec3 get_min(AABB* aabb);
glm::vec3 get_max(AABB* aabb);

struct PhysicsData
{
	glm::vec3 position;
	float heights[1000000];
};

void physicsUpdate(glm::vec3 *player, PhysicsData *data);

float raycast(AABB *aabb, Ray *ray);

#endif

#ifndef DC_H
#define DC_H

#include <stdlib.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "memory.h"

#define ArrayCount(array) (sizeof(array) / sizeof((array)[0]))

struct Cell
{
	uint8 material;
	uint8 x;
	uint8 y;
	uint8 z;
};

#define worldsize 32 * 32 * 32

struct Cube
{
	int index;
	glm::vec3 meshPt;
	glm::vec3 meshNormal;
};

struct densityField
{
	float* densities;
	Cube* cubes;
};

struct DcChunk
{
	Cell block[16 * 16 * 16];
	unsigned int vbo;
	int elements;
	glm::vec3 position;
	bool32 changed;
};

DcChunk * dcChunkCreate();
void dcChunkRender(DcChunk *chunk);


#endif

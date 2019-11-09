#ifndef EDVARDDC_H
#define EDVARDDC_H

#include <stdlib.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

#include "memory.h"

#define dcCHUNK_SIZE 32

#define ArrayCount(array) (sizeof(array) / sizeof((array)[0]))

struct DcVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct DcChunk
{
	float distances[32][32][32];
	
	
	uint16 cells[32][32][32];
	
	
	DcVertex vertices[32 * 32 * 32 * 5];
	uint32 vertexCount;

	uint16 indices[32 * 32 * 32 * 5];
	uint32 indexCount;
	
	
	unsigned int vertexBuffer;
	unsigned int indexBuffer;
	bool32 changed;
};

DcChunk * dcChunkCreate();
void dcChunkRender(DcChunk *chunk);
#endif

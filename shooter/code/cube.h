#ifndef CUBE_H
#define CUBE_H

#include <stdlib.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "memory.h"

#define CHUNK_SIZE 16

struct Chunk
{
	uint8 block[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
	unsigned int vbo;
	int elements;
	glm::vec3 position;
	bool32 changed;
};

Chunk * chunkCreate();
void chunkRender(Chunk *chunk);


#endif

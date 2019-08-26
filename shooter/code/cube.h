#ifndef CUBE_H
#define CUBE_H

#include <stdlib.h>
#include <glad/glad.h>

#include "memory.h"

#define CHUNK_SIZE 64

struct CubeVertex
{
	int8 x;
	int8 y;
	int8 z;
	int8 w;
};

struct Chunk
{
	int8 block[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
	unsigned int vbo;
	int elements;
	int8 changed;
};

static Chunk * chunkCreate()
{
	Chunk *chunk = (Chunk *)malloc(sizeof(Chunk));
	for (int z = 0; z < CHUNK_SIZE; ++z)
	for (int y = 0; y < CHUNK_SIZE; ++y)
	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		chunk->block[x][y][z] = 0;
	}
	chunk->elements = 0;
	chunk->changed = true;
	glGenBuffers(1, &chunk->vbo);
	return chunk;
}

static CubeVertex cubeVert(int8 x, int8 y, int8 z, int8 w)
{
	return { x, y, z, w };
}

static void chunkUpdate(Chunk *chunk)
{
	chunk->changed = false;
	
	CubeVertex *vertices = (CubeVertex *)malloc(sizeof(CubeVertex) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 6);
	int i = 0;

	for (int z = 0; z < CHUNK_SIZE; ++z)
	for (int y = 0; y < CHUNK_SIZE; ++y)
	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		int8 type = chunk->block[x][y][z];

		if (type != 0)
		{
			int neighbor = 0;
			if (x > 0) neighbor = chunk->block[x-1][y][z];
			if (neighbor == 0)
			{
				vertices[i++] = cubeVert(x    , y + 1, z + 1, type);
				vertices[i++] = cubeVert(x    , y + 1, z    , type);
				vertices[i++] = cubeVert(x    , y    , z    , type);
				vertices[i++] = cubeVert(x    , y    , z    , type);
				vertices[i++] = cubeVert(x    , y    , z + 1, type);
				vertices[i++] = cubeVert(x    , y + 1, z + 1, type);
			}

			neighbor = 0;
			if (x < CHUNK_SIZE - 1) neighbor = chunk->block[x+1][y][z];
			if (neighbor == 0)
			{
				vertices[i++] = cubeVert(x + 1, y + 1, z + 1, type);
				vertices[i++] = cubeVert(x + 1, y    , z + 1, type);
				vertices[i++] = cubeVert(x + 1, y    , z    , type);
				vertices[i++] = cubeVert(x + 1, y    , z    , type);
				vertices[i++] = cubeVert(x + 1, y + 1, z    , type);
				vertices[i++] = cubeVert(x + 1, y + 1, z + 1, type);
			}

			neighbor = 0;
			if (y > 0) neighbor = chunk->block[x][y-1][z];
			if (neighbor == 0)
			{
				vertices[i++] = cubeVert(x    , y    , z    , -type);
				vertices[i++] = cubeVert(x + 1, y    , z    , -type);
				vertices[i++] = cubeVert(x + 1, y    , z + 1, -type);
				vertices[i++] = cubeVert(x + 1, y    , z + 1, -type);
				vertices[i++] = cubeVert(x    , y    , z + 1, -type);
				vertices[i++] = cubeVert(x    , y    , z    , -type);
			}

			neighbor = 0;
			if (y < CHUNK_SIZE - 1) neighbor = chunk->block[x][y+1][z];
			if (neighbor == 0)
			{
				vertices[i++] = cubeVert(x    , y + 1, z    , -type);
				vertices[i++] = cubeVert(x    , y + 1, z + 1, -type);
				vertices[i++] = cubeVert(x + 1, y + 1, z + 1, -type);
				vertices[i++] = cubeVert(x + 1, y + 1, z + 1, -type);
				vertices[i++] = cubeVert(x + 1, y + 1, z    , -type);
				vertices[i++] = cubeVert(x    , y + 1, z    , -type);
			}

			neighbor = 0;
			if (z > 0) neighbor = chunk->block[x][y][z-1];
			if (neighbor == 0)
			{
				vertices[i++] = cubeVert(x    , y    , z    , type);
				vertices[i++] = cubeVert(x    , y + 1, z    , type);
				vertices[i++] = cubeVert(x + 1, y + 1, z    , type);
				vertices[i++] = cubeVert(x + 1, y + 1, z    , type);
				vertices[i++] = cubeVert(x + 1, y    , z    , type);
				vertices[i++] = cubeVert(x    , y    , z    , type);
			}

			neighbor = 0;
			if (z < CHUNK_SIZE - 1) neighbor = chunk->block[x][y][z+1];
			if (neighbor == 0)
			{
				vertices[i++] = cubeVert(x    , y    , z + 1, type);
				vertices[i++] = cubeVert(x + 1, y    , z + 1, type);
				vertices[i++] = cubeVert(x + 1, y + 1, z + 1, type);
				vertices[i++] = cubeVert(x + 1, y + 1, z + 1, type);
				vertices[i++] = cubeVert(x    , y + 1, z + 1, type);
				vertices[i++] = cubeVert(x    , y    , z + 1, type);
			}
			
			/*vertices[i++] = cubeVert(x    , y    , z    , type);
			vertices[i++] = cubeVert(x    , y + 1, z    , type);
			vertices[i++] = cubeVert(x + 1, y + 1, z    , type);
			vertices[i++] = cubeVert(x + 1, y + 1, z    , type);
			vertices[i++] = cubeVert(x + 1, y    , z    , type);
			vertices[i++] = cubeVert(x    , y    , z    , type);

			vertices[i++] = cubeVert(x    , y    , z + 1, type);
			vertices[i++] = cubeVert(x + 1, y    , z + 1, type);
			vertices[i++] = cubeVert(x + 1, y + 1, z + 1, type);
			vertices[i++] = cubeVert(x + 1, y + 1, z + 1, type);
			vertices[i++] = cubeVert(x    , y + 1, z + 1, type);
			vertices[i++] = cubeVert(x    , y    , z + 1, type);

			vertices[i++] = cubeVert(x    , y + 1, z + 1, type);
			vertices[i++] = cubeVert(x    , y + 1, z    , type);
			vertices[i++] = cubeVert(x    , y    , z    , type);
			vertices[i++] = cubeVert(x    , y    , z    , type);
			vertices[i++] = cubeVert(x    , y    , z + 1, type);
			vertices[i++] = cubeVert(x    , y + 1, z + 1, type);

			vertices[i++] = cubeVert(x + 1, y + 1, z + 1, type);
			vertices[i++] = cubeVert(x + 1, y    , z + 1, type);
			vertices[i++] = cubeVert(x + 1, y    , z    , type);
			vertices[i++] = cubeVert(x + 1, y    , z    , type);
			vertices[i++] = cubeVert(x + 1, y + 1, z    , type);
			vertices[i++] = cubeVert(x + 1, y + 1, z + 1, type);

			vertices[i++] = cubeVert(x    , y    , z    , -type);
			vertices[i++] = cubeVert(x + 1, y    , z    , -type);
			vertices[i++] = cubeVert(x + 1, y    , z + 1, -type);
			vertices[i++] = cubeVert(x + 1, y    , z + 1, -type);
			vertices[i++] = cubeVert(x    , y    , z + 1, -type);
			vertices[i++] = cubeVert(x    , y    , z    , -type);

			vertices[i++] = cubeVert(x    , y + 1, z    , -type);
			vertices[i++] = cubeVert(x    , y + 1, z + 1, -type);
			vertices[i++] = cubeVert(x + 1, y + 1, z + 1, -type);
			vertices[i++] = cubeVert(x + 1, y + 1, z + 1, -type);
			vertices[i++] = cubeVert(x + 1, y + 1, z    , -type);
			vertices[i++] = cubeVert(x    , y + 1, z    , -type);*/
		}
	}

	chunk->elements = i;
	glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
	glBufferData(GL_ARRAY_BUFFER, chunk->elements * sizeof(CubeVertex), vertices, GL_STATIC_DRAW);
	free(vertices);
}

static void chunkRender(Chunk *chunk)
{
	if (chunk->changed)
	{
		chunkUpdate(chunk);
	}

	if (chunk->elements > 0)
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_BYTE, GL_FALSE, sizeof(CubeVertex), 0);
		glDrawArrays(GL_TRIANGLES, 0, chunk->elements);
	}
}

#endif

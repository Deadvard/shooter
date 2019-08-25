#ifndef CUBE_H
#define CUBE_H

#include <stdlib.h>
#include <glad/glad.h>

#define CHUNK_SIZE 64
typedef unsigned char uint8;

struct CubeVertex
{
	uint8 x;
	uint8 y;
	uint8 z;
	uint8 w;
};

struct Chunk
{
	uint8 block[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
	unsigned int vbo;
	int elements;
	uint8 changed;
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

static CubeVertex cVert(uint8 x, uint8 y, uint8 z, uint8 w)
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
		uint8 type = chunk->block[x][y][z];

		if (type != 0)
		{
			//pos, uv, normal
			vertices[i++] = cVert(x    , y    , z    , type);
			vertices[i++] = cVert(x    , y + 1, z    , type);
			vertices[i++] = cVert(x + 1, y + 1, z    , type);
			vertices[i++] = cVert(x + 1, y + 1, z    , type);
			vertices[i++] = cVert(x + 1, y    , z    , type);
			vertices[i++] = cVert(x    , y    , z    , type);

			vertices[i++] = cVert(x    , y    , z + 1, type);
			vertices[i++] = cVert(x + 1, y    , z + 1, type);
			vertices[i++] = cVert(x + 1, y + 1, z + 1, type);
			vertices[i++] = cVert(x + 1, y + 1, z + 1, type);
			vertices[i++] = cVert(x    , y + 1, z + 1, type);
			vertices[i++] = cVert(x    , y    , z + 1, type);

			vertices[i++] = cVert(x    , y + 1, z + 1, type);
			vertices[i++] = cVert(x    , y + 1, z    , type);
			vertices[i++] = cVert(x    , y    , z    , type);
			vertices[i++] = cVert(x    , y    , z    , type);
			vertices[i++] = cVert(x    , y    , z + 1, type);
			vertices[i++] = cVert(x    , y + 1, z + 1, type);

			vertices[i++] = cVert(x + 1, y + 1, z + 1, type);
			vertices[i++] = cVert(x + 1, y    , z + 1, type);
			vertices[i++] = cVert(x + 1, y    , z    , type);
			vertices[i++] = cVert(x + 1, y    , z    , type);
			vertices[i++] = cVert(x + 1, y + 1, z    , type);
			vertices[i++] = cVert(x + 1, y + 1, z + 1, type);

			vertices[i++] = cVert(x    , y    , z    , type);
			vertices[i++] = cVert(x + 1, y    , z    , type);
			vertices[i++] = cVert(x + 1, y    , z + 1, type);
			vertices[i++] = cVert(x + 1, y    , z + 1, type);
			vertices[i++] = cVert(x    , y    , z + 1, type);
			vertices[i++] = cVert(x    , y    , z    , type);

			vertices[i++] = cVert(x    , y + 1, z    , type);
			vertices[i++] = cVert(x    , y + 1, z + 1, type);
			vertices[i++] = cVert(x + 1, y + 1, z + 1, type);
			vertices[i++] = cVert(x + 1, y + 1, z + 1, type);
			vertices[i++] = cVert(x + 1, y + 1, z    , type);
			vertices[i++] = cVert(x    , y + 1, z    , type);
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
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_BYTE, GL_FALSE, sizeof(CubeVertex), 0);
		glDrawArrays(GL_TRIANGLES, 0, chunk->elements);
	}
}

#endif

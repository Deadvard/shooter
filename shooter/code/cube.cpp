#include "cube.h"
#include <stdio.h>
enum FaceDirection
{
	facePosX = 0x1, // 0001
	facePosY = 0x2, // 0010
	facePosZ = 0x4, // 0100
	faceNegX = 0x9, // 1001
	faceNegY = 0xA, // 1010
	faceNegZ = 0xC  // 1100
};

Chunk * chunkCreate()
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

struct CubeVertex
{
	uint8 xy;
	uint8 zw;
	uint8 type;
};

CubeVertex cubeVert(uint8 x, uint8 y, uint8 z, uint8 w, uint8 type)
{
	uint8 xy = ((x & 0xF) << 4) + (y & 0xF);
	uint8 zw = ((z & 0xF) << 4) + (w & 0xF);
	return { xy, zw, type };
}


uint16 cubeVert(uint8 x, uint8 y, uint8 z)
{
	uint16 vertex = ((x & 0x1F) << 10) + ((y & 0x1F) << 5) + (z & 0x1F);
	return vertex;
}


void chunkUpdate(Chunk *chunk)
{
	chunk->changed = false;

	uint16 vertices[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 6];
	uint16 types[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 6];
	int i = 0;
	int t = 0;

	for (int z = 0; z < CHUNK_SIZE; ++z)
	for (int y = 0; y < CHUNK_SIZE; ++y)
	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		uint8 type = chunk->block[x][y][z];

		if (type != 0)
		{
			int neighbor = 0;
			if (x > 0) neighbor = chunk->block[x - 1][y][z];
			if (neighbor == 0)
			{
				vertices[i++] = cubeVert(x    , y + 1, z + 1);
				vertices[i++] = cubeVert(x    , y + 1, z    );
				vertices[i++] = cubeVert(x    , y    , z    );
				vertices[i++] = cubeVert(x    , y    , z    );
				vertices[i++] = cubeVert(x    , y    , z + 1);
				vertices[i++] = cubeVert(x    , y + 1, z + 1);
				
				types[t++] = (type << 4) + faceNegX;
				types[t++] = (type << 4) + faceNegX;
				types[t++] = (type << 4) + faceNegX;
				types[t++] = (type << 4) + faceNegX;
				types[t++] = (type << 4) + faceNegX;
				types[t++] = (type << 4) + faceNegX;
			}

			neighbor = 0;
			if (x < CHUNK_SIZE - 1) neighbor = chunk->block[x + 1][y][z];
			if (neighbor == 0)
			{
				vertices[i++] = cubeVert(x + 1, y + 1, z + 1);
				vertices[i++] = cubeVert(x + 1, y    , z + 1);
				vertices[i++] = cubeVert(x + 1, y    , z    );
				vertices[i++] = cubeVert(x + 1, y    , z    );
				vertices[i++] = cubeVert(x + 1, y + 1, z    );
				vertices[i++] = cubeVert(x + 1, y + 1, z + 1);
				
				types[t++] = (type << 4) + facePosX;
				types[t++] = (type << 4) + facePosX;
				types[t++] = (type << 4) + facePosX;
				types[t++] = (type << 4) + facePosX;
				types[t++] = (type << 4) + facePosX;
				types[t++] = (type << 4) + facePosX;
			}

			neighbor = 0;
			if (y > 0) neighbor = chunk->block[x][y - 1][z];
			if (neighbor == 0)
			{
				vertices[i++] = cubeVert(x    , y    , z    );
				vertices[i++] = cubeVert(x + 1, y    , z    );
				vertices[i++] = cubeVert(x + 1, y    , z + 1);
				vertices[i++] = cubeVert(x + 1, y    , z + 1);
				vertices[i++] = cubeVert(x    , y    , z + 1);
				vertices[i++] = cubeVert(x    , y    , z    );
				
				types[t++] = (type << 4) + faceNegY;
				types[t++] = (type << 4) + faceNegY;
				types[t++] = (type << 4) + faceNegY;
				types[t++] = (type << 4) + faceNegY;
				types[t++] = (type << 4) + faceNegY;
				types[t++] = (type << 4) + faceNegY;
			}

			neighbor = 0;
			if (y < CHUNK_SIZE - 1) neighbor = chunk->block[x][y + 1][z];
			if (neighbor == 0)
			{
				vertices[i++] = cubeVert(x    , y + 1, z    );
				vertices[i++] = cubeVert(x    , y + 1, z + 1);
				vertices[i++] = cubeVert(x + 1, y + 1, z + 1);
				vertices[i++] = cubeVert(x + 1, y + 1, z + 1);
				vertices[i++] = cubeVert(x + 1, y + 1, z    );
				vertices[i++] = cubeVert(x    , y + 1, z    );
				
				types[t++] = (type << 4) + facePosY;
				types[t++] = (type << 4) + facePosY;
				types[t++] = (type << 4) + facePosY;
				types[t++] = (type << 4) + facePosY;
				types[t++] = (type << 4) + facePosY;
				types[t++] = (type << 4) + facePosY;
			}

			neighbor = 0;
			if (z > 0) neighbor = chunk->block[x][y][z - 1];
			if (neighbor == 0)
			{
				vertices[i++] = cubeVert(x    , y    , z    );
				vertices[i++] = cubeVert(x    , y + 1, z    );
				vertices[i++] = cubeVert(x + 1, y + 1, z    );
				vertices[i++] = cubeVert(x + 1, y + 1, z    );
				vertices[i++] = cubeVert(x + 1, y    , z    );
				vertices[i++] = cubeVert(x    , y    , z    );
				
				types[t++] = (type << 4) + faceNegZ;
				types[t++] = (type << 4) + faceNegZ;
				types[t++] = (type << 4) + faceNegZ;
				types[t++] = (type << 4) + faceNegZ;
				types[t++] = (type << 4) + faceNegZ;
				types[t++] = (type << 4) + faceNegZ;
			}

			neighbor = 0;
			if (z < CHUNK_SIZE - 1) neighbor = chunk->block[x][y][z + 1];
			if (neighbor == 0)
			{
				vertices[i++] = cubeVert(x    , y    , z + 1);
				vertices[i++] = cubeVert(x + 1, y    , z + 1);
				vertices[i++] = cubeVert(x + 1, y + 1, z + 1);
				vertices[i++] = cubeVert(x + 1, y + 1, z + 1);
				vertices[i++] = cubeVert(x    , y + 1, z + 1);
				vertices[i++] = cubeVert(x    , y    , z + 1);
				
				types[t++] = (type << 4) + facePosZ;
				types[t++] = (type << 4) + facePosZ;
				types[t++] = (type << 4) + facePosZ;
				types[t++] = (type << 4) + facePosZ;
				types[t++] = (type << 4) + facePosZ;
				types[t++] = (type << 4) + facePosZ;
			}
		}
	}

	chunk->elements = i;
	glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
	glBufferData(GL_ARRAY_BUFFER, chunk->elements * sizeof(uint16) + t * sizeof(uint16), 0, GL_STATIC_DRAW);
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, chunk->elements * sizeof(uint16), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, chunk->elements * sizeof(uint16), t * sizeof(uint16), types);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void chunkRender(Chunk *chunk)
{
	if (chunk->changed)
	{
		chunkUpdate(chunk);
	}

	if (chunk->elements > 0)
	{	
		glBindVertexBuffer(0, chunk->vbo, 0, sizeof(uint16));
		glBindVertexBuffer(1, chunk->vbo, chunk->elements * sizeof(uint16), sizeof(uint16));

		glDrawArrays(GL_TRIANGLES, 0, chunk->elements);
	}
}
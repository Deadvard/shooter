#include "dc.h"
#include <stdio.h>

DcChunk * dcChunkCreate()
{
	DcChunk *chunk = (DcChunk *)malloc(sizeof(DcChunk));
	for (int i = 0; i < ArrayCount(chunk->block); ++i)
	{
		chunk->block[i].material = 0;
		chunk->block[i].x        = 0;
		chunk->block[i].y        = 0;
		chunk->block[i].z        = 0;
	}

	chunk->elements = 0;
	chunk->changed = true;
	glGenBuffers(1, &chunk->vbo);
	return chunk;
}


void dcChunkUpdate(DcChunk *chunk)
{
	chunk->changed = false;

	glm::vec3 vertices[ArrayCount(chunk->block) * 4];
	int c = 0;

	int size = 16;

	for (int i = 0; i < ArrayCount(chunk->block); ++i)
	{
		uint8 type = chunk->block[i].material;

		uint8 x =  i % size;
		uint8 y = (i / size) % size;
		uint8 z =  i / (size * size);

		unsigned int indices[8];
		indices[0] = 0 + i;
		indices[1] = 1 + i;
		indices[2] = 1 + i + size * size;
		indices[3] = 0 + i + size * size;
		indices[4] = 0 + i + size;
		indices[5] = 1 + i + size;
		indices[6] = 1 + i + size + size * size;
		indices[7] = 0 + i + size + size * size;

		/*if (type != 0)
		{
			int neighbor = 0;
			if (x > 0) neighbor = chunk->block[x - 1][y][z];
			if (neighbor == 0)
			{
				vertices[c++] = glm::vec3(x    , y + 1, z + 1);
				vertices[c++] = glm::vec3(x    , y + 1, z    );
				vertices[c++] = glm::vec3(x    , y    , z    );
				vertices[c++] = glm::vec3(x    , y    , z    );
				vertices[c++] = glm::vec3(x    , y    , z + 1);
				vertices[c++] = glm::vec3(x    , y + 1, z + 1);
			}

			neighbor = 0;
			if (x < size - 1) neighbor = chunk->block[x + 1][y][z];
			if (neighbor == 0)
			{
				vertices[c++] = glm::vec3(x + 1, y + 1, z + 1);
				vertices[c++] = glm::vec3(x + 1, y    , z + 1);
				vertices[c++] = glm::vec3(x + 1, y    , z    );
				vertices[c++] = glm::vec3(x + 1, y    , z    );
				vertices[c++] = glm::vec3(x + 1, y + 1, z    );
				vertices[c++] = glm::vec3(x + 1, y + 1, z + 1);
			}

			neighbor = 0;
			if (y > 0) neighbor = chunk->block[x][y - 1][z];
			if (neighbor == 0)
			{
				vertices[c++] = glm::vec3(x    , y    , z    );
				vertices[c++] = glm::vec3(x + 1, y    , z    );
				vertices[c++] = glm::vec3(x + 1, y    , z + 1);
				vertices[c++] = glm::vec3(x + 1, y    , z + 1);
				vertices[c++] = glm::vec3(x    , y    , z + 1);
				vertices[c++] = glm::vec3(x    , y    , z    );
			}

			neighbor = 0;
			if (y < size - 1) neighbor = chunk->block[x][y + 1][z];
			if (neighbor == 0)
			{
				vertices[c++] = glm::vec3(x    , y + 1, z    );
				vertices[c++] = glm::vec3(x    , y + 1, z + 1);
				vertices[c++] = glm::vec3(x + 1, y + 1, z + 1);
				vertices[c++] = glm::vec3(x + 1, y + 1, z + 1);
				vertices[c++] = glm::vec3(x + 1, y + 1, z    );
				vertices[c++] = glm::vec3(x    , y + 1, z    );
			}

			neighbor = 0;
			if (z > 0) neighbor = chunk->block[x][y][z - 1];
			if (neighbor == 0)
			{
				vertices[c++] = glm::vec3(x    , y    , z    );
				vertices[c++] = glm::vec3(x    , y + 1, z    );
				vertices[c++] = glm::vec3(x + 1, y + 1, z    );
				vertices[c++] = glm::vec3(x + 1, y + 1, z    );
				vertices[c++] = glm::vec3(x + 1, y    , z    );
				vertices[c++] = glm::vec3(x    , y    , z    );
			}

			neighbor = 0;
			if (z < size - 1) neighbor = chunk->block[x][y][z + 1];
			if (neighbor == 0)
			{
				vertices[c++] = glm::vec3(x    , y    , z + 1);
				vertices[c++] = glm::vec3(x + 1, y    , z + 1);
				vertices[c++] = glm::vec3(x + 1, y + 1, z + 1);
				vertices[c++] = glm::vec3(x + 1, y + 1, z + 1);
				vertices[c++] = glm::vec3(x    , y + 1, z + 1);
				vertices[c++] = glm::vec3(x    , y    , z + 1);
			}
		}*/
	}

	chunk->elements = c;
	glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
	glBufferData(GL_ARRAY_BUFFER, chunk->elements * sizeof(glm::vec3), 0, GL_STATIC_DRAW);
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, chunk->elements * sizeof(glm::vec3), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void dcChunkRender(DcChunk *chunk)
{
	if (chunk->changed)
	{
		dcChunkUpdate(chunk);
	}

	if (chunk->elements > 0)
	{	
		glBindVertexBuffer(0, chunk->vbo, 0, sizeof(uint16));
		glBindVertexBuffer(1, chunk->vbo, chunk->elements * sizeof(uint16), sizeof(uint16));

		glDrawArrays(GL_TRIANGLES, 0, chunk->elements);
	}
}
#include "edvarddc.h"

DcChunk * dcChunkCreate()
{
	DcChunk *chunk = (DcChunk *)malloc(sizeof(DcChunk));
	memset(chunk, 0, sizeof(DcChunk));

	chunk->changed = true;
	glGenBuffers(1, &chunk->vertexBuffer);
	glGenBuffers(1, &chunk->indexBuffer);
	return chunk;
}

DcVertex dcVert(uint8 x, uint8 y, uint8 z, float u, float v)
{
	DcVertex vertex;
	vertex.position = glm::vec3(x, y, z);
	vertex.normal = glm::vec3(0, 0, 0);
	vertex.uv = glm::vec2(u / 6.0f, v);
	return vertex;
}


void dcChunkUpdate(DcChunk *chunk)
{
	chunk->changed = false;
	chunk->indexCount = 0;
	
	for (int i = 0; i < ArrayCount(chunk->cells); ++i)
	{
		int x = i % 32;
		int y = (i / 32) % 32;
		int z = i / (32 * 32);

		static int const CHUNK_SIZE = 32;

		if (chunk->distances[x][y][z] > 0)
		{
			int neighbor = 0;
			if (x > 0) neighbor = chunk->distances[x - 1][y][z];
			if (neighbor < 0)
			{
				chunk->indices[i++] = chunk->cells[x][y + 1][z + 1];
				chunk->indices[i++] = chunk->cells[x][y + 1][z    ];
				chunk->indices[i++] = chunk->cells[x][y    ][z    ];
				chunk->indices[i++] = chunk->cells[x][y    ][z    ];
				chunk->indices[i++] = chunk->cells[x][y    ][z + 1];
				chunk->indices[i++] = chunk->cells[x][y + 1][z + 1];
			}

			neighbor = 0;
			if (x < CHUNK_SIZE - 1) neighbor = chunk->distances[x + 1][y][z];
			if (neighbor < 0)
			{
				chunk->indices[i++] = chunk->cells[x + 1][y + 1][z + 1];
				chunk->indices[i++] = chunk->cells[x + 1][y    ][z + 1];
				chunk->indices[i++] = chunk->cells[x + 1][y    ][z    ];
				chunk->indices[i++] = chunk->cells[x + 1][y    ][z    ];
				chunk->indices[i++] = chunk->cells[x + 1][y + 1][z    ];
				chunk->indices[i++] = chunk->cells[x + 1][y + 1][z + 1];
			}

			neighbor = 0;
			if (y > 0) neighbor = chunk->distances[x][y - 1][z];
			if (neighbor < 0)
			{
				chunk->indices[i++] = chunk->cells[x    ][y][z    ];
				chunk->indices[i++] = chunk->cells[x + 1][y][z    ];
				chunk->indices[i++] = chunk->cells[x + 1][y][z + 1];
				chunk->indices[i++] = chunk->cells[x + 1][y][z + 1];
				chunk->indices[i++] = chunk->cells[x    ][y][z + 1];
				chunk->indices[i++] = chunk->cells[x    ][y][z    ];
			}

			neighbor = 0;
			if (y < CHUNK_SIZE - 1) neighbor = chunk->distances[x][y + 1][z];
			if (neighbor < 0)
			{
				chunk->indices[i++] = chunk->cells[x    ][y + 1][z    ];
				chunk->indices[i++] = chunk->cells[x    ][y + 1][z + 1];
				chunk->indices[i++] = chunk->cells[x + 1][y + 1][z + 1];
				chunk->indices[i++] = chunk->cells[x + 1][y + 1][z + 1];
				chunk->indices[i++] = chunk->cells[x + 1][y + 1][z    ];
				chunk->indices[i++] = chunk->cells[x    ][y + 1][z    ];
			}

			neighbor = 0;
			if (z > 0) neighbor = chunk->distances[x][y][z - 1];
			if (neighbor < 0)
			{
				chunk->indices[i++] = chunk->cells[x    ][y    ][z];
				chunk->indices[i++] = chunk->cells[x    ][y + 1][z];
				chunk->indices[i++] = chunk->cells[x + 1][y + 1][z];
				chunk->indices[i++] = chunk->cells[x + 1][y + 1][z];
				chunk->indices[i++] = chunk->cells[x + 1][y    ][z];
				chunk->indices[i++] = chunk->cells[x    ][y    ][z];
			}

			neighbor = 0;
			if (z < CHUNK_SIZE - 1) neighbor = chunk->distances[x][y][z + 1];
			if (neighbor < 0)
			{
				chunk->indices[i++] = chunk->cells[x    ][y    ][z + 1];
				chunk->indices[i++] = chunk->cells[x + 1][y    ][z + 1];
				chunk->indices[i++] = chunk->cells[x + 1][y + 1][z + 1];
				chunk->indices[i++] = chunk->cells[x + 1][y + 1][z + 1];
				chunk->indices[i++] = chunk->cells[x    ][y + 1][z + 1];
				chunk->indices[i++] = chunk->cells[x    ][y    ][z + 1];
			}
	}

}

void dcVertexAttribPointers()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	int stride = sizeof(DcVertex);
	float *pointer = 0;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, pointer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (pointer + 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (pointer + 6));
}


void dcChunkRender(DcChunk *chunk)
{
	if (chunk->changed)
	{
		dcChunkUpdate(chunk);
	}

	if (chunk->elements > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
		dcVertexAttribPointers();

		glDrawArrays(GL_TRIANGLES, 0, chunk->elements);
	}
}

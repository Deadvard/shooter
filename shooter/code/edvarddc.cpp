#include "edvarddc.h"

DcChunk * dcChunkCreate()
{
	DcChunk *chunk = (DcChunk *)malloc(sizeof(DcChunk));
	memset(chunk, 0, sizeof(DcChunk));

	static int const CHUNK_SIZE = 32;

	for (int z = 0; z < CHUNK_SIZE / 2; ++z)
	for (int y = 0; y < CHUNK_SIZE; ++y)
	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		chunk->distances[x][y][z] = -10;
	}

	chunk->changed = true;
	glGenBuffers(1, &chunk->vertexBuffer);
	glGenBuffers(1, &chunk->indexBuffer);
	return chunk;
}

DcVertex dcVert(float x, float y, float z, float u, float v)
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

	static int const CHUNK_SIZE = 32;

	int cellIndex = 0;
	for (int z = 0; z < CHUNK_SIZE - 1; ++z)
	for (int y = 0; y < CHUNK_SIZE - 1; ++y)
	for (int x = 0; x < CHUNK_SIZE - 1; ++x)
	{		
		uint8 caseCode =
			int(chunk->distances[x+1][y  ][z+1] < 0) << 7 |
			int(chunk->distances[x+1][y+1][z+1] < 0) << 6 |
			int(chunk->distances[x  ][y+1][z+1] < 0) << 5 |
			int(chunk->distances[x  ][y  ][z+1] < 0) << 4 |
			int(chunk->distances[x+1][y+1][z+1] < 0) << 3 |
			int(chunk->distances[x+1][y+1][z  ] < 0) << 2 |
			int(chunk->distances[x+1][y  ][z  ] < 0) << 1 |
			int(chunk->distances[x  ][y  ][z  ] < 0) << 0;

		if (caseCode != 0 && caseCode != 255)
		{
			float fx = x + 0.5f;
			float fy = y + 0.5f;
			float fz = z + 0.5f;
			
			chunk->cells[x][y][z] = cellIndex;
			chunk->vertices[cellIndex] = dcVert(fx, fy, fz, 0, 0);
			++cellIndex;
		}
	}

	chunk->vertexCount = cellIndex;
	
	int i = 0;
	for (int z = 0; z < CHUNK_SIZE - 1; ++z)
	for (int y = 0; y < CHUNK_SIZE - 1; ++y)
	for (int x = 0; x < CHUNK_SIZE - 1; ++x)
	{
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
	chunk->indexCount = i;

	glBindBuffer(GL_ARRAY_BUFFER, chunk->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, chunk->vertexCount * sizeof(DcVertex), chunk->vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, chunk->indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, chunk->indexCount * sizeof(uint16), chunk->indices, GL_STATIC_DRAW);
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

	if (chunk->vertexCount > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, chunk->indexBuffer);
		dcVertexAttribPointers();

		glDrawElements(GL_TRIANGLES, chunk->indexCount, GL_UNSIGNED_SHORT, 0);
	}
}

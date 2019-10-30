#include "dc.h"
#include <stdio.h>

static int edgeTable[256] =
{
	0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
	0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
	0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
	0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
	0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
	0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
	0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
	0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
	0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
	0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
	0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
	0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
	0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
	0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
	0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
	0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
	0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
	0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
	0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
	0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
	0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
	0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
	0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
	0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
	0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
	0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
	0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
	0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
	0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
	0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
	0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
	0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0
};

static int intersectionTable[12][2] =
{
	{ 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 }, { 4, 5 }, { 5, 6 },
	{ 6, 7 }, { 7, 4 }, { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 }
};

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
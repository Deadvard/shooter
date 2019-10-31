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

glm::vec3 vertexInterpolation(glm::vec3 p1, glm::vec3 p2, float valp1, float valp2)
{
	double mu;
	double isolevel = 0.f;
	glm::vec3 out;

	if (abs(isolevel - valp1) < 0.00001)
		return(p1);
	if (abs(isolevel - valp2) < 0.00001)
		return(p2);
	if (abs(valp1 - valp2) < 0.00001)
		return(p1);
	mu = (isolevel - valp1) / (valp2 - valp1);
	out.x = p1.x + mu * (p2.x - p1.x);
	out.y = p1.y + mu * (p2.y - p1.y);
	out.z = p1.z + mu * (p2.z - p1.z);

	return(out);
}

void addBox(densityField* df, glm::vec3 pt)
{
	float height = 1.f;
	float depth = 1.f;
	int index = pt.x * height*depth + pt.y * depth + pt.z;
	for(int i = 0; i < 8; ++i)
		df->densities[index + i] = 1.f;
}

void initField(densityField* df)
{
	df->densities = new float[worldsize];
	df->cubes = new Cube[worldsize / 8];
	for (int i = 0; i < worldsize; ++i)
	{
		df->densities[i] = -1.f;
		if (i % 8 == 0)
		{
			df->cubes[i / 8].index = -1;
			df->cubes[i / 8].meshPt = nullptr;
		}
	}

	addBox(df, glm::vec3(0,0,0));
}

void computeCubes(densityField* df)
{
	for (int i = 0; i < worldsize / 8; ++i)
	{
		int index = 0;
		for(int j = 0; j < 8; ++j)
			if (df->densities[i * 8 + j] < 1.f) index |= (1 << j);

		int edgeInfo = edgeTable[index];
		if(edgeInfo == 0) continue;

		glm::vec3 corners[8];
		corners[0] = glm::vec3(0, 0, 0);
		corners[1] = glm::vec3(1, 0, 0);
		corners[2] = glm::vec3(1, 1, 0);
		corners[3] = glm::vec3(0, 1, 0);
		corners[4] = glm::vec3(0, 0, 1);
		corners[5] = glm::vec3(1, 0, 1);
		corners[6] = glm::vec3(1, 1, 1);
		corners[7] = glm::vec3(0, 1, 1);

		glm::vec3 points[12];
		glm::vec3 normals[12];
		glm::vec3 massPoint = glm::vec3(0);
		unsigned int numIntersections = 0;

		for (int j = 0; j < 12; ++j)
		{
			if (!(edgeInfo & (1 << j)))
				continue;

			int v1 = intersectionTable[j][0];
			int v2 = intersectionTable[j][1];
			glm::vec3 globalPos = glm::vec3(i / (32 * 32), (i / 32) % 32, i % 32);
			if(abs(df->densities[i * 8 + v1]) < 1e-3)
				points[j] = globalPos + corners[v1];
			else if (abs(df->densities[i * 8 + v2]) < 1e-3)
				points[j] = globalPos + corners[v2];
			else if (abs(df->densities[i * 8 + v1] - df->densities[i * 8 + v2]) < 1e-3)
				points[j] = globalPos + corners[v1];
			else
			{
				points[j] = globalPos + vertexInterpolation(corners[v1], corners[v2], df->densities[i * 8 + j] + v1, df->densities[i * 8 + j] + v2);
			}
			static glm::vec3 sphereOrigin = glm::vec3(5,5,5);
			normals[j] = points[j] - sphereOrigin;
			massPoint += points[j];
			numIntersections++;
		}

		massPoint /= (float)numIntersections;

		glm::vec3 newPointNormal = glm::vec3(0);
		double matrix[12][3];
		double vector[12];
		int rows = 0;

		for (int j = 0; j < 12; ++j)
		{
			if (!(edgeInfo & (1 << j)))
				continue;

			matrix[rows][0] = normals[j].x;
			matrix[rows][1] = normals[j].y;
			matrix[rows][2] = normals[j].z;

			glm::vec3 p = points[j] - massPoint;
			glm::vec3 n = normals[j] * p;
			vector[rows++] = (double)(n.x + n.y + n.z);

			newPointNormal += normals[j];

		}
	}
}

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
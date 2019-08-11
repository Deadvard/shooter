#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct Mesh
{
	int firstVertex;
	int sizeVertex;

	int firstIndex;
	int sizeIndex;
};

struct MeshStorage
{
	unsigned int numberOfVertices;
	unsigned int numberOfIndices;
	unsigned int vertexBuffer;
	unsigned int indexBuffer;
	unsigned int vertexArray;
};

Mesh meshFromBuffer(MeshStorage *storage, char *data);

MeshStorage meshStorageCreate(int size);
Mesh meshCreate(MeshStorage *storage, Vertex *vertices, int sizeVertices, unsigned int *indices, int sizeIndices);
void meshRender(MeshStorage *storage, Mesh *mesh);

#endif

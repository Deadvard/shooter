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
	int first;
	int size;
};

struct MeshStorage
{
	unsigned int numberOfVertices;
	unsigned int numberOfIndices;
	unsigned int vertexBuffer;
	unsigned int indexBuffer;
	unsigned int vertexArray;
};

Vertex * verticesFromBuffer(char *data, int *size);

MeshStorage meshStorageCreate(int size);
Mesh meshCreate(MeshStorage *storage, Vertex *vertices, int size);
void meshRender(MeshStorage *storage, Mesh *mesh);

#endif

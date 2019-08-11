#include "mesh.h"
#include <glad/glad.h>

#include <stdio.h>

Mesh meshFromBuffer(MeshStorage *storage, char *data)
{
	int sizeVertex = (int)*data;
	int sizeIndex = (int)*(data + sizeof(int));
	Vertex *vertices = (Vertex *)(data + 2 * sizeof(int));
	unsigned int *indices = (unsigned int *)(vertices + sizeVertex);
	return meshCreate(storage, vertices, sizeVertex, indices, sizeIndex);	
}

MeshStorage meshStorageCreate(int size)
{
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 5));

	MeshStorage result;
	result.numberOfVertices = 0;
	result.numberOfIndices = 0;
	result.vertexArray = vao;
	result.vertexBuffer = vbo;
	result.indexBuffer = ebo;

	return result;
}

Mesh meshCreate(MeshStorage *storage, Vertex *vertices, int sizeVertices, unsigned int *indices, int sizeIndices)
{
	Mesh result;
	result.firstVertex = storage->numberOfVertices;
	result.sizeVertex = sizeVertices;
	result.firstIndex = storage->numberOfIndices;
	result.sizeIndex = sizeIndices;
	storage->numberOfVertices += sizeVertices;
	storage->numberOfIndices += sizeIndices;
	
	glBindBuffer(GL_ARRAY_BUFFER, storage->vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex) * result.firstVertex, sizeof(Vertex) * result.sizeVertex, vertices);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, storage->indexBuffer);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * result.firstIndex, sizeof(unsigned int) * result.sizeIndex, indices);

	return result;
}

void meshRender(MeshStorage *storage, Mesh *mesh)
{
	glBindVertexArray(storage->vertexArray);
	//glDrawElements(GL_TRIANGLES, mesh->sizeIndex, GL_UNSIGNED_INT, (void *)(mesh->firstIndex * sizeof(unsigned int)));
	glDrawElementsBaseVertex(GL_TRIANGLES, mesh->sizeIndex, GL_UNSIGNED_INT,
		(void *)(mesh->firstIndex * sizeof(unsigned int)), mesh->firstVertex);

	glBindVertexArray(0);
}
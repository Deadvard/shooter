#include "mesh.h"
#include <glad/glad.h>

#include <stdio.h>

MeshStorage meshStorageCreate(int size)
{
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);

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
	result.indexBuffer = 0;

	return result;
}

Mesh meshCreate(MeshStorage *storage, Vertex *vertices, int size)
{
	Mesh result;
	result.first = storage->numberOfVertices;
	result.size = size;
	storage->numberOfVertices += size;
	
	glBindBuffer(GL_ARRAY_BUFFER, storage->vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex) * result.first, sizeof(Vertex) * result.size, vertices);

	return result;
}

void meshRender(MeshStorage *storage, Mesh *mesh)
{
	glBindVertexArray(storage->vertexArray);
	glDrawArrays(GL_TRIANGLES, mesh->first, mesh->size);
	glBindVertexArray(0);
}
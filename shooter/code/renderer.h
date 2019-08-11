#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

static const struct
{
	int length = 36;
	float data[36 * 8]
	{
		//pos, uv, normal
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	};
} box;

struct Vertex
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct Mesh
{
	unsigned int numberOfVertices;
	unsigned int numberOfIndices;
	unsigned int vertexArray;
	unsigned int vertexBuffer;
	unsigned int indexBuffer;
};

struct Model
{
	glm::vec3 position;
	glm::quat rotation;

	glm::quat jointRotation[32];
	unsigned int jointIndex[32];

	unsigned int modelIndex;
};

struct Camera
{
	float yaw;
	float pitch;
	
	glm::vec3 position;
	Model *focus;
};

struct Renderer
{
	unsigned int primaryShader;
	unsigned int textShader;
	unsigned int matrixBuffer;

	glm::mat4 projection;

	Mesh boxes;

	Camera cam;
	Model mod;
};

void rendererInitialize(Renderer *renderer);

static glm::mat4 matrix_view(Camera *c)
{
	return glm::lookAt(c->position, c->focus->position, glm::vec3(0.0f, 1.0f, 0.0f));
}

unsigned int shaderProgramCreate(const char *vertex_path, const char *fragment_path);

unsigned imageTextureCreate(const char *path);
unsigned textTextureCreate(const char *path, const char *text);
void cubeRender(Renderer *renderer);

#endif

#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "mesh.h"
#include "font.h"
#include "ui.h"
#include "cube.h"

struct Skybox
{
	unsigned int shader;
	unsigned int textureID;
	unsigned int vao;
	unsigned int vbo;
};

struct Lighting
{
	unsigned int toonTexture;
};

struct Model
{
	glm::vec3 position;
	glm::quat rotation;
	unsigned int meshIndex;
};

struct WeatherEffects
{
	glm::vec3 sunDirection;
};

struct Crosshair
{
	unsigned int vao;
	unsigned int vbo;
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
	unsigned int skyboxShader;
	unsigned int crosshairShader;

	glm::mat4 projection;

	Mesh meshes[128];
	unsigned int numMeshes;
	Model activeModels[128];
	unsigned int numActiveModels;

	Skybox skybox;
	WeatherEffects weather;
	Crosshair crosshair;

	Font font;

	MeshStorage meshStorage;
	Camera cam;

	Lighting lighting;

	unsigned int cubeShader;
	unsigned int cubeVAO;
	unsigned int cubeTexture;
	
	ImmidiateMode im;
	Primitives prim;
};

unsigned int rendererAddModel(Renderer* renderer, int meshIndex, glm::vec3 startingPos);
void rendererInitialize(Renderer *renderer);
void rendererUpdate(Renderer* renderer, float deltaTime);

static glm::mat4 matrixView(Camera *c)
{
	//return glm::lookAt(c->position, c->focus->position, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, -c->focus->position);
	glm::mat4 rotate = glm::mat4_cast(c->focus->rotation);
	return rotate * translate;
}

unsigned int shaderProgramCreate(const char *vertexPath, const char *fragmentPath);

unsigned imageTextureCreate(const char *path);
unsigned textTextureCreate(const char *path, const char *text);
void renderScene(Renderer *renderer, Chunk* chunks[], Chunk *selection);
void renderUI(Renderer *renderer, UserInterface *ui);

#endif

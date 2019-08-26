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
	unsigned int textureID;
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

struct QuadMesh
{
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
};

struct Gui
{
	QuadMesh quadMesh;
	unsigned int activeElements;
	glm::vec2 positions[128];
};

struct ThunderEffect
{
	bool isActive;
	unsigned int thunderVao;
	unsigned int thunderVbo;

	glm::vec3 positions[65];
	float strength[65];
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
	unsigned int thunderShader;
	unsigned int quadShader;
	unsigned int matrixBuffer;

	glm::mat4 projection;

	Mesh meshes[128];
	unsigned int numMeshes;
	Model activeModels[128];
	unsigned int numActiveModels;

	ThunderEffect thunderEffect;

	Font font;
	Gui gui;

	MeshStorage meshStorage;
	Camera cam;

	Lighting lighting;

	unsigned int cubeShader;
	unsigned int cubeVAO;
	unsigned int cubeTexture;
	Chunk *chunk;
	
	ImmidiateMode im;
	Primitives prim;
};

unsigned int rendererAddModel(Renderer* renderer, int meshIndex, glm::vec3 startingPos);
void rendererInitialize(Renderer *renderer);
void rendererUpdate(Renderer* renderer);
void rendererAddGuiElement(Renderer* renderer, glm::vec2* position);
void rendererRefreshThunder(Renderer* renderer);

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
void renderScene(Renderer *renderer);
void renderUI(Renderer *renderer, UserInterface *ui);

#endif

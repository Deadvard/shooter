#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include "game.h"
#include "renderer.h"
#include "cube.h"

struct Actor
{
	unsigned int modelIndex;
	float speed;
	glm::vec3 direction;
};

struct Gameplay
{
	Actor actors[128];
	unsigned int numActors;
};

void gameplayInitialize(Gameplay* gameplay, Renderer* renderer);
void gameplayUpdate(Gameplay* gameplay, Renderer* renderer, float deltaTime);

void gameplayShoot(Gameplay* gameplay, Model* cameraFocus, Renderer* renderer);

void gameplayAddBlock(Model* cameraFocus, glm::vec2* mousePos, Chunk* relevantChunk);

#endif
#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include "game.h"
#include "renderer.h"
#include <queue>

struct ThunderGameplay
{
	glm::vec3 start;
	glm::vec3 end;
	std::queue<glm::vec3> queue;
};

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
	ThunderGameplay thunderGameplay;
};

void gameplayInitialize(Gameplay* gameplay, Renderer* renderer);
void gameplayUpdate(Gameplay* gameplay, Renderer* renderer, float deltaTime);

void gameplayShoot(Gameplay* gameplay, Model* cameraFocus, Renderer* renderer);

void gameplayActivateThunder(Gameplay* gameplay, Model* cameraFocus, Renderer* renderer);

#endif
#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include "game.h"
#include "renderer.h"

struct Gameplay
{

};

struct Actor
{
	unsigned int modelIndex;
};


void gameplayInitialize(Renderer* renderer);
void gameplayUpdate();

void gameplayShoot(Model* cameraFocus, Renderer* renderer);

#endif
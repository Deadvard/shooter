#include "gameplay.h"

unsigned int addActor(Gameplay* gameplay, Renderer* renderer, unsigned int meshIndex, glm::vec3 startingPos)
{
	gameplay->actors[gameplay->numActors].modelIndex = rendererAddModel(renderer, meshIndex, startingPos);
	gameplay->actors[gameplay->numActors].speed = 0.f;
	gameplay->actors[gameplay->numActors].direction =glm::vec3(0);
	return gameplay->numActors++;
}

void updateMovement(Gameplay* gameplay, Renderer* renderer)
{
	for (unsigned int i = 0; i < gameplay->numActors; ++i)
	{
		renderer->activeModels[gameplay->actors[i].modelIndex].position += gameplay->actors[i].direction * gameplay->actors[i].speed;
	}
}

void gameplayInitialize(Gameplay* gameplay, Renderer* renderer)
{
	gameplay->numActors = 0;

	addActor(gameplay, renderer, 0, glm::vec3(0, 0, -10));
}

void gameplayUpdate(Gameplay* gameplay, Renderer* renderer, float deltaTime)
{
	updateMovement(gameplay, renderer);
}

void gameplayShoot(Gameplay* gameplay, Model* cameraFocus, Renderer* renderer)
{
	glm::vec3 forward = glm::normalize(glm::inverse(cameraFocus->rotation) * glm::vec3(0,0,-1));
	unsigned int bullet = addActor(gameplay, renderer, 0, cameraFocus->position);
	gameplay->actors[bullet].direction = forward;
	gameplay->actors[bullet].speed = 0.5f;
	renderer->activeModels[gameplay->actors[bullet].modelIndex].rotation = glm::inverse(cameraFocus->rotation);
}

void gameplayAddBlock(Model* cameraFocus, glm::vec2* mousePos, Chunk* relevantChunk)
{
	
}

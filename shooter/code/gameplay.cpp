#include "gameplay.h"

unsigned int addActor(Gameplay* gameplay, Renderer* renderer, unsigned int meshIndex, glm::vec3 startingPos)
{
	gameplay->actors[gameplay->numActors].modelIndex = rendererAddModel(renderer, meshIndex, startingPos);
	gameplay->actors[gameplay->numActors].speed = 0.f;
	gameplay->actors[gameplay->numActors].direction =glm::vec3(0);
	return gameplay->numActors++;
}

void gameplayInitialize(Gameplay* gameplay, Renderer* renderer)
{
	gameplay->numActors = 0;

	addActor(gameplay, renderer, 0, glm::vec3(0, 0, -10));
	addActor(gameplay, renderer, 1, glm::vec3(0, 20, 0));
	addActor(gameplay, renderer, 2, glm::vec3(5, 0, 0));
	addActor(gameplay, renderer, 3, glm::vec3(0, -5, 0));
}

void gameplayUpdate(Gameplay* gameplay, Renderer* renderer)
{
	for (int i = 0; i < gameplay->numActors; ++i)
	{
		renderer->activeModels[gameplay->actors[i].modelIndex].position += gameplay->actors[i].direction * gameplay->actors[i].speed;
	}
}

void gameplayShoot(Gameplay* gameplay, Model* cameraFocus, Renderer* renderer)
{
	glm::vec3 forward = glm::normalize(glm::inverse(cameraFocus->rotation) * glm::vec3(0,0,-1));
	unsigned int bullet = addActor(gameplay, renderer, 2, cameraFocus->position);
	gameplay->actors[bullet].direction = forward;
	gameplay->actors[bullet].speed = 0.5f;
	renderer->activeModels[gameplay->actors[bullet].modelIndex].rotation = glm::inverse(cameraFocus->rotation);
}

void gameplayLightning(Gameplay* gameplay, Model* cameraFocus, Renderer* renderer)
{
	glm::vec3 forward = glm::normalize(glm::inverse(cameraFocus->rotation) * glm::vec3(0, 0, -1));
	glm::vec3 start = cameraFocus->position;
	glm::vec3 end = cameraFocus->position + forward * 20.f;

	glm::vec3 middle = (end-start) * 0.5f;
	glm::vec3 offset = glm::normalize(glm::cross(end, start));
	middle *= offset;

	//renderer->lightning[0] = start;
	//renderer->lightning[1] = middle;
	//renderer->lightning[2] = end;

}

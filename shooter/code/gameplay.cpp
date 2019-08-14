#include "gameplay.h"

void gameplayInitialize(Renderer* renderer)
{
	addModelToScene(renderer, 0, glm::vec3(0, 0, -10));
	addModelToScene(renderer, 1, glm::vec3(0, 20, 0));
	addModelToScene(renderer, 2, glm::vec3(5, 0, 0));
	addModelToScene(renderer, 3, glm::vec3(0, -5, 0));
}

void gameplayUpdate(Renderer* renderer)
{
}

void gameplayShoot(Model* cameraFocus, Renderer* renderer)
{
	glm::vec3 forward = glm::normalize(cameraFocus->rotation * glm::vec3(0,0,-1));
	addModelToScene(renderer, 2, cameraFocus->position);

}

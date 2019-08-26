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

void gameplayUpdate(Gameplay* gameplay, Renderer* renderer, float deltaTime)
{
	for (int i = 0; i < gameplay->numActors; ++i)
	{
		renderer->activeModels[gameplay->actors[i].modelIndex].position += gameplay->actors[i].direction * gameplay->actors[i].speed;
	}

	if (renderer->thunderEffect.isActive)
	{
		static float timer = 10.0f;

		if (timer > deltaTime)
		{

			rendererRefreshThunder(renderer);

			timer = 0.0f;
		}
		else
			timer+= deltaTime;

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

void gameplayActivateThunder(Gameplay* gameplay, Model* cameraFocus, Renderer* renderer)
{
	renderer->thunderEffect.isActive = true;

	glm::vec3 forward = glm::normalize(glm::inverse(cameraFocus->rotation) * glm::vec3(0, 0, -1));
	gameplay->thunderGameplay.start = cameraFocus->position;
	gameplay->thunderGameplay.end = cameraFocus->position + forward * 20.f;

	glm::vec3 middle = (gameplay->thunderGameplay.end - gameplay->thunderGameplay.start) * 0.5f;
	glm::vec3 offset = glm::normalize(glm::cross(gameplay->thunderGameplay.end, gameplay->thunderGameplay.start));
	middle += offset;

	renderer->thunderEffect.positions[0] = gameplay->thunderGameplay.start;
	renderer->thunderEffect.positions[32] = middle;
	renderer->thunderEffect.positions[64] = gameplay->thunderGameplay.end;

	float interpolationOffset = 0.05f;
	glm::vec3 temp = glm::mix(gameplay->thunderGameplay.start, middle, interpolationOffset);
	for (int i = 1; i < 32; ++i)
	{
		renderer->thunderEffect.positions[i] = temp;
		temp = glm::mix(temp, middle, interpolationOffset);
	}

	temp = glm::mix(middle, gameplay->thunderGameplay.end, interpolationOffset);
	for (int i = 33; i < 64; ++i)
	{
		renderer->thunderEffect.positions[i] = temp;
		temp = glm::mix(temp, gameplay->thunderGameplay.end, interpolationOffset);
	}
}

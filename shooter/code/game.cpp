#include "game.h"

#include <SDL/SDL.h>
#include <glad.c>
#include <stdio.h>

#include "renderer.h"
#include "physics.h"
#include "gameplay.h"
#include "ui.h"
#include "cube.h"

#include "parser.h"
#include "input.h"

#define pi32 3.14159265359f

static const glm::vec3 FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);
static const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
static const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);

int pointInRectangle(float x, float y, float w, float h, float px, float py);

enum ButtonState
{
	stateNone,
	stateHover,
	statePressed
};

struct Button
{
	float x;
	float y;
	float w;
	float h;
	ButtonState state;
};

struct MenuState
{
	Button buttons[10];
};

struct Window
{
	SDL_Window* window;
	SDL_GLContext glContext;
};

struct Chunks
{
	Chunk* chunks[100];
};

void initializeChunks(Chunks* chunks, int numChunks);
void updateMenu(MenuState *menuState, glm::vec2 mousePosition, int lmbPressed);

static void editBlock(Model *cameraFocus, Chunks *chunks, uint8 type)
{
	Ray ray;
	ray.origin = cameraFocus->position;
	ray.direction = glm::normalize(glm::inverse(cameraFocus->rotation) * FORWARD);

	uint8 *block = 0;
	bool32 *changed = 0;
	float closest = 10.0f;
	
	for (int j = 0; j < 100; ++j)
	{
		Chunk *chunk = chunks->chunks[j];
		AABB chunkAAB;
		chunkAAB.position = chunk->position + 8.0f;
		chunkAAB.size = glm::vec3(8.0f, 8.0f, 8.0f);

		if (raycast(&chunkAAB, &ray) > 0.0f)
		{
			for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; ++i)
			{
				int x = i % CHUNK_SIZE;
				int y = (i / CHUNK_SIZE) % CHUNK_SIZE;
				int z = i / (CHUNK_SIZE * CHUNK_SIZE);

				if (chunk->block[x][y][z])
				{
					AABB right;
					right.position = chunk->position + glm::vec3(x, y, z) + 0.5f;
					right.size = glm::vec3(0.5f);

					float result = raycast(&right, &ray);

					if (result > 0.0f && result < closest)
					{
						closest = result;
						block = &chunk->block[x][y][z];
						changed = &chunk->changed;
					}
				}
			}
		}	
	}

	if (block && changed)
	{
		*block = type;
		*changed = true;
	}
}

static void windowInitialize(Window *window, const char* title, int width, int height)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL);

	if (!window->window)
	{
		printf("Unable to create window\n");
		printf("SLD Error : %s\n", SDL_GetError());
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	window->glContext = SDL_GL_CreateContext(window->window);

	SDL_GL_SetSwapInterval(1);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		printf("Failed to initialize GLAD");
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void run()
{
	float rot = 0.0f;
	Model cameraFocus;
	cameraFocus.position = glm::vec3(10 * 16 / 2, 16 + 3,10 * 16 / 2);

	UserInterface ui;
	ui.nrOfElements = 0;

	char *data = readEntireFile("resources/input.txt");
	StringPair16 pairs[32];
	memset(pairs, 0, sizeof(pairs));
	parse(data, pairs, 32);
	free(data);

	uint32 width = 0;
	uint32 height = 0;
	const char *title = 0;
	for (int i = 0; i < 32; ++i)
	{
		if (string16IsEqual(&pairs[i].key, &string16("title")))
		{
			title = pairs[i].value.data;
		}
		if (string16IsEqual(&pairs[i].key, &string16("width")))
		{
			width = string16ToUint(&pairs[i].value);
		}
		if (string16IsEqual(&pairs[i].key, &string16("height")))
		{
			height = string16ToUint(&pairs[i].value);
		}
	}
	
	Window window;
	windowInitialize(&window, title, width, height);

	Renderer renderer;
	rendererInitialize(&renderer);

	Gameplay gameplay;
	gameplayInitialize(&gameplay, &renderer);

	Chunks chunks;
	initializeChunks(&chunks, 100);

	int z = 0;
	for (int i = 0; i < 100; ++i)
	{
		chunks.chunks[i]->position = glm::vec3(10.0f + 16 * (i % 10), 0.0f, z);
		z = i % 10 == 9 ? z += 16 : z;
	}


	MenuState menuState;
	for (int i = 0; i < 10; ++i)
	{
		menuState.buttons[i].x = 10.0f;
		menuState.buttons[i].y = 10.0f;
		menuState.buttons[i].w = 10.0f;
		menuState.buttons[i].h = 10.0f;
		menuState.buttons[i].state = stateNone;
	}
	glm::vec2 mousePosition = glm::vec2(0.0f);
	int menuVisible = false;
	int lmbPressed = false;

	double lastTime = (double)SDL_GetTicks() / 1000.0;
	double deltaTime = 0.0;

	int physicsOn = true;

	float wx = 100.0f;
	float wy = 100.0f;
	float ww = 100.0f;
	float wh = 100.0f;
	int windowSelected = false;
	int resizeX = false;
	int resizeY = false;

	Keyboard keyboard;
	bindKeys(&keyboard, pairs, 32);
	for (int i = 0; i < 128; ++i) keyboard.isPressed[i] = false;

	int running = true;
	while (running)
	{
		double now = (double)SDL_GetTicks() / 1000.0;
		deltaTime = now - lastTime;
		lastTime = now;

		float dx = 0.0f;
		float dy = 0.0f;

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
			{
				running = false;
				break;
			}
			case SDL_MOUSEMOTION:
			{
				dx += e.motion.xrel;
				dy += e.motion.yrel;
				mousePosition.x = (float)e.motion.x;
				mousePosition.y = (float)e.motion.y;
				break;
			}
			case SDL_KEYDOWN:
			{
				if (e.key.keysym.sym < 128)
				{
					keyboard.isPressed[e.key.keysym.sym] = true;
				}
				break;
			}
			case SDL_KEYUP:
			{
				if (e.key.keysym.sym < 128)
				{
					keyboard.isPressed[e.key.keysym.sym] = false;
				}
	
				if (e.key.keysym.scancode == SDL_SCANCODE_Q)
				{
					physicsOn = !physicsOn;
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_I)
				{
					menuVisible = !menuVisible;
					SDL_SetRelativeMouseMode((SDL_bool)(!menuVisible));
				}
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					lmbPressed = true;
					editBlock(&cameraFocus, &chunks, 0);
				}
				if (e.button.button == SDL_BUTTON_RIGHT)
				{
					editBlock(&cameraFocus, &chunks, 1);
				}
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					lmbPressed = false;
				}
				else if (e.button.button == SDL_BUTTON_RIGHT)
				{
					//gameplayShoot(&gameplay, &cameraFocus, &renderer);
					//gameplayActivateThunder(&gameplay, &cameraFocus, &renderer);
				}
				break;
			}
			}
		}

		clear(&ui);
		if (menuVisible)
		{
			if (lmbPressed)
			{
				float halfW = ww * 0.5f;
				float halfH = wh * 0.5f;

				if (pointInRectangle(wx + ww * 0.9f, wy, halfW, halfH, mousePosition.x, mousePosition.y))
				{
					resizeX = true;
				}
				if (pointInRectangle(wx, wy + wh * 0.9f, halfW, halfH, mousePosition.x, mousePosition.y))
				{
					resizeY = true;
				}

				if (pointInRectangle(wx, wy, halfW * 0.9f, halfH * 0.9f, mousePosition.x, mousePosition.y))
				{
					windowSelected = true;
				}
			}
			else if (!lmbPressed)
			{
				resizeX = false;
				resizeY = false;
				windowSelected = false;			
			}

			if (windowSelected && !resizeX && !resizeY)
			{
				wx += dx;
				wy += dy;
			}
			if (resizeX && (ww + dx) > 10)
			{
				wx += dx * 0.5f;
				ww += dx;
			}
			if (resizeY && (wh + dy) > 10)
			{
				wy += dy * 0.5f;
				wh += dy;
			}
			
			uiwindow(&ui, "text", wx, wy, ww, wh);
		}		

		glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		rot += pi32 * (float)deltaTime;
		renderer.activeModels[1].rotation = glm::angleAxis(rot, glm::vec3(0, 1.f, 0));

		if (!menuVisible)
		{
			float sensitivity = 0.01f;
			float max = pi32 * 0.4f;
			float min = -max;

			renderer.cam.yaw += (float)dx * sensitivity;
			renderer.cam.pitch += (float)dy * sensitivity;
			if (renderer.cam.pitch < min) renderer.cam.pitch = min;
			if (renderer.cam.pitch > max) renderer.cam.pitch = max;
		}

		glm::quat qPitch = glm::angleAxis(renderer.cam.pitch, glm::vec3(1, 0, 0));
		glm::quat qYaw = glm::angleAxis(renderer.cam.yaw, glm::vec3(0, 1, 0));
		glm::quat qRotate = glm::normalize(qPitch * qYaw);

		cameraFocus.rotation = qRotate;
		glm::vec3 forward = glm::normalize(glm::inverse(qRotate) * FORWARD);
		
		if (physicsOn)
		{
			glm::vec3 forward = glm::normalize(glm::inverse(qYaw) * FORWARD);
		}
		
		glm::vec3 right = glm::normalize(glm::cross(forward, UP));

		glm::vec3 oldPos = cameraFocus.position;
		
		const unsigned char *keys = SDL_GetKeyboardState(0);
		float velocity = 0.1f;
		if (keys[SDL_SCANCODE_LSHIFT]) velocity *= 2.0f;
		if (isPressed(&keyboard, &string16("forward"))) cameraFocus.position += forward * velocity;
		if (isPressed(&keyboard, &string16("left"))) cameraFocus.position -= right * velocity;
		if (isPressed(&keyboard, &string16("backwards"))) cameraFocus.position -= forward * velocity;
		if (isPressed(&keyboard, &string16("right"))) cameraFocus.position += right * velocity;
		renderer.cam.focus = &cameraFocus;

		if (physicsOn)
		{
			glm::vec3 pos = cameraFocus.position;

			pos.y -= 0.9 * deltaTime;

			for (int j = 0; j < 100; ++j)
			{
				Chunk *chunk = chunks.chunks[j];
				AABB left;
				left.position = pos;
				left.size = glm::vec3(1.0f, 2.0f, 1.0f);

				AABB chunkAAB;
				chunkAAB.position = chunk->position + 8.0f;
				chunkAAB.size = glm::vec3(8.0f, 8.0f, 8.0f);
				chunkAAB.size += left.size;
				if (point_in_aabb(&chunkAAB, &pos))
				{
					for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; ++i)
					{
						if (chunk->block[i])
						{
							int x = i % CHUNK_SIZE;
							int y = (i / CHUNK_SIZE) % CHUNK_SIZE;
							int z = i / (CHUNK_SIZE * CHUNK_SIZE);

							AABB right;
							right.position = chunk->position + glm::vec3(x, y, z);
							right.size = glm::vec3(0.5f);

							right.size += left.size;

							if (point_in_aabb(&right, &glm::vec3(pos.x, oldPos.y, oldPos.z))) pos.x = oldPos.x;
							if (point_in_aabb(&right, &glm::vec3(oldPos.x, pos.y, oldPos.z))) pos.y = oldPos.y;
							if (point_in_aabb(&right, &glm::vec3(oldPos.x, oldPos.y, pos.z))) pos.z = oldPos.z;
						}
					}
				}
			}
			cameraFocus.position = pos;
		}

		gameplayUpdate(&gameplay, &renderer, (float)deltaTime);
		rendererUpdate(&renderer, (float)deltaTime);
		
		renderScene(&renderer, chunks.chunks);
		renderUI(&renderer, &ui);

		SDL_GL_SwapWindow(window.window);
		running = running && !isPressed(&keyboard, &string16("quit"));
	}
}

int pointInRectangle(float x, float y, float w, float h, float px, float py)
{
	if ((x - w) > px) return false;
	if ((y - h) > py) return false;
	if ((x + w) < px) return false;
	if ((y + h) < py) return false;

	return true;
}

void initializeChunks(Chunks* chunks, int numChunks)
{
	for (int i = 0; i < numChunks; ++i)
	{
		chunks->chunks[i] = chunkCreate();
		for (int z = 0; z < CHUNK_SIZE; ++z)
			for (int y = 0; y < CHUNK_SIZE; ++y)
				for (int x = 0; x < CHUNK_SIZE; ++x)
				{
					chunks->chunks[i]->block[x][y][z] = (rand() % 6);
				}
	}
}

void updateMenu(MenuState *menuState, glm::vec2 mousePosition, int lmbPressed)
{
	for (int i = 0; i < 10; ++i)
	{	
		Button *b = &menuState->buttons[i];
		b->state = stateNone;
		if (pointInRectangle(b->x, b->y, b->w, b->h, mousePosition.x, mousePosition.y))
		{
			b->state = stateHover;
			if (lmbPressed)
			{
				b->state = statePressed;
			}
		}
	}
	printf("%.3f\n", mousePosition.x);
}
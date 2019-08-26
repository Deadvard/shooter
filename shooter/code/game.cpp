#include "game.h"

#include <SDL/SDL.h>
#include <glad.c>
#include <stdio.h>

#include "renderer.h"
#include "physics.h"
#include "gameplay.h"
#include "ui.h"

#define pi32 3.14159265359f

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

void updateMenu(MenuState *menuState, glm::vec2 mousePosition, int lmbPressed);

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
	SDL_GL_SetSwapInterval(1);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	window->glContext = SDL_GL_CreateContext(window->window);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		printf("Failed to initialize GLAD");
	}
	glEnable(GL_DEPTH_TEST);
}

void run()
{
	float rot = 0.0f;
	Model cameraFocus;
	cameraFocus.position = glm::vec3(0.0f);

	PhysicsData *physicsData = (PhysicsData *)malloc(sizeof(PhysicsData));
	physicsData->position = glm::vec3(-50.0f);
	for (int i = 0; i < 1000000; ++i)
	{
		physicsData->heights[i] = 0.0f;
	}

	float h = 10.0f;
	for (int i = 0; i < 100; ++i)
	{
		h -= 0.1f;
		for (int j = i * 1000; j < (i * 1000 + 1000); ++j)
		{
			physicsData->heights[j] = h;
		}
	}

	UserInterface ui;
	ui.nrOfElements = 0;
	
	Window window;
	windowInitialize(&window, "shooter", 1280, 720);

	Renderer renderer;
	rendererInitialize(&renderer);

	Gameplay gameplay;
	gameplayInitialize(&gameplay, &renderer);

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
				mousePosition.x = e.motion.x;
				mousePosition.y = e.motion.y;
				break;
			}
			case SDL_KEYUP:
			{
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
				}
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					gameplayShoot(&gameplay, &cameraFocus, &renderer);
					lmbPressed = false;
				}
				else if (e.button.button == SDL_BUTTON_RIGHT)
				{
					gameplayActivateThunder(&gameplay, &cameraFocus, &renderer);
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

		static const glm::vec3 FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);
		static const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
		static const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);

		rot += pi32 * deltaTime;
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

		const unsigned char *keys = SDL_GetKeyboardState(0);
		float velocity = 0.1f;
		if (keys[SDL_SCANCODE_LSHIFT]) velocity *= 2.0f;
		if (keys[SDL_SCANCODE_W]) cameraFocus.position += forward * velocity;
		if (keys[SDL_SCANCODE_A]) cameraFocus.position -= right * velocity;
		if (keys[SDL_SCANCODE_S]) cameraFocus.position -= forward * velocity;
		if (keys[SDL_SCANCODE_D]) cameraFocus.position += right * velocity;
		renderer.cam.focus = &cameraFocus;

		gameplayUpdate(&gameplay, &renderer, (float)deltaTime);
		rendererUpdate(&renderer, (float)deltaTime);

		if (physicsOn)
		{
			physicsUpdate(&cameraFocus.position, physicsData);
		}
		
		renderScene(&renderer);
		renderUI(&renderer, &ui);
		
		SDL_GL_SwapWindow(window.window);

		running = running && !keys[SDL_SCANCODE_ESCAPE];
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
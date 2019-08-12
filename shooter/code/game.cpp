#include "game.h"

#include <SDL/SDL.h>
#include <glad.c>
#include <stdio.h>

#include <glm/gtc/type_ptr.hpp>

#include "renderer.h"
#include "font.h"

#define pi32 3.14159265359f

struct Window
{
	SDL_Window* window;
	SDL_GLContext glContext;
};

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
	
	Window window;
	windowInitialize(&window, "shooter", 1280, 720);

	Renderer renderer;
	rendererInitialize(&renderer);

	Font font = fontCreate("font.ttf");

	double lastTime = (double)SDL_GetTicks() / 1000.0;
	double deltaTime = 0.0;

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
				break;
			}
			}
		}
		glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(renderer.textShader);
		glUniformMatrix4fv(glGetUniformLocation(renderer.textShader, "projection"), 1, GL_FALSE, glm::value_ptr(renderer.projection));
		textRender(&font, "HELLO", 10.0f, 10.0f, 1.0f);

		static const glm::vec3 FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);
		static const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
		static const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);

		rot += pi32 * deltaTime;
		renderer.cube.rotation = glm::angleAxis(rot, glm::vec3(0, 1, 0));

		float sensitivity = 0.01f;
		float max = pi32 * 0.4f;
		float min = -max;

		renderer.cam.yaw += (float)dx * sensitivity;
		renderer.cam.pitch += (float)dy * sensitivity;
		if (renderer.cam.pitch < min) renderer.cam.pitch = min;
		if (renderer.cam.pitch > max) renderer.cam.pitch = max;

		glm::quat qPitch = glm::angleAxis(renderer.cam.pitch, glm::vec3(1, 0, 0));
		glm::quat qYaw = glm::angleAxis(renderer.cam.yaw, glm::vec3(0, 1, 0));
		glm::quat qRotate = glm::normalize(qPitch * qYaw);

		cameraFocus.rotation = qRotate;

		glm::vec3 forward = glm::normalize(glm::inverse(qRotate) * FORWARD);
		glm::vec3 right = glm::normalize(glm::cross(forward, UP));

		const unsigned char *keys = SDL_GetKeyboardState(0);
		float velocity = 0.1f;
		if (keys[SDL_SCANCODE_W]) cameraFocus.position += forward * velocity;
		if (keys[SDL_SCANCODE_A]) cameraFocus.position -= right * velocity;
		if (keys[SDL_SCANCODE_S]) cameraFocus.position -= forward * velocity;
		if (keys[SDL_SCANCODE_D]) cameraFocus.position += right * velocity;
		renderer.cam.focus = &cameraFocus;


		cubeRender(&renderer);
		
		SDL_GL_SwapWindow(window.window);

		running = running && !keys[SDL_SCANCODE_ESCAPE];
	}
}
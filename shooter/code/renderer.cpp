#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

#include "memory.h"
#include "importer.h"

static unsigned int shaderCreate(char *shaderCode, unsigned int shaderType, const char *shaderString)
{
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, 0);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, 0, infoLog);

		printf(
			"ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n\n",
			shaderString, infoLog);
	}
	return shader;
}

unsigned int shaderProgramCreate(const char *vertexPath, const char *fragmentPath)
{
	char *vertexCode = readEntireFile(vertexPath);
	char *fragmentCode = readEntireFile(fragmentPath);

	unsigned int vertex = shaderCreate(vertexCode, GL_VERTEX_SHADER, "VERTEX");
	unsigned int fragment = shaderCreate(fragmentCode, GL_FRAGMENT_SHADER, "FRAGMENT");

	unsigned int shader = glCreateProgram();
	glAttachShader(shader, vertex);
	glAttachShader(shader, fragment);
	glLinkProgram(shader);

	int success;
	char infoLog[512];
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader, 512, 0, infoLog);
		
		printf(
			"ERROR::PROGRAM_LINKING_ERROR of type: PROGRAM\n%s\n\n",
			infoLog);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	free(vertexCode);
	free(fragmentCode);

	return shader;
}

unsigned imageTextureCreate(const char *path)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	stbi_set_flip_vertically_on_load(true);

	int width, height, num_components;

	//char *file_data = readEntireFile(path);

	unsigned char* data = stbi_load(path, &width, &height, &num_components, 4);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,
			height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		printf("Could not load texture from path: %s\n", path);
	}

	return texture;
}

unsigned textTextureCreate(const char *path, const char *text)
{
	return 0;
}

void toonTextureCreate(Lighting* lighting)
{
	static const GLubyte toonTextureData[] =
	{
		0x44, 0x00, 0x00, 0xFF,
		0x88, 0x00, 0x00, 0xFF,
		0xCC, 0x00, 0x00, 0xFF,
		0xFF, 0x00, 0x00, 0xFF
	};

	glGenTextures(1, &lighting->toonTexture); //Generate texture
	glBindTexture(GL_TEXTURE_1D, lighting->toonTexture); //Bind texture, a 1D texture
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGB8, sizeof(toonTextureData) / 4);
	glTexSubImage1D(GL_TEXTURE_1D, 0,
		0, sizeof(toonTextureData) / 4,
		GL_RGBA, GL_UNSIGNED_BYTE,
		toonTextureData); //Pass the data
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //Conditions
}

void initializeSkyboxTextures(Renderer* renderer)
{
	glGenTextures(1, &renderer->skybox.textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, renderer->skybox.textureID);

	const char* paths[] = 
	{
		"resources/cubemaps/right.jpg",
		"resources/cubemaps/left.jpg",
		"resources/cubemaps/top.jpg",
		"resources/cubemaps/bottom.jpg",
		"resources/cubemaps/front.jpg",
		"resources/cubemaps/back.jpg"
	};

	int width, height, nrChannels;
	unsigned char *data;
	for (GLuint i = 0; i < 6; i++)
	{
		data = stbi_load(paths[i], &width, &height, &nrChannels, 0);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void initializeSkyboxMesh(Renderer* renderer)
{
	static const float skyboxVertices[] =
	{
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &renderer->skybox.vao);
	glBindVertexArray(renderer->skybox.vao);

	glGenBuffers(1, &renderer->skybox.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->skybox.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
}

void initializeThunder(Renderer* renderer)
{
	glGenVertexArrays(1, &renderer->thunderEffect.thunderVao);
	glBindVertexArray(renderer->thunderEffect.thunderVao);
	glGenBuffers(1, &renderer->thunderEffect.thunderVbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->thunderEffect.thunderVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 65, &renderer->thunderEffect.positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	renderer->thunderEffect.isActive = false;
}

void rendererRefreshThunder(Renderer* renderer)
{
	glBindBuffer(GL_ARRAY_BUFFER, renderer->thunderEffect.thunderVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 65, &renderer->thunderEffect.positions[0], GL_STATIC_DRAW);
}

void loadMeshes(Renderer* renderer)
{
	char *data = readEntireFile("resources/assets/result.bin");
	renderer->meshes[renderer->numMeshes++] = meshFromBuffer(&renderer->meshStorage, data);
	free(data);

	data = readEntireFile("resources/assets/gubbe.bin");
	renderer->meshes[renderer->numMeshes++] = meshFromBuffer(&renderer->meshStorage, data);
	free(data);

	renderer->meshes[renderer->numMeshes++] = importMesh(&renderer->meshStorage, "resources/assets/monkey.tfs");
	renderer->meshes[renderer->numMeshes++] = importMesh(&renderer->meshStorage, "resources/assets/floor.tfs");
}

unsigned int rendererAddModel(Renderer* renderer, int meshIndex, glm::vec3 startingPos)
{
	renderer->activeModels[renderer->numActiveModels].position = startingPos;
	renderer->activeModels[renderer->numActiveModels].rotation = glm::quat(glm::vec3(0));
	renderer->activeModels[renderer->numActiveModels].meshIndex = meshIndex;
	return renderer->numActiveModels++;
}

void rendererInitialize(Renderer *renderer)
{	
	renderer->primaryShader = shaderProgramCreate("resources/shaders/primary_shader.vert", "resources/shaders/primary_shader.frag");
	renderer->textShader = shaderProgramCreate("resources/shaders/text.vert", "resources/shaders/text.frag");
	renderer->thunderShader = shaderProgramCreate("resources/shaders/thunder.vert", "resources/shaders/thunder.frag");
	renderer->quadShader = shaderProgramCreate("resources/shaders/quad.vert", "resources/shaders/quad.frag");
	renderer->prim.shader = shaderProgramCreate("resources/shaders/primitive.vert", "resources/shaders/primitive.frag");
	renderer->skybox.shader = shaderProgramCreate("resources/shaders/cubemap.vert", "resources/shaders/cubemap.frag");
	renderer->octaShader = shaderProgramCreate("resources/shaders/octacube.vert", "resources/shaders/octacube.frag");
	
	renderer->cubeShader = shaderProgramCreate("resources/shaders/cube.vert", "resources/shaders/cube.frag");
	glGenVertexArrays(1, &renderer->cubeVAO);

	renderer->projection = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.01f, 1000.0f);

	renderer->numActiveModels = 0;
	renderer->numMeshes = 0;
	renderer->cam.yaw = 0.0f;
	renderer->cam.pitch = 0.0f;
	renderer->weather.sunDirection = glm::vec3(0,0,0);

	renderer->meshStorage = meshStorageCreate(1024 * 1024);

	toonTextureCreate(&renderer->lighting);

	loadMeshes(renderer);
	initializeThunder(renderer);
	initializeSkyboxTextures(renderer);
	initializeSkyboxMesh(renderer);
	fontCreate(&renderer->font, "resources/fonts/font.ttf");

	init(&renderer->prim);

	renderer->cubeTexture = imageTextureCreate("resources/textures/cube.png");
}

void rendererUpdate(Renderer* renderer, float deltaTime)
{
	static float timer = 0.f;
	timer += deltaTime * 0.5f;
	renderer->weather.sunDirection.y = glm::sin(timer);
	renderer->weather.sunDirection.x = glm::cos(timer);
}

void renderScene(Renderer *renderer, Chunk* chunks[])
{
	glUseProgram(renderer->primaryShader);
	glUniform3fv(glGetUniformLocation(renderer->primaryShader, "camPos"), 1, &renderer->cam.position[0]);

	glActiveTexture(GL_TEXTURE0);
	glActiveTexture(GL_TEXTURE1);


	glBindTexture(GL_TEXTURE_1D, renderer->lighting.toonTexture);
	glm::mat4 view = matrixView(&renderer->cam);

	glUniformMatrix4fv(glGetUniformLocation(renderer->primaryShader, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(renderer->primaryShader, "projection"), 1, GL_FALSE, &renderer->projection[0][0]);

	glBindVertexArray(renderer->meshStorage.vertexArray);

	for (unsigned int i = 0; i < renderer->numActiveModels; ++i)
	{
		glm::mat4 model = glm::mat4_cast(renderer->activeModels[i].rotation);
		model[3] = glm::vec4(renderer->activeModels[i].position, 1.0f);
		glUniformMatrix4fv(glGetUniformLocation(renderer->primaryShader, "model"), 1, GL_FALSE, &model[0][0]);
		glUniform3fv(glGetUniformLocation(renderer->primaryShader, "sunDirection"), 1, &renderer->weather.sunDirection[0]);
		glBindTexture(GL_TEXTURE_2D, renderer->cubeTexture);
		meshRender(&renderer->meshStorage, &renderer->meshes[renderer->activeModels[i].meshIndex]);
	}

	glBindVertexArray(0);

	if (renderer->thunderEffect.isActive) // Temp
	{
		glUseProgram(renderer->thunderShader);

		glBindVertexArray(renderer->thunderEffect.thunderVao);
		glUniformMatrix4fv(glGetUniformLocation(renderer->thunderShader, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(renderer->thunderShader, "projection"), 1, GL_FALSE, &renderer->projection[0][0]);
		glLineWidth(5.f);
		glDrawArrays(GL_LINES, 0, 64);
		glBindVertexArray(0);
	}
	int z = 0;
	for (int i = 0; i < 100; ++i)
	{
		glm::mat4 mvp = renderer->projection * view * glm::translate(glm::mat4(1.0f), glm::vec3(16 * (i % 10), 0.0f,z));
		z = i % 10 == 9?z+=16:z;

		glUseProgram(renderer->cubeShader);
		glBindVertexArray(renderer->cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderer->cubeTexture);
		glUniformMatrix4fv(0, 1, GL_FALSE, &mvp[0][0]);
		glUniform1i(1, 0);
		chunkRender(chunks[i]);
	}

	glDepthFunc(GL_LEQUAL);
	glUseProgram(renderer->skybox.shader);
	// ... set view and projection matrix
	glm::mat4 skyView = glm::mat4(glm::mat3(matrixView(&renderer->cam)));
	glUniformMatrix4fv(glGetUniformLocation(renderer->skybox.shader, "view"), 1, GL_FALSE, &skyView[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(renderer->skybox.shader, "projection"), 1, GL_FALSE, &renderer->projection[0][0]);
	glBindVertexArray(renderer->skybox.vao);
	glBindTexture(GL_TEXTURE_CUBE_MAP, renderer->skybox.textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
}

void renderUI(Renderer *renderer, UserInterface *ui)
{		
	Primitives *prim = &renderer->prim;
	glUseProgram(prim->shader);
	glBindVertexArray(prim->vao);
	
	glm::mat4 projection = glm::ortho(0.0f, 1280.f, 720.0f, 0.f);
	glm::mat4 m = glm::mat4(1.0f);

	for (int i = 0; i < ui->nrOfElements; ++i)
	{
		Element *e = &ui->elements[i];
		if (e->type == typeWindow)
		{			
			m[0][0] = e->w;
			m[1][1] = e->h;
			m[3] = glm::vec4(e->x, e->y, 0.0f, 1.0f);
			float color1[] = { 0.0f, 0.5f, 0.0f, 1.0f };
			drawRectangle(&(projection * m)[0][0], color1);

			m[0][0] += 10.0f;
			m[1][1] += 10.0f;
			float color2[] = { 0.0f, 0.5f, 0.5f, 1.0f };
			drawRectangle(&(projection * m)[0][0], color2);
		}	
	}

	projection = glm::ortho(0.0f, 1280.f, 0.0f, 720.f);
	glUseProgram(renderer->textShader);
	float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glUniform4fv(1, 1, color);
	glUniformMatrix4fv(0, 1, GL_FALSE, &projection[0][0]);
	textRender(&renderer->font, "hello", 100.0f, 100.0f, 1.0f);
}
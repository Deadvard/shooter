#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>

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
	char info_log[512];
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		printf(
			"ERROR::PROGRAM_LINKING_ERROR of type: PROGRAM\n%s\n\n",
			info_log);
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

	char *file_data = readEntireFile(path);

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

void rendererInitialize(Renderer *renderer)
{	
	renderer->primaryShader = shaderProgramCreate("shaders/primary_shader.vert", "shaders/primary_shader.frag");
	renderer->textShader = shaderProgramCreate("shaders/text.vert", "shaders/text.frag");

	renderer->projection = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.01f, 1000.0f);

	renderer->cube.meshIndex = 0;
	renderer->cube.position = glm::vec3(0.0f, 0.0f, -3.0f);
	renderer->cam.focus = &renderer->cube;
	renderer->cam.position = glm::vec3(0.0f);

	renderer->cam.yaw = 0.0f;
	renderer->cam.pitch = 0.0f;

	renderer->meshStorage = meshStorageCreate(1024 * 1024);

	char *data = readEntireFile("result.bin");
	renderer->meshes[renderer->cube.meshIndex] = meshFromBuffer(&renderer->meshStorage, data);
	free(data);
	
	data = readEntireFile("gubbe.bin");
	renderer->meshes[1] = meshFromBuffer(&renderer->meshStorage, data);
	free(data);

	toonTextureCreate(&renderer->lighting);

	renderer->meshes[2] = importMesh(&renderer->meshStorage, "test.obj");
}

void cubeRender(Renderer *renderer)
{
	glUseProgram(renderer->primaryShader);
	glUniform3fv(glGetUniformLocation(renderer->primaryShader, "camPos"), 1, &renderer->cam.position[0]);

	glBindTexture(GL_TEXTURE_1D, renderer->lighting.toonTexture);

	glm::mat4 model = glm::mat4_cast(renderer->cube.rotation);
	model[3] = glm::vec4(renderer->cube.position, 1.0f);

	glm::mat4 view = matrixView(&renderer->cam);

	glUniformMatrix4fv(glGetUniformLocation(renderer->primaryShader, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(renderer->primaryShader, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(renderer->primaryShader, "projection"), 1, GL_FALSE, &renderer->projection[0][0]);

	glBindVertexArray(renderer->meshStorage.vertexArray);
	meshRender(&renderer->meshStorage, &renderer->meshes[renderer->cube.meshIndex]);
	meshRender(&renderer->meshStorage, &renderer->meshes[1]);
	meshRender(&renderer->meshStorage, &renderer->meshes[2]);
	glBindVertexArray(0);
}

void modelsRender(Model *models, int length)
{

}
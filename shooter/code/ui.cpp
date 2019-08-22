#include "ui.h"

#include <stdlib.h>

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

void clear(UserInterface *ui)
{
	ui->nrOfElements = 0;
}

void uiwindow(UserInterface *ui, const char *text, float x, float y, float w, float h)
{
	Element *e = &ui->elements[ui->nrOfElements];
	e->type = typeWindow;
	e->x = x;
	e->y = y;
	e->w = w;
	e->h = h;
	e->text = text;
	e->parent = 0;
	++ui->nrOfElements;
}

unsigned button(UserInterface *ui, const char *text)
{
	Element *e = &ui->elements[ui->nrOfElements];
	
	e->type = typeButton;
	e->text = text;
	e->button = true;

	++ui->nrOfElements;
	return e->button;
}

void text(UserInterface *ui, const char *text)
{
	Element *e = &ui->elements[ui->nrOfElements];

	e->type = typeText;
	e->text = text;

	++ui->nrOfElements;
}

void inputText(UserInterface *ui, const char *text, char *buffer, int size)
{
	Element *e = &ui->elements[ui->nrOfElements];

	e->type = typeInputText;
	e->text = text;

	++ui->nrOfElements;
}

void slider(UserInterface *ui, const char *text, float *value, float min, float max)
{
	Element *e = &ui->elements[ui->nrOfElements];

	e->type = typeSlider;
	e->text = text;
	e->slider = *value;

	++ui->nrOfElements;
}

void immidiateModeInitialize(ImmidiateMode *im)
{
	im->vertices = (uiVertex *)malloc(sizeof(uiVertex) * 1024);
	im->numVertices = 0;
	im->maxVertices = 1024;

	glGenVertexArrays(1, &im->vao);
	glBindVertexArray(im->vao);
	glGenBuffers(1, &im->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, im->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uiVertex) * im->maxVertices, 0, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 2));
}

void immidiateModeTerminate(ImmidiateMode *im)
{
	glDeleteBuffers(1, &im->vbo);
	glDeleteVertexArrays(1, &im->vao);
}

void immidiateModeVertex(ImmidiateMode *im, uiVertex *vertices, unsigned int size)
{
	if ((im->numVertices + size) < im->maxVertices)
	{
		for (int i = im->numVertices; i < (im->numVertices + size); ++i)
		{
			im->vertices[i] = vertices[i];
		}
		im->numVertices += size;
	}
}

void immidiateModeDraw(ImmidiateMode *im)
{
	if (im->numVertices > 0)
	{
		glUseProgram(im->shader);
		glBindVertexArray(im->vao);
		glBindBuffer(GL_ARRAY_BUFFER, im->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uiVertex) * im->numVertices, im->vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glm::mat4 projection = glm::ortho(0.0f, 1280.f, 720.0f, 0.f);
		glUniformMatrix4fv(0, 1, GL_FALSE, &projection[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, im->numVertices);
		glBindVertexArray(0);
		glUseProgram(0);
		im->numVertices = 0;
	}
}

void uiColor(ImmidiateMode *im, float r, float g, float b, float a)
{
	im->color[0] = r;
	im->color[1] = g;
	im->color[2] = b;
	im->color[3] = a;
}

void uiWindow(ImmidiateMode *im, const char *text, float x, float y, float w, float h)
{
	uiVertex vertices[] =
	{
		{ { x, y + h     }, { im->color[0], im->color[1], im->color[2] } },
		{ { x, y         }, { im->color[0], im->color[1], im->color[2] } },
		{ { x + w, y     }, { im->color[0], im->color[1], im->color[2] } },

		{ { x, y + h     }, { im->color[0], im->color[1], im->color[2] } },
		{ { x + w, y     }, { im->color[0], im->color[1], im->color[2] } },
		{ { x + w, y + h }, { im->color[0], im->color[1], im->color[2] } }
	};

	immidiateModeVertex(im, vertices, 6);
}

void uiButton(ImmidiateMode *im, const char *text, int pressed)
{

}

void uiSlider(ImmidiateMode *im, const char *text, float value, float x, float y, float w, float h)
{
	float x1 = x + w * value;
	float w1 = w * 0.01f;

	float y2 = y + h * 0.5f;
	float h2 = h * 0.05f;
	
	uiVertex vertices[] =
	{
		{ { x1 - w1, y + h }, { im->color[0], im->color[1], im->color[2] } },
		{ { x1 - w1, y     }, { im->color[0], im->color[1], im->color[2] } },
		{ { x1 + w1, y     }, { im->color[0], im->color[1], im->color[2] } },

		{ { x1 - w1, y + h }, { im->color[0], im->color[1], im->color[2] } },
		{ { x1 + w1, y     }, { im->color[0], im->color[1], im->color[2] } },
		{ { x1 + w1, y + h }, { im->color[0], im->color[1], im->color[2] } },

		
		{ { x,     y2 + h2 }, { im->color[0], im->color[1], im->color[2] } },
		{ { x,     y2 - h2 }, { im->color[0], im->color[1], im->color[2] } },
		{ { x + w, y2 - h2 }, { im->color[0], im->color[1], im->color[2] } },

		{ { x,     y2 + h2 }, { im->color[0], im->color[1], im->color[2] } },
		{ { x + w, y2 - h2 }, { im->color[0], im->color[1], im->color[2] } },
		{ { x + w, y2 + h2 }, { im->color[0], im->color[1], im->color[2] } },
	};

	immidiateModeVertex(im, vertices, 12);
}
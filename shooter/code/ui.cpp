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
	glGenVertexArrays(1, &im->vao);
	glGenBuffers(1, &im->vbo);
	glBindVertexArray(im->vao);
	glBindBuffer(GL_ARRAY_BUFFER, im->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 5, 0, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void immidiateModeTerminate(ImmidiateMode *im)
{
	glDeleteBuffers(1, &im->vbo);
	glDeleteVertexArrays(1, &im->vao);
}

void immidiateModeBegin(ImmidiateMode *im)
{
	glDisable(GL_DEPTH_TEST);
	glUseProgram(im->shader);
	glBindVertexArray(im->vao);
	glBindBuffer(GL_ARRAY_BUFFER, im->vbo);
}

void immidiateModeEnd(ImmidiateMode *im)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
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
	uiQuad(im, x, y, w, h);
}

void uiButton(ImmidiateMode *im, const char *text, int pressed)
{

}

void uiSlider(ImmidiateMode *im, const char *text, float value, float x, float y, float w, float h)
{
	float x1 = x + w * value;
	float w1 = w * 0.05f;

	float y2 = y + h * 0.5f;
	float h2 = h * 0.05f;
	
	uiQuad(im, x1, y, w1, h);
	uiQuad(im, x, y2, w, h2);
}

void uiQuad(ImmidiateMode *im, float x, float y, float w, float h)
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
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
#include "font.h"

#include <stdio.h>
#include <stdlib.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <glad/glad.h>

#include "memory.h"

void fontCreate(Font* font, const char *path)
{
	stbtt_fontinfo stbfont;

	char *data = readEntireFile("resources/fonts/font.ttf");
	stbtt_InitFont(&stbfont, (unsigned char *)data, stbtt_GetFontOffsetForIndex((unsigned char *)data, 0));

	for (int c = 0; c < 128; ++c)
	{
		Character ch = {};
				
		unsigned char* bitmap = stbtt_GetCodepointBitmap(&stbfont, 0,
			stbtt_ScaleForPixelHeight(&stbfont, 32.0f), c, &ch.width, &ch.height, &ch.xoffset, &ch.yoffset);

		stbtt_GetCodepointHMetrics(&stbfont, c, &ch.advance, 0);
			
		stbtt_GetFontVMetrics(&stbfont, &ch.ascent, &ch.descent, 0);

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			ch.width,
			ch.height,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			bitmap
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbtt_FreeBitmap(bitmap, 0);

		font->characters[c].texture_index = texture;
		font->characters[c].advance = ch.advance;
		font->characters[c].width = ch.width;
		font->characters[c].height = ch.height;
		font->characters[c].xoffset = ch.xoffset;
		font->characters[c].yoffset = ch.yoffset;
		font->characters[c].ascent = ch.ascent;
		font->characters[c].descent = ch.descent;
	}

	font->fontScale = stbtt_ScaleForPixelHeight(&stbfont, 32.0f);

	free(data);

	glGenVertexArrays(1, &font->vao);
	glGenBuffers(1, &font->vbo);
	glBindVertexArray(font->vao);
	glBindBuffer(GL_ARRAY_BUFFER, font->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4,
		NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void textRender(Font *f, const char *text, float x, float y, float scale)
{
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(f->vao);

	while (*text)
	{
		Character *ch = &f->characters[*text];

		float xpos = x + ch->xoffset * scale;
		float ypos = y - (ch->height - ch->yoffset) * scale + f->fontScale * ch->ascent;

		float w = ch->width * scale;
		float h = ch->height * scale;

		float vertices[6][4] =
		{
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};

		glBindTexture(GL_TEXTURE_2D, ch->texture_index);
		glBindBuffer(GL_ARRAY_BUFFER, f->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += ch->advance * scale * f->fontScale;
		++text;
	}
}
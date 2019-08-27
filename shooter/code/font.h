#ifndef FONT_H
#define FONT_H

struct Character
{
	unsigned int texture_index;
	int advance;
	int width;
	int height;
	int xoffset;
	int yoffset;
	int ascent;
	int descent;
};

struct Font
{
	float fontScale;
	unsigned int vao,vbo;
	Character characters[128];
};

void fontCreate(Font* font, const char *path);
void textRender(Font *f, const char *text, float x, float y, float scale);

#endif

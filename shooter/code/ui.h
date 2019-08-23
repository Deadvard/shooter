#ifndef UI_H
#define UI_H

typedef enum
{
	typeWindow,
	typeButton,
	typeText,
	typeInputText,
	typeSlider
} ElementType;

typedef struct
{
	ElementType type;
	float x;
	float y;
	float w;
	float h;
	const char *text;
	unsigned int parent;
	union
	{	
		unsigned button;
		float slider;
		char *inputText;
	};
} Element;

typedef struct
{
	float position[2];
	float color[3];
} uiVertex;

typedef struct
{
	int nrOfElements;
	Element elements[64];
} UserInterface;

void clear(UserInterface *ui);
void uiwindow(UserInterface *ui, const char *text, float x, float y, float w, float h);
unsigned button(UserInterface *ui, const char *text);
void text(UserInterface *ui, const char *text);
void inputText(UserInterface *ui, const char *text, char *buffer, int size);
void slider(UserInterface *ui, const char *text, float *value, float min, float max);

typedef struct
{
	unsigned int shader;
	unsigned int vao;
	unsigned int vbo;

	float color[4];
} ImmidiateMode;

typedef unsigned int WindowHandle;

void immidiateModeInitialize(ImmidiateMode *im);
void immidiateModeTerminate(ImmidiateMode *im);

void immidiateModeBegin(ImmidiateMode *im);
void immidiateModeEnd(ImmidiateMode *im);

void uiColor(ImmidiateMode *im, float r, float g, float b, float a);
void uiWindow(ImmidiateMode *im, const char *text, float x, float y, float w, float h);
void uiButton(ImmidiateMode *im, const char *text, int pressed);
void uiSlider(ImmidiateMode *im, const char *text, float value, float x, float y, float w, float h);

void uiQuad(ImmidiateMode *im, float x, float y, float w, float h);

typedef struct
{
	unsigned int shader;
	unsigned int vao;
	unsigned int vbo;
} Primitives;

void init(Primitives *im);

void drawPrimitive(float *transform, float *rgba, unsigned int first, unsigned int count);
void drawCircle(float *transform, float *rgba);
void drawRectangle(float *transform, float *rgba);
void drawTriangle(float *transform, float *rgba);


#endif


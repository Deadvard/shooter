#version 430

layout (location = 0) in vec2 position;
out vec2 uv;

uniform mat4 model;

void main()
{
	uv = position;
	gl_Position = model * vec4(position, 0.0, 1.0);
}
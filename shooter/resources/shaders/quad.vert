#version 430

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;
out vec3 colr;

uniform mat4 model;
layout(location = 0) uniform mat4 projection;

void main()
{
	colr = color;
	gl_Position = projection * vec4(position, 0.0, 1.0);
}
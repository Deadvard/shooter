#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

layout (location = 0) uniform mat4 mvp;

out vec2 texcoord;

void main()
{	
	texcoord = uv;	
	gl_Position = mvp * vec4(position, 1.0);
}
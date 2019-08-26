#version 430

layout (location = 0) in vec4 coord;

layout(location = 0) uniform mat4 mvp;

out vec4 texcoord;

void main()
{
	texcoord = coord;
	
	gl_Position = mvp * vec4(coord.xyz, 1.0);
}
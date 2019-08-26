#version 430

layout(location = 1) uniform vec4 color;
out vec4 frag;

void main()
{
	frag = color;
}
#version 430

in vec2 texcoord;
in vec3 worldnormal;

out vec4 frag;

layout(location = 1) uniform sampler2D cubeTexture;
layout(location = 2) uniform vec4 ambient;

void main()
{
	frag = ambient * vec4(texture(cubeTexture, texcoord).xyz, 1.0);
}
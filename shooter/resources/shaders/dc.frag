#version 430

in vec2 texcoord;
out vec4 frag;

layout(location = 1) uniform sampler2D cubeTexture;

void main()
{
	frag = vec4(texture(cubeTexture, texcoord).xyz, 1.0);
}
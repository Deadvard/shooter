#version 430

in vec4 texcoord;
out vec4 frag;

layout(location = 1) uniform sampler2D cubeTexture;

void main()
{
	frag = vec4(texture(cubeTexture, vec2((texcoord.x + texcoord.w) / 6.0, texcoord.y)).xyz, 1.0);
}
#version 430

in vec4 texcoord;
out vec4 frag;

void main()
{
	frag = vec4(texcoord.w / 128.0, texcoord.w / 256.0, texcoord.w / 512.0, 1.0);
}
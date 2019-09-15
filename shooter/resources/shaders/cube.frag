#version 430

in vec2 texcoord;
in vec3 worldnormal;

out vec4 frag;

layout(location = 1) uniform sampler2D cubeTexture;
layout(location = 2) uniform vec4 ambient;

void main()
{
	/*if (bool(textype & 0x8000))
	{
		uint tt = textype - 0x8000;
		frag = ambient * vec4(texture(cubeTexture, vec2((fract(texcoord.x)  + tt) / 6.0, texcoord.z)).xyz, 1.0);
	}
	else
	{
		uint tt = textype;
		frag = ambient * vec4(vec3(texture(cubeTexture, vec2((fract(texcoord.x + texcoord.z) + tt) / 6.0, texcoord.y)) * 0.85), 1.0);
	}*/

	frag = ambient * vec4(texture(cubeTexture, texcoord).xyz, 1.0);
}
#version 430

in vec3 texcoord;
flat in uint textype;

out vec4 frag;

layout(location = 1) uniform sampler2D cubeTexture;

void main()
{
	if (bool(textype & 0x8000))
	{
		uint tt = textype - 0x8000;
		frag = vec4(texture(cubeTexture, vec2((fract(texcoord.x)  + tt) / 6.0, texcoord.z)).xyz, 1.0);
	}
	else
	{
		uint tt = textype;
		frag = vec4(vec3(texture(cubeTexture, vec2((fract(texcoord.x + texcoord.z) + tt) / 6.0, texcoord.y)) * 0.85), 1.0);
	}
}
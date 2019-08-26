#version 430

in vec4 texcoord;
out vec4 frag;

layout(location = 1) uniform sampler2D cubeTexture;

void main()
{
	if (texcoord.w < 0)
	{
		frag = vec4(texture(cubeTexture, vec2((fract(texcoord.x) + texcoord.w) / 6.0, texcoord.z)).xyz, 1.0);
	}
	else
	{
		frag = vec4(texture(cubeTexture, vec2((fract(texcoord.x + texcoord.z) + texcoord.w) / 6.0, texcoord.y)).xyz, 1.0) * 0.85;
	}
	
}
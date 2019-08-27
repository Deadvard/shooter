#version 430

layout (location = 0) in uint coord;
layout (location = 1) in uint type;

layout (location = 0) uniform mat4 mvp;

out vec3 texcoord;
flat out uint textype;

void main()
{	
	uint x = (coord >> 10) & 0x1F;
	uint y = (coord >> 5 ) & 0x1F;
	uint z =  coord        & 0x1F;
	
	vec3 vcoord = vec3(x, y, z);
	texcoord = vcoord;
	textype = type;
	
	gl_Position = mvp * vec4(vcoord, 1.0);
}
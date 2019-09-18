#version 430

layout (location = 0) in uint coord;
layout (location = 1) in uint type;

layout (location = 0) uniform mat4 mvp;

out vec2 texcoord;
out vec3 worldnormal;
out vec3 worldPos;

uniform vec3 chunkPos;

void main()
{	
	uint x = (coord >> 10) & 0x1F;
	uint y = (coord >> 5 ) & 0x1F;
	uint z =  coord        & 0x1F;	
	uint textype = (type >> 4);

	if (bool(type & 0x2))
	{
		texcoord = vec2((
			float(x % 2)  + textype) / 11.0, 
			z % 2);
	}
	else
	{
		texcoord = vec2((
			float((x + z) % 2) + textype) / 11.0, 
			y % 2);
	}

	vec3 normal = vec3(type & 0x1, (type & 0x2) >> 1, (type & 0x4) >> 2);
	if (bool(type & 0x8))
	{
		normal = -normal;
	}
	worldnormal = normal;

	vec3 vcoord = vec3(x, y, z);
	worldPos = chunkPos + vcoord;
	gl_Position = mvp * vec4(vcoord, 1.0);
}
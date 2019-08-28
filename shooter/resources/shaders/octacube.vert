#version 430

struct Cube
{
	uint type;
	uint vertices;
	uint children;
};

layout(std430, binding = 0) buffer octree
{
	Cube cubes[];
};

layout (location = 0) uniform mat4 mvp;

void main()
{	
	vec3 position = vec3(0.0, 0.0, 0.0);
	float scale = 1.0;
	uint index = 0;
	uint vertices = gl_VertexID;

	while (cubes[index].children != 0)
	{	
		for (uint i = 0; i < 8; ++i)
		{
			float fx = i % 2;
			float fy = (i / 2) % 2;
			float fz = i / (2 * 2);
			uint childVertices = cubes[cubes[index].children + i].vertices;
			if (vertices >= childVertices)
			{
				vertices -= childVertices;
			}
			else
			{
				position += vec3(fx, fy, fz) * scale;
				scale *= 0.5;
				index = i;
				break;
			}
		}
	}
	
	gl_Position = mvp * vec4(position, 1.0);
}
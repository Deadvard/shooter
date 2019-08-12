#version 430
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

out VS_OUT
{
	vec3 normal;
	vec3 world_pos;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 mv_position = view * model * vec4(position,1.0);

	vs_out.normal = mat3(model) * normal;
	vs_out.world_pos = mat3(model) * position;

	gl_Position = projection * mv_position;
}
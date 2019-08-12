#version 430

out vec4 frag_color;

in VS_OUT
{
	vec3 normal;
	vec3 world_pos;
} fs_in;

uniform mat4 view;
uniform vec3 camPos;

uniform sampler1D toonTexture;

uniform vec3 light_pos = vec3(0.0, 0.0, 5.0);

void main()
{	
	// Calculate per-pixel normal and light vector
    vec3 N = normalize(fs_in.normal);
    vec3 L = normalize(light_pos - fs_in.world_pos);
	// Simple N dot L diffuse lighting
    float tc = pow(max(0.0, dot(N, L)), 5.0);

	frag_color = texture(toonTexture, tc) * (tc * 0.8 + 0.2);
}
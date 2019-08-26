#version 430

out vec4 frag_color;

in VS_OUT
{
	vec3 normal;
	vec3 worldPos;
	vec2 texCoords;
} fs_in;

uniform mat4 view;
uniform vec3 camPos;

layout (binding = 0) uniform sampler1D toonTexture;
layout (binding = 1) uniform sampler2D tex;

uniform vec3 sunDirection;

void main()
{
	vec3 N = normalize(fs_in.normal);
	vec3 L = normalize(sunDirection);

	float pc = pow(max(0.0, dot(N, L)), 5.0);

	vec4 tempAmbient = texture(tex, fs_in.texCoords) * 0.3;

	vec4 color = texture(tex, fs_in.texCoords) * texture(toonTexture, pc) * (pc * 0.8 + 0.2);

	color += tempAmbient;

	frag_color = color;
}
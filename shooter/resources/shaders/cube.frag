#version 430

in vec2 texcoord;
in vec3 worldnormal;
in vec3 worldPos;

out vec4 frag;

uniform vec3 camPos;

layout(location = 1) uniform sampler2D cubeTexture;
layout(location = 2) uniform vec4 ambient;

void main()
{
	vec3 lightPos = vec3(0,40,0);
	vec3 n = normalize(worldnormal);
	vec3 lightDir = normalize(lightPos - worldPos);
	float diff = max(dot(n, lightDir), 0.0);

	vec3 diffuse = diff * vec3(1,0,1);

	frag = (ambient + vec4(diffuse.xyz, 0)) * vec4(texture(cubeTexture, texcoord).xyz, 1.0);
}
#version 430

out vec4 frag_color;

in VS_OUT
{
	vec3 normal;
	vec3 worldPos;
} fs_in;

uniform mat4 view;
uniform vec3 camPos;

layout (binding = 0) uniform sampler1D toonTexture;

uniform vec3 lightPos = vec3(0.0, 10.0, 10.0);

uniform vec3 dirLight = vec3(0.3, 10, 0);

uniform vec3 sunDirection;

void main()
{
	float tc;
	{
		// Calculate per-pixel normal and light vector
		 vec3 N = normalize(fs_in.normal);
    vec3 L = normalize(lightPos - fs_in.worldPos);
	// Simple N dot L diffuse lighting
    tc = pow(max(0.0, dot(N, L)), 5.0);
	}

	float pc;
	{
	// Calculate per-pixel normal and light vector
	vec3 N = normalize(fs_in.normal);
    vec3 L = normalize(-sunDirection);
	// Simple N dot L diffuse lighting
	
    pc = max(dot(N, L), 0.0);
    //pc = pow(max(0.0, dot(N, L)), 10.0);
	}

	//vec4 color = texture(toonTexture, tc) * (tc * 0.8 + 0.2);
	vec4 color = texture(toonTexture, pc) * (pc * 0.8 + 0.2);

	frag_color = color;
}
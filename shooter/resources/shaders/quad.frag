#version 430

in vec3 colr;
out vec4 frag_color;

uniform sampler2D texture1;

void main()
{
	frag_color = vec4(colr, 1.0); //vec4(texture(texture1, uv).r);
}
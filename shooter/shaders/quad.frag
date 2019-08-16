#version 430

in vec2 uv;
out vec4 frag_color;

uniform sampler2D texture1;

void main()
{
	frag_color = vec4(1,1,0,1);//vec4(texture(texture1, uv).r);
}
#version 430
out vec4 frag;

uniform vec2 resolution;

void main()
{
	if (abs(gl_FragCoord.x - 1280 / 2) < 5 && abs(gl_FragCoord.y - 720 / 2) < 5)
	{
		frag = vec4(0.0,0.0,0.0,1.0);
	}
}
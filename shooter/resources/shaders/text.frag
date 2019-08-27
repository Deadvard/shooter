#version 430

in vec2 tex_coord;

out vec4 color;

uniform sampler2D text;
layout (location = 1) uniform vec4 textColor;

void main()
{
    color = vec4(texture(text, tex_coord).r) * textColor;
} 
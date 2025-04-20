#version 330 core

layout (location = 0) out vec4 color;

in vec2 col;

void main()
{
	color = vec4(col,0,1);
}
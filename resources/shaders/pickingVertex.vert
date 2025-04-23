
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;
uniform mat4 PVM;

uniform float scale;
uniform vec3 modelPos;

out vec2 col;
out vec2 texCoords;

void main()
{
	gl_Position = PVM * vec4(modelPos + (aPos * scale), 1);
	texCoords = aTex;
	col = texCoords;

}
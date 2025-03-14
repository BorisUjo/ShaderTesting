#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float scale;

out vec2 texCoords;

void main()
{
	gl_Position = projection * view * model * vec4(aPos * scale, 1);
	texCoords = aTex;

}
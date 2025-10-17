#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 PVM;

void main()
{
	gl_Position = PVM * vec4(aPos, 1);

}
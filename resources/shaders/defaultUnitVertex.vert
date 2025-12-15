#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

uniform mat4 PVM;

out vec2 texCoord;

void main()
{
    gl_Position = PVM * vec4(aPos, 1.0);
    texCoord = aTex;
}
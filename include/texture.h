#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
struct Texture
{
	GLuint textureID;

	bool initialise(const char* path);
};
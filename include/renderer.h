#pragma once
#include <scene.h>
#include <glad/glad.h>
#include <openglDebug.h>
#include <GLFW/glfw3.h>
class Renderer
{
public:
	Renderer(GLFWwindow* window);
	~Renderer();

	void render(Scene& scene);

private:
	GLFWwindow* window;

};
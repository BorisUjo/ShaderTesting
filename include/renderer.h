#pragma once
#include <scene.h>
#include <glad/glad.h>
#include <openglDebug.h>
#include <GLFW/glfw3.h>
#include <pickingTexture.h>
class Renderer
{
public:
	Renderer();
	~Renderer();

	void init(GLFWwindow* window);
	void render(Scene& scene);
	void set_camera(Camera* camera);

private:
	GLFWwindow* window = nullptr;
	Camera* camera = nullptr;
	PickingTexture picker;
};
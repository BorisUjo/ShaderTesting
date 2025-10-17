#pragma once
#include <scene.h>
#include <glad/glad.h>
#include <openglDebug.h>
#include <GLFW/glfw3.h>
#include <pickingTexture.h>
#include <imgui.h>
class Renderer
{
public:
	Renderer();
	~Renderer();

	void init(GLFWwindow* window);
	void init_imgui(GLFWwindow* window, ImGuiIO& output);
	void render(Scene& scene);
	void render_ui(ImGuiIO& io);
	void set_camera(Camera* camera);

private:
	GLFWwindow* window = nullptr;
	Camera* camera = nullptr;
	PickingTexture picker;
};
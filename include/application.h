#pragma once
#include <renderer.h>
#include <scene.h>
#include <camera.h>
#include <iostream>
#include <input.h>
class Application
{
public:
	Application(int width, int height, const char* title);
	~Application();

	void run();

private:
	GLFWwindow* window = nullptr;
	Scene scene;
	Renderer renderer;
	InputManager inputManager;
	bool initialise(int width, int height, const char* title);
};
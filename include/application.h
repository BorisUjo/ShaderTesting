#pragma once
#include <renderer.h>
#include <scene.h>
#include <camera.h>
#include <iostream>
class Application
{
public:
	Application(int width, int height, const char* title);
	~Application();

	void run();

private:
	GLFWwindow* window;
	Renderer renderer;
	Scene scene;

	bool initialise(int width, int height, const char* title);
	void process_input(float deltaTime);
};
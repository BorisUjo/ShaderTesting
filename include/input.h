#pragma once
#include <iostream>
#include <unordered_map>
#include <functional>
#include <GLFW/glfw3.h>
class InputManager
{
public:

	void bindKey(int key, std::function<void()> action);
	void handleInput(GLFWwindow* window);

private:
	std::unordered_map<int, std::function<void()>> keyBindings;
	int previousKeyState = -1;
};
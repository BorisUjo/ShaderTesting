#include <input.h>


void InputManager::bindKey(int key, std::function<void()> action)
{
	keyBindings[key] = action;
}

void InputManager::handleInput(GLFWwindow* window)
{
	for (auto& pair : keyBindings)
	{
		if (glfwGetKey(window, pair.first) == GLFW_PRESS)
		{
			pair.second();
		}
	}
}

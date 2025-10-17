#include <input.h>

void InputManager::bindKey(int key, std::function<void()> action)
{
	keyBindings[key] = action;
}

void InputManager::bindKey(KeyCode key)
{
	keys[key.id] = key;
}

void InputManager::bindMouseKey(KeyCode key)
{
	mouseKeys[key.id] = key;
}

void InputManager::handleInput(GLFWwindow* window)
{

	int currentKeyState = 0;

	for (auto& pair : keyBindings)
	{
		currentKeyState = glfwGetKey(window, pair.first);

		if (currentKeyState == GLFW_PRESS && previousKeyState != GLFW_PRESS)
		{
			pair.second();
		}
	}

	previousKeyState = currentKeyState;


}

void InputManager::handleMouseInput(GLFWwindow* window)
{
	int currentKeyState = 0;
	auto& gm = GameManager::getInstance();

	for (auto& pair : mouseKeys)
	{
		currentKeyState = glfwGetMouseButton(window, pair.first);


		if (currentKeyState != GLFW_PRESS)
		{

			if (previousHeldKey == pair.first && pair.second.mode == HELD)
			{
				currentHoldTime = 0.0f;
				previousHeldKey = -1;
			}
			else if (previousHeldKey == pair.first && pair.second.mode == SINGLE_PRESS)
			{
				pair.second.callback();
				previousHeldKey = -1;
			}

			continue;
		}

		currentHeldKey = pair.first;

		if (previousMouseKeyState == currentKeyState && pair.second.mode == HELD)
		{
			if (currentHoldTime >= maxHoldTime)
			{
				pair.second.callback();
			}
			else
			{
				currentHoldTime += gm.deltaTime();
			}
		}

		previousHeldKey = currentHeldKey;
		previousMouseKeyState = currentKeyState;

	}

}

void InputManager::unbindKeyCallback()
{
	previousKeyState = -1;
}

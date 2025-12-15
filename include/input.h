#pragma once
#include <iostream>
#include <unordered_map>
#include <functional>
#include <gameManager.h>


enum InputMode
{
	SINGLE_PRESS,
	HELD
};


struct KeyCode
{
	int id;
	InputMode mode = SINGLE_PRESS;
	std::function<void()> callback;
	KeyCode() = default;
	KeyCode(int id, std::function<void()> callback)
	{
		this->id = id;
		this->callback = callback;
	}
	KeyCode(int id, std::function<void()> callback, InputMode mode)
	{
		this->id = id;
		this->callback = callback;
		this->mode = mode;
	}
};

class InputManager
{
public:

	void bindKey(int key, std::function<void()> action);
	void bindKey(KeyCode key);
	void bindMouseKey(KeyCode key);
	void handleInput(GLFWwindow* window);
	void handleMouseInput(GLFWwindow* window);
	void unbindKeyCallback();

private:
	std::unordered_map<int, std::function<void()>> keyBindings;

	std::unordered_map<int, KeyCode> keys;
	std::unordered_map<int, KeyCode> mouseKeys;

	int previousKeyState = -1;
	int previousMouseKeyState = -1;

	int previousHeldKey = -1;
	int currentHeldKey = 0;

	float maxHoldTime = 0.5f;
	float currentHoldTime = 0.0f;

};

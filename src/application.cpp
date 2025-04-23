#include "application.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Application::Application(int width, int height, const char* title)
{
	if (!initialise(width, height, title))
	{
		throw std::runtime_error("Failed to initialise Application");
	}

	scene.load_resources();
	scene.set_camera_params(width, height);

	renderer.init(window);
	renderer.set_camera(scene.sceneCamera);

}

Application::~Application()
{
	if (window != nullptr)
	{
		glfwDestroyWindow(window);
		window = nullptr;
	}


	glfwTerminate();
}

void Application::run()
{

	float lastFrame = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		scene.debug_input(window);
		renderer.render(scene);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

bool Application::initialise(int width, int height, const char* title)
{
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!window)
	{
		return false;
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, key_callback);

	return true;
}

void Application::process_input(float deltaTime)
{

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, 1);
	}
}

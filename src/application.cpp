#include "application.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void press_B()
{
	std::cout << "Pressed B" << std::endl;
}

Application::Application(int width, int height, const char* title)
{
	if (!initialise(width, height, title))
	{
		throw std::runtime_error("Failed to initialise Application");
	}

	scene.load_resources();
	scene.initialise_camera(width, height);

	renderer.init(window);
	renderer.set_camera(scene.sceneCamera);

	inputManager.bindKey(GLFW_KEY_B, std::bind(&Scene::spawn_debug_unit, &scene));

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

	auto& gameManager = GameManager::getInstance();

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		inputManager.handleInput(window);
		// TODO: input manager here

		gameManager.setDeltaTime(deltaTime);

		renderer.render(scene);
		scene.update();

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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, 1);
	}
}

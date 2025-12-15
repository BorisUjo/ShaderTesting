#include "application.h"
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Application::Application(int width, int height, const char* title)
{
	if (!initialise(width, height, title))
	{
		throw std::runtime_error("Failed to initialise Application");
	}

	scene.initialise();
	scene.initialise_camera(width, height);

	renderer.init(window);
	renderer.set_camera(scene.sceneCamera);

	inputManager.bindMouseKey(KeyCode(GLFW_MOUSE_BUTTON_1, std::bind(&Scene::mouse_left_click, &scene), SINGLE_PRESS));
	inputManager.bindMouseKey(KeyCode(GLFW_MOUSE_BUTTON_2, std::bind(&Scene::mouse_right_click, &scene), SINGLE_PRESS));

	//TO DO: expand input manager to support single user input
	// bind key -> 1. argument Keybind(KEY, CLICK_STATE), 2. argument function



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

	ImGuiIO io;;

	renderer.init_imgui(window, io);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		inputManager.handleInput(window);
		inputManager.handleMouseInput(window);

		// TODO: input manager here

		gameManager.setDeltaTime(deltaTime);

		renderer.render(scene);
		scene.update();

		//renderer.render_ui(io);
		scene.render_imgui();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

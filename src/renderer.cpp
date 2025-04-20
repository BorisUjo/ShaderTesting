#define GLFW_INCLUDE_NONE
#define GLM_ENABLE_EXPERIMENTAL
#include <renderer.h>
#include <camera.h>
#include <openglDebug.h>
Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::init(GLFWwindow* window)
{
	this->window = window;

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_DEPTH_TEST);
	glDebugMessageCallback(glDebugOutput, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}

void Renderer::render(Scene& scene)
{	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);
	glClearColor(0.1f, 0.3f, 0.7f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->input(window);
	scene.render_map();


}

void Renderer::set_camera(Camera* camera)
{
	this->camera = camera;
}

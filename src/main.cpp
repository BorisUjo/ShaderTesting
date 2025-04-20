//#include <glad/glad.h>
//#define GLFW_INCLUDE_NONE
//#define GLM_ENABLE_EXPERIMENTAL
//#include <GLFW/glfw3.h>
//#include <openglDebug.h>
//#include <meshInformation.h>
//#include <renderObject.h>
//#include <camera.h>
//#include <iostream>
//#include <demoShaderLoader.h>
#include <application.h>
#define USE_GPU_ENGINE 0
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = USE_GPU_ENGINE;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = USE_GPU_ENGINE;
}


static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
	
	Application app(640,480, "TEST");

	app.run();

	return 0;
}

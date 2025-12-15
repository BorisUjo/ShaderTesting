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



int main(void)
{
	int factor = 10;
	//int width = 840 + (840/factor);
	int width = 1080 + (840/factor);
	int height = 680 + (680 / factor);
	Application app(width,height, "TEST");

	app.run();

	return 0;
}

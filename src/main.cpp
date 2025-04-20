#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#define GLM_ENABLE_EXPERIMENTAL
#include <GLFW/glfw3.h>
#include <openglDebug.h>
#include <meshInformation.h>
#include <renderObject.h>
#include <camera.h>
#include <iostream>
#include <demoShaderLoader.h>

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
void initialise_platforms(RenderObject* objects, MeshData& data)
{
	unsigned int objCounter = 0;

	float X_OFFSET = -0.25f;
	float Y_OFFSET = 0.5f;

	float X_STEP = 0.85f;
	float Y_STEP = 0.75f;

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			objects[objCounter].initialise(data);
			objects[objCounter].position = glm::vec3((x*X_STEP) + X_OFFSET, 0, (-y*Y_STEP) + Y_OFFSET);
			objects[objCounter].scale = 0.8f;
			objCounter++;
		}
	}
}

int main(void)
{

	if (!glfwInit())
		return -1;

	 
#pragma region report opengl errors to std
	//enable opengl debugging output.
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#pragma endregion


	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //you might want to do this when testing the game for shipping


	GLFWwindow *window = window = glfwCreateWindow(640, 480, "TEST", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	
	

#pragma region report opengl errors to std
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_DEPTH_TEST);
	glDebugMessageCallback(glDebugOutput, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#pragma endregion

	const std::streamsize MODEL_LOAD_BUFFER_SIZE = 65536;
	const unsigned int MAP_PLATFORM_SIZE = 64;

	Camera camera(640.0f, 480.0f);

	Shader default_shader;

	MeshData mapMesh;
	MeshData mapPlatformMesh;

	RenderObject mapRO;
	RenderObject platformObjects[MAP_PLATFORM_SIZE];



	loadModelFromFile(MODEL_LOAD_BUFFER_SIZE, RESOURCES_PATH "models/ROMAN_BATTLEFIELD.obj", mapMesh);
	loadModelFromFile(MODEL_LOAD_BUFFER_SIZE, RESOURCES_PATH "models/Pillar.obj", mapPlatformMesh);

	default_shader.loadShaderProgramFromFile(RESOURCES_PATH "shaders/vertex.vert", RESOURCES_PATH "shaders/fragment.frag");

		
	mapRO.initialise(mapMesh);
	initialise_platforms(platformObjects, mapPlatformMesh);

	while (!glfwWindowShouldClose(window))
	{
		int width = 0, height = 0;

		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.3f, 0.7f, 1.0f);

		camera.input(window);

		parse_shader_info(default_shader, mapRO, camera);

		mapRO.bind(GL_FILL);

		for (int i = 0; i < MAP_PLATFORM_SIZE; i++)
		{
			parse_shader_info(default_shader, platformObjects[i], camera);
			platformObjects[i].bind(GL_FILL);
		}




		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
#include <demoShaderLoader.h>
#include <pickingTexture.h>
#include <gameObject.h>
class Camera
{
public:
	glm::vec3 Position = glm::vec3(0,0,0);
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);


	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	glm::vec3 cameraPosition = glm::vec3(0.0f,-0.0f,-5.0f); 

	PickingTexture::PixelInfo pixelInfo;
	
	int selectedObjectID = -1;
	int previousSelectedID = -2;



private:
	float FOV = 45.0f;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;

	float window_width = 0.0f;
	float window_height = 0.0f;

	float speed = 0.075f;
	float sensitivity = 100.0f;


	bool mouseDragging = false;
	bool firstClick = true;
	bool interactMode = false;
	int previousKey = GLFW_RELEASE;

public:
	double cursorPosX = 0.0;
	double cursorPosY = 0.0;

public:

	Camera(float width, float height) : window_width(width), window_height(height)
	{
		//Position = glm::vec3(12.981f, 30.3077f, 29.4453f);
		//Orientation = glm::vec3(0.0f, -0.60481f, -0.795468f);

		Position = glm::vec3(50.882f, 49.7382f,  6.5838f);
		Orientation = glm::vec3(-0.643032f, -0.764899f, -0.0007f);

	}

	
	GameObject* get_selected_object();
	void free_move(GLFWwindow* window);
	void input(GLFWwindow* window);
	void matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
	glm::mat4 get_matrix();

	glm::mat4 get_PV_static();

	inline void setCursorPos(GLFWwindow* window)
	{
		glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
	}
	template<typename T, typename Base>
	T* get_gameObject(Base* base);

};
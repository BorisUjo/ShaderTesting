#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <meshInformation.h>

struct RenderObject
{
	GLuint vaoID;
	GLuint meshIndicesSize = 0;

	glm::vec3 position = glm::vec3(0,0,0);

	float scale = 1.0f;

	void test_initialise();
	void initialise(MeshData& mesh);

	void bind();
	void bind(GLenum render_mode);

};

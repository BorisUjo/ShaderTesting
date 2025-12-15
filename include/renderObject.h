#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <meshInformation.h>
#include <texture.h>
struct RenderObject
{
	GLuint vaoID = 0;
	GLuint meshIndicesSize = 0;
	GLuint meshID = 0;
	GLuint textureID = 0;

	glm::vec3 position = glm::vec3(0,0,0);
	glm::vec3 rotation = glm::vec3(0, 1, 0);

	float scale = 1.0f;
	float rotationAngle = 180.0f;

	void test_initialise();
	void initialise(MeshData& mesh);

	void bind();
	void bind(GLenum render_mode);
	void assign_texture(const GLuint id);
	void assign_texture(const Texture& texture);
	void bind_texture();
};

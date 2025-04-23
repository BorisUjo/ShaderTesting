#pragma once
#pragma once
#include <glad/glad.h>
#include <renderObject.h>
#include <iostream>
#include <fstream>

extern class Camera;

class Shader
{
public:
	GLuint id = 0;

	bool loadShaderProgramFromData(const char *vertexShaderData, const char *fragmentShaderData);
	bool loadShaderProgramFromData(const char *vertexShaderData,
		const char *geometryShaderData, const char *fragmentShaderData);

	bool loadShaderProgramFromFile(const char *vertexShader, const char *fragmentShader);
	bool loadShaderProgramFromFile(const char *vertexShader,
		const char *geometryShader, const char *fragmentShader);

	void bind() const;

	void clear();

	GLint getUniform(const char *name);
};

GLint getUniform(GLuint shaderId, const char *name);

void parse_shader_info(Shader& shader, RenderObject& obj, Camera& camera);
void parse_picking_shader_info(Shader& shader, RenderObject& obj, Camera& camera);


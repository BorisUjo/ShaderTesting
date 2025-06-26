#include <demoShaderLoader.h>
#include <pickingTexture.h>
#include <camera.h>
//path is used for error reporting
GLint createShaderFromData(const char *data, GLenum shaderType, const char *path = 0)
{
	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &data, nullptr);
	glCompileShader(shaderId);

	GLint rezult = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &rezult);

	if (!rezult)
	{
		char *message = 0;
		int   l = 0;

		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &l);

		if (l)
		{
			message = new char[l];

			glGetShaderInfoLog(shaderId, l, &l, message);

			message[l - 1] = 0;

			std::cout << "error compiling shader: ";
			if (path) { std::cout << path; }
			std::cout << "\n" << message << "\n";
			delete[] message;
		}
		else
		{
			if (path) { std::cout << path << " "; }
			std::cout<< "unknown error while compiling shader :(\n";
		}

		glDeleteShader(shaderId);

		shaderId = 0;
		return shaderId;
	}

	return shaderId;
}


GLint createShaderFromFile(const char *name, GLenum shaderType)
{
	std::ifstream f(name);
	std::string str;

	if (!f.is_open())
	{
		std::cout << "Error opening file: " + std::string(name) << "\n";
		return 0;
	}

	f.seekg(0, std::ios::end);
	str.reserve(f.tellg());
	f.seekg(0, std::ios::beg);

	if (str.capacity() <= 0) 
	{
		std::cout << "Error opening file: " + std::string(name) << "\n";
		return 0; 
	}

	str.assign((std::istreambuf_iterator<char>(f)),
		std::istreambuf_iterator<char>());

	
	auto rez = createShaderFromData(str.c_str(), shaderType, name);

	return rez;
}

bool Shader::loadShaderProgramFromData(const char *vertexShaderData, const char *fragmentShaderData)
{
	auto vertexId = createShaderFromData(vertexShaderData, GL_VERTEX_SHADER);
	auto fragmentId = createShaderFromData(fragmentShaderData, GL_FRAGMENT_SHADER);

	if (vertexId == 0 || fragmentId == 0)
	{
		return 0;
	}

	id = glCreateProgram();

	glAttachShader(id, vertexId);
	glAttachShader(id, fragmentId);

	glLinkProgram(id);

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);

	GLint info = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &info);

	if (info != GL_TRUE)
	{
		char *message = 0;
		int   l = 0;

		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &l);

		message = new char[l];

		glGetProgramInfoLog(id, l, &l, message);

		std::cout << std::string("Link error: ") + message << "\n";

		delete[] message;

		glDeleteProgram(id);
		id = 0;
		return 0;
	}

	glValidateProgram(id);

	return true;
}

bool Shader::loadShaderProgramFromData(const char *vertexShaderData, const char *geometryShaderData, const char *fragmentShaderData)
{
	auto vertexId = createShaderFromData(vertexShaderData, GL_VERTEX_SHADER);
	auto geometryId = createShaderFromData(geometryShaderData, GL_GEOMETRY_SHADER);
	auto fragmentId = createShaderFromData(fragmentShaderData, GL_FRAGMENT_SHADER);

	if (vertexId == 0 || fragmentId == 0 || geometryId == 0)
	{
		return 0;
	}

	id = glCreateProgram();

	glAttachShader(id, vertexId);
	glAttachShader(id, geometryId);
	glAttachShader(id, fragmentId);

	glLinkProgram(id);

	glDeleteShader(vertexId);
	glDeleteShader(geometryId);
	glDeleteShader(fragmentId);

	GLint info = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &info);

	if (info != GL_TRUE)
	{
		char *message = 0;
		int   l = 0;

		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &l);

		message = new char[l];

		glGetProgramInfoLog(id, l, &l, message);

		std::cout << std::string("Link error: ") + message << "\n";

		delete[] message;

		glDeleteProgram(id);
		id = 0;
		return 0;
	}

	glValidateProgram(id);

	return true;
}

bool Shader::loadShaderProgramFromFile(const char *vertexShader, const char *fragmentShader)
{

	auto vertexId = createShaderFromFile(vertexShader, GL_VERTEX_SHADER);
	auto fragmentId = createShaderFromFile(fragmentShader, GL_FRAGMENT_SHADER);


	if (vertexId == 0 || fragmentId == 0)
	{
		return 0;
	}

	id = glCreateProgram();

	glAttachShader(id, vertexId);
	glAttachShader(id, fragmentId);

	glLinkProgram(id);

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);

	GLint info = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &info);

	if (info != GL_TRUE)
	{
		char *message = 0;
		int   l = 0;

		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &l);

		message = new char[l];

		glGetProgramInfoLog(id, l, &l, message);

		std::cout << std::string("Link error: ") + message << "\n";

		delete[] message;

		glDeleteProgram(id);
		id = 0;
		return 0;
	}

	glValidateProgram(id);

	return true;
}

bool Shader::loadShaderProgramFromFile(const char *vertexShader, const char *geometryShader, const char *fragmentShader)
{
	auto vertexId = createShaderFromFile(vertexShader, GL_VERTEX_SHADER);
	auto geometryId = createShaderFromFile(geometryShader, GL_GEOMETRY_SHADER);
	auto fragmentId = createShaderFromFile(fragmentShader, GL_FRAGMENT_SHADER);

	if (vertexId == 0 || fragmentId == 0 || geometryId == 0)
	{
		return 0;
	}

	id = glCreateProgram();

	glAttachShader(id, vertexId);
	glAttachShader(id, geometryId);
	glAttachShader(id, fragmentId);

	glLinkProgram(id);

	glDeleteShader(vertexId);
	glDeleteShader(geometryId);
	glDeleteShader(fragmentId);

	GLint info = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &info);

	if (info != GL_TRUE)
	{
		char *message = 0;
		int   l = 0;

		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &l);

		message = new char[l];

		glGetProgramInfoLog(id, l, &l, message);

		std::cout << std::string("Link error: ") + message << "\n";

		delete[] message;

		glDeleteProgram(id);
		id = 0;
		return 0;
	}

	glValidateProgram(id);

	return true;
}

void Shader::bind() const
{
	glUseProgram(id);
}

void Shader::clear()
{
	glDeleteProgram(id);
	id = 0;
}

GLint Shader::getUniform(const char *name)
{
	return ::getUniform(this->id, name);
}

GLint getUniform(GLuint shaderId, const char *name)
{
	GLint uniform = glGetUniformLocation(shaderId, name);
	if (uniform == -1)
	{
		std::cout << "uniform error " + std::string(name);
	}
	return uniform;
}

void parse_shader_info(Shader& shader, RenderObject& obj, Camera& camera)
{
	shader.bind();

	if (camera.selectedObjectID == obj.meshID)
	{
		//std::cout << "SELECTED OBJ: " << obj.objectID << std::endl;
	}

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, obj.position);

	glUniformMatrix4fv(shader.getUniform("PVM"), 1, false, glm::value_ptr(camera.get_matrix() * model));
	glUniform3f(shader.getUniform("modelPos"), obj.position.x, obj.position.y, obj.position.z);
	glUniform1f(shader.getUniform("scale"), obj.scale);

}

void parse_picking_shader_info(Shader& shader, RenderObject& obj, Camera& camera)
{
	shader.bind();

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, obj.position);

	glUniformMatrix4fv(shader.getUniform("PVM"), 1, false, glm::value_ptr(camera.get_matrix() * model));
	glUniform3f(shader.getUniform("modelPos"), obj.position.x, obj.position.y, obj.position.z);
	glUniform1f(shader.getUniform("scale"), obj.scale);

	glUniform1ui(shader.getUniform("objectIndex"), obj.meshID);
	glUniform1ui(shader.getUniform("drawIndex"), obj.meshID);
}


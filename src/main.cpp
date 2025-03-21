#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include <GLFW/glfw3.h>
#include <openglDebug.h>
#include <demoShaderLoader.h>
#include <iostream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <sstream>

#define uint unsigned int
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
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

struct Vec3
{
	float x = 0, y = 0, z = 0;
	Vec3() = default;
	Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Vec2
{
	float x = 0, y = 0;

	Vec2() = default;
	Vec2(float x, float y) : x(x), y(y) {}

};

struct Vertex
{
	Vec3 position;
	Vec3 normal;
	Vec2 uv;
};

struct MeshData
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
};

struct Face 
{
	std::vector<int> vertexIndices;
	std::vector<int> normalIndices;
	std::vector<int> texCoordIndices;
};

struct Mesh
{

	MeshData meshData;

	bool initialise(const char* file)
	{		

		parse_mesh(file, meshData);

		return true;

	}

	bool parse_mesh(const char* filepath, MeshData& result)
	{

		std::cout << "Parsing mesh" << std::endl;

		std::ifstream file(filepath);

		if (!file.is_open())
		{
			std::cout << "Failed to open file: " << filepath << std::endl;
			return false;
		}

		std::vector<Vec3> vertices;
		std::vector<Vec3> normals;
		std::vector<Vec2> uvs;
		std::vector<Face> faces;

		std::string line;
		while (std::getline(file, line))
		{
			std::istringstream iss(line);

			std::string prefix;
			iss >> prefix;

			//std::cout << prefix << std::endl;

			if (prefix == "f")
			{
				Face face;
				std::string faceInfo;

				while (iss >> faceInfo)
				{
					std::istringstream vertexStream(faceInfo);
					std::string vStr, vtStr, vnStr;

					std::getline(vertexStream, vStr, '/');
					std::getline(vertexStream, vtStr, '/');
					std::getline(vertexStream, vnStr, '/');

					if (!vStr.empty()) {
						int vIndex = std::stoi(vStr) - 1; 
						face.vertexIndices.push_back(vIndex);
					}
					if (!vnStr.empty())
					{
						int vnIndex = std::stoi(vnStr) - 1;
						face.normalIndices.push_back(vnIndex);

					}

					if (!vtStr.empty())
					{
						int vtIndex = std::stoi(vtStr) - 1;
						face.texCoordIndices.push_back(vtIndex);
					}
				}

				faces.push_back(face);


			}
			else if (prefix == "v")
			{
				float tempx;
				float tempy;
				float tempz;

				(void)sscanf(line.c_str(), "v %f %f %f", &tempx, &tempy, &tempz);
				vertices.push_back(Vec3(tempx, tempy, tempz));
			}
			else if (prefix == "vn")
			{
				float tempx;
				float tempy;
				float tempz;

				(void)sscanf(line.c_str(), "vn %f %f %f", &tempx, &tempy, &tempz);
				normals.push_back( Vec3(tempx, tempy, tempz));
			}
			else if (prefix == "vt")
			{
				float tempx;
				float tempy;

				(void)sscanf(line.c_str(), "vt %f %f ", &tempx, &tempy);
				uvs.push_back(Vec2(tempx, tempy));
			}
			
		}

		size_t vertSize = vertices.size();

		/*for (size_t i = 0; i < vertSize; i++)
		{
			Vertex vert;
			vert.position = vertices[i];
			result.vertices.push_back(vert);
		}


		for (size_t i = 0; i < faces.size(); i++)
		{
			for (size_t j = 0; j < faces[i].vertexIndices.size(); j++)
			{
				GLuint index = faces[i].vertexIndices[j];
				result.indices.push_back(index);

				if (index < result.vertices.size())
				{
					if (j < faces[i].normalIndices.size() && faces[i].normalIndices[j] >= 0)
						result.vertices[index].normal = normals[faces[i].normalIndices[j]];

					if (j < faces[i].texCoordIndices.size() && faces[i].texCoordIndices[j] >= 0)
						result.vertices[index].uv = uvs[faces[i].texCoordIndices[j]];
				}


			}
		}*/

		for (size_t i = 0; i < faces.size(); i++)
		{
			for (size_t j = 0; j < faces[i].vertexIndices.size(); j++)
			{
				Vertex vertex;
				vertex.position = vertices[faces[i].vertexIndices[j]];
				vertex.normal = normals[faces[i].normalIndices[j]];
				vertex.uv = uvs[faces[i].texCoordIndices[j]];

				result.vertices.push_back(vertex);

			}
		}

		
		return true;

	}

};

struct Texture
{
	GLuint textureID;


	bool initialise(const char* path)
	{
		int width = 0;
		int height = 0;
		int comp = 0;
		
		stbi_set_flip_vertically_on_load(true);

		unsigned char* data = stbi_load(path, &width, &height, &comp, 0);

		if (!data)
		{
			std::cout << "Failed to load image from file: "  << path << std::endl;
			return false;
		}

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

		return true;
	}

	void bind()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}


};

struct RenderObject
{

	GLuint VAO = 0;
	GLuint indicesSize = 0;

	// test rect render
	bool test_initialise()
	{
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		GLuint vbo;

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, false, 5*(sizeof(float)), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, 5*(sizeof(float)), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);

		return true;

	}

	bool initialise(MeshData& mesh)
	{
		std::cout << "Initialising Mesh" << std::endl;
		GLuint vbo;
		GLuint ebo;

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() *sizeof(Vertex), mesh.vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//glGenBuffers(1, &ebo);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(GLuint), mesh.indices.data(), GL_STATIC_DRAW);


		glBindVertexArray(0);

		indicesSize = mesh.vertices.size();

		return true;
	}
	

	void bind()
	{
		//glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0); // deprecated
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, indicesSize);
	}

	void bind(GLenum render_mode)
	{
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, render_mode);
		glDrawArrays(GL_TRIANGLES, 0, indicesSize);
	}

};

struct PickingTexture
{
	GLuint framebuffer;
	GLuint pickingTexture;
	GLuint depthTexture;

	struct PixelInfo
	{
		uint objectID = 0;
		uint drawID = 0;
		uint primID = 0;
	};

	bool initialise(int width, int height)
	{
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glGenTextures(1, &pickingTexture);
		glBindTexture(GL_TEXTURE_2D, pickingTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, width, height, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pickingTexture, 0);

		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Error initialising framebuffer";
			return false;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		return true;
	}

	PixelInfo read_pixel(int x, int y)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		PixelInfo pixel;
		GLuint data[3];
		glReadPixels(x, y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, data);

		pixel.objectID = data[0];
		pixel.drawID = data[1];
		pixel.primID = data[2];

		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);


		return pixel;
	}

	void enable_writing()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
	}

	void disable_writing()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
};

struct MouseState
{
	bool isPressed = false;

	double x = 0;
	double y = 0;
};

int main(void)
{

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);



	GLFWwindow* window = window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TEST", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	Mesh mesh;

	RenderObject suzanne;
	Texture texture;

	mesh.initialise(RESOURCES_PATH "Monkey_UV.obj");
	texture.initialise(RESOURCES_PATH "Suzanne.png");
	suzanne.initialise(mesh.meshData);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glDebugMessageCallback(glDebugOutput, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	//shader loading example
	Shader shader;
	Shader picking;
	Shader outline;

	shader.loadShaderProgramFromFile(RESOURCES_PATH "vertex.vert", RESOURCES_PATH "fragment.frag");
	outline.loadShaderProgramFromFile(RESOURCES_PATH "vertex.vert", RESOURCES_PATH "color.frag");
	picking.loadShaderProgramFromFile(RESOURCES_PATH "picking.vert", RESOURCES_PATH "picking.frag");

	shader.bind();


	PickingTexture picker;

	picker.initialise(WINDOW_WIDTH, WINDOW_HEIGHT);

	MouseState mouseState;

	uint selected_object_id = 0;


	float angle = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_STENCIL_BUFFER_BIT);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glClearColor(0.1f, 0.3f, 0.7f, 1.0f);


		glfwGetCursorPos(window, &mouseState.x, &mouseState.y);

		if (glfwGetMouseButton(window, 0) == GLFW_PRESS)
		{
			mouseState.isPressed = true;
		}
		else
		{
			mouseState.isPressed = false;
		}

		//-------------------------------------------------
		//---SHOULD REALLY MOVE SHADER SHIT ELSEWHERE------
		//-------------------------------------------------

		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		float scale = 1.0f;

		model = glm::translate(model, glm::vec3(0, 0, -2));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

		projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));


		if (mouseState.isPressed)
		{
			int width = 0, height = 0;
			glfwGetFramebufferSize(window, &width, &height);

			glViewport(0, 0, width, height);

			//shader.bind();

			// ---- framebuffer test start

			picker.enable_writing();

			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_DEPTH_BUFFER_BIT);

			picking.bind();

			glUniform1ui(picking.getUniform("objectIndex"), (uint)1);
			glUniform1ui(picking.getUniform("drawIndex"), (uint)1);
			glUniformMatrix4fv(picking.getUniform("projection"), 1, false, glm::value_ptr(projection));
			glUniformMatrix4fv(picking.getUniform("view"), 1, false, glm::value_ptr(view));
			glUniformMatrix4fv(picking.getUniform("model"), 1, false, glm::value_ptr(model));

			glUniform1f(picking.getUniform("scale"), scale);


			suzanne.bind();

			picker.disable_writing();

			// ---- framebuffer test end

			PickingTexture::PixelInfo pixel = picker.read_pixel(mouseState.x, WINDOW_HEIGHT - mouseState.y - 1);

			selected_object_id = pixel.objectID;

		}

		shader.bind();

		texture.bind(); 

		if (selected_object_id == 1) // object id should probably be something set inside the renderObject
		{
			glUniform1i(shader.getUniform("selected"), 1);
		}
		else
		{
			glUniform1i(shader.getUniform("selected"), 0);
		}

		glUniformMatrix4fv(shader.getUniform("projection"), 1, false, glm::value_ptr(projection));
		glUniform1f(shader.getUniform("scale"), scale);
		glUniformMatrix4fv(shader.getUniform("view"), 1, false, glm::value_ptr(view));
		glUniformMatrix4fv(shader.getUniform("model"), 1, false, glm::value_ptr(model));


		
		GLint tex = shader.getUniform("tex0"); 
		glUniform1i(tex, 0); 

		if (angle > 360)
		{
			angle = 0;
		}

		angle++;

		// Commented out the object outline, move to renderObject?

		/*glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);*/

		suzanne.bind();

		/*glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);*/
		//outline.bind();

		/*scale = 1.25f;

		glUniform1f(outline.getUniform("scale"), scale);
		glUniformMatrix4fv(outline.getUniform("projection"), 1, false, glm::value_ptr(projection));
		glUniformMatrix4fv(outline.getUniform("view"), 1, false, glm::value_ptr(view));
		glUniformMatrix4fv(outline.getUniform("model"), 1, false, glm::value_ptr(model));

		suzanne.bind();*/

		/*glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);*/

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	return 0;

}

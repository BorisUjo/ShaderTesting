#pragma once
#include <iostream>
#include <demoShaderLoader.h>
#include <meshInformation.h>
#include <renderObject.h>
#include <tile.h>
#include <GLFW/glfw3.h>
class Scene
{
public:
	Scene();
	~Scene();
	void render_map(); // default framebuffer
	void picking_phase_render_map();
	void set_camera_params(float width, float height);
	bool load_resources();
	void debug_input(GLFWwindow* window);

private:
	void initialise_map(RenderObject* objects, MeshData& data);
public:
	Camera* sceneCamera;
private:

	int RENDER_OBJECT_COUNTER = 0;

	RenderObject mapTiles[64];
	RenderObject mapMesh;

	Shader defaultShader;
	Shader pickingShader;

	float width = 0.0f;
	float height = 0.0f;

	Tile tiles[64];

};
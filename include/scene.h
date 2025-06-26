#pragma once
#include <iostream>
#include <demoShaderLoader.h>
#include <meshInformation.h>
#include <renderObject.h>
#include <tile.h>
#include <GLFW/glfw3.h>
#include <gameManager.h>
#include <texture.h>
#include <gameStateManager.h>
class Scene
{
public:
	Scene();
	~Scene();
	void render_map(); // default framebuffer
	void picking_phase_render_map();
	void initialise_camera(float width, float height);
	void debug_input(GLFWwindow* window);
	void update();
	bool load_resources();

	void spawn_debug_unit();

private:
	void initialise_map(MeshData& tileMesh, Shader* shader);
public:
	Camera* sceneCamera;
private:

	int RENDER_OBJECT_COUNTER = 0;

	
	GameStateManager gameStateManager;
	RenderObject mapMesh;

	Shader defaultShader;
	Shader pickingShader;

	Texture tileTexture;

	int previous_key;

	float width = 0.0f;
	float height = 0.0f;

	Tile tiles[64];

};
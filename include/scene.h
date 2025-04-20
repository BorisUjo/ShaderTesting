#pragma once
#include <iostream>
#include <demoShaderLoader.h>
#include <meshInformation.h>
#include <renderObject.h>
class Scene
{
public:
	Scene();
	~Scene();
	void render_map();
	void set_camera_params(float width, float height);

private:
	bool load_resources();
	void initialise_platform(RenderObject* objects, MeshData& data);
private:
	Camera* sceneCamera;

	RenderObject mapTiles[64];
	RenderObject mapMesh;

	Shader defaultShader;

	float width = 0.0f;
	float height = 0.0f;


};
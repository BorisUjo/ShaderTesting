#pragma once
#include <iostream>
#include <renderObject.h>
#include <demoShaderLoader.h>
class GameObject
{
public:
	RenderObject mesh;
	Shader* shader = nullptr;
public:
	virtual void OnEntityPressed();
	virtual void parseShader();
	virtual void parsePickingShader(Shader& pickingShader);
	virtual void initialise();
	virtual void update();
};
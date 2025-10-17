#pragma once
#include <iostream>
#include <renderObject.h>
#include <demoShaderLoader.h>
class GameObject
{
public:
	RenderObject mesh;
	Shader* shader = nullptr;
	Shader* highlightShader = nullptr;
	Texture* texture = nullptr;
	std::string gameObjectTag = "GameObject";
	int gameObjectID = 0;

	bool objectSelected = false;

public:
	virtual void OnEntityPressed();
	virtual void OnEntityExit();
	virtual void parseShader();
	virtual void parseHighlightShader();
	virtual void parsePickingShader(Shader& pickingShader);
	virtual void initialise();
	virtual void update();
	virtual void render();
	virtual void highlight();
	virtual GameObject* get();
	void setTag(std::string newTag);

	inline bool isSelected() { return objectSelected; }

};
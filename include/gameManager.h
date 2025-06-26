#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <gameObject.h>
class GameManager
{
private:
	unsigned int m_renderObjectCount = 0;
	std::vector<GameObject> gameObjects;
	Camera* main_camera = nullptr;
	float m_deltaTime = 0.0f;
public:

	static GameManager& getInstance()
	{
		static GameManager instance;

		return instance;
	}
	
	inline float deltaTime() { return m_deltaTime; }
	inline int getRenderObjectCount() { return m_renderObjectCount; }
	inline void countRenderObject() { m_renderObjectCount++;  }
	inline Camera* getMainCamera() { return main_camera; }
	void getGameObjects(GameObject** ptr, std::size_t& size) { *ptr = gameObjects.data();  size = gameObjects.size(); }
	void setMainCamera(Camera* camera) { main_camera = camera; }
	void setDeltaTime(const float dTime) { m_deltaTime = dTime; }
	GameObject& instantiate() { gameObjects.emplace_back(); return gameObjects.back(); }

private:
	GameManager() {}
	~GameManager() {}
public:

	GameManager(const GameManager&) = delete;
	GameManager(GameManager&&) = delete;
	void operator=(const GameManager&) = delete;

};
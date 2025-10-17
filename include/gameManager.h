#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <gameObject.h>
#include <unit.h>
class GameManager
{
private:
	unsigned int m_renderObjectCount = 0;
	std::vector<std::unique_ptr<GameObject>> gameObjects;
	std::vector<GameObject*> tempPtrs; // used for getGameObjects
	std::map<int, GameObject&> objectID;
	std::vector<UnitData> units;

	Camera* main_camera = nullptr;
	Shader defaultHighlight;

	int selectedGameObjectID = -1;
	int selectedUnitID = -1;
	float m_deltaTime = 0.0f;

public:

	UnitController* selectedUnit = nullptr;

	static GameManager& getInstance()
	{
		static GameManager instance;

		return instance;
	}
	
	inline float deltaTime() { return m_deltaTime; }
	inline int getRenderObjectCount() { return m_renderObjectCount; }
	inline void countRenderObject() { m_renderObjectCount++;  }
	inline Shader* getHighlightShader() { return &defaultHighlight; }
	inline Camera* getMainCamera() { return main_camera; }
	inline std::vector<UnitData> getUnits() { return units; }
	//void getGameObjects(GameObject** ptr, std::size_t& size) { *ptr = gameObjects.data();  size = gameObjects.size(); }
	void getGameObjects(GameObject*** ptr, std::size_t& size) {
		tempPtrs.clear();
		tempPtrs.reserve(gameObjects.size());
		for (auto& uptr : gameObjects) {
			tempPtrs.push_back(uptr.get()); // raw pointer
		}
		*ptr = tempPtrs.data();
		size = tempPtrs.size();
	}

	void setMainCamera(Camera* camera) { main_camera = camera; }
	void setDeltaTime(const float dTime) { m_deltaTime = dTime; }
	void setUnits(const std::vector<UnitData>& data) { units = data; }
	void setHighlightShader(Shader& shader) { defaultHighlight = shader; }
	void addObjectID(int id, GameObject* go)
	{
		objectID.insert({ id, *go });
	}
	template<typename T, typename... Args>
	T& instantiate(Args&&... args)
	{
		static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject");
		gameObjects.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		gameObjects.back()->initialise();
		return static_cast<T&>(*gameObjects.back());
	}

	GameObject& grabFromID(int id) {
		auto it = objectID.find(id);
		if (it == objectID.end()) {
			//std::cout << "GameObject not found" << std::endl;
		}
		return it->second;
	}

	void selectGameObject(int id)
	{
		selectedGameObjectID = id;
		//std::cout << "Selecting ID: " << id << '\n';
	}

	void selectUnitID(int id)
	{
		selectedUnitID = id;
	}
	GameObject& grabSelectedObject()
	{
		return grabFromID(selectedGameObjectID);
	}

private:
	GameManager() {}
	~GameManager() {}
public:

	GameManager(const GameManager&) = delete;
	GameManager(GameManager&&) = delete;
	void operator=(const GameManager&) = delete;

};
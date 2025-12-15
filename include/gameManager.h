#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <gameObject.h>
#include <unit.h>
#include <functional>

struct TickManager
{

	const int TICK_RATE = 15;
	int currentTickRate = 0;

	std::vector<std::function<void()>> callbacks;

	void tick()
	{
		if (currentTickRate < TICK_RATE)
		{
			currentTickRate++;
		}
		else
		{
			currentTickRate = 0;
			for (auto& func : callbacks)
			{
				func();
			}
		}
	}
};

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

	TickManager tickManager;
	UnitController* selectedUnit = nullptr;
	GameObject* selectedObj = nullptr;


	std::map<std::string, MeshData> loadedModels;

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
	inline UnitData getUnitFromID(const char* id)
	{
		for (auto& unit : units)
		{
			if (unit.ID == id)
			{
				return unit;
			}
		}
	}
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

	void view_unit_deployment(bool state)
	{
		std::vector<Tile*> tiles;
		GetGameObjectByType<Tile>(tiles);

		for (auto& tile : tiles)
		{
			if (tile->tileData->yGrid <= 2)
			{
				tile->DebugSelect = state;
			}
		}


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

	GameObject* grabFromID(int id) {
		auto it = objectID.find(id);
		if (it == objectID.end()) {
			return nullptr;
		}
		return &it->second;
	}

	template<typename T>
	T* grabFromID(int id)
	{

		static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject");

		auto it = objectID.find(id);
		if (it == objectID.end()) {
			return nullptr;
		}
		return &it->second;
	}

	template<typename T>
	void GetGameObjectByType(std::vector<T*>& typeObjs)
	{
		static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject");

		for (auto& go : gameObjects)
		{
			if (T* type = dynamic_cast<T*>(go.get()))
			{
				typeObjs.push_back(type);
			}
		}
	}

	void subscribeTick(std::function<void()> func)
	{
		tickManager.callbacks.push_back(func);
	}

	bool isModelLoaded(const char* path)
	{
		for (auto key : loadedModels)
		{
			char keyC[1024];
			strncpy(keyC, key.first.c_str(), sizeof(keyC));
			if (strcmp(keyC, path) == 0)
			{
				return true;
			}
		}

	}

	void selectGameObject(int id)
	{
		selectedGameObjectID = id;
		//std::cout << "Selecting ID: " << id << '\n';
	}

	void selectGameObject(GameObject* obj)
	{
		selectedObj = obj;
	}

	void selectUnitID(int id)
	{
		selectedUnitID = id;
	}
	GameObject& grabSelectedObject()
	{
		return *grabFromID(selectedGameObjectID);
	}

	Tile* findTileFromData(const TileData& data)
	{
		std::vector<Tile*> tiles;
		GetGameObjectByType<Tile>(tiles);

		for (auto& tile : tiles)
		{
			if (&data == tile->tileData)
			{
				return tile;
			}
		}

		return nullptr;
	}

	TileData* findByGrid(int x, int y)
	{
		std::vector<Tile*> tiles;
		GetGameObjectByType<Tile>(tiles);

		for (auto& tile : tiles)
		{
			if (tile->tileData->xGrid == x && tile->tileData->yGrid == y)
			{
				return tile->tileData;
			}
		}

		return nullptr;

	}

	Tile* findTileByGrid(int x, int y)
	{
		std::vector<Tile*> tiles;
		GetGameObjectByType<Tile>(tiles);

		for (auto& tile : tiles)
		{
			if (tile->tileData->xGrid == x && tile->tileData->yGrid == y)
			{
				return tile;
			}
		}

		return nullptr;

	}


private:
	GameManager() {}
	~GameManager() {}
public:

	GameManager(const GameManager&) = delete;
	GameManager(GameManager&&) = delete;
	void operator=(const GameManager&) = delete;

};
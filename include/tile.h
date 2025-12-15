#pragma once
#include <glm/glm.hpp>
#include <gameObject.h>

#define NOT_OCCUPIED -1

struct TileData
{
	int xGrid = 0;
	int yGrid = 0;

	// mesh ID of the unit on this tile
	int unitID = NOT_OCCUPIED;

	glm::vec3 worldPos;
	bool isOccupied = false;

	void initialise(int x, int y, const glm::vec3& pos)
	{
		xGrid = x;
		yGrid = y;
		worldPos = pos;
	}

	void set_occupied(bool flag)
	{
		isOccupied = flag;
	}
	void getNeighborData(std::vector<TileData*>& data);
};

class Tile : public GameObject
{
public:
	
	TileData* tileData;
	bool moveHighlight = false;
	bool DebugSelect = false;
	
	virtual void parseShader();
	/*virtual void OnEntityPressed()
	{
		auto& go = GameManager::getInstance();

		go.selectGameObject(this);
		go.selectGameObject(mesh.meshID);
	}*/


};
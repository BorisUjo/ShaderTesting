#pragma once
#include <glm/glm.hpp>
#include <gameObject.h>
struct TileData
{
	int xGrid = 0;
	int yGrid = 0;
	
	glm::vec3 worldPos;
	bool isOccupied = false;

	void initialise(int x, int y, glm::vec3 pos)
	{
		xGrid = x;
		yGrid = y;
		worldPos = pos;
	}

	void set_occupied(bool flag)
	{
		isOccupied = flag;
	}
};

class Tile : public GameObject
{
public:
	TileData* tileData;
};
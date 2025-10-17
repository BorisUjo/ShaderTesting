#pragma once
#include <tile.h>
extern class Unit;
class UnitController : public GameObject
{
private:
	Unit* m_unit;
public:
	TileData* currentTile = nullptr;
public:
	virtual void OnEntityPressed();
	void move_to_tile(TileData& tile);
	void move_unit(glm::vec3 pos);
	void initialise_unit(Unit* unit);
};


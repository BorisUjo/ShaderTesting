#pragma once
#include <tile.h>
#include <taskManager.h>
extern class Unit;

enum UnitDir
{
	NORTH,
	SOUTH,
	EAST,
	WEST,
};

class UnitController : public GameObject
{
private:
	TaskManager m_taskManager;
	UnitDir m_facingDirection;
public:
	Unit* unit;
	UnitController* targetUnit = nullptr;
	TileData* nextTile = nullptr;
	TileData* currentTile = nullptr;
	bool unitDeployed = false;
public:
	virtual void OnEntityPressed();
	virtual void parseShader();
	void move_to_tile(TileData* tile);
	void initialise_unit(Unit* unit);
	void move_unit(glm::vec3 pos);
	void find_target_unit();
	void tick_init();
	void tick();
	void destroy(void);
	int getPlayerID();

	bool unitInRange();
	void cast_attack();
	void set_facing_direction(UnitDir direction);
};


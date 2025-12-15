#pragma once
#include <iostream>
#include <cassert>
#include <unitInventory.h>
#include <unitController.h>
#include <tile.h>
#include <reserveTile.h>

struct UnitJsonData
{
	std::string ID;
	std::string modelPath;
};

struct UnitData
{
	std::string ID = "";
	std::string name = "";
	std::string description = "";

	uint16_t health = 0;
	uint16_t attackShield = 0;
	uint16_t magicShield = 0;

	uint16_t attackDamage = 0;
	uint16_t magicDamage = 0;

	uint16_t moveRange = 1;
};

class Unit
{
public:
	int playerID = -1;

	UnitData battleData;
private:
	UnitData m_baseData;
	UnitController* m_controller;
	UnitInventory<3> m_unitInventory;

	int m_unitMana = 0;
	int m_unitHealth = 0;

	bool reserveStatus = true; // is unit in reserve 


public:
	Unit(UnitData data); // obsolete
	Unit(const UnitData data, Shader* shader, Texture* texture);
	~Unit();
	void init_controller();
	void set_reserve_status(bool status);
	void refresh_stats();
	void move_to_reserve(ReserveTile& tile);
	void move_tile(TileData& tile);
	void setPlayerID(GLuint id);

	int getPlayerID();
	int get_health();
	int get_attack_damage();
	int get_move_range();


	virtual void cast_ultimate();

	UnitController& get_controller(void) { return *m_controller; }
};

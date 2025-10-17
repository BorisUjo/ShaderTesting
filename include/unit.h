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
	std::string ID;
	std::string name;
	std::string description;

	uint16_t health;
	uint16_t attackShield;
	uint16_t magicShield;

	uint16_t attackDamage;
	uint16_t magicDamage;

	uint16_t moveRange = 1;
};

class Unit
{
private:
	UnitData m_baseData;
	UnitData m_battleData;
	UnitController* m_controller;
	UnitInventory<3> m_unitInventory;

	int m_unitMana = 0;
	int m_unitHealth = 0;

	bool reserveStatus = true; // is unit in reserve 

	int get_health();
	int get_attack_damage();
	int get_move_range();

public:
	Unit(UnitData data); // obsolete
	Unit(UnitData data, Shader* shader, Texture* texture);
	~Unit();
	void init_controller();
	void set_reserve_status(bool status);
	void refresh_stats();
	void move_to_reserve(ReserveTile& tile);

	virtual void cast_ultimate();

	UnitController& get_controller(void) { return *m_controller; }
};

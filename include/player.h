#pragma once
#include <unit.h>
#include <reserveTile.h>
#include <vector>
class Player
{
private:
	std::vector<Unit> m_playerUnits;
	ReserveTile* reserve[5];
	int m_playerHealth = -1;
	int m_playerBalance = -1;
	int playerID = 0;
	bool playerIsNPC = true;

public:
	void add_reserve_tile(ReserveTile* tile, int index);
	void add_unit(UnitData& data);
	void add_unit(const UnitData& data, Shader* shader, Texture* texture);
	void initialise(bool isNPC = true)
	{
		playerIsNPC = isNPC;
		playerIsNPC ? playerID = 0 : playerID = 1;

		m_playerBalance = 0;
		m_playerHealth = 100;

		m_playerUnits.reserve(25);
	}

	ReserveTile* find_available_reserve();

	std::vector<Unit>& get_units(void) { return m_playerUnits; }
};
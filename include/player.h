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
	bool isNPC = false;

public:
	void add_reserve_tile(ReserveTile* tile, int index);
	void add_unit(UnitData& data);
	void add_unit(UnitData& data, Shader* shader, Texture* texture);
	//data[rnd]; m_player1.get_units().push_back(Unit(gameManager.getUnits()[0], &defaultShader, &dummyTexture));
	void initialise(bool isNPC)
	{
		isNPC = isNPC;

		m_playerBalance = 0;
		m_playerHealth = 100;

	}

	ReserveTile& find_available_reserve();

	std::vector<Unit>& get_units(void) { return m_playerUnits; }
};
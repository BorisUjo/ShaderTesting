#include <player.h>

void Player::add_reserve_tile(ReserveTile* tile, int index)
{
	reserve[index] = tile;
}

void Player::add_unit(UnitData& data)
{

}

void Player::add_unit(const UnitData& data, Shader* shader, Texture* texture)
{
	m_playerUnits.emplace_back(UnitData(data), shader, texture);
	auto& unit = m_playerUnits.back();
	unit.setPlayerID(playerID);
	unit.init_controller();

	if (auto* tile = find_available_reserve())
	{
		unit.move_to_reserve(*tile);
		unit.get_controller().unitDeployed = false;
	}

}

ReserveTile* Player::find_available_reserve()
{
	for (auto& tile : reserve)
	{
		if (!tile->occupied)
		{
			return tile;
		}

	}

	return nullptr;

}

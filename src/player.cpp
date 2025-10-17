#include <player.h>

void Player::add_reserve_tile(ReserveTile* tile, int index)
{
	reserve[index] = tile;
}

void Player::add_unit(UnitData& data)
{

}

void Player::add_unit(UnitData& data, Shader* shader, Texture* texture)
{
	auto& unit = Unit(data, shader, texture);
	//unit.init_controller();
	unit.move_to_reserve(find_available_reserve());
	m_playerUnits.push_back(unit);
}

ReserveTile& Player::find_available_reserve()
{
	for (auto& tile : reserve)
	{
		if (!tile->occupied)
		{
			return *tile;
		}

	}
}

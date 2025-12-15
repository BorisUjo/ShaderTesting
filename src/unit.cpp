#include <unit.h>
#include <iostream>
#include <gameManager.h>

extern std::ostream& operator<<(std::ostream& os, const glm::vec3 pos);

void Unit::cast_ultimate()
{
	std::cout << "Ultimate cast!" << '\n';
}

Unit::Unit(UnitData data)
{
	m_baseData = data;
	battleData = data;

	auto& gameManager = GameManager::getInstance();

	auto& go = gameManager.instantiate<UnitController>();
	MeshData meshData;
	std::string path = RESOURCES_PATH "models/Debug/" + m_baseData.ID + ".obj";
	const char* cPath = path.c_str();
	std::cout << cPath << '\n';

	loadModelFromFile(cPath, meshData);
	//m_controller = dynamic_cast<UnitController&>(go);
}

Unit::Unit(const UnitData data, Shader* shader, Texture* texture)
{
	m_baseData = UnitData(data);
	battleData = UnitData(data);

	auto& gameManager = GameManager::getInstance();

	auto& go = gameManager.instantiate<UnitController>();

	MeshData meshData;
	std::string path = RESOURCES_PATH "models/Debug/" + m_baseData.ID + ".obj";
	//path = RESOURCES_PATH "models/Pillar.obj"; // DEBUUUUUUUUUUUUUUUUUUUUUUUG
	const char* cPath = path.c_str();
	std::cout << "Unit model path: " << cPath << '\n';

	loadModelFromFile(cPath, meshData);
	go.mesh.initialise(meshData);
	go.shader = shader;
	go.highlightShader = gameManager.getHighlightShader();
	go.texture = texture; 
	go.mesh.textureID = texture->textureID;
	gameManager.subscribeTick(std::bind(&UnitController::tick, &go));
	m_controller = &go;
	m_controller->tick_init();


}

Unit::~Unit()
{
//	std::cout << "Unit destructor called" << '\n';
}

void Unit::init_controller()
{
	m_controller->unit = this;
	//m_controller->initialise_unit(this);
}

void Unit::set_reserve_status(bool status)
{
	reserveStatus = status;
}

int Unit::get_health()
{
	int modifiers = 0;

	for (auto& item : m_unitInventory.items)
	{
		if (item.get_id() == 0)
		{
			continue;
		}

		modifiers += item.get_health();

	}

	return (m_baseData.health + modifiers);

}

int Unit::get_attack_damage()
{
	int modifiers = 0;
	for (auto& item : m_unitInventory.items)
	{
		if (item.get_id() == 0)
		{
			continue;
		}

		modifiers += item.get_attack();
	}

	return (m_baseData.attackDamage + modifiers);
}

int Unit::get_move_range()
{
	return m_baseData.moveRange;
}

void Unit::refresh_stats()
{
	battleData.attackDamage = get_attack_damage();
	battleData.health = get_health();

}
void Unit::move_to_reserve(ReserveTile& tile)
{
	tile.setOccupation(true);
	glm::vec3 pos = tile.mesh.position;
	pos.y = 1;
	m_controller->move_unit(pos);
}

void Unit::move_tile(TileData& tile)
{
	auto& pos = tile.worldPos;
	m_controller->move_unit(pos);
	if (m_controller->currentTile != nullptr)
	{
		m_controller->currentTile->unitID = NOT_OCCUPIED;
	}

	tile.unitID = m_controller->mesh.meshID;

	//m_controller->currentTile = &tile;
	m_controller->move_to_tile(&tile);
	
}

void Unit::setPlayerID(GLuint id)
{
	playerID = id;
	std::cout << "PLAYER ID: " << playerID << '\n';
}

int Unit::getPlayerID()
{
	return playerID;
}



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

	auto& gameManager = GameManager::getInstance();

	auto& go = gameManager.instantiate<UnitController>();
	MeshData meshData;
	std::string path = RESOURCES_PATH "models/Debug/" + m_baseData.ID + ".obj";
	const char* cPath = path.c_str();
	std::cout << cPath << '\n';

	loadModelFromFile(cPath, meshData);
	//m_controller = dynamic_cast<UnitController&>(go);
}

Unit::Unit(UnitData data, Shader* shader, Texture* texture)
{
	m_baseData = data;

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
	m_controller = &go;


}

Unit::~Unit()
{
//	std::cout << "Unit destructor called" << '\n';
}

void Unit::init_controller()
{
	m_controller->initialise_unit(this);
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
	m_battleData.attackDamage = get_attack_damage();
	m_battleData.health = get_health();

}
void Unit::move_to_reserve(ReserveTile& tile)
{
	tile.setOccupation(true);
	glm::vec3 pos = tile.mesh.position;
	pos.y = 1;
	m_controller->move_unit(pos);
}



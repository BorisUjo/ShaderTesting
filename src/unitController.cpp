#include <unitController.h>
#include <unit.h>
#include <gameManager.h>
#include <camera.h>
#include <taskManager.h>

extern std::ostream& operator<<(std::ostream& os, const glm::vec3 vec);
void UnitController::OnEntityPressed() 
{

	std::cout << " [Unit Controller] : Selecting Unit \n";

	auto& gameManager = GameManager::getInstance();
	gameManager.selectUnitID(mesh.meshID);
	gameManager.selectedUnit = this;

}

void UnitController::move_to_tile(TileData* tile)
{
	if (tile->isOccupied) // this should never be getting called
	{
		std::cout << "[Unit Controller] : Move error, tile already occupied \n";
		return;
	}

	if (currentTile != nullptr)
	{ 
		currentTile->set_occupied(false);
	}
	move_unit(tile->worldPos);

	tile->set_occupied(true);
	currentTile = tile;
}

void UnitController::parseShader()
{
	shader->bind();

	Camera* camera = GameManager::getInstance().getMainCamera();

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, mesh.position);
	model = glm::rotate(model, glm::radians(mesh.rotationAngle), mesh.rotation);
	model = glm::scale(model, glm::vec3(mesh.scale));
	

	glUniformMatrix4fv(shader->getUniform("PVM"), 1, false, glm::value_ptr(camera->get_matrix() * model));
	bool isPlayer; unit->getPlayerID() == 1 ? isPlayer = true : isPlayer = false;
	glUniform1i(shader->getUniform("isMainPlayer"), isPlayer);

	glUniform1i(shader->getUniform("tex0"), 0);
	mesh.bind_texture();
}

void UnitController::initialise_unit(Unit* unit)
{
	this->unit = unit;
}

void UnitController::move_unit(glm::vec3 pos)
{

	if (mesh.position.z > pos.z)
		set_facing_direction(WEST);
	else if (mesh.position.z < pos.z)
		set_facing_direction(EAST);
	else if (mesh.position.x > pos.x)
		set_facing_direction(SOUTH);
	else
		set_facing_direction(NORTH);

	pos.y = 2;
	mesh.position = pos;
}

int UnitController::getPlayerID()
{
	return unit->playerID;
}

void UnitController::tick()
{
	if (unit->battleData.health <= 0
		|| unitDeployed == false)
	{

		return;
	}

	m_taskManager.DoTask();
}

void UnitController::tick_init()
{
	m_taskManager = TaskManager(this);
}

bool UnitController::unitInRange()
{

	if (targetUnit == nullptr)
	{
		return false;
	}

	int targetX = targetUnit->currentTile->xGrid;
	int targetY = targetUnit->currentTile->yGrid;

	int unitX = currentTile->xGrid;
	int unitY = currentTile->yGrid;

	if (abs(targetX - unitX) <= 1 && abs(targetY - unitY) <= 1)
	{
		return true;
	}

	return false;

}

void UnitController::destroy(void)
{
	std::cout << "Destroying object \n";
}

void UnitController::set_facing_direction(UnitDir direction)
{
	m_facingDirection = direction;

	float angle = 0.0f;

	switch (m_facingDirection)
	{
	case NORTH:
		angle = 0.0f;
		break;
	case SOUTH:
		angle = 180.0f;
		break;
	case WEST:
		angle = 90.0f;
		break;
	case EAST:
		angle = 270.0f;
		break;
	}

	mesh.rotationAngle = angle;

}

void UnitController::cast_attack()
{
	//std::cout << "Attacking Target: CURRENT HP : " << targetUnit->unit->battleData.health << '\n';
	//std::cout << "TARGET: " << targetUnit->unit->battleData.name << '\n';

	targetUnit->unit->battleData.health -= unit->get_attack_damage();
}

void UnitController::find_target_unit()
{
	//std::cout << "Finding new target \n";

	auto& gm = GameManager::getInstance();
	std::vector<Tile*> tiles;
	std::vector<UnitController*> units;
	gm.GetGameObjectByType<Tile>(tiles);
	gm.GetGameObjectByType<UnitController>(units);

	UnitController* closest = nullptr;

	int shortest = 1000;

	int unitX = currentTile->xGrid;
	int unitY = currentTile->yGrid;

	for (auto& eUnit : units)
	{

		if (eUnit->unit->getPlayerID() == unit->getPlayerID())
		{
			continue;
		}

		int x = eUnit->currentTile->xGrid;
		int y = eUnit->currentTile->yGrid;

		int distance = abs(x - unitX) + abs(y - unitY);

		if (distance < shortest)
		{
			shortest = distance;
			closest = eUnit;
		}

	}
	if (closest == nullptr)
	{
		return;
	}

	//Tile* tile = gm.findTileFromData(*closest->currentTile);
	//tile->moveHighlight = true;

	targetUnit = closest;



}

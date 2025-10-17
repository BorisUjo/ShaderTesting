#include <unitController.h>
#include <unit.h>
#include <gameManager.h>
void UnitController::OnEntityPressed() 
{

	std::cout << " [Unit Controller] : Selecting Unit \n";

	auto& gameManager = GameManager::getInstance();
	gameManager.selectUnitID(mesh.meshID);
	gameManager.selectedUnit = this;

}

void UnitController::move_to_tile(TileData& tile)
{
	if (tile.isOccupied)
	{
		std::cout << "[Unit Controller] : Move error, tile already occupied \n";
		return;
	}

	if (currentTile != nullptr)
	{
		currentTile->set_occupied(false);
	}
	move_unit(tile.worldPos);

	tile.set_occupied(true);
	currentTile = &tile;
}

void UnitController::initialise_unit(Unit* unit)
{
	m_unit = unit;
}

void UnitController::move_unit(glm::vec3 pos)
{
	pos.y = 1;
	mesh.position = pos;
}

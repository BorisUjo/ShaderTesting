#include <taskManager.h>
#include <unitController.h>
#include <gameManager.h>
#include <map>
void TaskManager::QueueTask()
{

	unit->find_target_unit();

	if (unit->targetUnit == nullptr)
	{
		return;
	}

	if (unit->unitInRange())
	{
		//std::cout << "[Task Manager] : Queued ATTACK task\n";
		currentTask = ATTACK_TASK;
	}
	else
	{
		if (currentQueue.empty() || currentGoal != unit->targetUnit->currentTile)
		{
			findPath(*unit->currentTile, *unit->targetUnit->currentTile);
		}
		
		//std::cout << "[Task Manager] : Queued MOVE task\n";
		currentTask = MOVE_TASK;
	}
}

// TODO: refactor to call queue task only if idle
// re-path when moving to see if a new target is closer 

void TaskManager::DoTask()
{

	if (currentTask == IDLE)
	{
		QueueTask();
		return;
	}

	auto& gm = GameManager::getInstance();

	if (currentTask == MOVE_TASK 
		&& !currentQueue.empty())
	{
		auto& tile = currentQueue.front();
		if (tile->isOccupied) // if next tile is occupied, remake the path
		{
			std::swap(currentQueue, std::queue<TileData*>());
			return;
		}
		unit->move_to_tile(tile);
		currentQueue.pop();
	}
	else if (currentTask == ATTACK_TASK)
	{
		unit->cast_attack();
	}


	currentTask = IDLE;
	
}

bool exist_in_queue(std::queue<TileData*> data, const TileData& element)
{
	while (!data.empty())
	{
		auto& tile = data.front();

		if (tile->xGrid == element.xGrid && tile->yGrid == element.yGrid)
		{
			return true;
		}

		data.pop();
	}

	return false;

}

void TaskManager::reconstruct_path(std::unordered_map<TileData*, TileData*> path, TileData* goal)
{
	
	//std::cout << "[Task Manager] : Reconstructing path \n";
	std::vector<TileData*> reversePath;
	TileData* current = goal;
	currentGoal = goal;

	auto& gm = GameManager::getInstance();

	while (path.count(current))
	{
		reversePath.push_back(current);
		current = path[current];

		/*auto* t = gm.findTileFromData(*current);
		t->moveHighlight = true;*/

	}

	for (auto it = reversePath.rbegin(); it != reversePath.rend(); ++it)
	{
		currentQueue.push(*it);
	}
}
void TaskManager::findPath(TileData& start, TileData& goal)
{
	//std::cout << "[Task Manager] : Finding path\n";

	currentQueue = std::queue<TileData*>();

	auto& gm = GameManager::getInstance();
	std::vector<Tile*> tileObjs;
	gm.GetGameObjectByType<Tile>(tileObjs);

	// Costs to reach each node
	std::unordered_map<TileData*, int> gCost;
	std::unordered_map<TileData*, TileData*> cameFrom;

	// Initialize all costs to infinity
	for (auto& tile : tileObjs)
		gCost[tile->tileData] = std::numeric_limits<int>::max();

	// Starting tile cost = 0
	gCost[&start] = 0;

	// Priority queue (min-heap) based on gCost
	auto cmp = [&](TileData* a, TileData* b) {
		return gCost[a] > gCost[b];
	};

	std::priority_queue<TileData*,
		std::vector<TileData*>,
		decltype(cmp)
	> frontier(cmp);

	frontier.push(&start);

	while (!frontier.empty())
	{
		TileData* current = frontier.top();
		frontier.pop();

		// Goal reached
		if (current == &goal)
		{
			reconstruct_path(cameFrom, &goal);
			return;
		}

		std::vector<TileData*> neighbors;
		current->getNeighborData(neighbors);

		for (auto& neighbor : neighbors)
		{
			int tentative = gCost[current] + 1; // movement cost

			if (tentative < gCost[neighbor])
			{
				gCost[neighbor] = tentative;
				cameFrom[neighbor] = current;
				frontier.push(neighbor);
			}
		}
	}

	std::cout << "No path found!\n";

}
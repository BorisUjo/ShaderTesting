#pragma once
#include <tile.h>
#include <queue>
#include <unordered_map>

extern class UnitController;

enum TaskState
{
	IDLE,
	MOVE_TASK,
	ATTACK_TASK
};

class TaskManager
{
public:
	TaskState currentTask = IDLE;

	UnitController* unit = nullptr;
	TileData* currentGoal;

	std::queue<TileData*> currentQueue;

	TaskManager() = default;
	TaskManager(UnitController* controller)
	{
		unit = controller;
	}
	void QueueTask();
	void DoTask();
	void findPath(TileData& start, TileData& goal);
	void TaskManager::reconstruct_path(std::unordered_map<TileData*, TileData*> path, TileData* goal);
};
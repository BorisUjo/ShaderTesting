#pragma once
#include <iostream>
#include <functional>


class GameStateManager
{
public:

	enum GameState
	{
		GameStateIdle = 0,
		GameStateShop = 1,
		GameStateBattle = 2,
	};

	std::function<void()> onGameStart;
	std::function<void()> onIdleStateEntered;
	std::function<void()> onShopStateEntered;
	std::function<void()> onBattleStateEntered;

private:
	GameState m_gameState = GameStateIdle;
	int m_gameStateCounter = 0;

	float m_gameStateTimer = 0.0f;
	float m_maxStateTime = 15.0f;
	//float m_maxStateTime = 3.0f; DEBUG

public:
	inline GameState GetGameState() { return m_gameState; }

	std::string state_to_string();

	void get_game_state_values(float* current, float* max, float* min);

	void debug_change_state(GameState state);
	void tick();

};
#pragma once

class GameStateManager
{
public:

	enum GameState
	{
		GameStateIdle = 0,
		GameStatePurchase = 1,
		GameStateBattle = 2,
	};


private:
	GameState m_gameState = GameStateIdle;

	float m_gameStateTimer = 0.0f;
	float m_maxStateTime = 4.0f;

public:
	inline GameState GetGameState() { return m_gameState; }
	void tick();
};
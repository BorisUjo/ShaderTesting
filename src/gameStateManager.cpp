#include <gameStateManager.h>
#include <iostream>
#include <gameManager.h>

void GameStateManager::debug_change_state(GameState state)
{
	std::cout << "[DEBUG] Switching game state:" << '\n';

	std::cout << "Old state: " << state_to_string() << '\n';

	m_gameState = state;

	switch (m_gameState)
	{
	case GameStateIdle:
		onIdleStateEntered();
	case GameStateShop:
		onShopStateEntered();
	case GameStateBattle:
		onBattleStateEntered();
	}
	std::cout << "New state: " << state_to_string() << '\n';
}

std::string GameStateManager::state_to_string()
{
	switch (m_gameState)
	{
	case GameStateIdle:
		return "Idle";
	case GameStateShop:
		return "Shop";
	case GameStateBattle:
		return "Battle";
	default:
		return "???";
	}
}

void GameStateManager::get_game_state_values(float* current, float* max, float* min)
{
	*current = m_gameStateTimer;
	*max = m_maxStateTime;
	*min = 0.0f;
}

void GameStateManager::tick()
{
	m_gameStateTimer += GameManager::getInstance().deltaTime();

	if (m_gameStateTimer > m_maxStateTime)
	{
		std::cout << "[GAME_STATE_MANAGER] : Switching states" << '\n';
		std::cout << "Old state: " << state_to_string() << '\n';

		m_gameStateCounter++;

		if (m_gameStateCounter > GameStateBattle)
		{
			m_gameStateCounter = 0;
		}

		m_gameState = (GameState)m_gameStateCounter;
		std::cout << "New state: " << state_to_string() << '\n';

		m_gameStateTimer = 0.0f;

		switch (m_gameState)
		{
		case GameStateIdle:
			onIdleStateEntered();
		case GameStateShop:
			onShopStateEntered();
		case GameStateBattle:
			onBattleStateEntered();
		}

	}

}

#include <gameStateManager.h>
#include <iostream>
#include <gameManager.h>

void GameStateManager::tick()
{
	m_gameStateTimer += GameManager::getInstance().deltaTime();

	if (m_gameStateTimer > m_maxStateTime)
	{
		std::cout << "Switching states" << std::endl;
		m_gameStateTimer = 0.0f;
	}

}

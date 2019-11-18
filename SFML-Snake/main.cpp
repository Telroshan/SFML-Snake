#include "Engine.h"
#include <iostream>

int main()
{
	Engine* snakeGame = Engine::GetInstance();

	snakeGame->Init();

	sf::Clock clock;
	while (snakeGame->IsRunning())
	{
		sf::Time deltaTime = clock.restart();

		snakeGame->UpdateInput();

		snakeGame->Update(deltaTime.asSeconds());

		snakeGame->Render();
	}

	delete snakeGame;

	return 0;
}
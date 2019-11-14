#include "Engine.h"
#include <iostream>

int main()
{
	sf::Vector2i windowSize(600, 500);

	Engine* snakeGame = Engine::GetInstance();

	snakeGame->Init("Snake", windowSize, 100, 20.f);

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
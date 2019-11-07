#include "Engine.h"

int main()
{
	sf::Vector2i windowSize(600, 400);

	Engine snakeGame("Snake", windowSize);

	snakeGame.SetCellSize(20);
	snakeGame.SetMoveSpeed(5.f);

	snakeGame.Init();

	sf::Clock clock;
	while (snakeGame.IsRunning())
	{
		sf::Time deltaTime = clock.restart();

		snakeGame.UpdateInput();

		snakeGame.Update(deltaTime.asSeconds());

		snakeGame.Render();
	}

	return 0;
}
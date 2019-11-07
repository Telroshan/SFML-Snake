#include "Engine.h"

int main()
{
	sf::Vector2i windowSize(600, 400);

	Engine snakeGame("Snake", windowSize);

	sf::Vector2f cellSize(20, 20);

	snakeGame.SetCellSize(cellSize);
	snakeGame.SetMoveSpeed(2.f);

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
#include "Engine.h"

int main()
{
	sf::Vector2i windowSize(600, 500);

	Engine& snakeGame = Engine::GetInstance();

	snakeGame.Init("Snake", windowSize, 100);

	snakeGame.SetCellSize(20);
	snakeGame.SetMoveSpeed(5.f);

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
#include "Engine.h"

int main()
{
	sf::Vector2i windowSize(600, 400);

	Engine engine("Snake", windowSize);
	
	sf::Vector2f rectangleSize(20, 20);
	
	engine.Init(rectangleSize);

	sf::Clock clock;
	while (engine.IsRunning())
	{
		sf::Time deltaTime = clock.restart();

		engine.UpdateInput();

		engine.Update(deltaTime.asMilliseconds());

		engine.Render();
	}

	return 0;
}
#include "Engine.h"

int main()
{
	sf::Vector2i windowSize(600, 400);

	Engine engine("Snake", windowSize);
	
	sf::Vector2f rectangleSize(20, 20);
	
	engine.Init(rectangleSize);

	while (engine.IsRunning())
	{
		engine.UpdateInput();

		engine.Update();

		engine.Render();
	}

	return 0;
}
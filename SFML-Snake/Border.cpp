#include "Border.h"
#include "Engine.h"

Border::Border(sf::Vector2i rectanglesCount)
{
	Engine& engine = Engine::GetInstance();
	float cellSize = engine.GetCellSize();
	sf::Vector2i windowSize = engine.GetWindowSize();
	float gameUiHeight = engine.GetGameUiHeight();

	const int colorsLength = 2;
	sf::Color colors[colorsLength] = { sf::Color::Color(30, 30, 30), sf::Color::Color(60, 60, 60) };
	int colorIndex = 0;

	// Top left => top right
	for (int x = 0; x < rectanglesCount.x - 1; ++x)
	{
		std::shared_ptr<sf::RectangleShape> cell = std::make_shared<sf::RectangleShape>(sf::Vector2f(cellSize, cellSize));
		cell->setFillColor(colors[colorIndex]);
		cell->setPosition(x * cellSize, 0);
		engine.RegisterDrawable(cell, Mode::Game);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Top right => bottom right
	for (int y = 0; y < rectanglesCount.y - 1; ++y)
	{
		std::shared_ptr<sf::RectangleShape> cell = std::make_shared<sf::RectangleShape>(sf::Vector2f(cellSize, cellSize));
		cell->setFillColor(colors[colorIndex]);
		cell->setPosition(windowSize.x - cellSize, y * cellSize);
		engine.RegisterDrawable(cell, Mode::Game);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Bottom right => bottom left
	for (int x = rectanglesCount.x - 1; x > 0; --x)
	{
		std::shared_ptr<sf::RectangleShape> cell = std::make_shared<sf::RectangleShape>(sf::Vector2f(cellSize, cellSize));
		cell->setFillColor(colors[colorIndex]);
		cell->setPosition(x * cellSize, (windowSize.y - gameUiHeight) - cellSize);
		engine.RegisterDrawable(cell, Mode::Game);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Bottom left => top left
	for (int y = 1; y < rectanglesCount.y; ++y)
	{
		std::shared_ptr<sf::RectangleShape> cell = std::make_shared<sf::RectangleShape>(sf::Vector2f(cellSize, cellSize));
		cell->setFillColor(colors[colorIndex]);
		cell->setPosition(0, y * cellSize);
		engine.RegisterDrawable(cell, Mode::Game);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}
}

void Border::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

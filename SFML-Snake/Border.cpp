#include "Border.h"
#include "Engine.h"

Border::Border(sf::Vector2i rectanglesCount)
{
	Engine& engine = Engine::GetInstance();
	float cellSize = engine.GetCellSize();
	sf::Vector2i windowSize = engine.GetWindowSize();
	int gameUiHeight = engine.GetGameUiHeight();

	_rectanglesCount = rectanglesCount;

	const int colorsLength = 2;
	sf::Color colors[colorsLength] = { sf::Color::Color(30, 30, 30), sf::Color::Color(60, 60, 60) };
	int colorIndex = 0;

	// Top left => top right
	for (int x = 0; x < _rectanglesCount.x - 1; ++x)
	{
		std::shared_ptr<sf::RectangleShape> cell = std::make_shared<sf::RectangleShape>(sf::Vector2f(cellSize, cellSize));
		cell->setFillColor(colors[colorIndex]);
		cell->setPosition(x * cellSize, 0);
		_cells.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Top right => bottom right
	for (int y = 0; y < _rectanglesCount.y - 1; ++y)
	{
		std::shared_ptr<sf::RectangleShape> cell = std::make_shared<sf::RectangleShape>(sf::Vector2f(cellSize, cellSize));
		cell->setFillColor(colors[colorIndex]);
		cell->setPosition(windowSize.x - cellSize, y * cellSize);
		_cells.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Bottom right => bottom left
	for (int x = _rectanglesCount.x - 1; x > 0; --x)
	{
		std::shared_ptr<sf::RectangleShape> cell = std::make_shared<sf::RectangleShape>(sf::Vector2f(cellSize, cellSize));
		cell->setFillColor(colors[colorIndex]);
		cell->setPosition(x * cellSize, (windowSize.y - gameUiHeight) - cellSize);
		_cells.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Bottom left => top left
	for (int y = 1; y < _rectanglesCount.y; ++y)
	{
		std::shared_ptr<sf::RectangleShape> cell = std::make_shared<sf::RectangleShape>(sf::Vector2f(cellSize, cellSize));
		cell->setFillColor(colors[colorIndex]);
		cell->setPosition(0, y * cellSize);
		_cells.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}
}

bool Border::Collides(sf::Vector2i gridPosition) const
{
	return gridPosition.x == 0 || gridPosition.x == _rectanglesCount.x - 1
		|| gridPosition.y == 0 || gridPosition.y == _rectanglesCount.y - 1;
}

void Border::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (size_t i = 0; i < _cells.size(); ++i)
	{
		target.draw(*_cells[i]);
	}
}

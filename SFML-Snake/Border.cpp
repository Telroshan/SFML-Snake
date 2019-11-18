#include "Border.h"
#include "Engine.h"

Border::Border(sf::Vector2i gridSize, sf::Vector2i gameArea)
{
	Engine* engine = Engine::GetInstance();
	float cellSize = engine->GetCellSize();

	_gridSize = gridSize;

	// Top left => top right
	for (int x = 0; x < _gridSize.x - 1; ++x)
	{
		AddCell(cellSize, x * cellSize, 0);
	}

	// Top right => bottom right
	for (int y = 0; y < _gridSize.y - 1; ++y)
	{
		AddCell(cellSize, gameArea.x - cellSize, y * cellSize);
	}

	// Bottom right => bottom left
	for (int x = _gridSize.x - 1; x > 0; --x)
	{
		AddCell(cellSize, x * cellSize, gameArea.y - cellSize);
	}

	// Bottom left => top left
	for (int y = 1; y < _gridSize.y; ++y)
	{
		AddCell(cellSize, 0, y * cellSize);
	}
}

bool Border::Collides(sf::Vector2i gridPosition) const
{
	return gridPosition.x == 0 || gridPosition.x == _gridSize.x - 1
		|| gridPosition.y == 0 || gridPosition.y == _gridSize.y - 1;
}

void Border::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (size_t i = 0; i < _cells.size(); ++i)
	{
		target.draw(*_cells[i]);
	}
}

void Border::AddCell(float cellSize, float posX, float posY)
{
	std::shared_ptr<sf::RectangleShape> cell = std::make_shared<sf::RectangleShape>(sf::Vector2f(cellSize, cellSize));
	// Alternate cells colors
	cell->setFillColor(_colors[_cells.size() % _colors.size()]);
	cell->setPosition(posX, posY);
	_cells.push_back(cell);
}

#include "Fruit.h"
#include "Engine.h"

Fruit::Fruit(sf::Vector2f size) :
	sf::RectangleShape(size)
{
	setFillColor(sf::Color::White);
}

bool Fruit::Collides(sf::Vector2i gridPosition) const
{
	sf::Vector2i fruitGridPosition = Engine::GetInstance()->WorldPositionToGridPosition(getPosition());
	return gridPosition.x == fruitGridPosition.x && gridPosition.y == fruitGridPosition.y;
}

void Fruit::Spawn()
{
	sf::Vector2i gridPosition;
	sf::Vector2i gridSize = Engine::GetInstance()->GetGridSize();
	do
	{
		// Random between borders
		gridPosition = sf::Vector2i(rand() % (gridSize.x - 1) + 1, rand() % (gridSize.y - 1) + 1);
	} while (Engine::GetInstance()->Collides(gridPosition));

	setPosition(Engine::GetInstance()->GridPositionToWorldPosition(gridPosition));
}

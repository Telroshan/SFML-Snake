#include "Fruit.h"
#include "Engine.h"

Fruit::Fruit(sf::Vector2f size) :
	sf::RectangleShape(size)
{
	setFillColor(sf::Color::White);
}

bool Fruit::Collides(sf::Vector2i gridPosition) const
{
	sf::Vector2i fruitGridPosition = Engine::GetInstance().WorldPositionToGridPosition(getPosition());
	return gridPosition.x == fruitGridPosition.x && gridPosition.y == fruitGridPosition.y;
}

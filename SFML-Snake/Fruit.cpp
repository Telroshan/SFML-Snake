#include "Fruit.h"

Fruit::Fruit(sf::Vector2f size) :
	sf::RectangleShape(size)
{
	setFillColor(sf::Color::White);
}

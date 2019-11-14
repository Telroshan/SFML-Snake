#pragma once

#include <SFML/Graphics.hpp>
#include "Collidable.h"

class Fruit : public sf::RectangleShape, public Collidable
{
public:
	Fruit(sf::Vector2f size, sf::Vector2i gridSize);

public:
	virtual bool Collides(sf::Vector2i gridPosition) const;
	void Spawn();

private:
	sf::Vector2i _gridSize;
};
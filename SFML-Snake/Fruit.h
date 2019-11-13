#pragma once

#include <SFML/Graphics.hpp>
#include "Collidable.h"

class Fruit : public sf::RectangleShape, public Collidable
{
public:
	Fruit(sf::Vector2f size);

public:
	virtual bool Collides(sf::Vector2i gridPosition) const;
};
#pragma once

#include "SFML/System.hpp"

class Collidable
{
public:
	virtual bool Collides(sf::Vector2i gridPosition) const = 0;
};
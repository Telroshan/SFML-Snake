#pragma once

#include "SFML/System.hpp"

// For objects that should collide with other Collidables
class Collidable
{
public:
	// Collisions per-cell ; no float coordinates, only cell-snapped positions
	virtual bool Collides(sf::Vector2i gridPosition) const = 0;
};
#pragma once

#include <SFML/Graphics.hpp>
#include "Collidable.h"

class Border : public sf::Drawable, public Collidable
{
public:
	Border(sf::Vector2i rectanglesCount);

public:
	virtual bool Collides(sf::Vector2i gridPosition) const;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Vector2i _rectanglesCount;
};
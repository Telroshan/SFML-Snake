#pragma once

#include <SFML/Graphics.hpp>
#include "Collidable.h"

class Border : public sf::Drawable, public Collidable
{
public:
	Border(sf::Vector2i gridSize, sf::Vector2i gameArea);

public:
	virtual bool Collides(sf::Vector2i gridPosition) const;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<std::shared_ptr<sf::RectangleShape>> _cells;
	sf::Vector2i _gridSize;
};
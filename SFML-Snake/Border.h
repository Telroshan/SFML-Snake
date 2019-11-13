#pragma once

#include <SFML/Graphics.hpp>

class Border : public sf::Drawable
{
public:
	Border(sf::Vector2i rectanglesCount);

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
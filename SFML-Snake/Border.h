#pragma once

#include <SFML/Graphics.hpp>
#include "Collidable.h"

// The map border, kills the player on collision
class Border : public sf::Drawable, public Collidable
{
public:
	Border(sf::Vector2i gridSize, sf::Vector2i gameArea);

public:
	virtual bool Collides(sf::Vector2i gridPosition) const;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	void AddCell(float cellSize, float posX, float posY);

private:
	std::vector<std::shared_ptr<sf::RectangleShape>> _cells;
	sf::Vector2i _gridSize;

	const std::vector<sf::Color> _colors = { sf::Color::Color(30, 30, 30), sf::Color::Color(60, 60, 60) };
};
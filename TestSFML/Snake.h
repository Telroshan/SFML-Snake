#pragma once

#include <SFML/Graphics.hpp>

class Snake
{
public:
	Snake(float radius, sf::Vector2f position);

public:
	void Render(sf::RenderWindow* window) const;
	const sf::Vector2f& GetPosition() const;
	void Move(sf::Vector2f movement);

private:
	std::vector<sf::CircleShape> _body;
};
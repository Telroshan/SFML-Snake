#pragma once

#include <SFML/Graphics.hpp>

class Snake
{
public:
	Snake(int size, float radius, sf::Vector2f position, sf::Vector2i direction);

public:
	void Render(sf::RenderWindow* window) const;
	const sf::Vector2f& GetPosition() const;
	void Move(sf::Vector2f movement);
	const sf::Vector2i& GetDirection() const;

	void SetDirection(const sf::Vector2i& direction);
	void InvertDirectionX();
	void InvertDirectionY();

private:
	std::vector<sf::CircleShape> _body;
	sf::Vector2i _direction;
};
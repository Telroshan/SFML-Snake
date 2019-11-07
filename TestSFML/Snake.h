#pragma once

#include <SFML/Graphics.hpp>

class Snake
{
public:
	Snake(int size, float length, sf::Vector2f position, sf::Vector2i direction);

public:
	void Render(sf::RenderWindow* window) const;
	const sf::Vector2f& GetHeadPosition() const;
	void Move(sf::Vector2f movement);
	const sf::Vector2i& GetDirection() const;

	void SetDirection(const sf::Vector2i& direction);
	void InvertDirection();

	void Grow();

	void Die();

	bool IsPositionInSnake(sf::Vector2i gridPosition, bool ignoreLastPart) const;

private:
	std::vector<sf::CircleShape> _body;
	sf::Vector2i _direction;
};
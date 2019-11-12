#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Snake : public sf::Drawable
{
public:
	Snake(int size, float length, sf::Vector2f position, sf::Vector2i direction);

public:
	const sf::Vector2f& GetHeadPosition() const;
	void Move(sf::Vector2f movement);
	const sf::Vector2i& GetDirection() const;

	void SetDirection(const sf::Vector2i& direction);
	void InvertDirection();

	void Grow();

	void Die();

	bool IsPositionInSnake(sf::Vector2i gridPosition, bool ignoreLastPart) const;

	bool IsDead() const;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<sf::CircleShape> _body;
	sf::Vector2i _direction;

	bool _dead;

	std::vector<sf::Color> _colors = { sf::Color::Green, sf::Color::Cyan };
};
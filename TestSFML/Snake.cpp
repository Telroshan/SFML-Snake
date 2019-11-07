#include "Snake.h"

Snake::Snake(int size, float radius, sf::Vector2f position, sf::Vector2i direction) :
	_body(size),
	_direction(direction)
{
	for (int i = 0; i < size; ++i) {
		_body[i] = sf::CircleShape(radius);
		_body[i].setPosition(position);
		_body[i].setFillColor(sf::Color::Green);
	}
}

void Snake::Render(sf::RenderWindow* window) const
{
	for (int i = (int) _body.size() - 1; i >= 0; --i)
	{
		window->draw(_body[i]);
	}
}

const sf::Vector2f& Snake::GetPosition() const
{
	return _body[0].getPosition();
}

void Snake::Move(sf::Vector2f movement)
{
	for (int i = (int) _body.size() - 1; i >= 0; --i)
	{
		_body[i].move(movement);
	}
}

const sf::Vector2i& Snake::GetDirection() const
{
	return _direction;
}

void Snake::SetDirection(const sf::Vector2i& direction)
{
	_direction = direction;
}

void Snake::InvertDirectionX()
{
	_direction.x *= -1;
}

void Snake::InvertDirectionY()
{
	_direction.y *= -1;
}

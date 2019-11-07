#include "Snake.h"

Snake::Snake(float radius, sf::Vector2f position) :
	_body(1)
{
	_body[0] = sf::CircleShape(radius);
	_body[0].setPosition(position);
	_body[0].setFillColor(sf::Color::Green);
}

void Snake::Render(sf::RenderWindow* window) const
{
	for (int i = _body.size() - 1; i >= 0; --i)
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
	for (int i = _body.size() - 1; i >= 0; --i)
	{
		_body[i].move(movement);
	}
}

#include "Snake.h"
#include <iostream>
#include "Engine.h"

Snake::Snake(int length, float radius, sf::Vector2f position, sf::Vector2i direction) :
	_body(length),
	_direction(direction)
{
	float bodypartWidth = radius * 2;
	for (int i = 0; i < length; ++i) {
		_body[i] = sf::CircleShape(radius);
		_body[i].setPosition(position - sf::Vector2f(i * direction.x * bodypartWidth, i * direction.y * bodypartWidth));
		_body[i].setFillColor(sf::Color::Green);
	}
	_body[0].setFillColor(sf::Color::Red);
}

void Snake::Render(sf::RenderWindow* window) const
{
	for (int i = (int)_body.size() - 1; i >= 0; --i)
	{
		window->draw(_body[i]);
	}
}

const sf::Vector2f& Snake::GetHeadPosition() const
{
	return _body[0].getPosition();
}

void Snake::Move(sf::Vector2f movement)
{
	for (int i = (int)_body.size() - 1; i >= 0; --i)
	{
		if (i > 0)
		{
			_body[i].setPosition(_body[i - 1].getPosition());
		}
		else
		{
			_body[i].move(movement);
		}
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

void Snake::InvertDirection()
{
	_direction *= -1;
}

void Snake::Die()
{
	std::cout << "DIE" << std::endl;
}

bool Snake::IsPositionInSnake(sf::Vector2i gridPosition, bool ignoreLastPart) const
{
	int max = (int)_body.size();
	if (ignoreLastPart) --max;
	for (int i = 0; i < max; ++i)
	{
		sf::Vector2i bodyPartPosition = Engine::GetInstance()->WorldPositionToGridPosition(_body[i].getPosition());
		if (bodyPartPosition.x == gridPosition.x && bodyPartPosition.y == gridPosition.y)
		{
			return true;
		}
	}

	return false;
}

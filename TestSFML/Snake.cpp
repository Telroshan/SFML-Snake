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

void Snake::Grow()
{
	float radius = _body[0].getRadius();
	float size = radius * 2.f;
	int bodyLength = (int)_body.size();

	sf::CircleShape bodypart = sf::CircleShape(radius);
	sf::CircleShape previous = _body[bodyLength - 1];
	sf::Vector2f position = previous.getPosition();
	sf::Vector2i gridPosition = Engine::GetInstance()->WorldPositionToGridPosition(position);

	sf::Vector2i direction = bodyLength > 1
		// If there's more than 2 parts, take the opposite of the direction between the 2 last parts
		? gridPosition - Engine::GetInstance()->WorldPositionToGridPosition(_body[bodyLength - 2].getPosition())
		// If there's only the head, take the opposite of the movement direction
		: _direction * -1;

	bodypart.setPosition(position - sf::Vector2f(direction.x * size, direction.y * size));
	bodypart.setFillColor(sf::Color::Green);
	_body.push_back(bodypart);
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

#include "Snake.h"
#include <iostream>
#include "Engine.h"

Snake::Snake(int length, float radius, sf::Vector2f position, sf::Vector2i direction) :
	_body(1),
	_direction(direction),
	_dead(false)
{
	_body[0] = sf::CircleShape(radius);
	_body[0].setPosition(position);
	_body[0].setFillColor(sf::Color::White);
	while (_body.size() < length)
	{
		Grow();
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
			_body[i].setPosition(_body[(size_t)i - 1].getPosition());
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
	size_t bodyLength = _body.size();

	sf::CircleShape bodypart = sf::CircleShape(radius);
	sf::CircleShape previous = _body[bodyLength - 1];
	sf::Vector2f position = previous.getPosition();
	sf::Vector2i gridPosition = Engine::GetInstance().WorldPositionToGridPosition(position);

	sf::Vector2i direction = bodyLength > 1
		// If there's more than 2 parts, take the direction between the 2 last parts
		? Engine::GetInstance().WorldPositionToGridPosition(_body[bodyLength - 2].getPosition()) - gridPosition
		// If there's only the head, take the movement direction
		: _direction;

	// Append to the opposite of the direction (growing from the tail, not the head)
	bodypart.setPosition(position - sf::Vector2f(direction.x * size, direction.y * size));
	bodypart.setFillColor(_colors[_body.size() % _colors.size()]);
	_body.push_back(bodypart);
}

void Snake::Die()
{
	_dead = true;
	for (int i = 0; i < _body.size(); ++i)
	{
		_body[i].setFillColor(sf::Color::Red);
	}
}

bool Snake::IsPositionInSnake(sf::Vector2i gridPosition, bool ignoreLastPart) const
{
	int max = (int)_body.size();
	if (ignoreLastPart) --max;
	for (int i = 0; i < max; ++i)
	{
		sf::Vector2i bodyPartPosition = Engine::GetInstance().WorldPositionToGridPosition(_body[i].getPosition());
		if (bodyPartPosition.x == gridPosition.x && bodyPartPosition.y == gridPosition.y)
		{
			return true;
		}
	}

	return false;
}

bool Snake::IsDead() const
{
	return _dead;
}

void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = (int)_body.size() - 1; i >= 0; --i)
	{
		target.draw(_body[i]);
	}
}
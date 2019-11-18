#include "Snake.h"
#include "Engine.h"
#include "GameData.h"
#include "AudioManager.h"

Snake::Snake(int length, float radius, sf::Vector2f position, sf::Vector2i direction) :
	_body(1),
	_direction(direction),
	_dead(false)
{
	_body[0] = sf::CircleShape(radius);
	_body[0].setPosition(position);
	_body[0].setFillColor(sf::Color(56, 255, 125));
	while (_body.size() < length)
	{
		Grow();
	}
	_moveInterval = _initialMoveInterval;
}

void Snake::Update(float deltaTime)
{
	if (IsDead()) return;

	_moveTimer += deltaTime;
	Engine* engine = Engine::GetInstance();
	float cellSize = engine->GetCellSize();

	if (_moveTimer > _moveInterval)
	{
		// Collision check in preview (thus before actually moving)
		sf::Vector2f nextHeadPosition = GetHeadPosition() + sf::Vector2f(_direction.x * cellSize, _direction.y * cellSize);
		sf::Vector2i nextHeadGridPosition = engine->WorldPositionToGridPosition(nextHeadPosition);

		sf::Vector2i lastPartGridPosition = engine->WorldPositionToGridPosition(_body[_body.size() - 1].getPosition());

		// Exclude the last part from collisions (as it will be moving forward)
		if (engine->Collides(nextHeadGridPosition) && nextHeadGridPosition != lastPartGridPosition)
		{
			Die();
			return;
		}

		if (_fruit && _fruit->Collides(nextHeadGridPosition))
		{
			Grow();
			++GameData::Score;
			_fruit->Spawn();
			AudioManager::PlaySound("Eat");
		}

		Move(sf::Vector2f(_direction.x * cellSize, _direction.y * cellSize));
		_moveTimer = 0.f;
	}
}

bool Snake::Collides(sf::Vector2i gridPosition) const
{
	int max = (int)_body.size();
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

const sf::Vector2f& Snake::GetHeadPosition() const
{
	return _body[0].getPosition();
}

void Snake::Move(sf::Vector2f movement)
{
	for (int i = (int)_body.size() - 1; i >= 0; --i)
	{
		if (i == 0)
		{
			// Only move the head in the specified direction
			_body[i].move(movement);
		}
		else
		{
			// The rest of the body just follows the ahead bodypart's direction
			_body[i].setPosition(_body[(size_t)i - 1].getPosition());
		}
	}
}

const sf::Vector2i& Snake::GetDirection() const
{
	return _direction;
}

void Snake::SetDirection(const sf::Vector2i& direction)
{
	if (!CanMoveTowards(direction))
	{
		return;
	}
	_direction = direction;
}

void Snake::Grow()
{
	float radius = _body[0].getRadius();
	float size = radius * 2.f;
	size_t bodyLength = _body.size();

	sf::CircleShape bodypart = sf::CircleShape(radius);
	sf::CircleShape previous = _body[bodyLength - 1];
	sf::Vector2f position = previous.getPosition();
	sf::Vector2i gridPosition = Engine::GetInstance()->WorldPositionToGridPosition(position);

	sf::Vector2i direction = bodyLength > 1
		// If there's more than 2 parts, take the direction between the 2 last parts
		? Engine::GetInstance()->WorldPositionToGridPosition(_body[bodyLength - 2].getPosition()) - gridPosition
		// If there's only the head, take the movement direction
		: _direction;

	// Append to the opposite of the direction (growing from the tail, not the head)
	bodypart.setPosition(position - sf::Vector2f(direction.x * size, direction.y * size));
	bodypart.setFillColor(_colors[_body.size() % _colors.size()]);
	_body.push_back(bodypart);

	// Accelerate
	_moveInterval *= _moveIntervalMultiplier;
	if (_moveInterval < _minMoveInterval)
	{
		_moveInterval = _minMoveInterval;
	}
}

void Snake::Die()
{
	_dead = true;
	for (int i = 0; i < _body.size(); ++i)
	{
		_body[i].setFillColor(sf::Color::Red);
	}
	AudioManager::PlaySound(GameData::Score > GameData::HighScore ? "GameOverNewHighScore" : "GameOver");
}

bool Snake::IsDead() const
{
	return _dead;
}

float Snake::GetMoveSpeed() const
{
	return _initialMoveInterval / _moveInterval;
}

void Snake::SetFruit(std::shared_ptr<Fruit> fruit)
{
	_fruit = fruit;
}

bool Snake::ReachedMaxSpeed() const
{
	return _moveInterval <= _minMoveInterval;
}

void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = (int)_body.size() - 1; i >= 0; --i)
	{
		target.draw(_body[i]);
	}
}

bool Snake::CanMoveTowards(sf::Vector2i direction) const
{
	if (_body.size() <= 1)
	{
		return true;
	}

	sf::Vector2i headGridPosition = Engine::GetInstance()->WorldPositionToGridPosition(_body[0].getPosition());
	sf::Vector2i secondPartGridPosition = Engine::GetInstance()->WorldPositionToGridPosition(_body[1].getPosition());

	return secondPartGridPosition - headGridPosition != direction;
}

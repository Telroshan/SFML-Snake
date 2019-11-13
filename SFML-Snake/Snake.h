#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Updatable.h"
#include "Collidable.h"
#include "Fruit.h"

class Snake : public sf::Drawable, public Updatable, public Collidable
{
public:
	Snake(int size, float length, sf::Vector2f position, sf::Vector2i direction);

public:
	virtual void Update(float deltaTime);
	virtual bool Collides(sf::Vector2i gridPosition) const;

	const sf::Vector2f& GetHeadPosition() const;
	void Move(sf::Vector2f movement);
	const sf::Vector2i& GetDirection() const;

	void SetDirection(const sf::Vector2i& direction);
	void InvertDirection();

	void Grow();

	void Die();

	bool IsDead() const;

	float GetMoveSpeed() const;

	void SetFruit(std::shared_ptr<Fruit> fruit);

	bool ReachedMaxSpeed() const;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	bool IsPositionInSnake(sf::Vector2i gridPosition, bool ignoreLastPart) const;
	bool CanMoveTowards(sf::Vector2i direction) const;

public:
	int score = 0;

private:
	std::vector<sf::CircleShape> _body;
	sf::Vector2i _direction;

	bool _dead;

	std::vector<sf::Color> _colors = { sf::Color::Green, sf::Color::Cyan };

	float _initialMoveInterval = .2f;
	float _moveInterval = _initialMoveInterval;
	float _moveTimer = 0.f;
	float _moveIntervalMultiplier = .9f;
	float _minMoveInterval = _initialMoveInterval / 3.f;

	std::shared_ptr<Fruit> _fruit;
};
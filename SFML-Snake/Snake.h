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
	// Moves the snake's entire body to the specified direction
	void Move(sf::Vector2f movement);
	const sf::Vector2i& GetDirection() const;

	void SetDirection(const sf::Vector2i& direction);

	// Grows the snake's body from 1 cell
	void Grow();

	void Die();

	bool IsDead() const;

	float GetMoveSpeed() const;

	void SetFruit(std::shared_ptr<Fruit> fruit);

	bool ReachedMaxSpeed() const;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	// Checks if the player can move in the specified direction (to prevent it from going backwards)
	bool CanMoveTowards(sf::Vector2i direction) const;

private:
	std::vector<sf::CircleShape> _body;
	sf::Vector2i _direction;

	bool _dead;

	std::vector<sf::Color> _colors = { sf::Color(242, 136, 43), sf::Color(59, 142, 165) };

	float _initialMoveInterval = .2f;
	// The used value to control the speed is the reverse of the human-friendly speed value => the lower the interval, the higher the speed
	float _moveInterval = _initialMoveInterval;
	float _moveTimer = 0.f;
	// The multiplier that should be applied to the current moveInterval each time the player eats a fruit
	float _moveIntervalMultiplier = .95f;
	// The minimum interval (ie the maximum speed) the player can reach
	float _minMoveInterval = _initialMoveInterval / 2.f;

	std::shared_ptr<Fruit> _fruit;
};
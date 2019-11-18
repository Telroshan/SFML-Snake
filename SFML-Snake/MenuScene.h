#pragma once

#include "Scene.h"
#include "Snake.h"

class MenuScene : public Scene
{
public:
	virtual void Init();
	virtual void UpdateInput();
	virtual void Update(float deltaTime);

private:
	std::shared_ptr<Snake> _snake;

	// The bounds in which the snake should animate
	sf::FloatRect _menuSnakePatrolBounds;
};
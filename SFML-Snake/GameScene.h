#pragma once

#include "Scene.h"
#include "Snake.h"
#include "Border.h"
#include "Fruit.h"

class GameScene : public Scene
{
public:
	virtual void Init();
	virtual void UpdateInput();
	virtual void Update(float deltaTime);

private:
	sf::Vector2i _gridSize;
	int _gameUiHeight = 100;

	std::shared_ptr<Border> _border;

	std::shared_ptr<Snake> _player;

	std::shared_ptr<Fruit> _fruit;

	float _timeElapsed = 0.f;
	float _gameOverDelay = 2.f;
	float _gameOverTimer = 0.f;
	std::shared_ptr<sf::Text> _timeText;

	std::shared_ptr<sf::Text> _scoreLabel;
	std::shared_ptr<sf::Text> _scoreText;

	std::shared_ptr<sf::Text> _speedLabel;
	std::shared_ptr<sf::Text> _speedText;
};
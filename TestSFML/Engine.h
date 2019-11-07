#pragma once

#include <SFML/Graphics.hpp>
#include "Snake.h"

class Engine
{
public:
	Engine(std::string title, sf::Vector2i windowSize);

	~Engine();

public:
	void UpdateInput();
	void Update(float deltaTime);
	void Render();
	//void Init();

	bool IsRunning() const;

private:
	void BuildBorder(float cellSize);
	void CheckCollisions(sf::Vector2f nextPosition);
	void DisplayScore();

	sf::Vector2i WorldPositionToGridPosition(sf::Vector2f position) const;
	bool IsPositionInBorder(sf::Vector2i gridPosition) const;

public:
	void SetCellSize(float cellSize);
	void SetMoveSpeed(float speed);

private:
	sf::RenderWindow* _window;
	sf::Vector2i _windowSize;

	float _cellRadius;
	sf::Vector2i _rectanglesCount;

	std::vector<sf::RectangleShape> _border;

	Snake* _player;

	float _moveInterval;

	float _moveTimestamp;

	int _score;
	sf::Text _scoreText;
	sf::Font _font;
};
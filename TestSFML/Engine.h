#pragma once

#include <SFML/Graphics.hpp>
#include "Snake.h"

enum Mode
{
	Menu = 0,
	Game = 1,
	GameOver = 2,
};

class Engine
{
public:
	Engine(std::string title, sf::Vector2i windowSize);

	~Engine();

	static const Engine* GetInstance();

public:
	void UpdateInput();
	void Update(float deltaTime);
	void Render();
	void Init();

	bool IsRunning() const;

	sf::Vector2i WorldPositionToGridPosition(sf::Vector2f position) const;

private:
	void BuildBorder(float cellSize);
	void CheckCollisions(sf::Vector2f nextPosition);
	void DisplayScore();

	bool IsPositionInBorder(sf::Vector2i gridPosition) const;

	void PlaceFruit();

	void UpdateInputMenu();
	void UpdateInputGame();

	void UpdatePlayer(float deltaTime);

	void RenderMenu();
	void RenderGame();
	void RenderEndScreen();

public:
	void SetCellSize(float cellSize);
	void SetMoveSpeed(float speed);

private:
	static Engine* _instance;

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

	sf::RectangleShape _fruit;

	Mode _mode;

	sf::Text _playText;
};
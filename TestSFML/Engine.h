#pragma once

#include <SFML/Graphics.hpp>
#include "Snake.h"

enum Mode
{
	Menu = 0,
	Game = 1,
	Endscreen = 2,
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

	bool IsPositionInBorder(sf::Vector2i gridPosition) const;

	void PlaceFruit();

	void UpdateInputMenu();
	void UpdateInputGame();

	void UpdateGame(float deltaTime);

	void RenderMenu();
	void RenderGame();
	void RenderEndScreen();
	
	void InitGame();

	void SetMode(Mode mode);

	void SetScore(int score);

	void InitText(sf::Text& text);

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

	float _moveTimer;

	sf::Text _gameTitle;

	int _score;
	sf::Text _scoreText;
	sf::Font _font;

	sf::RectangleShape _fruit;
	sf::Text _playText;
	sf::Text _exitText;

	Mode _mode;

	float _gameOverDelay;
	float _gameOverTimer;

	sf::Text _gameOverText;
	sf::Text _beatScoreText;
	sf::Text _finalScoreText;
};
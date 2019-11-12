#pragma once

#include <SFML/Graphics.hpp>
#include "Snake.h"

enum class Mode
{
	Menu,
	Game,
	Endscreen,
};

class Engine
{
public:
	static Engine& GetInstance();

private:
	Engine();
	~Engine();

public:
	void UpdateInput();
	void Update(float deltaTime);
	void Render();
	void Init(std::string title, sf::Vector2i windowSize, int gameUiHeight, float cellSize, float moveSpeed, float moveSpeedMultiplier);

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

	void SetupTexts();
	void InitText(sf::Text& text);

	std::string IntToStringWithZeros(int value, int textLength) const;

public:
	void SetCellSize(float cellSize);

private:
	static Engine _instance;

	std::shared_ptr<sf::RenderWindow> _window;
	sf::Vector2i _windowSize;

	int _gameUiHeight = 0;

	float _cellRadius = 20.f;
	sf::Vector2i _rectanglesCount;

	std::vector<sf::RectangleShape> _border;

	std::shared_ptr<Snake> _player;

	float _moveInterval = 1.f;
	float _moveTimer = 0.f;

	float _moveIntervalMultiplier = 1.f;

	sf::Text _gameTitle;

	int _score = 0;
	sf::Text _timeLabel;
	sf::Text _timeText;
	sf::Text _scoreLabel;
	sf::Text _scoreText;
	sf::Font _font;

	sf::RectangleShape _fruit;
	sf::Text _playText;
	sf::Text _exitText;

	Mode _mode = Mode::Menu;

	float _gameOverDelay = 1.f;
	float _gameOverTimer = 0.f;

	float _timeElapsed = 0.f;

	sf::Text _gameOverText;
	sf::Text _beatScoreText;
	sf::Text _finalScoreText;
};
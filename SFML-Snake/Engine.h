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
	void Init(std::string title, sf::Vector2i windowSize, int gameUiHeight, float cellSize);

	bool IsRunning() const;

	sf::Vector2i WorldPositionToGridPosition(sf::Vector2f position) const;

	void CheckCollisions(sf::Vector2f nextPosition);

private:
	void BuildBorder();

	bool IsPositionInBorder(sf::Vector2i gridPosition) const;

	void PlaceFruit();

	void UpdateInputMenu();
	void UpdateInputGame();
	void UpdateInputEndscreen();

	void UpdateGame(float deltaTime);

	void InitMenu();
	void InitGame();
	void InitEndscreen();

	void SetMode(Mode mode);

	void SetScore(int score);

	std::shared_ptr<sf::Text> InitText(Mode mode, const std::string& content);

	void RegisterDrawable(std::shared_ptr<sf::Drawable> drawable, Mode mode);
	void RegisterUpdatable(std::shared_ptr<Updatable> updatable, Mode mode);

	std::string GetFormattedNumericString(const std::string& string, int textLength) const;

	void ReadHighScore();
	void SaveHighScore();

public:
	void SetCellSize(float cellSize);
	float GetCellSize() const;

private:
	static Engine _instance;

	std::shared_ptr<sf::RenderWindow> _window;
	sf::Vector2i _windowSize;

	Mode _mode = Mode::Menu;

	std::map<Mode, std::vector<std::shared_ptr<sf::Drawable>>> _drawables;
	std::map<Mode, std::vector<std::shared_ptr<Updatable>>> _updatables;

	int _gameUiHeight = 0;

	float _cellSize = 20.f;
	sf::Vector2i _rectanglesCount;

	std::shared_ptr<Snake> _player;
	std::shared_ptr<sf::RectangleShape> _fruit;

	const char* scoresFilename = "scores.data";
	int _score = 0;
	int _highScore = 0;

	sf::Font _font;
	std::shared_ptr<sf::Text> _timeText;
	std::shared_ptr<sf::Text> _scoreLabel;
	std::shared_ptr<sf::Text> _scoreText;
	std::shared_ptr<sf::Text> _speedText;

	float _gameOverDelay = 1.f;
	float _gameOverTimer = 0.f;

	float _timeElapsed = 0.f;
};
#pragma once

#include <SFML/Graphics.hpp>
#include "Snake.h"
#include "Fruit.h"
#include "Border.h"

enum class Scene
{
	Menu,
	Game,
	Endscreen,
};

class Engine
{
public:
	static Engine* GetInstance();

	~Engine();

private:
	Engine();

public:
	void UpdateInput();
	void Update(float deltaTime);
	void Render();
	void Init(std::string title, sf::Vector2i windowSize, int gameUiHeight, float cellSize);

	bool IsRunning() const;

	sf::Vector2i WorldPositionToGridPosition(sf::Vector2f position) const;
	sf::Vector2f GridPositionToWorldPosition(sf::Vector2i position) const;

	bool Collides(sf::Vector2i gridPosition) const;

private:
	void UpdateInputMenu();
	void UpdateInputGame();
	void UpdateInputEndscreen();

	void UpdateMenu(float deltaTime);
	void UpdateGame(float deltaTime);

	void InitMenu();
	void InitGame();
	void InitEndscreen();

	void SetScene(Scene scene);

	std::shared_ptr<sf::Text> InitText(const std::string& content);

	void RegisterDrawable(std::shared_ptr<sf::Drawable> drawable);
	void RegisterUpdatable(std::shared_ptr<Updatable> updatable);
	void RegisterCollidable(std::shared_ptr<Collidable> collidable);

	std::string GetFormattedNumericString(const std::string& string, int textLength) const;

	void ReadHighScore();
	void SaveHighScore();

	sf::Color GetScoreColor() const;

public:
	void SetCellSize(float cellSize);
	float GetCellSize() const;
	const sf::Vector2i GetWindowSize() const;
	const sf::Vector2i GetGridSize() const;
	const int GetGameUiHeight() const;

private:
	static Engine* _instance;

	std::shared_ptr<sf::RenderWindow> _window;
	sf::Vector2i _windowSize;

	Scene _scene = Scene::Menu;

	std::vector<std::shared_ptr<sf::Drawable>> _drawables;
	std::vector<std::shared_ptr<Updatable>> _updatables;
	std::vector<std::shared_ptr<Collidable>> _collidables;

	int _gameUiHeight = 0;

	float _cellSize = 20.f;
	sf::Vector2i _gridSize;

	std::shared_ptr<Border> _border;
	std::shared_ptr<Snake> _player;
	std::shared_ptr<Fruit> _fruit;

	const char* scoresFilename = "scores.data";
	int _highScore = 0;

	sf::Font _font;
	std::shared_ptr<sf::Text> _timeText;
	std::shared_ptr<sf::Text> _scoreLabel;
	std::shared_ptr<sf::Text> _scoreText;
	std::shared_ptr<sf::Text> _speedLabel;
	std::shared_ptr<sf::Text> _speedText;

	float _gameOverDelay = 1.f;
	float _gameOverTimer = 0.f;

	float _timeElapsed = 0.f;

	sf::FloatRect _menuSnakePatrolBounds;
};
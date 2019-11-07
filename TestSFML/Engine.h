#pragma once

#include <SFML/Graphics.hpp>

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
	void BuildBorder(sf::Vector2f cellSize);
	void CheckCollisions(sf::Vector2f nextPosition);
	sf::Vector2i GetPlayerGridPosition() const;
	void DisplayScore();

public:
	void SetCellSize(sf::Vector2f cellSize);
	void SetMoveSpeed(float speed);

private:
	sf::RenderWindow* _window;
	sf::Vector2i _windowSize;

	sf::Vector2f _cellSize;
	sf::Vector2i _rectanglesCount;

	std::vector<sf::RectangleShape> _border;

	sf::RectangleShape* _player;

	sf::Vector2i _playerDirection;

	float _moveInterval;

	float _moveTimestamp;

	int _score;
	sf::Text _scoreText;
	sf::Font _font;
};
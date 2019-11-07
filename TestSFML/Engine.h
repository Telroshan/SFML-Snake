#pragma once

#include <SFML/Graphics.hpp>

class Engine
{
public:
	Engine(std::string title, sf::Vector2i windowSize);

	~Engine();

public:
	void Init(sf::Vector2f borderCellSize);
	void UpdateInput();
	void Update();
	void Render();

	bool IsRunning() const;

private:
	void BuildBorder(sf::Vector2f cellSize);

private:
	sf::RenderWindow* _window;
	sf::Vector2i _windowSize;

	std::vector<sf::RectangleShape> _border;
};
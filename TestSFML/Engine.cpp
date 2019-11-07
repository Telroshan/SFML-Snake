#include "Engine.h"
#include <iostream>

Engine::Engine(std::string title, sf::Vector2i windowSize) :
	_windowSize(windowSize),
	_playerDirection(1.f, 0.f),
	_score(0)
{
	_window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), title);

	if (!_font.loadFromFile("Fonts\\arial.ttf"))
	{
		std::cerr << "Couldn't load score font" << std::endl;
	}
	_scoreText.setFont(_font);
	_scoreText.setString("000");
	_scoreText.setFillColor(sf::Color::White);
	_scoreText.setPosition(windowSize.x - 75, windowSize.y - 60);
}

Engine::~Engine()
{
	delete _window;
	delete _player;
}

void Engine::UpdateInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		_playerDirection.x = 0.f;
		_playerDirection.y = -1.f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		_playerDirection.x = 0.f;
		_playerDirection.y = 1.f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_playerDirection.x = 1.f;
		_playerDirection.y = 0.f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		_playerDirection.x = -1.f;
		_playerDirection.y = 0.f;
	}
}

void Engine::Update(float deltaTime)
{
	sf::Event event;
	while (_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			_window->close();
			return;
		}
	}

	_moveTimestamp += deltaTime;
	if (_moveTimestamp > _moveInterval) {
		sf::Vector2f nextPosition = _player->getPosition() + sf::Vector2f(_playerDirection.x * _cellSize.x, _playerDirection.y * _cellSize.y);
		CheckCollisions(nextPosition);
		_player->move(sf::Vector2f(_playerDirection.x * _cellSize.x, _playerDirection.y * _cellSize.y));
		_moveTimestamp = 0.f;
	}
}

void Engine::Render()
{
	_window->clear();

	for (int i = 0; i < _border.size(); ++i)
	{
		_window->draw(_border.at(i));
	}

	_window->draw(*_player);

	DisplayScore();

	_window->display();
}

bool Engine::IsRunning() const
{
	return _window->isOpen();
}

void Engine::BuildBorder(sf::Vector2f cellSize)
{
	_border.empty();

	const int colorsLength = 2;
	sf::Color colors[colorsLength] = { sf::Color::Cyan, sf::Color::Blue };
	int colorIndex = 0;

	// Top left => top right
	for (int x = 0; x < _rectanglesCount.x - 1; ++x)
	{
		sf::RectangleShape cell(cellSize);
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(x * cellSize.x, 0);
		_border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Top right => bottom right
	for (int y = 0; y < _rectanglesCount.y - 1; ++y)
	{
		sf::RectangleShape cell(cellSize);
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(_windowSize.x - cellSize.x, y * cellSize.y);
		_border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Bottom right => bottom left
	for (int x = _rectanglesCount.x - 1; x > 0; --x)
	{
		sf::RectangleShape cell(cellSize);
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(x * cellSize.x, _windowSize.y - cellSize.y);
		_border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Bottom left => top left
	for (int y = 1; y < _rectanglesCount.y; ++y)
	{
		sf::RectangleShape cell(cellSize);
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(0, y * cellSize.y);
		_border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}
}

void Engine::CheckCollisions(sf::Vector2f nextPosition)
{
	sf::Vector2i playerGridPosition = GetPlayerGridPosition();
	std::cout << playerGridPosition.x << ", " << playerGridPosition.y << " | " << _rectanglesCount.x << ", " << _rectanglesCount.y << std::endl;
	if (playerGridPosition.x <= 1 || playerGridPosition.x >= _rectanglesCount.x - 2)
	{
		_playerDirection.x *= -1.f;
	}
	else if (playerGridPosition.y <= 1 || playerGridPosition.y >= _rectanglesCount.y - 2)
	{
		_playerDirection.y *= -1.f;
	}
}

sf::Vector2i Engine::GetPlayerGridPosition() const
{
	sf::Vector2f playerPosition = _player->getPosition();
	sf::Vector2i gridPosition(playerPosition.x / _cellSize.x, playerPosition.y / _cellSize.y);
	return gridPosition;
}

void Engine::DisplayScore()
{
	std::string text = std::to_string(_score);
	while (text.length() < 3) {
		text = "0" + text;
	}
	_scoreText.setString(text);
	_window->draw(_scoreText);
}

void Engine::SetCellSize(sf::Vector2f cellSize)
{
	_cellSize = cellSize;

	_rectanglesCount = sf::Vector2i(_windowSize.x / (int)cellSize.x, _windowSize.y / (int)cellSize.y);

	_player = new sf::RectangleShape(cellSize);
	_player->setPosition((_rectanglesCount.x / 2) * cellSize.x, (_rectanglesCount.y / 2) * cellSize.y);
	_player->setFillColor(sf::Color::Green);

	BuildBorder(cellSize);
}

void Engine::SetMoveSpeed(float speed)
{
	_moveInterval = 1.f / speed;
}

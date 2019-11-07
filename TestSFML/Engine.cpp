#include "Engine.h"
#include <iostream>

Engine::Engine(std::string title, sf::Vector2i windowSize) :
	_windowSize(windowSize),
	_playerDirection(1, 0),
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
	_scoreText.setPosition(windowSize.x - 75.f, windowSize.y - 60.f);
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
		_playerDirection.x = 0;
		_playerDirection.y = -1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		_playerDirection.x = 0;
		_playerDirection.y = 1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_playerDirection.x = 1;
		_playerDirection.y = 0;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		_playerDirection.x = -1;
		_playerDirection.y = 0;
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
		sf::Vector2f nextPosition = _player->getPosition() + sf::Vector2f(_playerDirection.x * _cellRadius, _playerDirection.y * _cellRadius);
		CheckCollisions(nextPosition);
		_player->move(sf::Vector2f(_playerDirection.x * _cellRadius, _playerDirection.y * _cellRadius));
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

void Engine::BuildBorder(float cellRadius)
{
	_border.empty();

	const int colorsLength = 2;
	sf::Color colors[colorsLength] = { sf::Color::Cyan, sf::Color::Blue };
	int colorIndex = 0;

	// Top left => top right
	for (int x = 0; x < _rectanglesCount.x - 1; ++x)
	{
		sf::RectangleShape cell(sf::Vector2f(cellRadius, cellRadius));
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(x * cellRadius, 0);
		_border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Top right => bottom right
	for (int y = 0; y < _rectanglesCount.y - 1; ++y)
	{
		sf::RectangleShape cell(sf::Vector2f(cellRadius, cellRadius));
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(_windowSize.x - cellRadius, y * cellRadius);
		_border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Bottom right => bottom left
	for (int x = _rectanglesCount.x - 1; x > 0; --x)
	{
		sf::RectangleShape cell(sf::Vector2f(cellRadius, cellRadius));
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(x * cellRadius, _windowSize.y - cellRadius);
		_border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Bottom left => top left
	for (int y = 1; y < _rectanglesCount.y; ++y)
	{
		sf::RectangleShape cell(sf::Vector2f(cellRadius, cellRadius));
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(0, y * cellRadius);
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
		_playerDirection.x *= -1;
	}
	else if (playerGridPosition.y <= 1 || playerGridPosition.y >= _rectanglesCount.y - 2)
	{
		_playerDirection.y *= -1;
	}
}

sf::Vector2i Engine::GetPlayerGridPosition() const
{
	sf::Vector2f playerPosition = _player->getPosition();
	sf::Vector2i gridPosition((int) (playerPosition.x / _cellRadius), (int) (playerPosition.y / _cellRadius));
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

void Engine::SetCellSize(float cellRadius)
{
	_cellRadius = cellRadius;

	_rectanglesCount = sf::Vector2i(_windowSize.x / (int)cellRadius, _windowSize.y / (int)cellRadius);

	_player = new sf::CircleShape(cellRadius / 2.f);
	_player->setPosition((_rectanglesCount.x / 2) * cellRadius, (_rectanglesCount.y / 2) * cellRadius);
	_player->setFillColor(sf::Color::Green);

	BuildBorder(cellRadius);
}

void Engine::SetMoveSpeed(float speed)
{
	_moveInterval = 1.f / speed;
}

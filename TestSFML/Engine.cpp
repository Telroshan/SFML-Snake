#include "Engine.h"
#include <iostream>

Engine* Engine::_instance{ nullptr };

Engine::Engine(std::string title, sf::Vector2i windowSize) :
	_windowSize(windowSize),
	_score(0)
{
	_instance = this;

	_window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), title);

	if (!_font.loadFromFile("Fonts\\arial.ttf"))
	{
		std::cerr << "Couldn't load score font" << std::endl;
	}
	_scoreText.setFont(_font);
	_scoreText.setFillColor(sf::Color::White);
	_scoreText.setPosition(windowSize.x - 75.f, windowSize.y - 60.f);
}

Engine::~Engine()
{
	delete _window;
	delete _player;
}

const Engine* Engine::GetInstance()
{
	return Engine::_instance;
}

void Engine::UpdateInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		_player->SetDirection(sf::Vector2i(0, -1));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		_player->SetDirection(sf::Vector2i(0, 1));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_player->SetDirection(sf::Vector2i(1, 0));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		_player->SetDirection(sf::Vector2i(-1, 0));
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
		sf::Vector2i direction = _player->GetDirection();
		sf::Vector2f nextHeadPosition = _player->GetHeadPosition() + sf::Vector2f(direction.x * _cellRadius, direction.y * _cellRadius);
		CheckCollisions(nextHeadPosition);
		direction = _player->GetDirection();
		_player->Move(sf::Vector2f(direction.x * _cellRadius, direction.y * _cellRadius));
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

	_player->Render(_window);

	DisplayScore();

	_window->display();
}

void Engine::Init()
{
	_score = 0;

	_scoreText.setString("000");

	_player = new Snake(3, _cellRadius / 2.f, sf::Vector2f((_rectanglesCount.x / 2) * _cellRadius, (_rectanglesCount.y / 2) * _cellRadius), sf::Vector2i(1, 0));
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

void Engine::CheckCollisions(sf::Vector2f nextHeadPosition)
{
	sf::Vector2i nextHeadGridPosition = WorldPositionToGridPosition(nextHeadPosition);

	std::cout << "Grid pos : " << nextHeadGridPosition.x << ", " << nextHeadGridPosition.y
		<< " \tGrid size : " << _rectanglesCount.x << ", " << _rectanglesCount.y
		<< " \tWorld pos : " << nextHeadPosition.x << ", " << nextHeadPosition.y
		<< " \tDirection : " << _player->GetDirection().x << ", " << _player->GetDirection().y << std::endl;

	if (IsPositionInBorder(nextHeadGridPosition))
	{
		_player->InvertDirection();
	}

	if (_player->IsPositionInSnake(nextHeadGridPosition, true))
	{
		_player->Die();
	}
}

sf::Vector2i Engine::WorldPositionToGridPosition(sf::Vector2f position) const
{
	sf::Vector2i gridPosition((int)(position.x / _cellRadius), (int)(position.y / _cellRadius));
	return gridPosition;
}

bool Engine::IsPositionInBorder(sf::Vector2i gridPosition) const
{
	return gridPosition.x == 0 || gridPosition.x == _rectanglesCount.x - 1
		|| gridPosition.y == 0 || gridPosition.y == _rectanglesCount.y - 1;
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

	BuildBorder(cellRadius);
}

void Engine::SetMoveSpeed(float speed)
{
	_moveInterval = 1.f / speed;
}

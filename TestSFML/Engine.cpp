#include "Engine.h"

Engine::Engine(std::string title, sf::Vector2i windowSize) :
	_windowSize(windowSize),
	_playerDirection(1.f, 0.f)
{
	_window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), title);
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

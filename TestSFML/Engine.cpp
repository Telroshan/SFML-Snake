#include "Engine.h"

Engine::Engine(std::string title, sf::Vector2i windowSize) : _windowSize(windowSize)
{
	_window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), title);
}

Engine::~Engine()
{
	delete _window;
}

void Engine::Init(sf::Vector2f borderCellSize)
{
	BuildBorder(borderCellSize);
}

void Engine::UpdateInput()
{

}

void Engine::Update()
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
}

void Engine::Render()
{
	_window->clear();

	for (int i = 0; i < _border.size(); ++i)
	{
		_window->draw(_border.at(i));
	}

	_window->display();
}

bool Engine::IsRunning() const
{
	return _window->isOpen();
}

void Engine::BuildBorder(sf::Vector2f cellSize)
{
	sf::Vector2i rectanglesCount(_windowSize.x / (int)cellSize.x, _windowSize.y / (int)cellSize.y);

	const int colorsLength = 2;
	sf::Color colors[colorsLength] = { sf::Color::Cyan, sf::Color::Blue };
	int colorIndex = 0;

	// Top left => top right
	for (int x = 0; x < rectanglesCount.x - 1; ++x)
	{
		sf::RectangleShape cell(cellSize);
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(x * cellSize.x, 0);
		_border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Top right => bottom right
	for (int y = 0; y < rectanglesCount.y - 1; ++y)
	{
		sf::RectangleShape cell(cellSize);
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(_windowSize.x - cellSize.x, y * cellSize.y);
		_border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Bottom right => bottom left
	for (int x = rectanglesCount.x - 1; x > 0; --x)
	{
		sf::RectangleShape cell(cellSize);
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(x * cellSize.x, _windowSize.y - cellSize.y);
		_border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Bottom left => top left
	for (int y = 1; y < rectanglesCount.y; ++y)
	{
		sf::RectangleShape cell(cellSize);
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(0, y * cellSize.y);
		_border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}
}

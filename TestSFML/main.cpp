#include <SFML/Graphics.hpp>

void traceBorder(sf::Vector2i windowSize, sf::Vector2f cellSize, std::vector<sf::RectangleShape>& border)
{
	sf::Vector2i rectanglesCount(windowSize.x / (int)cellSize.x, windowSize.y / (int)cellSize.y);

	const int colorsLength = 2;
	sf::Color colors[colorsLength] = { sf::Color::Cyan, sf::Color::Blue };
	int colorIndex = 0;

	for (int x = 0; x < rectanglesCount.x - 1; ++x)
	{
		sf::RectangleShape cell(cellSize);
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(x * cellSize.x, 0);
		border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	for (int y = 0; y < rectanglesCount.y - 1; ++y)
	{
		sf::RectangleShape cell(cellSize);
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(windowSize.x - cellSize.x, y * cellSize.y);
		border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	for (int x = rectanglesCount.x - 1; x > 0; --x)
	{
		sf::RectangleShape cell(cellSize);
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(x * cellSize.x, windowSize.y - cellSize.y);
		border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	for (int y = 1; y < rectanglesCount.y; ++y)
	{
		sf::RectangleShape cell(cellSize);
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(0, y * cellSize.y);
		border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}
}

int main()
{
	sf::Vector2i windowSize(600, 400);

	sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Snake");

	sf::Vector2f rectangleSize(20, 20);
	float left = 0.f;
	float top = 0.f;
	float right = windowSize.x - rectangleSize.x;
	float bottom = windowSize.y - rectangleSize.y;

	std::vector<sf::RectangleShape> border;
	traceBorder(windowSize, rectangleSize, border);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		for (int i = 0; i < border.size(); ++i)
		{
			window.draw(border.at(i));
		}

		window.display();
	}

	return 0;
}
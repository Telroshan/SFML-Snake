#include <SFML/Graphics.hpp>

void traceBorder(sf::Vector2i windowSize, sf::Vector2f cellSize, std::vector<sf::RectangleShape>& border)
{
	sf::Vector2i rectanglesCount(windowSize.x / (int)cellSize.x, windowSize.y / (int)cellSize.y);

	for (int x = 0; x < rectanglesCount.x; ++x)
	{
		for (int y = 0; y < 2; ++y)
		{
			sf::RectangleShape cell(cellSize);
			cell.setFillColor(sf::Color::Cyan);
			cell.setPosition(x * cellSize.x, y * (windowSize.y - cellSize.y));
			border.push_back(cell);
		}
	}

	for (int y = 0; y < rectanglesCount.y; ++y)
	{
		for (int x = 0; x < 2; ++x)
		{
			sf::RectangleShape cell(cellSize);
			cell.setFillColor(sf::Color::Cyan);
			cell.setPosition(x * (windowSize.x - cellSize.x), y * cellSize.y);
			border.push_back(cell);
		}
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

	sf::RectangleShape rectangle1(rectangleSize);
	rectangle1.setFillColor(sf::Color::Green);
	rectangle1.setPosition((windowSize.x - rectangleSize.x) / 2.f, (windowSize.y - rectangleSize.y) / 2.f);

	sf::RectangleShape rectangle2(rectangleSize);
	rectangle2.setFillColor(sf::Color::Red);
	rectangle2.setPosition(left, top);

	sf::RectangleShape rectangle3(rectangleSize);
	rectangle3.setFillColor(sf::Color::Blue);
	rectangle3.setPosition(left, bottom);

	sf::RectangleShape rectangle4(rectangleSize);
	rectangle4.setFillColor(sf::Color::Magenta);
	rectangle4.setPosition(right, top);

	sf::RectangleShape rectangle5(rectangleSize);
	rectangle5.setFillColor(sf::Color::Yellow);
	rectangle5.setPosition(right, bottom);

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

		window.draw(rectangle1);
		window.draw(rectangle2);
		window.draw(rectangle3);
		window.draw(rectangle4);
		window.draw(rectangle5);

		window.display();
	}

	return 0;
}
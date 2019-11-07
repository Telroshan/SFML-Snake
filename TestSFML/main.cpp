#include <SFML/Graphics.hpp>

int main()
{
	int width = 600;
	int height = 400;

	sf::RenderWindow window(sf::VideoMode(width, height), "Snake");

	sf::Vector2f rectangleSize(20.f, 20.f);
	sf::RectangleShape shape(rectangleSize);
	shape.setFillColor(sf::Color::Green);
	shape.setPosition((width - rectangleSize.x) / 2.f, (height - rectangleSize.y) / 2.f);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		
		window.draw(shape);

		window.display();
	}

	return 0;
}
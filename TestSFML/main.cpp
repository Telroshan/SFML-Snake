#include <SFML/Graphics.hpp>

int main()
{
	int width = 600;
	int height = 400;

	sf::RenderWindow window(sf::VideoMode(width, height), "Snake");

	sf::RectangleShape shape(sf::Vector2f(20.f, 20.f));
	shape.setFillColor(sf::Color::Green);
	shape.setPosition(0, 0);

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
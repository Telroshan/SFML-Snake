#include <SFML/Graphics.hpp>

int main()
{
	int width = 600;
	int height = 400;

	sf::RenderWindow window(sf::VideoMode(width, height), "Snake");

	sf::Vector2f rectangleSize(20.f, 20.f);
	float left = 0.f;
	float top = 0.f;
	float right = width - rectangleSize.x;
	float bottom = height - rectangleSize.y;

	sf::RectangleShape rectangle1(rectangleSize);
	rectangle1.setFillColor(sf::Color::Green);
	rectangle1.setPosition((width - rectangleSize.x) / 2.f, (height - rectangleSize.y) / 2.f);

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

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		
		window.draw(rectangle1);
		window.draw(rectangle2);
		window.draw(rectangle3);
		window.draw(rectangle4);
		window.draw(rectangle5);

		window.display();
	}

	return 0;
}
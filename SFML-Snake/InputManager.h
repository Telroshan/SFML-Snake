#pragma once

#include <SFML/Window.hpp>
#include <map>

class InputManager
{
private:
	InputManager();

public:
	static void Update();
	static bool IsKeyPressed(sf::Keyboard::Key key);
	static bool WasKeyPressedThisFrame(sf::Keyboard::Key key);

private:
	static InputManager _instance;

	std::map<sf::Keyboard::Key, bool> _inputsPressedThisFrame;
	std::map<sf::Keyboard::Key, bool> _inputsPressed;
};
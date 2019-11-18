#pragma once

#include <SFML/Window.hpp>
#include <map>

// A class to handle inputs across frames
class InputManager
{
private:
	InputManager();

public:
	// Updates the inputs states
	static void Update();
	// Returns true if a key is currently pressed
	static bool IsKeyPressed(sf::Keyboard::Key key);
	// Returns true if a key was pressed this frame (thus wasn't pressed the last frame)
	static bool WasKeyPressedThisFrame(sf::Keyboard::Key key);

private:
	static InputManager _instance;

	std::map<sf::Keyboard::Key, bool> _inputsPressedThisFrame;
	std::map<sf::Keyboard::Key, bool> _inputsPressed;
};
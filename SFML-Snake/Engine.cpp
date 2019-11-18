#include "Engine.h"
#include <iostream>
#include "InputManager.h"
#include "MenuScene.h"

Engine* Engine::_instance{ nullptr };

Engine* Engine::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new Engine();
	}
	return _instance;
}

Engine::Engine()
{
	if (!_font.loadFromFile("Fonts\\PressStart2P.ttf"))
	{
		std::cerr << "Couldn't load font" << std::endl;
	}
}

Engine::~Engine()
{
}

void Engine::UpdateInput()
{
	InputManager::Update();

	if (_scene)
	{
		_scene->UpdateInput();
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

	for (size_t i = 0; i < _updatables.size(); ++i)
	{
		_updatables[i]->Update(deltaTime);
	}

	if (_scene)
	{
		_scene->Update(deltaTime);
	}
}

void Engine::Render()
{
	_window->clear();

	for (size_t i = 0; i < _drawables.size(); ++i)
	{
		_window->draw(*_drawables[i]);
	}

	_window->display();
}

void Engine::Init()
{
	_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(_windowSize.x, _windowSize.y), "Snake", sf::Style::Titlebar | sf::Style::Close);

	_window->setFramerateLimit(60);

	sf::Image icon;
	icon.loadFromFile("Images\\icon.png");
	_window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	LoadScene<MenuScene>();
}

bool Engine::IsRunning() const
{
	return _window->isOpen();
}

bool Engine::HasFocus() const
{
	return _window->hasFocus();
}

bool Engine::Collides(sf::Vector2i gridPosition) const
{
	for (size_t i = 0; i < _collidables.size(); ++i)
	{
		if (_collidables[i]->Collides(gridPosition))
		{
			return true;
		}
	}

	return false;
}

void Engine::Quit()
{
	_window->close();
}

sf::Vector2i Engine::WorldPositionToGridPosition(sf::Vector2f position) const
{
	sf::Vector2i gridPosition((int)(position.x / _cellSize), (int)(position.y / _cellSize));
	return gridPosition;
}

sf::Vector2f Engine::GridPositionToWorldPosition(sf::Vector2i position) const
{
	sf::Vector2f worldPosition(position.x * _cellSize, position.y * _cellSize);
	return worldPosition;
}

std::shared_ptr<sf::Text> Engine::InitText(const std::string& content)
{
	std::shared_ptr<sf::Text> text = std::make_shared<sf::Text>();
	text->setFont(_font);
	text->setFillColor(sf::Color::White);
	text->setString(content);

	RegisterDrawable(text);

	return text;
}

void Engine::RegisterDrawable(std::shared_ptr<sf::Drawable> drawable)
{
	_drawables.push_back(drawable);
}

void Engine::RegisterUpdatable(std::shared_ptr<Updatable> updatable)
{
	_updatables.push_back(updatable);
}

void Engine::RegisterCollidable(std::shared_ptr<Collidable> collidable)
{
	_collidables.push_back(collidable);
}

float Engine::GetCellSize() const
{
	return _cellSize;
}

const sf::Vector2i Engine::GetWindowSize() const
{
	return _windowSize;
}

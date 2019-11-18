#pragma once

#include <SFML/Graphics.hpp>
#include "Updatable.h"
#include "Collidable.h"
#include "Scene.h"

// The master out there, basically setting up and controlling everything
class Engine
{
public:
	// The classic singleton
	static Engine* GetInstance();

	~Engine();

private:
	Engine();

public:
	// Retrieves the inputs that occurred this frame
	void UpdateInput();
	// Updates every object in the current scene
	void Update(float deltaTime);
	// Renders all the Drawable objects
	void Render();
	// Sets the game up
	void Init();

	bool IsRunning() const;
	bool HasFocus() const;

	sf::Vector2i WorldPositionToGridPosition(sf::Vector2f position) const;
	sf::Vector2f GridPositionToWorldPosition(sf::Vector2i position) const;

	// Checks if any registered Collidable collides on the specified grid position
	bool Collides(sf::Vector2i gridPosition) const;

	// Loads the specified scene (the passed class must inherit from Scene)
	template<class T> void LoadScene()
	{
		_drawables.clear();
		_updatables.clear();
		_collidables.clear();
		_scene = std::make_shared<T>();
		_scene->Init();
	}

	void Quit();

	// Creates, sets up and registers a basic sf::Text
	std::shared_ptr<sf::Text> InitText(const std::string& content);

	// Registers a Drawable, resulting in it being rendered each frame until it's removed
	void RegisterDrawable(std::shared_ptr<sf::Drawable> drawable);
	// Registers an Updatable, resulting in it being updated each frame until it's removed
	void RegisterUpdatable(std::shared_ptr<Updatable> updatable);
	// Registers a Collidable, resulting in it being collision-checked whenever Engine::Collides is called
	void RegisterCollidable(std::shared_ptr<Collidable> collidable);

public:
	float GetCellSize() const;
	const sf::Vector2i GetWindowSize() const;

private:
	static Engine* _instance;

	std::shared_ptr<sf::RenderWindow> _window;
	sf::Vector2i _windowSize = sf::Vector2i(600, 500);

	std::shared_ptr<Scene> _scene = nullptr;

	std::vector<std::shared_ptr<sf::Drawable>> _drawables;
	std::vector<std::shared_ptr<Updatable>> _updatables;
	std::vector<std::shared_ptr<Collidable>> _collidables;

	float _cellSize = 20.f;

	sf::Font _font;
};

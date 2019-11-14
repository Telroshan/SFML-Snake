#pragma once

#include <SFML/Graphics.hpp>
#include "Snake.h"
#include "Fruit.h"
#include "Border.h"
#include "Scene.h"

class Engine
{
public:
	static Engine* GetInstance();

	~Engine();

private:
	Engine();

public:
	void UpdateInput();
	void Update(float deltaTime);
	void Render();
	void Init(std::string title, sf::Vector2i windowSize, int gameUiHeight, float cellSize);

	bool IsRunning() const;

	sf::Vector2i WorldPositionToGridPosition(sf::Vector2f position) const;
	sf::Vector2f GridPositionToWorldPosition(sf::Vector2i position) const;

	bool Collides(sf::Vector2i gridPosition) const;

	template<class T> void LoadScene()
	{
		_drawables.clear();
		_updatables.clear();
		_collidables.clear();
		_scene = std::make_shared<T>();
		_scene->Init();
	}

	void Quit();

	static std::string GetFormattedNumericString(const std::string& string, int textLength);

	std::shared_ptr<sf::Text> InitText(const std::string& content);

	void RegisterDrawable(std::shared_ptr<sf::Drawable> drawable);
	void RegisterUpdatable(std::shared_ptr<Updatable> updatable);
	void RegisterCollidable(std::shared_ptr<Collidable> collidable);

	void SaveHighScore();

private:
	void ReadHighScore();

public:
	float GetCellSize() const;
	const sf::Vector2i GetWindowSize() const;

private:
	static Engine* _instance;

	std::shared_ptr<sf::RenderWindow> _window;
	sf::Vector2i _windowSize;

	std::map<std::string, Scene*> _scenes;
	std::shared_ptr<Scene> _scene = nullptr;

	std::vector<std::shared_ptr<sf::Drawable>> _drawables;
	std::vector<std::shared_ptr<Updatable>> _updatables;
	std::vector<std::shared_ptr<Collidable>> _collidables;

	float _cellSize = 20.f;

	const char* scoresFilename = "scores.data";

	sf::Font _font;
};

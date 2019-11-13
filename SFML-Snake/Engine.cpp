#include "Engine.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include "InputManager.h"

Engine Engine::_instance{};

Engine& Engine::GetInstance()
{
	return _instance;
}

Engine::Engine()
{
	if (!_font.loadFromFile("Fonts\\PressStart2P.ttf"))
	{
		std::cerr << "Couldn't load font" << std::endl;
	}

	ReadHighScore();
}

Engine::~Engine()
{
}

void Engine::UpdateInput()
{
	InputManager::Update();

	switch (_mode)
	{
	case Mode::Menu:
		UpdateInputMenu();
		break;
	case Mode::Game:
		UpdateInputGame();
		break;
	case Mode::Endscreen:
		UpdateInputEndscreen();
		break;
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

	for (size_t i = 0; i < _updatables[_mode].size(); ++i)
	{
		_updatables[_mode][i]->Update(deltaTime);
	}

	switch (_mode)
	{
	case Mode::Menu:
		break;
	case Mode::Game:
		UpdateGame(deltaTime);
		break;
	case Mode::Endscreen:
		break;
	}
}

void Engine::Render()
{
	_window->clear();

	for (size_t i = 0; i < _drawables[_mode].size(); ++i)
	{
		_window->draw(*_drawables[_mode][i]);
	}

	_window->display();
}

void Engine::Init(std::string title, sf::Vector2i windowSize, int gameUiHeight, float cellSize)
{
	_windowSize = windowSize;
	_score = 0;

	_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(windowSize.x, windowSize.y), title, sf::Style::Titlebar | sf::Style::Close);
	_gameUiHeight = gameUiHeight;

	SetCellSize(cellSize);

	SetMode(Mode::Menu);
}

bool Engine::IsRunning() const
{
	return _window->isOpen();
}

void Engine::BuildBorder()
{
	const int colorsLength = 2;
	sf::Color colors[colorsLength] = { sf::Color::Color(30, 30, 30), sf::Color::Color(60, 60, 60) };
	int colorIndex = 0;

	// Top left => top right
	for (int x = 0; x < _rectanglesCount.x - 1; ++x)
	{
		std::shared_ptr<sf::RectangleShape> cell = std::make_shared<sf::RectangleShape>(sf::Vector2f(_cellSize, _cellSize));
		cell->setFillColor(colors[colorIndex]);
		cell->setPosition(x * _cellSize, 0);
		RegisterDrawable(cell, Mode::Game);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Top right => bottom right
	for (int y = 0; y < _rectanglesCount.y - 1; ++y)
	{
		std::shared_ptr<sf::RectangleShape> cell = std::make_shared<sf::RectangleShape>(sf::Vector2f(_cellSize, _cellSize));
		cell->setFillColor(colors[colorIndex]);
		cell->setPosition(_windowSize.x - _cellSize, y * _cellSize);
		RegisterDrawable(cell, Mode::Game);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Bottom right => bottom left
	for (int x = _rectanglesCount.x - 1; x > 0; --x)
	{
		std::shared_ptr<sf::RectangleShape> cell = std::make_shared<sf::RectangleShape>(sf::Vector2f(_cellSize, _cellSize));
		cell->setFillColor(colors[colorIndex]);
		cell->setPosition(x * _cellSize, (_windowSize.y - _gameUiHeight) - _cellSize);
		RegisterDrawable(cell, Mode::Game);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Bottom left => top left
	for (int y = 1; y < _rectanglesCount.y; ++y)
	{
		std::shared_ptr<sf::RectangleShape> cell = std::make_shared<sf::RectangleShape>(sf::Vector2f(_cellSize, _cellSize));
		cell->setFillColor(colors[colorIndex]);
		cell->setPosition(0, y * _cellSize);
		RegisterDrawable(cell, Mode::Game);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}
}

void Engine::CheckCollisions(sf::Vector2f nextHeadPosition)
{
	sf::Vector2i nextHeadGridPosition = WorldPositionToGridPosition(nextHeadPosition);

	sf::Vector2f fruitPosition = _fruit->getPosition();
	sf::Vector2i fruitGridPosition = WorldPositionToGridPosition(fruitPosition);
	if (nextHeadGridPosition.x == fruitGridPosition.x && nextHeadGridPosition.y == fruitGridPosition.y)
	{
		_player->Grow();
		if (_score == _highScore)
		{
			_scoreLabel->setFillColor(sf::Color::Green);
			_scoreText->setFillColor(sf::Color::Green);
		}
		SetScore(_score + 1);
		PlaceFruit();
	}
}

sf::Vector2i Engine::WorldPositionToGridPosition(sf::Vector2f position) const
{
	sf::Vector2i gridPosition((int)(position.x / _cellSize), (int)(position.y / _cellSize));
	return gridPosition;
}

bool Engine::IsPositionInBorder(sf::Vector2i gridPosition) const
{
	return gridPosition.x == 0 || gridPosition.x == _rectanglesCount.x - 1
		|| gridPosition.y == 0 || gridPosition.y == _rectanglesCount.y - 1;
}

void Engine::PlaceFruit()
{
	sf::Vector2i position;
	do
	{
		// Random between borders
		position = sf::Vector2i(rand() % (_rectanglesCount.x - 1) + 1, rand() % (_rectanglesCount.y - 1) + 1);
	} while (_player->IsPositionInSnake(position, false) || IsPositionInBorder(position));

	_fruit->setPosition(sf::Vector2f(position.x * _cellSize, position.y * _cellSize));
}

void Engine::UpdateInputMenu()
{
	if (InputManager::WasKeyPressedThisFrame(sf::Keyboard::Space))
	{
		SetMode(Mode::Game);
	}

	if (InputManager::WasKeyPressedThisFrame(sf::Keyboard::Escape))
	{
		_window->close();
	}
}

void Engine::UpdateInputGame()
{
	if (InputManager::IsKeyPressed(sf::Keyboard::Z))
	{
		_player->SetDirection(sf::Vector2i(0, -1));
	}
	else if (InputManager::IsKeyPressed(sf::Keyboard::S))
	{
		_player->SetDirection(sf::Vector2i(0, 1));
	}
	else if (InputManager::IsKeyPressed(sf::Keyboard::D))
	{
		_player->SetDirection(sf::Vector2i(1, 0));
	}
	else if (InputManager::IsKeyPressed(sf::Keyboard::Q))
	{
		_player->SetDirection(sf::Vector2i(-1, 0));
	}

	if (InputManager::WasKeyPressedThisFrame(sf::Keyboard::Escape))
	{
		SetMode(Mode::Menu);
	}
}

void Engine::UpdateInputEndscreen()
{
	if (InputManager::WasKeyPressedThisFrame(sf::Keyboard::Space))
	{
		SetMode(Mode::Menu);
	}

	if (InputManager::WasKeyPressedThisFrame(sf::Keyboard::Escape))
	{
		_window->close();
	}
}

void Engine::UpdateGame(float deltaTime)
{
	if (!_player->IsDead())
	{
		_timeElapsed += deltaTime;
		_timeText->setString(GetFormattedNumericString(std::to_string((int)_timeElapsed), 3));
		_speedText->setString(GetFormattedNumericString(std::to_string(_player->GetMoveSpeed()), 3));
	}
	else
	{
		_gameOverTimer += deltaTime;
		if (_gameOverTimer >= _gameOverDelay)
		{
			SetMode(Mode::Endscreen);
		}
	}
}

void Engine::InitMenu()
{
	std::shared_ptr<sf::Text> gameTitle = InitText(Mode::Menu, "SNAKE");
	gameTitle->setCharacterSize(60);
	gameTitle->setPosition(_windowSize.x / 2 - gameTitle->getLocalBounds().width / 2.f, 60.f);

	std::shared_ptr<sf::Text> playText = InitText(Mode::Menu, "Press space to play");
	playText->setCharacterSize(20);
	playText->setPosition(_windowSize.x / 2 - playText->getLocalBounds().width / 2.f, _windowSize.y - 120.f);

	std::shared_ptr<sf::Text> exitText = InitText(Mode::Menu, "Press escape to exit");
	exitText->setCharacterSize(20);
	exitText->setPosition(_windowSize.x / 2 - exitText->getLocalBounds().width / 2.f, _windowSize.y - 60.f);

	float space = 10.f;
	std::shared_ptr<sf::Text> highScoreLabel = InitText(Mode::Menu, "High score");
	std::shared_ptr<sf::Text> highScoreText = InitText(Mode::Menu, GetFormattedNumericString(std::to_string(_highScore), 3));
	highScoreText->setCharacterSize(40);
	highScoreLabel->setPosition(_windowSize.x / 2 - highScoreLabel->getLocalBounds().width / 2.f,
		_windowSize.y / 2 - (highScoreLabel->getLocalBounds().height + highScoreText->getLocalBounds().height + space) / 2.f);
	highScoreText->setPosition(_windowSize.x / 2 - highScoreText->getLocalBounds().width / 2.f,
		highScoreLabel->getPosition().y + highScoreLabel->getLocalBounds().height + space);
}

void Engine::InitGame()
{
	srand((unsigned int)time(NULL));

	BuildBorder();

	float horizontalPadding = 20.f;
	float space = 10.f;

	std::shared_ptr<sf::Text> timeLabel = InitText(Mode::Game, "Time");
	_timeText = InitText(Mode::Game, GetFormattedNumericString(std::to_string(0), 3));
	timeLabel->setCharacterSize(24);
	_timeText->setCharacterSize(24);
	timeLabel->setPosition(horizontalPadding,
		_windowSize.y - _gameUiHeight / 2.f - (timeLabel->getLocalBounds().height + _timeText->getLocalBounds().height + space) / 2.f);
	_timeText->setPosition(timeLabel->getPosition().x + (timeLabel->getLocalBounds().width - _timeText->getLocalBounds().width) / 2.f,
		timeLabel->getPosition().y + timeLabel->getLocalBounds().height + space);

	_scoreLabel = InitText(Mode::Game, "Score");
	_scoreText = InitText(Mode::Game, GetFormattedNumericString(std::to_string(0), 3));
	_scoreLabel->setCharacterSize(35);
	_scoreText->setCharacterSize(35);
	_scoreLabel->setFillColor(sf::Color::Yellow);
	_scoreText->setFillColor(sf::Color::Yellow);
	_scoreLabel->setPosition(_windowSize.x / 2.f - _scoreLabel->getLocalBounds().width / 2.f,
		_windowSize.y - _gameUiHeight / 2.f - (_scoreLabel->getLocalBounds().height + _scoreText->getLocalBounds().height + space) / 2.f);
	_scoreText->setPosition(_windowSize.x / 2.f - _scoreText->getLocalBounds().width / 2.f,
		_scoreLabel->getPosition().y + _scoreLabel->getLocalBounds().height + space);

	std::shared_ptr<sf::Text> speedLabel = InitText(Mode::Game, "Speed");
	_speedText = InitText(Mode::Game, GetFormattedNumericString(std::to_string(1.f), 3));
	speedLabel->setCharacterSize(24);
	_speedText->setCharacterSize(24);
	speedLabel->setPosition(_windowSize.x - speedLabel->getLocalBounds().width - horizontalPadding,
		_windowSize.y - _gameUiHeight / 2.f - (speedLabel->getLocalBounds().height + _speedText->getLocalBounds().height + space) / 2.f);
	_speedText->setPosition(speedLabel->getPosition().x + (speedLabel->getLocalBounds().width - _speedText->getLocalBounds().width) / 2.f,
		speedLabel->getPosition().y + speedLabel->getLocalBounds().height + space);

	SetScore(0);

	_timeElapsed = 0.f;

	_player = std::make_shared<Snake>(Snake(3,
		_cellSize / 2.f,
		sf::Vector2f((_rectanglesCount.x / 2) * _cellSize, (_rectanglesCount.y / 2) * _cellSize),
		sf::Vector2i(1, 0)));
	RegisterDrawable(_player, Mode::Game);
	RegisterUpdatable(_player, Mode::Game);

	_fruit = std::make_shared<Fruit>(sf::Vector2f(_cellSize, _cellSize));
	RegisterDrawable(_fruit, Mode::Game);
	PlaceFruit();

	_gameOverTimer = 0;
}

void Engine::InitEndscreen()
{
	std::shared_ptr<sf::Text> gameOverText = InitText(Mode::Endscreen, "GAME OVER");
	gameOverText->setCharacterSize(60);
	gameOverText->setPosition(_windowSize.x / 2 - gameOverText->getLocalBounds().width / 2, 30.f);

	float space = 10.f;
	std::shared_ptr<sf::Text> finalScoreLabel = InitText(Mode::Endscreen, "Score");
	std::shared_ptr<sf::Text> finalScoreText = InitText(Mode::Endscreen, GetFormattedNumericString(std::to_string(_score), 3));
	finalScoreLabel->setCharacterSize(50);
	finalScoreText->setCharacterSize(50);
	finalScoreLabel->setFillColor(_score > _highScore ? sf::Color::Green : sf::Color::Yellow);
	finalScoreText->setFillColor(_score > _highScore ? sf::Color::Green : sf::Color::Yellow);
	finalScoreLabel->setPosition(_windowSize.x / 2 - finalScoreLabel->getLocalBounds().width / 2.f,
		_windowSize.y / 2 - (finalScoreLabel->getLocalBounds().height + finalScoreText->getLocalBounds().height + space) / 2.f);
	finalScoreText->setPosition(_windowSize.x / 2 - finalScoreText->getLocalBounds().width / 2.f,
		finalScoreLabel->getPosition().y + finalScoreLabel->getLocalBounds().height + space);

	std::shared_ptr<sf::Text> playText = InitText(Mode::Endscreen, "Press space to return to menu");
	playText->setCharacterSize(20);
	playText->setPosition(_windowSize.x / 2 - playText->getLocalBounds().width / 2, _windowSize.y - 120.f);

	std::shared_ptr<sf::Text> exitText = InitText(Mode::Endscreen, "Press escape to exit");
	exitText->setCharacterSize(20);
	exitText->setPosition(_windowSize.x / 2 - exitText->getLocalBounds().width / 2, _windowSize.y - 60.f);

	if (_score > _highScore)
	{
		std::shared_ptr<sf::Text> beatHighscoreText = InitText(Mode::Endscreen, "You beat the high score !");
		beatHighscoreText->setCharacterSize(20);
		beatHighscoreText->setFillColor(sf::Color::Green);
		beatHighscoreText->setPosition(_windowSize.x / 2 - beatHighscoreText->getLocalBounds().width / 2.f,
			finalScoreText->getPosition().y + finalScoreText->getLocalBounds().height + space * 2.5f);
		_highScore = _score;
		SaveHighScore();
	}
}

void Engine::SetMode(Mode mode)
{
	_mode = mode;
	_drawables.clear();
	_updatables.clear();
	switch (_mode)
	{
	case Mode::Menu:
		InitMenu();
		break;
	case Mode::Game:
		InitGame();
		break;
	case Mode::Endscreen:
		InitEndscreen();
		break;
	default:
		break;
	}
}

void Engine::SetScore(int score)
{
	_score = score;
	_scoreText->setString(GetFormattedNumericString(std::to_string(_score), 3));
}

std::shared_ptr<sf::Text> Engine::InitText(Mode mode, const std::string& content)
{
	std::shared_ptr<sf::Text> text = std::make_shared<sf::Text>();
	text->setFont(_font);
	text->setFillColor(sf::Color::White);
	text->setString(content);

	RegisterDrawable(text, mode);

	return text;
}

void Engine::RegisterDrawable(std::shared_ptr<sf::Drawable> drawable, Mode mode)
{
	if (_drawables.find(mode) == _drawables.end())
	{
		_drawables.insert({ mode, std::vector<std::shared_ptr<sf::Drawable>>() });
	}
	_drawables[mode].push_back(drawable);
}

void Engine::RegisterUpdatable(std::shared_ptr<Updatable> updatable, Mode mode)
{
	if (_updatables.find(mode) == _updatables.end())
	{
		_updatables.insert({ mode, std::vector<std::shared_ptr<Updatable>>() });
	}
	_updatables[mode].push_back(updatable);
}

std::string Engine::GetFormattedNumericString(const std::string& string, int textLength) const
{
	std::string text = string;
	if (text.length() > textLength)
	{
		text = text.substr(0, textLength);
	}
	while (text.length() < textLength)
	{
		text = "0" + text;
	}
	return text;
}

void Engine::ReadHighScore()
{
	std::ifstream stream;
	stream.open(scoresFilename);
	if (!stream.is_open())
	{
		_highScore = 0;
		return;
	}

	stream >> _highScore;

	stream.close();
}

void Engine::SaveHighScore()
{
	std::ofstream stream;
	stream.open(scoresFilename);
	if (!stream.is_open())
	{
		std::cerr << "Couldn't save high score" << std::endl;
		return;
	}

	stream << _highScore;

	stream.close();
}

void Engine::SetCellSize(float cellSize)
{
	_cellSize = cellSize;

	_rectanglesCount = sf::Vector2i(_windowSize.x / (int)cellSize, (_windowSize.y - _gameUiHeight) / (int)cellSize);
}

float Engine::GetCellSize() const
{
	return _cellSize;
}

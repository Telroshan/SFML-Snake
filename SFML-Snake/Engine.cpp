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
		UpdateMenu(deltaTime);
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

	_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(windowSize.x, windowSize.y), title, sf::Style::Titlebar | sf::Style::Close);
	_gameUiHeight = gameUiHeight;

	SetCellSize(cellSize);

	SetMode(Mode::Menu);
}

bool Engine::IsRunning() const
{
	return _window->isOpen();
}

bool Engine::Collides(sf::Vector2i gridPosition) const
{
	if (!_collidables.count(_mode))
	{
		return false;
	}

	for (size_t i = 0; i < _collidables.at(_mode).size(); ++i)
	{
		if (_collidables.at(_mode)[i]->Collides(gridPosition))
		{
			return true;
		}
	}

	return false;
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

void Engine::UpdateMenu(float deltaTime)
{
	_menuDirectionSwitchTimer += deltaTime;
	if (_menuDirectionSwitchTimer > 1.f)
	{
		_menuDirectionSwitchTimer = 0.f;
		sf::Vector2i direction = sf::Vector2i(-_player->GetDirection().y, _player->GetDirection().x);
		_player->SetDirection(direction);
	}
}

void Engine::UpdateGame(float deltaTime)
{
	if (!_player->IsDead())
	{
		_timeElapsed += deltaTime;
		_timeText->setString(GetFormattedNumericString(std::to_string((int)_timeElapsed), 3));

		_scoreText->setString(GetFormattedNumericString(std::to_string(_player->score), 3));
		_scoreLabel->setFillColor(GetScoreColor());
		_scoreText->setFillColor(GetScoreColor());

		_speedText->setString(GetFormattedNumericString(std::to_string(_player->GetMoveSpeed()), 3));
		sf::Color speedColor = _player->ReachedMaxSpeed() ? sf::Color(185, 45, 185) : sf::Color::White;
		_speedText->setFillColor(speedColor);
		_speedLabel->setFillColor(speedColor);
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
	_player = std::make_shared<Snake>(Snake(3,
		_cellSize / 2.f,
		sf::Vector2f((_gridSize.x / 2) * _cellSize, (_gridSize.y / 2) * _cellSize),
		sf::Vector2i(1, 0)));
	RegisterDrawable(_player, Mode::Menu);
	RegisterUpdatable(_player, Mode::Menu);

	_menuDirectionSwitchTimer = 0.f;

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

	_timeElapsed = 0.f;
	_gameOverTimer = 0;

	_border = std::make_shared<Border>(_gridSize);
	RegisterDrawable(_border, Mode::Game);
	RegisterCollidable(_border, Mode::Game);

	_player = std::make_shared<Snake>(Snake(3,
		_cellSize / 2.f,
		sf::Vector2f((_gridSize.x / 2) * _cellSize, (_gridSize.y / 2) * _cellSize),
		sf::Vector2i(1, 0)));
	RegisterDrawable(_player, Mode::Game);
	RegisterUpdatable(_player, Mode::Game);
	RegisterCollidable(_player, Mode::Game);

	_fruit = std::make_shared<Fruit>(sf::Vector2f(_cellSize, _cellSize));
	RegisterDrawable(_fruit, Mode::Game);
	_fruit->Spawn();
	_player->SetFruit(_fruit);

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
	_scoreLabel->setFillColor(GetScoreColor());
	_scoreText->setFillColor(GetScoreColor());
	_scoreLabel->setPosition(_windowSize.x / 2.f - _scoreLabel->getLocalBounds().width / 2.f,
		_windowSize.y - _gameUiHeight / 2.f - (_scoreLabel->getLocalBounds().height + _scoreText->getLocalBounds().height + space) / 2.f);
	_scoreText->setPosition(_windowSize.x / 2.f - _scoreText->getLocalBounds().width / 2.f,
		_scoreLabel->getPosition().y + _scoreLabel->getLocalBounds().height + space);

	_speedLabel = InitText(Mode::Game, "Speed");
	_speedText = InitText(Mode::Game, GetFormattedNumericString(std::to_string(1.f), 3));
	_speedLabel->setCharacterSize(24);
	_speedText->setCharacterSize(24);
	_speedLabel->setPosition(_windowSize.x - _speedLabel->getLocalBounds().width - horizontalPadding,
		_windowSize.y - _gameUiHeight / 2.f - (_speedLabel->getLocalBounds().height + _speedText->getLocalBounds().height + space) / 2.f);
	_speedText->setPosition(_speedLabel->getPosition().x + (_speedLabel->getLocalBounds().width - _speedText->getLocalBounds().width) / 2.f,
		_speedLabel->getPosition().y + _speedLabel->getLocalBounds().height + space);
}

void Engine::InitEndscreen()
{
	std::shared_ptr<sf::Text> gameOverText = InitText(Mode::Endscreen, "GAME OVER");
	gameOverText->setCharacterSize(60);
	gameOverText->setPosition(_windowSize.x / 2 - gameOverText->getLocalBounds().width / 2, 30.f);

	float space = 10.f;
	std::shared_ptr<sf::Text> finalScoreLabel = InitText(Mode::Endscreen, "Score");
	std::shared_ptr<sf::Text> finalScoreText = InitText(Mode::Endscreen, GetFormattedNumericString(std::to_string(_player->score), 3));
	finalScoreLabel->setCharacterSize(50);
	finalScoreText->setCharacterSize(50);
	finalScoreLabel->setFillColor(GetScoreColor());
	finalScoreText->setFillColor(GetScoreColor());
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

	if (_player->score > _highScore)
	{
		std::shared_ptr<sf::Text> beatHighscoreText = InitText(Mode::Endscreen, "You beat the high score !");
		beatHighscoreText->setCharacterSize(20);
		beatHighscoreText->setFillColor(sf::Color::Green);
		beatHighscoreText->setPosition(_windowSize.x / 2 - beatHighscoreText->getLocalBounds().width / 2.f,
			finalScoreText->getPosition().y + finalScoreText->getLocalBounds().height + space * 2.5f);
		_highScore = _player->score;
		SaveHighScore();
	}
}

void Engine::SetMode(Mode mode)
{
	_mode = mode;
	_drawables.clear();
	_updatables.clear();
	_collidables.clear();
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

void Engine::RegisterCollidable(std::shared_ptr<Collidable> collidable, Mode mode)
{
	if (_collidables.find(mode) == _collidables.end())
	{
		_collidables.insert({ mode, std::vector<std::shared_ptr<Collidable>>() });
	}
	_collidables[mode].push_back(collidable);
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

sf::Color Engine::GetScoreColor() const
{
	return _player->score > _highScore ? sf::Color::Green : sf::Color::Yellow;
}

void Engine::SetCellSize(float cellSize)
{
	_cellSize = cellSize;

	_gridSize = sf::Vector2i(_windowSize.x / (int)cellSize, (_windowSize.y - _gameUiHeight) / (int)cellSize);
}

float Engine::GetCellSize() const
{
	return _cellSize;
}

const sf::Vector2i Engine::GetWindowSize() const
{
	return _windowSize;
}

const sf::Vector2i Engine::GetGridSize() const
{
	return _gridSize;
}

const int Engine::GetGameUiHeight() const
{
	return _gameUiHeight;
}
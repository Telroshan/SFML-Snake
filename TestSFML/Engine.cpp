#include "Engine.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

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
}

Engine::~Engine()
{
}

void Engine::UpdateInput()
{
	switch (_mode)
	{
	case Mode::Menu:
	case Mode::Endscreen:
		UpdateInputMenu();
		break;
	case Mode::Game:
		UpdateInputGame();
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

void Engine::Init(std::string title, sf::Vector2i windowSize, int gameUiHeight, float cellSize, float moveSpeed, float moveSpeedMultiplier)
{
	_windowSize = windowSize;
	_score = 0;
	_drawables.clear();

	_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(windowSize.x, windowSize.y), title, sf::Style::Titlebar | sf::Style::Close);
	_gameUiHeight = gameUiHeight;

	SetCellSize(cellSize);

	_moveIntervalMultiplier = 1.f / moveSpeedMultiplier;
	_moveInterval = 1.f / moveSpeed;
	_initialMoveInterval = _moveInterval;

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

	if (IsPositionInBorder(nextHeadGridPosition))
	{
		_player->Die();
		return;
	}

	if (_player->IsPositionInSnake(nextHeadGridPosition, true))
	{
		_player->Die();
		return;
	}

	sf::Vector2f fruitPosition = _fruit->getPosition();
	sf::Vector2i fruitGridPosition = WorldPositionToGridPosition(fruitPosition);
	if (nextHeadGridPosition.x == fruitGridPosition.x && nextHeadGridPosition.y == fruitGridPosition.y)
	{
		_player->Grow();
		SetScore(_score + 1);
		PlaceFruit();
		SetMoveInterval(_moveInterval * _moveIntervalMultiplier);
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		SetMode(Mode::Game);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		_window->close();
	}
}

void Engine::UpdateInputGame()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		_player->SetDirection(sf::Vector2i(0, -1));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		_player->SetDirection(sf::Vector2i(0, 1));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_player->SetDirection(sf::Vector2i(1, 0));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		_player->SetDirection(sf::Vector2i(-1, 0));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		SetMode(Mode::Menu);
	}
}

void Engine::UpdateGame(float deltaTime)
{
	if (!_player->IsDead())
	{
		_timeElapsed += deltaTime;
		_timeText->setString(GetFormattedNumericString(std::to_string((int)_timeElapsed), 3));

		_moveTimer += deltaTime;
		if (_moveTimer > _moveInterval) {
			sf::Vector2i direction = _player->GetDirection();
			sf::Vector2f nextHeadPosition = _player->GetHeadPosition() + sf::Vector2f(direction.x * _cellSize, direction.y * _cellSize);
			CheckCollisions(nextHeadPosition);
			if (!_player->IsDead())
			{
				direction = _player->GetDirection();
				_player->Move(sf::Vector2f(direction.x * _cellSize, direction.y * _cellSize));
				_moveTimer = 0.f;
			}
		}
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
	gameTitle->setPosition(_windowSize.x / 2 - gameTitle->getLocalBounds().width / 2, 60.f);

	std::shared_ptr<sf::Text> playText = InitText(Mode::Menu, "Press space to play");
	playText->setPosition(_windowSize.x / 2 - playText->getLocalBounds().width / 2, _windowSize.y - 120.f);

	std::shared_ptr<sf::Text> exitText = InitText(Mode::Menu, "Press escape to exit");
	exitText->setPosition(_windowSize.x / 2 - exitText->getLocalBounds().width / 2, _windowSize.y - 60.f);
}

void Engine::InitGame()
{
	srand((unsigned int)time(NULL));

	BuildBorder();

	float padding = 20.f;

	std::shared_ptr<sf::Text> timeLabel = InitText(Mode::Game, "Time");
	timeLabel->setCharacterSize(20);
	timeLabel->setPosition(padding, _windowSize.y - _gameUiHeight + padding);
	_timeText = InitText(Mode::Game, GetFormattedNumericString(std::to_string(0), 3));
	_timeText->setPosition(padding + (timeLabel->getLocalBounds().width - _timeText->getLocalBounds().width) / 2.f,
		_windowSize.y - padding - _timeText->getLocalBounds().height);

	std::shared_ptr<sf::Text> scoreLabel = InitText(Mode::Game, "Score");
	scoreLabel->setPosition(_windowSize.x / 2.f - scoreLabel->getLocalBounds().width / 2.f, _windowSize.y - _gameUiHeight + padding - (scoreLabel->getLocalBounds().height - timeLabel->getLocalBounds().height) / 2.f);
	_scoreText = InitText(Mode::Game, GetFormattedNumericString(std::to_string(0), 3));
	_scoreText->setPosition(_windowSize.x / 2.f - _scoreText->getLocalBounds().width / 2.f,
		_windowSize.y - padding - _scoreText->getLocalBounds().height);

	std::shared_ptr<sf::Text> speedLabel = InitText(Mode::Game, "Speed");
	speedLabel->setCharacterSize(20);
	speedLabel->setPosition(_windowSize.x - speedLabel->getLocalBounds().width - padding, _windowSize.y - _gameUiHeight + padding);
	_speedText = InitText(Mode::Game, GetFormattedNumericString(std::to_string(1.f), 3));
	_speedText->setPosition(_windowSize.x - _speedText->getLocalBounds().width - padding - (speedLabel->getLocalBounds().width - _speedText->getLocalBounds().width) / 2.f,
		_windowSize.y - padding - _speedText->getLocalBounds().height);

	SetScore(0);

	_moveTimer = 0;
	SetMoveInterval(_initialMoveInterval);
	_timeElapsed = 0.f;

	_player = std::make_shared<Snake>(Snake(3,
		_cellSize / 2.f,
		sf::Vector2f((_rectanglesCount.x / 2) * _cellSize, (_rectanglesCount.y / 2) * _cellSize),
		sf::Vector2i(1, 0)));
	RegisterDrawable(_player, Mode::Game);

	_fruit = std::make_shared<sf::RectangleShape>(sf::Vector2f(_cellSize, _cellSize));
	_fruit->setFillColor(sf::Color::White);
	RegisterDrawable(_fruit, Mode::Game);
	PlaceFruit();

	_gameOverTimer = 0;
}

void Engine::InitEndscreen()
{
	std::shared_ptr<sf::Text> gameOverText = InitText(Mode::Endscreen, "GAME OVER");
	gameOverText->setCharacterSize(60);
	gameOverText->setPosition(_windowSize.x / 2 - gameOverText->getLocalBounds().width / 2, _windowSize.y / 2 - gameOverText->getLocalBounds().height / 2);

	std::shared_ptr<sf::Text> playText = InitText(Mode::Endscreen, "Press space to play");
	playText->setPosition(_windowSize.x / 2 - playText->getLocalBounds().width / 2, _windowSize.y - 120.f);

	std::shared_ptr<sf::Text> exitText = InitText(Mode::Endscreen, "Press escape to exit");
	exitText->setPosition(_windowSize.x / 2 - exitText->getLocalBounds().width / 2, _windowSize.y - 60.f);

	_finalScoreText = InitText(Mode::Endscreen, GetFormattedNumericString(std::to_string(0), 3));
	_finalScoreText->setPosition(_windowSize.x / 2.f - _finalScoreText->getLocalBounds().width / 2.f, 50.f);
}

void Engine::SetMode(Mode mode)
{
	_mode = mode;
	_drawables.clear();
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

void Engine::SetMoveInterval(float moveInterval)
{
	_moveInterval = moveInterval;
	_speedText->setString(GetFormattedNumericString(std::to_string(_initialMoveInterval / _moveInterval), 3));
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

void Engine::SetCellSize(float cellSize)
{
	_cellSize = cellSize;

	_rectanglesCount = sf::Vector2i(_windowSize.x / (int)cellSize, (_windowSize.y - _gameUiHeight) / (int)cellSize);
}
#include "Engine.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

Engine* Engine::_instance{ nullptr };

Engine::Engine(std::string title, sf::Vector2i windowSize) :
	_windowSize(windowSize),
	_score(0),
	_mode(Mode::Menu),
	_gameOverDelay(1.f)
{
	_instance = this;

	_window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), title, sf::Style::Titlebar | sf::Style::Close);

	if (!_font.loadFromFile("Fonts\\arial.ttf"))
	{
		std::cerr << "Couldn't load score font" << std::endl;
	}

	InitText(_gameTitle);
	_gameTitle.setString("Snake");
	_gameTitle.setCharacterSize(60);

	_gameTitle.setPosition(_windowSize.x / 2 - _gameTitle.getLocalBounds().width / 2, 60.f);

	InitText(_scoreText);
	_scoreText.setPosition(windowSize.x - 75.f, windowSize.y - 60.f);

	InitText(_playText);
	_playText.setString("Press space to play");
	_playText.setPosition(_windowSize.x / 2 - _playText.getLocalBounds().width / 2, _windowSize.y - 120.f);

	InitText(_exitText);
	_exitText.setString("Press escape to exit");
	_exitText.setPosition(_windowSize.x / 2 - _exitText.getLocalBounds().width / 2, _windowSize.y - 60.f);

	InitText(_gameOverText);
	_gameOverText.setString("GAME OVER");
	_gameOverText.setCharacterSize(60);
	_gameOverText.setPosition(_windowSize.x / 2 - _gameOverText.getLocalBounds().width / 2, _windowSize.y / 2 - _gameOverText.getLocalBounds().height / 2);

	InitText(_beatScoreText);
	_beatScoreText.setString("You beat the high score !");
	_beatScoreText.setCharacterSize(45);

	InitText(_finalScoreText);
}

Engine::~Engine()
{
	delete _window;
	delete _player;
}

const Engine* Engine::GetInstance()
{
	return Engine::_instance;
}

void Engine::UpdateInput()
{
	switch (_mode)
	{
	case Menu:
	case Endscreen:
		UpdateInputMenu();
		break;
	case Game:
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
	case Menu:
		break;
	case Game:
		UpdateGame(deltaTime);
		break;
	case Endscreen:
		break;
	}
}

void Engine::Render()
{
	_window->clear();

	switch (_mode)
	{
	case Menu:
		RenderMenu();
		break;
	case Game:
		RenderGame();
		break;
	case Endscreen:
		RenderEndScreen();
		break;
	}

	_window->display();
}

void Engine::Init()
{
	switch (_mode)
	{
	case Menu:
		break;
	case Game:
		InitGame();
		break;
	case Endscreen:
		break;
	}
}

bool Engine::IsRunning() const
{
	return _window->isOpen();
}

void Engine::BuildBorder(float cellRadius)
{
	_border.empty();

	const int colorsLength = 2;
	sf::Color colors[colorsLength] = { sf::Color::Cyan, sf::Color::Blue };
	int colorIndex = 0;

	// Top left => top right
	for (int x = 0; x < _rectanglesCount.x - 1; ++x)
	{
		sf::RectangleShape cell(sf::Vector2f(cellRadius, cellRadius));
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(x * cellRadius, 0);
		_border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Top right => bottom right
	for (int y = 0; y < _rectanglesCount.y - 1; ++y)
	{
		sf::RectangleShape cell(sf::Vector2f(cellRadius, cellRadius));
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(_windowSize.x - cellRadius, y * cellRadius);
		_border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Bottom right => bottom left
	for (int x = _rectanglesCount.x - 1; x > 0; --x)
	{
		sf::RectangleShape cell(sf::Vector2f(cellRadius, cellRadius));
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(x * cellRadius, _windowSize.y - cellRadius);
		_border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}

	// Bottom left => top left
	for (int y = 1; y < _rectanglesCount.y; ++y)
	{
		sf::RectangleShape cell(sf::Vector2f(cellRadius, cellRadius));
		cell.setFillColor(colors[colorIndex]);
		cell.setPosition(0, y * cellRadius);
		_border.push_back(cell);

		++colorIndex;
		if (colorIndex >= colorsLength) colorIndex = 0;
	}
}

void Engine::CheckCollisions(sf::Vector2f nextHeadPosition)
{
	sf::Vector2i nextHeadGridPosition = WorldPositionToGridPosition(nextHeadPosition);

	std::cout << "Grid pos : " << nextHeadGridPosition.x << ", " << nextHeadGridPosition.y
		<< " \tGrid size : " << _rectanglesCount.x << ", " << _rectanglesCount.y
		<< " \tWorld pos : " << nextHeadPosition.x << ", " << nextHeadPosition.y
		<< " \tDirection : " << _player->GetDirection().x << ", " << _player->GetDirection().y << std::endl;

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

	sf::Vector2f fruitPosition = _fruit.getPosition();
	sf::Vector2i fruitGridPosition = WorldPositionToGridPosition(fruitPosition);
	if (nextHeadGridPosition.x == fruitGridPosition.x && nextHeadGridPosition.y == fruitGridPosition.y)
	{
		_player->Grow();
		SetScore(_score + 1);
		PlaceFruit();
	}
}

sf::Vector2i Engine::WorldPositionToGridPosition(sf::Vector2f position) const
{
	sf::Vector2i gridPosition((int)(position.x / _cellRadius), (int)(position.y / _cellRadius));
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

	_fruit.setPosition(sf::Vector2f(position.x * _cellRadius, position.y * _cellRadius));

	std::cout << _fruit.getPosition().x << ", " << _fruit.getPosition().y << std::endl;
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
}

void Engine::UpdateGame(float deltaTime)
{
	if (!_player->IsDead())
	{
		_moveTimer += deltaTime;
		if (_moveTimer > _moveInterval) {
			sf::Vector2i direction = _player->GetDirection();
			sf::Vector2f nextHeadPosition = _player->GetHeadPosition() + sf::Vector2f(direction.x * _cellRadius, direction.y * _cellRadius);
			CheckCollisions(nextHeadPosition);
			if (!_player->IsDead())
			{
				direction = _player->GetDirection();
				_player->Move(sf::Vector2f(direction.x * _cellRadius, direction.y * _cellRadius));
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

void Engine::RenderMenu()
{
	_window->draw(_gameTitle);
	_window->draw(_playText);
	_window->draw(_exitText);
}

void Engine::RenderGame()
{
	for (int i = 0; i < _border.size(); ++i)
	{
		_window->draw(_border.at(i));
	}

	_player->Render(_window);

	_window->draw(_fruit);

	_window->draw(_scoreText);
}

void Engine::RenderEndScreen()
{
	_window->draw(_gameOverText);
	_window->draw(_playText);
	_window->draw(_exitText);
}

void Engine::InitGame()
{
	srand((unsigned int)time(NULL));

	SetScore(0);

	_moveTimer = 0;

	_player = new Snake(3, _cellRadius / 2.f, sf::Vector2f((_rectanglesCount.x / 2) * _cellRadius, (_rectanglesCount.y / 2) * _cellRadius), sf::Vector2i(1, 0));

	_fruit = sf::RectangleShape(sf::Vector2f(_cellRadius, _cellRadius));
	_fruit.setFillColor(sf::Color::White);
	PlaceFruit();

	_gameOverTimer = 0;
}

void Engine::SetMode(Mode mode)
{
	_mode = mode;
	switch (_mode)
	{
	case Menu:
		break;
	case Game:
		InitGame();
		break;
	case Endscreen:
		break;
	default:
		break;
	}
}

void Engine::SetScore(int score)
{
	_score = score;
	std::string text = std::to_string(_score);
	while (text.length() < 3) {
		text = "0" + text;
	}
	_scoreText.setString(text);
}

void Engine::InitText(sf::Text& text)
{
	text.setFont(_font);
	text.setFillColor(sf::Color::White);
}

void Engine::SetCellSize(float cellRadius)
{
	_cellRadius = cellRadius;

	_rectanglesCount = sf::Vector2i(_windowSize.x / (int)cellRadius, _windowSize.y / (int)cellRadius);

	BuildBorder(cellRadius);
}

void Engine::SetMoveSpeed(float speed)
{
	_moveInterval = 1.f / speed;
}

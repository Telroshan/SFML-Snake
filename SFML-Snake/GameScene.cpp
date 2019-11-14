#include "GameScene.h"
#include "InputManager.h"
#include "Engine.h"
#include "MenuScene.h"
#include "EndscreenScene.h"
#include "GameData.h"
#include "Utils.h"

void GameScene::Init()
{
	srand((unsigned int)time(NULL));

	Engine* engine = Engine::GetInstance();

	sf::Vector2i windowSize = engine->GetWindowSize();
	float cellSize = engine->GetCellSize();
	_gridSize = sf::Vector2i(windowSize.x / (int)cellSize, (windowSize.y - _gameUiHeight) / (int)cellSize);

	GameData::Score = 0;
	_timeElapsed = 0.f;
	_gameOverTimer = 0;

	_border = std::make_shared<Border>(_gridSize, sf::Vector2i(windowSize.x, windowSize.y - _gameUiHeight));
	engine->RegisterDrawable(_border);
	engine->RegisterCollidable(_border);

	_player = std::make_shared<Snake>(Snake(3,
		cellSize / 2.f,
		sf::Vector2f((_gridSize.x / 2) * cellSize, (_gridSize.y / 2) * cellSize),
		sf::Vector2i(1, 0)));
	engine->RegisterDrawable(_player);
	engine->RegisterUpdatable(_player);
	engine->RegisterCollidable(_player);

	_fruit = std::make_shared<Fruit>(sf::Vector2f(cellSize, cellSize), _gridSize);
	engine->RegisterDrawable(_fruit);
	_fruit->Spawn();
	_player->SetFruit(_fruit);

	float horizontalPadding = 20.f;
	float space = 10.f;

	std::shared_ptr<sf::Text> timeLabel = engine->InitText("Time");
	_timeText = engine->InitText(Utils::GetFormattedNumericString(std::to_string(0), 3));
	timeLabel->setCharacterSize(24);
	_timeText->setCharacterSize(24);
	timeLabel->setPosition(horizontalPadding,
		windowSize.y - _gameUiHeight / 2.f - (timeLabel->getLocalBounds().height + _timeText->getLocalBounds().height + space) / 2.f);
	_timeText->setPosition(timeLabel->getPosition().x + (timeLabel->getLocalBounds().width - _timeText->getLocalBounds().width) / 2.f,
		timeLabel->getPosition().y + timeLabel->getLocalBounds().height + space);

	_scoreLabel = engine->InitText("Score");
	_scoreText = engine->InitText(Utils::GetFormattedNumericString(std::to_string(0), 3));
	_scoreLabel->setCharacterSize(35);
	_scoreText->setCharacterSize(35);
	_scoreLabel->setFillColor(_player->GetScoreColor());
	_scoreText->setFillColor(_player->GetScoreColor());
	_scoreLabel->setPosition(windowSize.x / 2.f - _scoreLabel->getLocalBounds().width / 2.f,
		windowSize.y - _gameUiHeight / 2.f - (_scoreLabel->getLocalBounds().height + _scoreText->getLocalBounds().height + space) / 2.f);
	_scoreText->setPosition(windowSize.x / 2.f - _scoreText->getLocalBounds().width / 2.f,
		_scoreLabel->getPosition().y + _scoreLabel->getLocalBounds().height + space);

	_speedLabel = engine->InitText("Speed");
	_speedText = engine->InitText(Utils::GetFormattedNumericString(std::to_string(1.f), 3));
	_speedLabel->setCharacterSize(24);
	_speedText->setCharacterSize(24);
	_speedLabel->setPosition(windowSize.x - _speedLabel->getLocalBounds().width - horizontalPadding,
		windowSize.y - _gameUiHeight / 2.f - (_speedLabel->getLocalBounds().height + _speedText->getLocalBounds().height + space) / 2.f);
	_speedText->setPosition(_speedLabel->getPosition().x + (_speedLabel->getLocalBounds().width - _speedText->getLocalBounds().width) / 2.f,
		_speedLabel->getPosition().y + _speedLabel->getLocalBounds().height + space);
}

void GameScene::UpdateInput()
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
		Engine::GetInstance()->LoadScene<MenuScene>();
	}
}

void GameScene::Update(float deltaTime)
{
	if (!_player->IsDead())
	{
		_timeElapsed += deltaTime;
		_timeText->setString(Utils::GetFormattedNumericString(std::to_string((int)_timeElapsed), 3));

		_scoreText->setString(Utils::GetFormattedNumericString(std::to_string(GameData::Score), 3));
		_scoreLabel->setFillColor(_player->GetScoreColor());
		_scoreText->setFillColor(_player->GetScoreColor());

		_speedText->setString(Utils::GetFormattedNumericString(std::to_string(_player->GetMoveSpeed()), 3));
		sf::Color speedColor = _player->ReachedMaxSpeed() ? sf::Color(185, 45, 185) : sf::Color::White;
		_speedText->setFillColor(speedColor);
		_speedLabel->setFillColor(speedColor);
	}
	else
	{
		_gameOverTimer += deltaTime;
		if (_gameOverTimer >= _gameOverDelay)
		{
			Engine::GetInstance()->LoadScene<EndscreenScene>();
		}
	}
}

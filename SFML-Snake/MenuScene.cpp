#include "MenuScene.h"
#include "InputManager.h"
#include "Engine.h"
#include "GameScene.h"
#include "GameData.h"
#include "Utils.h"

void MenuScene::Init()
{
	Engine* engine = Engine::GetInstance();
	sf::Vector2i windowSize = engine->GetWindowSize();
	float cellSize = engine->GetCellSize();

	// Title
	std::shared_ptr<sf::Text> gameTitle = engine->InitText("SNAKE");
	gameTitle->setCharacterSize(60);
	gameTitle->setPosition(windowSize.x / 2 - gameTitle->getLocalBounds().width / 2.f, 60.f);

	// Controls
	std::shared_ptr<sf::Text> playText = engine->InitText("Press space to play");
	playText->setCharacterSize(20);
	playText->setPosition(windowSize.x / 2 - playText->getLocalBounds().width / 2.f, windowSize.y - 120.f);
	std::shared_ptr<sf::Text> exitText = engine->InitText("Press escape to exit");
	exitText->setCharacterSize(20);
	exitText->setPosition(windowSize.x / 2 - exitText->getLocalBounds().width / 2.f, windowSize.y - 60.f);

	// Current high score
	GameData::ReadHighScore();
	float space = 10.f;
	std::shared_ptr<sf::Text> highScoreLabel = engine->InitText("High score");
	std::shared_ptr<sf::Text> highScoreText = engine->InitText(Utils::GetFormattedNumericString(std::to_string(GameData::HighScore), 3));
	highScoreText->setCharacterSize(40);
	// Center the group vertically
	highScoreLabel->setPosition(windowSize.x / 2 - highScoreLabel->getLocalBounds().width / 2.f,
		windowSize.y / 2 - (highScoreLabel->getLocalBounds().height + highScoreText->getLocalBounds().height + space) / 2.f);
	highScoreText->setPosition(windowSize.x / 2 - highScoreText->getLocalBounds().width / 2.f,
		highScoreLabel->getPosition().y + highScoreLabel->getLocalBounds().height + space);

	// Snake animation, walking around the high score text group
	_menuSnakePatrolBounds = sf::FloatRect(highScoreLabel->getPosition().x - cellSize - space,
		highScoreLabel->getPosition().y - cellSize - space,
		highScoreLabel->getLocalBounds().width + cellSize + space,
		(highScoreText->getPosition().y - highScoreLabel->getPosition().y) + highScoreText->getLocalBounds().height + cellSize + space);
	_snake = std::make_shared<Snake>(Snake(3,
		cellSize / 2.f,
		sf::Vector2f(_menuSnakePatrolBounds.left, _menuSnakePatrolBounds.top),
		sf::Vector2i(1, 0)));
	engine->RegisterDrawable(_snake);
	engine->RegisterUpdatable(_snake);

	SetMusic("Menu");
}

void MenuScene::UpdateInput()
{
	if (InputManager::WasKeyPressedThisFrame(sf::Keyboard::Space))
	{
		Engine::GetInstance()->LoadScene<GameScene>();
	}

	if (InputManager::WasKeyPressedThisFrame(sf::Keyboard::Escape))
	{
		Engine::GetInstance()->Quit();
	}
}

void MenuScene::Update(float deltaTime)
{
	// Change the snake's direction once it reached the bounds
	sf::Vector2i direction = _snake->GetDirection();
	sf::Vector2f snakeHeadPosition = _snake->GetHeadPosition();
	if (direction.x > 0.f && snakeHeadPosition.x >= _menuSnakePatrolBounds.left + _menuSnakePatrolBounds.width ||
		direction.y > 0.f && snakeHeadPosition.y >= _menuSnakePatrolBounds.top + _menuSnakePatrolBounds.height ||
		direction.x < 0.f && snakeHeadPosition.x <= _menuSnakePatrolBounds.left ||
		direction.y < 0.f && snakeHeadPosition.y <= _menuSnakePatrolBounds.top)
	{
		sf::Vector2i newDirection = sf::Vector2i(-_snake->GetDirection().y, _snake->GetDirection().x);
		_snake->SetDirection(newDirection);
	}
}

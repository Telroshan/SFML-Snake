#include "EndscreenScene.h"
#include "InputManager.h"
#include "Engine.h"
#include "MenuScene.h"
#include "GameData.h"
#include "Utils.h"
#include "AudioManager.h"

void EndscreenScene::Init()
{
	Engine* engine = Engine::GetInstance();
	sf::Vector2i windowSize = engine->GetWindowSize();

	// Game over
	std::shared_ptr<sf::Text> gameOverText = engine->InitText("GAME OVER");
	gameOverText->setCharacterSize(60);
	gameOverText->setPosition(windowSize.x / 2 - gameOverText->getLocalBounds().width / 2, 60.f);

	// Score
	float space = 10.f;
	std::shared_ptr<sf::Text> finalScoreLabel = engine->InitText("Score");
	std::shared_ptr<sf::Text> finalScoreText = engine->InitText(Utils::GetFormattedNumericString(std::to_string(GameData::Score), 3));
	finalScoreLabel->setCharacterSize(50);
	finalScoreText->setCharacterSize(50);
	finalScoreLabel->setFillColor(GameData::GetScoreColor());
	finalScoreText->setFillColor(GameData::GetScoreColor());
	// Center the group vertically
	finalScoreLabel->setPosition(windowSize.x / 2 - finalScoreLabel->getLocalBounds().width / 2.f,
		windowSize.y / 2 - (finalScoreLabel->getLocalBounds().height + finalScoreText->getLocalBounds().height + space) / 2.f);
	finalScoreText->setPosition(windowSize.x / 2 - finalScoreText->getLocalBounds().width / 2.f,
		finalScoreLabel->getPosition().y + finalScoreLabel->getLocalBounds().height + space);

	// Controls
	std::shared_ptr<sf::Text> playText = engine->InitText("Press space to return to menu");
	playText->setCharacterSize(20);
	playText->setPosition(windowSize.x / 2 - playText->getLocalBounds().width / 2, windowSize.y - 120.f);
	std::shared_ptr<sf::Text> exitText = engine->InitText("Press escape to exit");
	exitText->setCharacterSize(20);
	exitText->setPosition(windowSize.x / 2 - exitText->getLocalBounds().width / 2, windowSize.y - 60.f);

	// The contextual text if the player beats the high score
	if (GameData::Score > GameData::HighScore)
	{
		AudioManager::PlaySound("NewHighScore");

		std::shared_ptr<sf::Text> beatHighscoreText = engine->InitText("You beat the high score !");
		beatHighscoreText->setCharacterSize(20);
		beatHighscoreText->setFillColor(sf::Color::Green);
		float spaceBeathHighScore = space * 2.5f;
		float offset = (beatHighscoreText->getLocalBounds().height + spaceBeathHighScore) / 2.f;
		// Center the group vertically
		finalScoreLabel->setPosition(finalScoreLabel->getPosition().x, finalScoreLabel->getPosition().y - offset);
		finalScoreText->setPosition(finalScoreText->getPosition().x, finalScoreText->getPosition().y - offset);
		beatHighscoreText->setPosition(windowSize.x / 2 - beatHighscoreText->getLocalBounds().width / 2.f,
			finalScoreText->getPosition().y + finalScoreText->getLocalBounds().height + spaceBeathHighScore);

		// Also save the new high score
		GameData::HighScore = GameData::Score;
		GameData::SaveHighScore();
	}
}

void EndscreenScene::UpdateInput()
{
	if (InputManager::WasKeyPressedThisFrame(sf::Keyboard::Space))
	{
		Engine::GetInstance()->LoadScene<MenuScene>();
	}

	if (InputManager::WasKeyPressedThisFrame(sf::Keyboard::Escape))
	{
		Engine::GetInstance()->Quit();
	}
}

void EndscreenScene::Update(float deltaTime)
{
}

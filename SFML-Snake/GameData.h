#pragma once

#include <SFML/Graphics.hpp>

// A utility class to hold game data across scenes
class GameData
{
private:
	GameData();

public:
	// Reads the high score from the scores file
	static void ReadHighScore();
	// Write the best high score in the scores file
	static void SaveHighScore();

	// Returns a different color whether the current score is higher or lower than the current high score
	static sf::Color GetScoreColor();

public:
	static int Score;
	static int HighScore;

private:
	static const char* scoresFilename;
};
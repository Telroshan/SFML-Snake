#include "GameData.h"
#include <iostream>
#include <fstream>

int GameData::Score = 0;
int GameData::HighScore = 0;

const char* GameData::scoresFilename = "scores.data";

void GameData::ReadHighScore()
{
	std::ifstream stream;
	stream.open(GameData::scoresFilename);
	if (!stream.is_open())
	{
		GameData::HighScore = 0;
		return;
	}

	stream >> GameData::HighScore;

	stream.close();
}

void GameData::SaveHighScore()
{
	std::ofstream stream;
	stream.open(scoresFilename);
	if (!stream.is_open())
	{
		std::cerr << "Couldn't save high score" << std::endl;
		return;
	}

	stream << GameData::HighScore;

	stream.close();
}

sf::Color GameData::GetScoreColor()
{
	return Score > HighScore ? sf::Color::Green : sf::Color::Yellow;
}

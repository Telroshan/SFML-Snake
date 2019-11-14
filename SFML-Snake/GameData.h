#pragma once

class GameData
{
private:
	GameData();

public:
	static void ReadHighScore();
	static void SaveHighScore();

public:
	static int Score;
	static int HighScore;

private:
	static const char* scoresFilename;
};
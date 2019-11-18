#include "AudioManager.h"
#include <iostream>

std::map<std::string, std::unique_ptr<AudioManager::Sound>> AudioManager::_sounds = {};

void AudioManager::PlaySound(const std::string& soundName)
{
	if (_sounds.find(soundName) == _sounds.end())
	{
		_sounds.insert({ soundName, std::make_unique<Sound>(soundName) });
	}

	_sounds.at(soundName)->Play();
}

const std::string AudioManager::GetAudioPath(const std::string& fileName)
{
	return "Audio\\" + fileName + ".ogg";
}

AudioManager::Sound::Sound(const std::string& soundName)
{
	if (!_soundBuffer.loadFromFile(GetAudioPath(soundName)))
	{
		std::cerr << "Couldn't load sound " << soundName << std::endl;
	}
	_sound.setBuffer(_soundBuffer);
}

void AudioManager::Sound::Play()
{
	_sound.play();
}

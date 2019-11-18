#pragma once

#include <SFML/Audio.hpp>
#include <map>

class AudioManager
{
	class Sound
	{
	public:
		Sound(const std::string& soundName);
		Sound(const Sound& other) = delete;
		Sound(Sound&& other) = delete;

	public:
		void Play();

	private:
		sf::SoundBuffer _soundBuffer;
		sf::Sound _sound;
	};

public:
	static void PlaySound(const std::string& soundName);

	static const std::string GetAudioPath(const std::string& fileName);

private:
	static std::map<std::string, std::unique_ptr<Sound>> _sounds;
};
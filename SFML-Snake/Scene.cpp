#include "Scene.h"
#include <iostream>
#include "AudioManager.h"

void Scene::SetMusic(const std::string& musicName)
{
	if (!_music.openFromFile(AudioManager::GetAudioPath(musicName)))
	{
		std::cerr << "Couldn't load music" << std::endl;
	}

	_music.setLoop(true);

	_music.play();
}

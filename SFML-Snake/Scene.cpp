#include "Scene.h"
#include <iostream>
#include "AudioManager.h"

void Scene::SetMusic(const std::string& musicName)
{
	if (!music.openFromFile(AudioManager::GetAudioPath(musicName)))
	{
		std::cerr << "Couldn't load music" << std::endl;
	}

	music.setLoop(true);

	music.play();
}

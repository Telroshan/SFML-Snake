#include "Scene.h"
#include <iostream>

void Scene::SetMusic(const std::string& musicName)
{
	if (!_music.openFromFile("Audio\\" + musicName + ".ogg"))
	{
		std::cerr << "Couldn't load music" << std::endl;
	}

	_music.setLoop(true);

	_music.play();
}

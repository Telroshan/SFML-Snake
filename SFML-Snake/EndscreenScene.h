#pragma once

#include "Scene.h"
#include "Snake.h"

class EndscreenScene : public Scene
{
public:
	virtual void Init();
	virtual void UpdateInput();
	virtual void Update(float deltaTime);

private:
	std::shared_ptr<Snake> _player;
};
#pragma once

#include "Scene.h"
#include "Snake.h"

// The scene once the player died
class EndscreenScene : public Scene
{
public:
	virtual void Init();
	virtual void UpdateInput();
	virtual void Update(float deltaTime);
};
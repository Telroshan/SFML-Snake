#pragma once

class Scene
{
public:
	virtual void Init() = 0;
	virtual void UpdateInput() = 0;
	virtual void Update(float deltaTime) = 0;
};
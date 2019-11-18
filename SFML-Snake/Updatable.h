#pragma once

// For objects that should be updated each frame
class Updatable
{
public:
	virtual void Update(float deltaTime) = 0;
};
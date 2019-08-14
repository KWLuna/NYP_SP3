#pragma once
#include "Source/Camera.h"

class Player : public Camera
{
public:
	Player();
	~Player();

	virtual void Update(double dt);
};


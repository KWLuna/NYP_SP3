#pragma once

#include "Weapons.h"

class Sword : public Weapons
{
public:
	Sword();
	virtual ~Sword();

	void Init(void);
	void UpdateAnimal(const double dt, Vector3 dir, Vector3 origin, std::vector<CAnimal*> animalist);
	void Response();
};


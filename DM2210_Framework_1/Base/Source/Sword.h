#pragma once

#include "Weapons.h"

class Sword : public Weapons
{
public:
	Sword();
	virtual ~Sword();

	void Init(void);
	void Update(const double dt);
	void Response();
};


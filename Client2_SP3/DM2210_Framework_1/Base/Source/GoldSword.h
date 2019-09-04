#pragma once

#include "Weapons.h"

class GoldSword : public Weapons
{
public:
	GoldSword();
	virtual ~GoldSword();

	void Init(void);
	void UpdateAnimal(const double dt, Vector3 dir, Vector3 origin, std::vector<CAnimal*> animalist, float m_fPlayersDamage);
	void UpdateEnemy(const double dt, Vector3 dir, Vector3 origin, std::vector<CEnemy*> enemylist, float m_fPlayersDamage);
	void UpdateTile(const double dt, Vector3 dir, Vector3 origin, char tilearray[]);
	void Response();
};


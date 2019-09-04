#pragma once

#include "Weapons.h"

class WoodenPickaxe : public Weapons
{
public:
	WoodenPickaxe();
	virtual ~WoodenPickaxe();

	void Init(void);
	void UpdateAnimal(const double dt, Vector3 dir, Vector3 origin, std::vector<CAnimal*> animalist, float m_fPlayersDamage);
	void UpdateEnemy(const double dt, Vector3 dir, Vector3 origin, std::vector<CEnemy*> enemylist, float m_fPlayersDamage);
	void UpdateTile(const double dt, Vector3 dir, Vector3 origin, char tilearray[]);
	void Response();
};


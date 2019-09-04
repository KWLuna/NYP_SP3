#include "WoodenSword.h"

WoodenSword::WoodenSword()
{
}

WoodenSword::~WoodenSword()
{
}

void WoodenSword::Init()
{
	Weapons::Init();
	m_range = 50.0f;
	damage = 10.0f;
	type = MELEE;
	rightmaxr = -40.5;
	rightmaxt = 0.9;
	leftmaxr = 40.5;
	leftmaxt = -0.9;
	attacktiltup = 29.0;
	swingtime = 0.3;
	side = false;
	rightrestr = -18.5;
	rightrestt = 0.5;
	leftrestr = 18.5;
	leftrestt = -0.5;
	resttiltup = 45.0;
	curr = rightmaxr;
	curt = rightmaxt;
	hitonce = false;
	//std::cout << rightmaxt;
}

void WoodenSword::UpdateAnimal(const double dt, Vector3 dir, Vector3 origin, std::vector<CAnimal*> animalist, float m_fPlayersDamage)
{
	Weapons::UpdateAnimal(dt, dir, origin, animalist);
	Response();
}
void WoodenSword::UpdateEnemy(const double dt, Vector3 dir, Vector3 origin, std::vector<CEnemy*> enemylist, float m_fPlayersDamage)
{
	Weapons::UpdateEnemy(dt, dir, origin, enemylist);
	Response();
}

void WoodenSword::UpdateTile(const double dt, Vector3 dir, Vector3 origin, char tilearray[])
{
	Weapons::UpdateTile(dt, dir, origin, tilearray);
	Response();
}

void WoodenSword::Response()
{

}

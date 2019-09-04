#include "GoldSword.h"

GoldSword::GoldSword()
{
}

GoldSword::~GoldSword()
{
}

void GoldSword::Init()
{
	Weapons::Init();
	m_range = 70.0f;
	damage = 20.0f;
	type = MELEE;
	rightmaxr = -40.5;
	rightmaxt = 0.9;
	leftmaxr = 40.5;
	leftmaxt = -0.9;
	attacktiltup = 29.0;
	swingtime = 0.2;
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

void GoldSword::UpdateAnimal(const double dt, Vector3 dir, Vector3 origin, std::vector<CAnimal*> animalist, float m_fPlayersDamage)
{
	Weapons::UpdateAnimal(dt, dir, origin, animalist);
	Response();
}
void GoldSword::UpdateEnemy(const double dt, Vector3 dir, Vector3 origin, std::vector<CEnemy*> enemylist, float m_fPlayersDamage)
{
	Weapons::UpdateEnemy(dt, dir, origin, enemylist);
	Response();
}

void GoldSword::UpdateTile(const double dt, Vector3 dir, Vector3 origin, char tilearray[])
{
	Weapons::UpdateTile(dt, dir, origin, tilearray);
	Response();
}

void GoldSword::Response()
{

}

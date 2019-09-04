#include "GoldHoe.h"

GoldHoe::GoldHoe()
{
}

GoldHoe::~GoldHoe()
{
}

void GoldHoe::Init()
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
	swingtime = 0.2;
	side = false;
	rightrestr = -18.5;
	rightrestt = 0.5;
	leftrestr = 18.5;
	leftrestt = -0.5;
	resttiltup = 45.0;
	curr = rightmaxr;
	curt = rightmaxt;
	hitonce = true;
	maxswings = 3;
	//std::cout << rightmaxt;
}

void GoldHoe::UpdateAnimal(const double dt, Vector3 dir, Vector3 origin, std::vector<CAnimal*> animalist, float m_fPlayersDamage)
{
	Weapons::UpdateAnimal(dt, dir, origin, animalist);
	Response();
}
void GoldHoe::UpdateEnemy(const double dt, Vector3 dir, Vector3 origin, std::vector<CEnemy*> enemylist, float m_fPlayersDamage)
{
	Weapons::UpdateEnemy(dt, dir, origin, enemylist);
	Response();
}

void GoldHoe::UpdateTile(const double dt, Vector3 dir, Vector3 origin, char tilearray[])
{
	Weapons::UpdateTile(dt, dir, origin, tilearray);
	Response();
}

void GoldHoe::Response()
{

}

#include "Sword.h"

Sword::Sword()
{
}

Sword::~Sword()
{
}

void Sword::Init()
{
	Weapons::Init();
	m_range = 5.0f;
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
	//std::cout << rightmaxt;
}

void Sword::UpdateAnimal(const double dt, Vector3 dir, Vector3 origin, std::vector<CAnimal*> animalist)
{
	Weapons::UpdateAnimal(dt, dir, origin, animalist);
	Response();
}

void Sword::UpdateTile(const double dt, Vector3 dir, Vector3 origin, char tilearray[])
{
	Weapons::UpdateTile(dt, dir, origin, tilearray);
	Response();
}

void Sword::Response()
{

}

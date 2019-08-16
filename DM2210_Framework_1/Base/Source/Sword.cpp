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
}

void Sword::Update(const double dt)
{
	Weapons::Update(dt);
	Response();
}

void Sword::Response()
{

}

#include "Weapons.h"

Weapons::Weapons()
	:m_range(3.0f)
{
}

Weapons::~Weapons()
{
}

void Weapons::Init(void)
{
}

void Weapons::Update(const double dt)
{
	if (type == MELEE)
	{

	}
	else if (type == RANGE)
	{

	}
}

void Weapons::SetPosition(Vector3 vposition)
{
	this->vPosition = vposition;
}

Vector3 Weapons::GetPosition(void) const
{
	return vPosition;
}

void Weapons::SetRotation(const float fRotation)
{
	this->fRotation = fRotation;
}


float Weapons::GetRotation(void) const
{
	return fRotation;
}

void Weapons::SetScale(Vector3 vScale)
{
	this->vScale = vScale;
}

Vector3 Weapons::GetScale(void) const
{
	return vScale;
}

void Weapons::Render(void)
{
}
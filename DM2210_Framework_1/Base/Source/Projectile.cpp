#include "Projectile.h"


ProjectileObject::ProjectileObject(PROJECTILE_TYPE typeValue)
	: Type(typeValue),
	Pos(0, 0, 0),
	TargetPos(0, 0, 0),
	Scale(1, 1, 1),
	m_fRotation(0),
	m_fRotationSpeed(0),
	m_bActive(false),
	m_fSpeed(10.f),
	m_fTimeTravelled(0),
	m_bGotPlayersPos(false)
{

}

ProjectileObject::~ProjectileObject(void)
{

}

void ProjectileObject::Update(double dt)
{
	if (TargetPos.x < -1)
		TargetPos.x = 0;
	if (TargetPos.z < -1)
		TargetPos.z = 0;

	if (m_bActive)
	{
		m_fTimeTravelled += dt;

		Vel += m_fSpeed * static_cast<float>(dt);
		Pos += Vel * static_cast<float>(dt);

		Vector3 temp = (TargetPos - Pos) * dt;
		Pos += temp;
		Vector3 dir;

		dir = (TargetPos - Pos).Normalize();

		m_fRotation = atan2(dir.x, dir.z);
	}
}
//Get Functions
PROJECTILE_TYPE ProjectileObject::GetType()
{
	return Type;
}
Vector3 ProjectileObject::GetPos()
{
	return Pos;
}
Vector3 ProjectileObject::GetScale()
{
	return Scale;
}
bool ProjectileObject::GetActive()
{
	return m_bActive;
}
float ProjectileObject::GetRotation()
{
	return m_fRotation;
}
float ProjectileObject::GetTimeTravelled()
{
	return m_fTimeTravelled;
}
bool ProjectileObject::GetGotPlayersPos()
{
	return m_bGotPlayersPos;
}
//Set Functions
void ProjectileObject::SetType(PROJECTILE_TYPE Type)
{
	this->Type = Type;
}
void ProjectileObject::SetPos(Vector3 pos)
{
	this->Pos = Pos;
}
void ProjectileObject::SetTargetPos(Vector3 TargetPos)
{
	this->TargetPos = TargetPos;
}
void ProjectileObject::SetVelocity(Vector3 vel)
{
	this->Vel = Vel;
}
void ProjectileObject::SetScale(Vector3 Scale)
{
	this->Scale = Scale;
}
void ProjectileObject::SetRotationSpeed(float m_RotationSpeed)
{
	this->m_fRotationSpeed = m_fRotationSpeed;
}
void ProjectileObject::SetActive(bool m_bActive)
{
	this->m_bActive = m_bActive;
}
void ProjectileObject::SetGotPlayersPos(bool m_bGotPlayersPos)
{
	this->m_bGotPlayersPos = m_bGotPlayersPos;
}
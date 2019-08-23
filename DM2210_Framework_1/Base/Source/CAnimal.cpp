#include "CAnimal.h"

CAnimal::CAnimal(ANIMAL_TYPE typeValue)
	: type(typeValue),
	m_iAnimalType(0),
	pos(1, 1, 1),
	Targetpos(1, 1, 1),
	scale(6, 6, 6),
	gravity(0, -9.8f, 0),
	dir(0, 0, 0),
	m_bActive(false),
	m_bSpawned(false),
	m_fMass(1.f),
	m_fHP(100),
	m_iCurrentBehaviour(0),
	m_fIsResting(0.f),
	m_fRandRestTime(0.f),
	m_fAngle(0.f),
	m_fWalkingTarget(0.f),
	m_bRotated(false),
	m_bDistracted(false),
	m_fDistractedCooldown(5.f),
	m_bFed(false),
	m_bIsABaby(false),
	m_fGrowUpTimer(0.f),
	m_bBreeded(false)
{
	m_fRandRestTime = Math::RandFloatMinMax(0.f, 12.f);
	switch (typeValue)
	{
	case CAnimal::GO_PIG:
		m_fMass = 55.f;
		break;
	case CAnimal::GO_COW:
		m_fMass = 60.f;
		break;
	case CAnimal::GO_CHICKEN:
		m_fMass = 50.f;
		break;
	default:
		break;
	}
	
	if (m_bIsABaby)
	{
		scale.Set(3, 3, 3);
	}
	else
	{
		scale.Set(6, 6, 6);
	}
	
	m_iAnimalType = type;
	//LoadData();
}

CAnimal::~CAnimal()
{
}

void CAnimal::Update(double dt)
{
	m_iAnimalType = type;
	type = (ANIMAL_TYPE)(m_iAnimalType);
	theCurrentBehaviour = (Behaviour)(m_iCurrentBehaviour);

	switch (theCurrentBehaviour)
	{
	case CAnimal::DOINGNOTHING: //0
		m_fIsResting += static_cast<float>(dt);
		if (m_fIsResting > m_fRandRestTime)
		{
			m_fIsResting = 0.f;
			m_iCurrentBehaviour = 1;
			m_fRandRestTime = Math::RandFloatMinMax(0.f, 12.f);
		}
		break;
	case CAnimal::WANDERING: //1
	{
		if (Targetpos == pos)
		{
			Targetpos.Set(Math::RandFloatMinMax(pos.x - 200.f, pos.x + 500.f), 1, Math::RandFloatMinMax(pos.z - 500.f, pos.z + 500.f));
			m_iCurrentBehaviour = 0;
		}
		if (Targetpos.x < -1)
			Targetpos.x = 0;
		if (Targetpos.z < -1)
			Targetpos.z = 0;

	
		if (m_fWalkingTarget > 6.f)
		{
			Targetpos.Set(Math::RandFloatMinMax(pos.x - 200.f, pos.x + 500.f), 1, Math::RandFloatMinMax(pos.z - 500.f, pos.z + 500.f));
			m_iCurrentBehaviour = 0;
			m_fWalkingTarget = 0;
			m_bRotated = false;
		}
		else
		{
			if (!m_bRotated)
			{
				dir = (Targetpos - pos).Normalize();
				float m_fTempAngle = atan2(dir.x, dir.z);
				if (Math::DegreeToRadian(m_fAngle) < m_fTempAngle)
				{
					m_fAngle += 5 * static_cast<float>(dt);
				}
				else
					m_bRotated = true;
			}
			else
			{
				m_fWalkingTarget += static_cast<float>(dt);

				float m_fspeed = Math::RandFloatMinMax(0.1f, 0.5f);
				Vector3 temp = (Targetpos - pos) * (m_fspeed / m_fMass);
				Vector3 prevpos;
				prevpos = pos;
				pos += temp;

				if ((pos - prevpos).z != 0 || (pos - prevpos).x != 0)
					dir = (pos - prevpos).Normalize();

				m_fAngle = atan2(dir.x, dir.z);

				m_fAngle = Math::RadianToDegree(Math::RandFloatMinMax(m_fAngle - 0.045f, m_fAngle + 0.04f));
			}
			
		}
		
	}
	break;
	case CAnimal::FOLLOWING: //2
		//if player holds the specific food the animal likes in his hand, they follow the player.
			if (Targetpos.x < -1)
				Targetpos.x = 0;
			if (Targetpos.z < -1)
				Targetpos.z = 0;

			if (!m_bRotated)
			{
				dir = (Targetpos - pos).Normalize();
				float m_fTempAngle = atan2(dir.x, dir.z);
				if (Math::DegreeToRadian(m_fAngle) < m_fTempAngle)
				{
					m_fAngle += 5 * static_cast<float>(dt);
				}
				else
					m_bRotated = true;
			}
			else
			{
				float m_fspeed = Math::RandFloatMinMax(0.1f, 0.5f);
				Vector3 temp = (Targetpos - pos) * (m_fspeed / m_fMass);
				Vector3 prevpos;
				prevpos = pos;
				pos += temp;

				if ((pos - prevpos).z != 0 || (pos - prevpos).x != 0)
					dir = (pos - prevpos).Normalize();

				m_fAngle = atan2(dir.x, dir.z);

				m_fAngle = Math::RadianToDegree(Math::RandFloatMinMax(m_fAngle - 0.045f, m_fAngle + 0.04f));
			}
		break;
	case CAnimal::WANTFOOD: //3

		if (m_bDistracted)
		{
			m_fDistractedCooldown -= dt;

			if (m_fDistractedCooldown < 0)
			{
				m_bDistracted = false;
				m_bRotated = false;
				m_iCurrentBehaviour = 0;
				m_fDistractedCooldown = 5;
			}
		}

		break;
	case CAnimal::ATTACKED: //4
		//if killed, set spawned & active to false. and pop the Object.
		break;
	
	default:
		m_iCurrentBehaviour = 0;
		break;
	}

	if (m_bIsABaby)
	{
		m_fGrowUpTimer += dt;
		if (m_fGrowUpTimer > 10.f)
		{
			m_bIsABaby = false;
		}
	}
	if (m_bBreeded) 
	{
		if (m_fGrowUpTimer > 10.f)
		{
			m_bBreeded = false;
			m_bFed = false;
		}
	}
}
//GetFunctions
int CAnimal::GetCurrentBehaviour()
{
	return m_iCurrentBehaviour;
}
Vector3 CAnimal::GetPosition()
{
	return pos;
}
Vector3 CAnimal::GetScale()
{
	return scale;
}
float CAnimal::GetAngle()
{
	return m_fAngle;
}
bool CAnimal::GetActive()
{
	return m_bActive;
}
bool CAnimal::GetSpawned()
{
	return m_bSpawned;
}
float CAnimal::GetHP()
{
	return m_fHP;
}
bool CAnimal::GetFed()
{
	return m_bFed;
}
bool CAnimal::GetBreeded()
{
	return m_bBreeded;
}
bool CAnimal::GetIsABaby()
{
	return m_bIsABaby;
}
float CAnimal::GetGrowUpTimer()
{
	return m_fGrowUpTimer;
}
//Set Functions
void CAnimal::SetBehaviour(int m_iCurrentBehaviour)
{
	this->m_iCurrentBehaviour = m_iCurrentBehaviour;
}
void CAnimal::SetPosition(Vector3 P)
{
	pos = P;
}
void CAnimal::SetTargetPos(Vector3 T)
{
	Targetpos = T;
}
void CAnimal::SetScale(Vector3 S)
{
	scale = S;
}
void CAnimal::SetActive(bool m_bActive)
{
	this->m_bActive = m_bActive;
}
void CAnimal::SetSpawned(bool m_bSpawned)
{
	this->m_bSpawned = m_bSpawned;
}
void CAnimal::SetDistracted(bool m_bDistracted)
{
	this->m_bDistracted = m_bDistracted;
}
void CAnimal::SetFed(bool m_bFed)
{
	this->m_bFed = m_bFed;
}
void CAnimal::SetIsBaby(bool m_bIsABaby)
{
	this->m_bIsABaby = m_bIsABaby;
}
void CAnimal::SetBreeded(bool m_bBreeded)
{
	this->m_bBreeded = m_bBreeded;
}

void CAnimal::SetAngle(float m_fAngle)
{
	this->m_fAngle = m_fAngle;
}
void CAnimal::SetGrowUpTimer(float m_fGrowUpTimer)
{
	this->m_fGrowUpTimer = m_fGrowUpTimer;
}
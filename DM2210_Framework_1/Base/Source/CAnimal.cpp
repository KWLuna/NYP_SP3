#include "CAnimal.h"



CAnimal::CAnimal(ANIMAL_TYPE typeValue)
	: type(typeValue),
	pos(1, 1, 1),
	Targetpos(1, 1, 1),
	scale(1, 1, 1),
	gravity(0, 0, 0),
	dir(0, 0, 0),
	m_bActive(false),
	m_bSpawned(false),
	m_fMass(1.f),
	m_fHP(100),
	currentBehaviour(0),
	m_fIsResting(0.f),
	m_fRandRestTime(0.f),
	m_fAngle(0.f),
	m_fWalkingTarget(0.f),
	m_bRotated(false)
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
}


CAnimal::~CAnimal()
{
}


void CAnimal::Update(double dt)
{

	theCurrentBehaviour = (Behaviour)(currentBehaviour);

	if (theCurrentBehaviour == NUM_Behaviour)
		theCurrentBehaviour = DOINGNOTHING;

	switch (theCurrentBehaviour)
	{
	case CAnimal::DOINGNOTHING:
		m_fIsResting += static_cast<float>(dt);
		if (m_fIsResting > m_fRandRestTime)
		{
			m_fIsResting = 0.f;
			currentBehaviour = 1;
			m_fRandRestTime = Math::RandFloatMinMax(0.f, 12.f);
		}
		break;
	case CAnimal::WANDERING:
	{
		if (Targetpos == pos)
		{
			Targetpos.Set(Math::RandFloatMinMax(pos.x - 200.f, pos.x + 500.f), 1, Math::RandFloatMinMax(pos.z - 500.f, pos.z + 500.f));
			currentBehaviour = 0;
		}
		if (Targetpos.x < -1)
			Targetpos.x = 0;
		if (Targetpos.z < -1)
			Targetpos.z = 0;

	
		if (m_fWalkingTarget > 6.f)
		{
			Targetpos.Set(Math::RandFloatMinMax(pos.x - 200.f, pos.x + 500.f), 1, Math::RandFloatMinMax(pos.z - 500.f, pos.z + 500.f));
			currentBehaviour = 0;
			m_fWalkingTarget = 0;
			m_bRotated = false;
		}
		else
		{
			if (!m_bRotated)
			{
				dir = (Targetpos - pos).Normalize();
				float m_fTempAngle = atan2(dir.x, dir.z);
				std::cout << m_fTempAngle << std::endl;
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
	case CAnimal::ATTACKED:
		//if killed, set spawned & active to false. and pop the Object.
		break;
	case CAnimal::FOLLOWING:
		//if player holds a carrot in his hand, follow the player.
		break;
	default:
		currentBehaviour = 5;
		break;
	}
}

int CAnimal::getcurrentBehaviour()
{
	return currentBehaviour;
}

void CAnimal::SetBehaviour(int m_iB)
{
	currentBehaviour = m_iB;
}

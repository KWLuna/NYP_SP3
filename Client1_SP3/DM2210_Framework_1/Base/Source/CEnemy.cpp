#include "CEnemy.h"
#include "SoundEngine.h"

CEnemy::CEnemy(ENEMY_TYPE typeValue)
	: type(typeValue),
	pos(1, 1, 1),
	Targetpos(1, 1, 1),
	Scale(6, 6, 6),
	gravity(0, -9.8f, 0),
	dir(1, 1, 1),
	m_bActive(false),
	m_bSpawned(false),
	m_fMass(55.f),
	m_fHP(100),
	m_iCurrentBehaviour(0),
	m_fStrength(5.f),
	m_fAttackingTime(0.f),
	m_bAttacked(false),
	Newpos(false),
	m_fPlayersDamage(0.f)
{
	m_fRandRestTime = Math::RandFloatMinMax(0.f, 12.f);
	m_fSpeed = Math::RandFloatMinMax(0.1f, 0.5f);
	Targetpos.Set(Math::RandFloatMinMax(pos.x - 100.f, pos.x + 100.f), 1, Math::RandFloatMinMax(pos.z - 100.f, pos.z + 100.f));
}
CEnemy::CEnemy(int type, Vector3 pos, float HP, float Strength, bool spawned)
	: Targetpos(1, 1, 1),
	Scale(6, 6, 6),
	gravity(0, -9.8f, 0),
	dir(1, 1, 1),
	m_bActive(false),
	m_bSpawned(false),
	m_fMass(55.f),
	m_fHP(100),
	m_iCurrentBehaviour(0),
	m_fStrength(5.f),
	m_fAttackingTime(0.f),
	m_bAttacked(false),
	m_bRotated(false),
	m_fAngle(0.f),
	Newpos(false),
	m_fPlayersDamage(0.f)
{
	m_fRandRestTime = Math::RandFloatMinMax(0.f, 12.f);
	m_fSpeed = Math::RandFloatMinMax(0.1f, 0.5f);
	Targetpos.Set(Math::RandFloatMinMax(pos.x - 100.f, pos.x + 100.f), 1, Math::RandFloatMinMax(pos.z - 100.f, pos.z + 100.f));

	this->type = (ENEMY_TYPE)(type);
	this->pos = pos;
	this->m_fHP = HP;
	this->m_fStrength = Strength;
	this->m_bSpawned = spawned;
}

CEnemy::~CEnemy()
{
}

void CEnemy::Update(double dt, std::vector<Vector3*> WorldObjectPositionList)
{
	if (Targetpos.x < 0)
		Targetpos.x = 0;
	else if (Targetpos.x > 25000)
		Targetpos.x = 25000;
	if (Targetpos.z < 0)
		Targetpos.z = 0;
	else if (Targetpos.z > 25000)
		Targetpos.z = 25000;

	theCurrentBehaviour = (Enemy_Behaviour)(m_iCurrentBehaviour);

	switch (theCurrentBehaviour)
	{
	case CEnemy::DOINGNOTHING: //0
		m_fIsResting += static_cast<float>(dt);
		if (m_fIsResting > m_fRandRestTime)
		{
			m_fIsResting = 0.f;
			m_iCurrentBehaviour = 1;
			m_fRandRestTime = Math::RandFloatMinMax(0.f, 12.f);
			if (type == GO_ZOMBIE)
				CSoundEngine::GetInstance()->PlayASound3D("Zombie_Resting", pos);
			else if (type == GO_WITCH)
				CSoundEngine::GetInstance()->PlayASound3D("Witch_Resting", pos);
		}
		break;
	case CEnemy::WANDERING: //1
	{
		m_fSpeed = Math::RandFloatMinMax(0.1f, 0.5f);

		if (Targetpos == pos)
		{
			Targetpos.Set(Math::RandFloatMinMax(pos.x - 100.f, pos.x + 100.f), 1, Math::RandFloatMinMax(pos.z - 100.f, pos.z + 100.f));
			m_iCurrentBehaviour = 0;
		}
		if (Targetpos.x < 0)
			Targetpos.x = 0;
		else if (Targetpos.x > 25000)
			Targetpos.x = 25000;
		if (Targetpos.z < 0)
			Targetpos.z = 0;
		else if (Targetpos.z > 25000)
			Targetpos.z = 25000;
	
		if (m_fWalkingTarget > 6.f)
		{
			Targetpos.Set(Math::RandFloatMinMax(pos.x - 100.f, pos.x + 100.f), 1, Math::RandFloatMinMax(pos.z - 100.f, pos.z + 100.f));
			m_iCurrentBehaviour = 0;
			m_fWalkingTarget = 0;
			m_bRotated = false;
		}
		else
		{
			if (!m_bRotated)
			{
				if ((Targetpos.x - pos.x) != 0 && (Targetpos.z - pos.z) != 0)
				{
					dir = (Targetpos - pos).Normalize();
				}

				float m_fTempAngle = atan2(dir.x, dir.z);
				if (Math::DegreeToRadian(m_fAngle) < m_fTempAngle)
				{
					m_fAngle += 6 * static_cast<float>(dt);
				}
				else
					m_bRotated = true;
			}
			else
			{
				m_fWalkingTarget += static_cast<float>(dt);

				Vector3 temp = (Targetpos - pos) * (m_fSpeed / m_fMass);
				Vector3 prevpos;
				prevpos = pos;
				Vector3* last = WorldObjectPositionList[WorldObjectPositionList.size() - 1];

				for (std::vector<Vector3*>::iterator it2 = WorldObjectPositionList.begin(); it2 != WorldObjectPositionList.end(); ++it2)
				{
					Vector3* WorldObjectPos = (Vector3*)*it2;

					if (pos.x + temp.x > WorldObjectPos->x - 50 && pos.x + temp.x < WorldObjectPos->x + 50)
					{
						if (pos.z + temp.z > WorldObjectPos->z - 50 && pos.z + temp.z < WorldObjectPos->z + 50)
						{
							if (!Newpos)
							{
								Targetpos.Set(Math::RandFloatMinMax(pos.x - 100.f, pos.x + 100.f), 1, Math::RandFloatMinMax(pos.z - 100.f, pos.z + 100.f));
								Newpos = true;
								m_bRotated = false;
							}
							break;
						}
					}
					else if (WorldObjectPos == last)
					{
						pos += temp;
						Newpos = false;
					}
				}

				if ((pos.x - prevpos.x) != 0 && (pos.z - prevpos.z) != 0)
				{
					dir = (pos - prevpos).Normalize();
				}

				m_fAngle = atan2(dir.x, dir.z);

				m_fAngle = Math::RadianToDegree(Math::RandFloatMinMax(m_fAngle - 0.045f, m_fAngle + 0.04f));
			}
		}
	}
	break;
	case CEnemy::FOLLOWING: //2
		m_fSpeed = Math::RandFloatMinMax(0.1f, 1.0f);

		//if player holds the specific food the animal likes in his hand, they follow the player.
		if (Targetpos.x < 0)
			Targetpos.x = 0;
		else if (Targetpos.x > 25000)
			Targetpos.x = 25000;
		if (Targetpos.z < 0)
			Targetpos.z = 0;
		else if (Targetpos.z > 25000)
			Targetpos.z = 25000;

		if (Targetpos == pos)
		{
			Targetpos.x = Targetpos.x - 1;
			Targetpos.z = Targetpos.z - 1;
		}
			
		if (!m_bRotated)
		{
			if ((Targetpos.x - pos.x) != 0 && (Targetpos.z - pos.z) != 0)
			{
				dir = (Targetpos - pos).Normalize();
			}

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

			Vector3 temp = (Targetpos - pos) * (m_fSpeed / m_fMass);
			Vector3 prevpos;
			prevpos = pos;
			
			Vector3* last = WorldObjectPositionList[WorldObjectPositionList.size() - 1];
			for (std::vector<Vector3*>::iterator it2 = WorldObjectPositionList.begin(); it2 != WorldObjectPositionList.end(); ++it2)
			{
				Vector3* WorldObjectPos = (Vector3*)*it2;

				if (pos.x + temp.x > WorldObjectPos->x - 50 && pos.x + temp.x < WorldObjectPos->x + 50)
				{
					if (pos.z + temp.z > WorldObjectPos->z - 50 && pos.z + temp.z < WorldObjectPos->z + 50)
					{
						if (!Newpos)
						{
							Newpos = true;
							m_bRotated = false;
						}
						break;
					}
				}
				else if (WorldObjectPos == last)
				{
					pos += temp;
					Newpos = false;
				}
			}

			if ((pos.x - prevpos.x) != 0 && (pos.z - prevpos.z) != 0)
			{
				dir = (pos - prevpos).Normalize();
			}

			m_fAngle = atan2(dir.x, dir.z);

			m_fAngle = Math::RadianToDegree(Math::RandFloatMinMax(m_fAngle - 0.045f, m_fAngle + 0.04f));
		}
		break;
	case CEnemy::ATTACKING: //3
		if (m_bAttacked == false)
			m_fAttackingTime -= static_cast<float>(dt);

		if (m_fAttackingTime < 0)
		{
			m_bAttacked = true;
			m_fAttackingTime = 3;
		}
		else
			m_bAttacked = false;
		break;
	case CEnemy::ATTACKED: //4
		//if killed, set spawned & active to false. 
		m_fHP -= m_fPlayersDamage;
		if (m_fHP < 0)
		{
			m_fHP = 100;
			m_bSpawned = false;
			m_bActive = false;
			if (type == GO_ZOMBIE)
				CSoundEngine::GetInstance()->PlayASound3D("Zombie_Dying", pos);
			else if (type == GO_WITCH)
				CSoundEngine::GetInstance()->PlayASound3D("Witch_Dying", pos);
		}
		pos.y -= 5;
		m_iCurrentBehaviour = 2;

		break;
	default:
		m_iCurrentBehaviour = 0;
		break;
	}
}
//GetFunctions
int CEnemy::GetCurrentBehaviour()
{
	return m_iCurrentBehaviour;
}
Vector3 CEnemy::GetPosition()
{
	return pos;
}
Vector3 CEnemy::GetScale()
{
	return Scale;
}
float CEnemy::GetAngle()
{
	return m_fAngle;
}
bool CEnemy::GetActive()
{
	return m_bActive;
}
bool CEnemy::GetSpawned()
{
	return m_bSpawned;
}
float CEnemy::GetHP()
{
	return m_fHP;
}
float CEnemy::GetSpeed()
{
	return m_fSpeed;
}
float CEnemy::GetStrength()
{
	return m_fStrength;
}
bool CEnemy::GetAttackedPlayer()
{
	return m_bAttacked;
}
bool CEnemy::GetRotated()
{
	return m_bRotated;
}
//Set Functions
void CEnemy::SetBehaviour(int m_iCurrentBehaviour)
{
	this->m_iCurrentBehaviour = m_iCurrentBehaviour;
	theCurrentBehaviour = (Enemy_Behaviour)(m_iCurrentBehaviour);
}
void CEnemy::SetPosition(Vector3 P)
{
	pos = P;
}
void CEnemy::SetTargetPos(Vector3 T)
{
	Targetpos = T;
}
void CEnemy::SetScale(Vector3 Scale)
{
	this->Scale = Scale;
}
void CEnemy::SetActive(bool m_bActive)
{
	this->m_bActive = m_bActive;
}
void CEnemy::SetSpawned(bool m_bSpawned)
{
	this->m_bSpawned = m_bSpawned;
}
void CEnemy::SetHP(float m_fHP)
{
	this->m_fHP = m_fHP;
}
void CEnemy::SetSpeed(float m_fSpeed)
{
	this->m_fSpeed = m_fSpeed;
}
void CEnemy::SetStrength(float m_fStrength)
{
	this->m_fStrength = m_fStrength;
}
void CEnemy::SetAttacking(bool m_bAttacked)
{
	this->m_bAttacked = m_bAttacked;
}
void CEnemy::SetPlayersDamage(float m_fPlayersDamage)
{
	this->m_fPlayersDamage = m_fPlayersDamage;
}
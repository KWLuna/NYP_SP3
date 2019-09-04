#ifndef ENEMY_H
#define ENEMY_H

#include "Vector3.h"
#include "Vertex.h"
#include <vector>

class CEnemy
{
public:
	enum ENEMY_TYPE
	{
		GO_ZOMBIE = 0,
		GO_WITCH,
	};
	enum Enemy_Behaviour
	{
		DOINGNOTHING = 0,
		WANDERING,	//Move randomly to any location
		FOLLOWING,	//Face Direction of player -> Move towards player
		ATTACKING,	//Attacking Player
		ATTACKED,	//Knocked backed
	};
	ENEMY_TYPE type;

	CEnemy(ENEMY_TYPE typeValue = GO_ZOMBIE);
	CEnemy(int type, Vector3 pos, float HP, float Strength, bool spawned);
	void Update(double dt, std::vector<Vector3*> WorldObjectPositionList);
	~CEnemy();

	//Get Functions
	int GetCurrentBehaviour();
	Vector3 GetPosition();
	Vector3 GetScale();
	float GetAngle();
	bool GetActive();
	bool GetSpawned();
	float GetHP();
	float GetSpeed();
	float GetStrength();
	bool GetAttackedPlayer();
	bool GetRotated();
	//Set Functions
	void SetBehaviour(int m_iCurrentBehaviour);
	void SetPosition(Vector3 P);
	void SetTargetPos(Vector3 T);
	void SetScale(Vector3 S);
	void SetActive(bool m_bActive);
	void SetSpawned(bool m_bSpawned);
	void SetHP(float m_fHP);
	void SetSpeed(float m_fSpeed);
	void SetStrength(float m_fStrength);
	void SetAttacking(bool m_bAttacked);

	void SetPlayersDamage(float m_fPlayersDamage);

private:
	//Behaviour
	Enemy_Behaviour theCurrentBehaviour;
	int m_iCurrentBehaviour;

	Vector3 pos;
	Vector3 Targetpos;
	Vector3 Scale;
	Vector3 gravity;//for physics
	Vector3 dir;
	bool m_bActive;
	bool m_bSpawned;
	float m_fMass;//for physics
	float m_fHP;
	float m_fAngle;
	float m_fSpeed;
	//Resting
	float m_fIsResting;
	float m_fRandRestTime;
	//For Walking
	float m_fWalkingTarget;
	bool m_bRotated;
	//For Attacking Player
	float m_fStrength;
	float m_fAttackingTime;
	bool m_bAttacked;
	float m_fPlayersDamage;

	bool Newpos;

};

#endif
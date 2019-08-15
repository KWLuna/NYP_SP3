#ifndef ANIMAL_H
#define ANIMAL_H

#include "Vector3.h"
#include "Vertex.h"

class CAnimal
{
public:
	enum ANIMAL_TYPE
	{
		GO_NONE = 0,
		GO_PIG,
		GO_COW,
		GO_CHICKEN,
		GO_TOTAL,
	};
	ANIMAL_TYPE type;

	Vector3 pos;
	Vector3 scale;
	Vector3 gravity;
	Vector3 dir;
	Vector3 Targetpos;
	bool m_bSpawned;
	bool m_bActive;
	float m_fMass;
	float m_fHP;
	float m_fAngle;

	CAnimal(ANIMAL_TYPE typeValue = GO_PIG);
	void Update(double dt);
	~CAnimal();

	//GetFunctions
	int getcurrentBehaviour();
	//Set Functions
	void SetBehaviour(int m_iB);
protected: 
	enum Behaviour
	{
		DOINGNOTHING = 0,
		WANDERING,	//Move randomly to any location
		FOLLOWING,	//Face Direction of player -> Move towards player
		ATTACKED,	//Knocked backed -> run away
		NUM_Behaviour,
	};
	Behaviour theCurrentBehaviour;
	int currentBehaviour;

	//Resting
	float m_fIsResting;
	float m_fRandRestTime;
	float m_fWalkingTarget;
	bool m_bRotated;
};

#endif
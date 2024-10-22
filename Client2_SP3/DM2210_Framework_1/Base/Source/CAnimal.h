#ifndef ANIMAL_H
#define ANIMAL_H

#include "Vector3.h"
#include "Vertex.h"
#include <iostream>
#include <string>
#include <vector>
#include "SoundEngine.h"

class CAnimal
{
public:
	enum ANIMAL_TYPE
	{
		GO_PIG = 0,
		GO_COW,
		GO_CHICKEN,
	};
	enum Behaviour
	{
		DOINGNOTHING = 0,
		WANDERING,	//Move randomly to any location
		FOLLOWING,	//Face Direction of player -> Move towards player
		WANTFOOD,
		ATTACKED,	//Knocked backed -> run away
	};
	
	ANIMAL_TYPE type;
	int m_iAnimalType;

	CAnimal(ANIMAL_TYPE typeValue = GO_PIG);

	CAnimal(int animalType, Vector3 Pos, Vector3 Scale, bool spawned, bool baby, double growTimer);

	void Update(double dt, std::vector<Vector3*> WorldObjectPositionList);
	~CAnimal();

	//Get Functions
	int GetCurrentBehaviour();
	Vector3 GetPosition();
	Vector3 GetScale();
	float GetAngle();
	bool GetActive();
	bool GetSpawned();
	float GetHP();
	bool GetFed();
	bool GetBreeded();
	bool GetIsABaby();
	double GetGrowUpTimer();
	//Set Functions
	void SetBehaviour(int m_iCurrentBehaviour);
	void SetPosition(Vector3 P);
	void SetTargetPos(Vector3 T);
	void SetScale(Vector3 S);
	void SetActive(bool m_bActive);
	void SetSpawned(bool m_bSpawned);
	void SetDistracted(bool m_bDistracted);
	void SetFed(bool m_bFed);
	void SetIsBaby(bool m_bIsABaby);
	void SetBreeded(bool m_bBreeded);
	void SetAngle(float m_fAngle);
	void SetGrowUpTimer(double m_fGrowUpTimer);

	void SetPlayersDamage(float m_fPlayersDamage);
private:
	//Behaviour
	Behaviour theCurrentBehaviour;
	int m_iCurrentBehaviour;

	Vector3 pos;
	Vector3 Targetpos;
	Vector3 scale;
	Vector3 gravity;//for physics
	Vector3 dir;
	bool m_bActive;
	bool m_bSpawned;
	float m_fMass;//for physics
	float m_fHP;
	float m_fAngle;

	//Resting
	float m_fIsResting;
	float m_fRandRestTime;
	//For Walking
	float m_fWalkingTarget;
	bool m_bRotated;
	bool m_bDistracted;
	float m_fDistractedCooldown;
	//For Taming&Breeding
	bool m_bFed;
	bool m_bIsABaby;
	double m_fGrowUpTimer;
	bool m_bBreeded;


	float m_fPlayersDamage;
	bool Newpos;
};

#endif
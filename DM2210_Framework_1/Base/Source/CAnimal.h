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
	Vector3 Targetpos;
	bool spawned;
	bool active;
	float mass;
	float HP;

	CAnimal(ANIMAL_TYPE typeValue = GO_PIG);
	void Update(double dt);
	~CAnimal();

	//GetFunctions
	int getcurrentBehaviour();
	//Set Functions
	void SetBehaviour(int B);
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
	float IsResting;
	float RandRestTime;

};

#endif
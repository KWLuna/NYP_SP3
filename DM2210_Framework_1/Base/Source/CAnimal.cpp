#include "CAnimal.h"



CAnimal::CAnimal(ANIMAL_TYPE typeValue)
	: type(typeValue),
	pos(1, 1, 1),
	Targetpos(1, 1, 1),
	scale(1, 1, 1),
	gravity(0, 0, 0),
	active(false),
	spawned(false),
	mass(1.f),
	HP(100),
	currentBehaviour(0),
	IsResting(0.f),
	RandRestTime(0.f)
{
	RandRestTime = Math::RandFloatMinMax(5.f, 20.f);


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
		IsResting += static_cast<float>(dt);
		if (IsResting > RandRestTime)
		{
			IsResting = 0.f;
			currentBehaviour = 1;
			RandRestTime = Math::RandFloatMinMax(5.f, 20.f);
		}
		break;
	case CAnimal::WANDERING:
	{
		if (Targetpos.x < -1)
			Targetpos.x = 0;
		if (Targetpos.z < -1)
			Targetpos.z = 0;

		Vector3 temp = (Targetpos - pos) * 0.01f;
		pos += temp;
		if (pos == Targetpos)
		{
			Targetpos.Set(Math::RandFloatMinMax(pos.x - 200.f, pos.x + 500.f), 1, Math::RandFloatMinMax(pos.z - 500.f, pos.z + 500.f));
			currentBehaviour = 0;
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

void CAnimal::SetBehaviour(int B)
{
	currentBehaviour = B;
}

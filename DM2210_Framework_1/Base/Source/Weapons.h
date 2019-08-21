#pragma once
#include "Vector3.h"
#include "Physics.h"
#include "CAnimal.h"

class Weapons
{
public:
	Weapons();
	virtual ~Weapons();
protected:
	enum WEAPON_TYPE
	{
		MELEE,
		RANGE,
	};
	WEAPON_TYPE type;
	Vector3 vPosition;
	float fRotation;
	Vector3 vScale;
	float m_range;

	float leftmaxr;
	float leftmaxt;
	float rightmaxr;
	float rightmaxt;
	float attacktiltup;

	float leftrestt;
	float leftrestr;
	float rightrestt;
	float rightrestr;
	float resttiltup;

	float curr;
	float curt;
	float swingtime;

	bool curswing;
	bool side;

	Physics weaponphysics;

public:
	virtual void Init(void);
	virtual void UpdateAnimal(const double dt, Vector3 dir, Vector3 origin, std::vector<CAnimal*> animalist);

	void SetPosition(Vector3 vPosition);
	Vector3 GetPosition(void) const;

	void SetRotation(const float iRotation);
	float GetRotation(void) const;

	void SetScale(Vector3 vScale);
	Vector3 GetScale(void) const;

	virtual void Render(void);

	void PrintSelf(void);

	float GetLeftMaxR();
	float GetLeftMaxT();
	float GetRightMaxR();
	float GetRightMaxT();
	float GetAttackUpTilt();

	float GetLeftRestR();
	float GetLeftRestT();
	float GetRightRestR();
	float GetRightRestT();
	float GetRestUpTilt();

	float GetCurT();
	float GetCurR();

	void SetCurSwing();
	bool GetCurSwing();
	bool GetSide();
};


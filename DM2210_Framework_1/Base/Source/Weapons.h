#pragma once
#include "Vector3.h"
#include "Physics.h"

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
public:
	virtual void Init(void);
	virtual void Update(const double dt);

	void SetPosition(Vector3 vPosition);
	Vector3 GetPosition(void) const;

	void SetRotation(const float iRotation);
	float GetRotation(void) const;

	void SetScale(Vector3 vScale);
	Vector3 GetScale(void) const;

	virtual void Render(void);

	void PrintSelf(void);
};


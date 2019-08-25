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

	bool activeonce;

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

	char tiletype;
	bool standingonem;
	Vector3 blockset;

	bool activatefurnace;
	int FurnaceID;

	bool activateonce;

	Physics weaponphysics;

public:
	virtual void Init(void);
	virtual void UpdateAnimal(const double dt, Vector3 dir, Vector3 origin, std::vector<CAnimal*> animalist);
	void UpdateFurnace(const double dt, Vector3 dir, Vector3 origin, std::vector<int> FurnaceX, std::vector<int> FurnaceZ);
	virtual void UpdateTile(const double dt, Vector3 dir, Vector3 origin, char tilearray[]);

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

	void SetTileType(char type);
	char GetTileType();
	void SetStandinOn(bool stand);
	bool GetStandinOn();
	Vector3 GetBlockPlacement();

	bool GetFurnace();
	void SetFurnaceID(int ID);
	int GetFurnaceID();
};


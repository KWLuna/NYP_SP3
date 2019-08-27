#pragma once
#include "Vector3.h"
#include "Physics.h"
#include "CAnimal.h"
#include "CEnemy.h"
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

	int maxswings;

	bool curswing;
	bool side;

	char tiletype;
	bool standingonem;
	Vector3 blockset;

	bool rclick;

	bool furnaceclick;
	int FurnaceID;
	bool activateonce;

	bool berryclick;

	bool waterclick;

	bool feedanimalpig;
	bool feedanimalcow;
	bool feedanimalchicken;
	int animalID;
	int foodheld;

	Physics weaponphysics;

public:
	virtual void Init(void);
	virtual void UpdateAnimal(const double dt, Vector3 dir, Vector3 origin, std::vector<CAnimal*> animalist, float m_fPlayersDamage);
	virtual void UpdateEnemy(const double dt, Vector3 dir, Vector3 origin, std::vector<CEnemy*> enemylist, float m_fPlayersDamage);
	virtual void UpdateTile(const double dt, Vector3 dir, Vector3 origin, char tilearray[]);
	void UpdateFurnace(std::vector<char> XFurnace, std::vector<char> ZFurnace);

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

	void SetRClick();
	void SetLClick();
	bool GetClick();

	bool GetFurnaceClick();
	void SetFurnaceClick(bool set);
	int GetFurnaceID();
	void ResetFurnaceID();

	bool GetFeedAnimalPig();
	void SetFeedAnimalPig(bool set);
	bool GetFeedAnimalCow();
	void SetFeedAnimalCow(bool set);
	bool GetFeedAnimalChicken();
	void SetFeedAnimalChicken(bool set);
	void SetAnimalID(int set);
	int GetAnimalID();
	void SetFoodHeld(int held);

	bool GetBerryClick();
	void SetBerryClick(bool set);

	bool GetWaterClick();
	void SetWaterClick(bool set);

	int GetIntMaxSwings();
};


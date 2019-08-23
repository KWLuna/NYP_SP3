#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "Vector3.h"

enum PROJECTILE_TYPE
{
	P_FIREBALL,
};

class ProjectileObject
{
public:
	ProjectileObject(PROJECTILE_TYPE = P_FIREBALL);
	~ProjectileObject(void);
	void Update(double dt);

	//Get Functions
	PROJECTILE_TYPE GetType();
	Vector3 GetPos();
	Vector3 GetScale();
	bool GetActive();
	float GetRotation();
	float GetTimeTravelled();
	bool GetGotPlayersPos();
	//Set Functions
	void SetType(PROJECTILE_TYPE Type);
	void SetPos(Vector3 pos);
	void SetTargetPos(Vector3 TargetPos);
	void SetVelocity(Vector3 vel);
	void SetScale(Vector3 Scale);
	void SetRotationSpeed(float m_RotationSpeed);
	void SetActive(bool m_bActive);
	void SetGotPlayersPos(bool m_bGotPlayersPos);
	void SetTimeTravelled(float m_fTimeTravelled);
private:
	PROJECTILE_TYPE Type; //Type of Projectile
	Vector3 Pos;	//Position of Projectile
	Vector3 TargetPos;//Position of where its suppose to go
	Vector3 Vel;	//Velocity of Projectile
	Vector3 Scale;	//Scale of Projectile
	float m_fRotation;	//Rotation of Projectile
	float m_fRotationSpeed;	//Rotational speed of Projectile
	bool m_bActive;
	float m_fSpeed;
	float m_fTimeTravelled;
	bool m_bGotPlayersPos;
};

#endif
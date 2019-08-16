#ifndef PARTICLE_H
#define PARTICLE_H
#include "Vector3.h"

enum ParticleObject_TYPE
{
	P_WATER = 0,
	P_WATERSPLAT,
	P_SKYLANTERN,
	P_CUBERRY,
	P_GLOW,
	P_TOTAL,
};

class ParticleObject
{
public:
	ParticleObject(ParticleObject_TYPE = P_WATER);
	~ParticleObject(void);

	ParticleObject_TYPE type; //Type of Particle
	Vector3 m_gravity;
	Vector3 pos;	//Position of Particle
	Vector3 vel;	//Velocity of Particle
	Vector3 scale;	//Scale of Particle
	float rotation;	//Rotation of Particle
	float rotationSpeed;	//Rotational speed of particle

	bool active;
};

#endif
#ifndef PARTICLE_H
#define PARTICLE_H
#include "Vector3.h"

enum ParticleObject_TYPE
{
	P_WATER = 0,
	P_WATERSPLAT,
	P_SNOWFLAKE,
	P_LEAF,
	P_DEADLEAF,
	P_TOTAL,
};

class ParticleObject
{
public:
	ParticleObject(ParticleObject_TYPE = P_WATER);
	~ParticleObject(void);
	void Update(double dt);

	ParticleObject_TYPE type; //Type of Particle
	Vector3 m_gravity;
	Vector3 wind;
	Vector3 pos;	//Position of Particle
	Vector3 vel;	//Velocity of Particle
	Vector3 scale;	//Scale of Particle
	float rotation;	//Rotation of Particle
	float rotationSpeed;	//Rotational speed of particle

	bool active;
};

#endif
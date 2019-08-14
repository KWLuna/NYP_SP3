#include "Particle.h"


ParticleObject::ParticleObject(ParticleObject_TYPE typeValue)
	: type(typeValue),
	pos(0,0,0),
	scale(1,1,1),
	rotation(0),
	rotationSpeed(0),
	m_gravity(0,-9.8f,0),
	active(false)
{

}

ParticleObject::~ParticleObject(void)
{

}
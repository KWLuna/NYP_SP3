#include "Particle.h"


ParticleObject::ParticleObject(ParticleObject_TYPE typeValue)
	: type(typeValue),
	pos(0,0,0),
	scale(1,1,1),
	rotation(0),
	rotationSpeed(0),
	m_gravity(0,-9.8f,0),
	wind (1,1,1),
	active(false)
{

}

ParticleObject::~ParticleObject(void)
{

}

void ParticleObject::Update(double dt)
{
	float m_fpseed = 10.f;
	if (active)
	{
		vel += (wind + m_gravity) * m_fpseed * static_cast<float>(dt);
		pos += vel * static_cast<float>(dt);
		/*
			particle->vel += particle->m_gravity * 10 *  (float)dt;
				particle->pos += particle->vel * (float)dt * 15.f;
		*/
	}
}
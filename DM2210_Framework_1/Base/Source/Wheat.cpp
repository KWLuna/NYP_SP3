#include "Crops.h"

Crops::Crops()
{
	m_dTimeSincePlanted = 0;
	m_iCropType = 0;
}


Crops::~Crops()
{
}

void Crops::update(double dt)
{
	m_dTimeSincePlanted += 1 * dt;
}
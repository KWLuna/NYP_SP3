#include "Crops.h"

Crops::Crops(int TYPE)
{
	m_dTimeSincePlanted = 0;
	m_iCropType = TYPE;
}

Crops::~Crops()
{
}

void Crops::update(double dt)
{
	m_dTimeSincePlanted += 1 * dt;
}
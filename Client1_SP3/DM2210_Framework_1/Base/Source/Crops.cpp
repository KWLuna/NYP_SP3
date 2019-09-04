#include "Crops.h"
#include <iostream>
Crops::Crops(int TYPE , int xPos , int zPos , double timeSincePlanted)
{
	m_iCropType = TYPE;
	m_iCropState = 0;

	m_iXTile = xPos;
	m_iZTile = zPos;

	m_dTimeSincePlanted = timeSincePlanted;
}

int Crops::GetXTile()
{
	return m_iXTile;
}

int Crops::GetCropType()
{
	return m_iCropType;
}

int Crops::GetZTile()
{
	return m_iZTile;
}

void Crops::SetXTile(int x)
{
	m_iXTile = x;
}

void Crops::SetZTile(int z)
{
	m_iZTile = z;
}

int Crops::GetState()
{
	return m_iCropState;
}

Crops::~Crops()
{
}

double Crops::getTimeSincePlanted()
{
	return m_dTimeSincePlanted;
}

void Crops::setTimeSincePlanted(double time)
{
	m_dTimeSincePlanted = time;
}

void Crops::update(double dt)
{
	if (m_dTimeSincePlanted >= TIME_TO_GROW)
	{
		m_iCropState = 1;
		m_bFinishedGrowing = true;
	}
	else
		m_dTimeSincePlanted += 1 * dt;
}
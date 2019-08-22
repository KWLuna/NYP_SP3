#include "Crops.h"
#include <iostream>
Crops::Crops(int TYPE , int xPos , int zPos)
{
	m_dTimeSincePlanted = 0;
	m_iCropType = TYPE;
	m_iCropState = 0;

	m_iXTile = xPos;
	m_iZTile = zPos;
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

void Crops::SetXPos(int x)
{
	m_iXTile = x;
}

void Crops::SetZPos(int z)
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

void Crops::update(double dt)
{
	m_dTimeSincePlanted += 1 * dt;

	if (m_dTimeSincePlanted >= TIME_TO_GROW)
	{
		m_iCropState = 1;
		m_bFinishedGrowing = true;
	}
}
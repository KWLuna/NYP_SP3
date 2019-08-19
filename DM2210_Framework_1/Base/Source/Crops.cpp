#include "Crops.h"
#include <iostream>
Crops::Crops(int TYPE , int xPos , int zPos)
{
	m_dTimeSincePlanted = 0;
	m_iCropType = TYPE;
	m_iCropState = 0;

	m_iXpos = xPos;
	m_iZpos = zPos;
}

int Crops::GetXPos()
{
	return m_iXpos;
}

int Crops::GetCropType()
{
	return m_iCropType;
}

int Crops::GetZPos()
{
	return m_iZpos;
}

void Crops::SetXPos(int x)
{
	m_iXpos = x;
}

void Crops::SetZPos(int z)
{
	m_iZpos = z;
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

	std::cout << m_dTimeSincePlanted << " " << TIME_TO_GROW << std::endl;
}
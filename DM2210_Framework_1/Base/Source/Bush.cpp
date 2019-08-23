#include "Bush.h"

Bush::Bush(int tileX , int tileZ)
{
	m_dTimeSinceHarvest = 0;
	m_bHarvested = false;

	m_iTileX = tileX;
	m_iTileZ = tileZ;
}

Bush::~Bush()
{

}

int Bush::GetXTile()
{
	return m_iTileX;
}
int Bush::GetZTile()
{
	return m_iTileZ;
}

bool Bush::GetRespawnYet()
{
	return m_bRespawn;
}

void Bush::Update(double dt)
{
	m_dTimeSinceHarvest += 1 * dt;

	if (m_dTimeSinceHarvest >= RESPAWN_TIME)
		m_bRespawn = true;
}

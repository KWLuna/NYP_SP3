#include "Season.h"



Season::Season()
{
}


Season::~Season()
{
}

void Season::Update(double dt)
{
	m_fSeasonTime = 1 * dt;
	if (m_fSeasonTime > 10)
	{
		m_iSeasonChanger = Math::RandIntMinMax(0, 4);
	}
	seasons = (TYPE_SEASON)(m_iSeasonChanger);

	if (seasons == NUM_SEASON)
		seasons = SPRING;

}
int Season::getSeason()
{
	return m_iSeasonChanger;
}
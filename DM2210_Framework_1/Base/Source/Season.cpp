#include "Season.h"



Season::Season()
	: m_iSeasonChanger(0),
	m_fSeasonTime (0)
{
}


Season::~Season()
{
}

void Season::Update(double dt)
{
	m_fSeasonTime += 1 * dt;
	if (m_fSeasonTime > 10)
	{
		switch (m_iSeasonChanger)
		{
		case 0: //spring
			m_iSeasonChanger = 1;
			break;
		case 1: //summer
			m_iSeasonChanger = 2;
			break;
		case 2: //fall
			m_iSeasonChanger = 3;
			break;
		case 3: //winter
			m_iSeasonChanger = 0;
			break;
		default:
			break;
		}
		m_fSeasonTime = -1;
	}
	seasons = (TYPE_SEASON)(m_iSeasonChanger);

	if (seasons == NUM_SEASON)
		seasons = SPRING;

}
int Season::getSeason()
{
	return m_iSeasonChanger;
}
void Season::setSeason(int m_iSeason)
{
	m_iSeasonChanger = m_iSeason;
}
int Season::getTimer()
{
	return m_fSeasonTime;
}
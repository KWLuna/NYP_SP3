#ifndef SEASON_H
#define SEASON_H
#include <iostream>
#include "Vector3.h"

class Season
{
public:
	enum TYPE_SEASON
	{
		SPRING = 0,
		SUMMER,
		FALL,
		WINTER,
		NUM_SEASON,
	};

	TYPE_SEASON seasons;
	Season();
	~Season();
	void Update(double dt);
	int getSeason();
	void setSeason(int m_iSeason);
	int getTimer();
private:
	int m_iSeasonChanger;
	float m_fSeasonTime;
};

#endif
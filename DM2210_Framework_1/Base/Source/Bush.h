#pragma once
class Bush
{
public:
	Bush(int tileX , int tileZ);
	~Bush();

	void Update(double dt);
	bool GetRespawnYet();

	int GetXTile();
	int GetZTile();

private:
	double m_dTimeSinceHarvest;
	bool m_bHarvested;
	bool m_bRespawn;

	int m_iTileX;
	int m_iTileZ;

	const int RESPAWN_TIME = 60;
};


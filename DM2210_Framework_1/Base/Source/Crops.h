#pragma once
class Crops
{
public:
	enum CROP_TYPE
	{
		CARROT,
		SEED,
	}; 

	Crops(int TYPE , int xPos , int zPos);
	~Crops();
	
	bool GetGrowth;
	void update(double dt);
	int GetState();

	void SetXPos(int x);
	void SetZPos(int z);

	int GetXPos();
	int GetZPos();
	int GetCropType();

private:
	bool m_bFinishedGrowing;
	double m_dTimeSincePlanted;
	const int TIME_TO_GROW = 100;

	//CROP_TYPE
	int m_iCropType;
	//CROP_STATE
	int m_iCropState;

	int m_iXpos, m_iZpos;
};


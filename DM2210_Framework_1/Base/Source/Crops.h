#pragma once
class Crops
{
public:
	enum CROP_TYPE
	{
		CARROT,
		SEED,
	}; 

	Crops(int TYPE , int xPos , int zPos , double timeSincePlanted);
	~Crops();
	
	bool GetGrowth;
	void update(double dt);
	int GetState();

	double getTimeSincePlanted();
	void setTimeSincePlanted(double time);

	int GetXTile();
	int GetZTile();

	void SetXTile(int x);
	void SetZTile(int z);

	int GetCropType();

private:
	bool m_bFinishedGrowing;
	double m_dTimeSincePlanted;
	const int TIME_TO_GROW = 5;

	//CROP_TYPE
	int m_iCropType;
	//CROP_STATE
	int m_iCropState;

	int m_iXTile, m_iZTile;
};


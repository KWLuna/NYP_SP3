#pragma once
class Crops
{
public:
	enum CROP_TYPE
	{
		CARROT,
		SEED,
		//WHEAT, technically not a crop.
	}; 
	
	Crops(int TYPE);
	~Crops();
	
	void update(double dt);

private:
	double m_dTimeSincePlanted;
	int m_iCropType;
};


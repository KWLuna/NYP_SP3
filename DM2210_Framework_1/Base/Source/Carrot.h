#pragma once
#include "Crops.h"
class Carrot : public Crops 
{
	enum CROP_TYPE
	{
		CARROT,
		SEED,
		//WHEAT, technically not a crop.
	};

public:
	Carrot();

	void update(double dt);

private:
	double m_dTimeSincePlanted;
	int m_iCropType;
};


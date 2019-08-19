#pragma once
class Crops
{
	enum CROP_TYPE
	{
		CARROT,
		SEED,
		//WHEAT, technically not a crop.
	};

public:
	Crops();
	~Crops();

	void update(double dt);

private:
	double m_dTimeSincePlanted;
	int m_iCropType;
};


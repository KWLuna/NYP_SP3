#pragma once

#include "PlayerInformation.h"
class Furnace
{
	enum SLOT
	{
		START,
		SMELTING,
		FUEL,
		RESULT,
		END
	};

public:
	Furnace(int xPos , int zPos);

	Furnace(int xPos, int zPos , int fuelID , int fuelTotal ,
		    int smeltingID , int smeltingTotal , int resultID , int resultTotal,
		    double smeltingTime , double fuelTime);


	~Furnace();

	// Pass in the player from the scene along with dt.
	void update(double dt , PlayerInformation * player);

	int GetSmeltingID();
	int GetSmeltingTotal();

	int GetFuelID();
	int GetFuelTotal();

	bool GetStatus();
	void SetStatus(bool condition);

	int GetResultID();
	int GetResultTotal();

	int GetXTile();
	int GetZTile();

	double GetSmeltingTime();
	double GetFuelTime();

	int GetSlot();
private:

	void SmeltingProccess(double dt);
	void AccessFurnace(double dt , PlayerInformation * player);
	//Get the resultant from smelting.
	int ReturnResult(int ID);

	int m_iSmeltingID;
	int m_iSmeltingTotal;

	int m_iFuelID;
	int m_iFuelTotal;

	double m_dBounceTime;

	bool m_bAccessFurnace;

	int m_iCurrentSlot;
	
	const int SMELT_TIME = 8;

	double m_dSmelt_Time;
	double m_dFuel_Time;

	int m_iResultID;
	int m_iResultTotal;

	int m_iTileX;
	int m_iTileZ;

	std::vector <int> Smeltables;
	std::vector <int> Burnables;
};


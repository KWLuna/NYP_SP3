#include "Furnace.h"

Furnace::Furnace()
{
	m_iFuelID = 0;
	m_iFuelTotal = 0;

	m_iSmeltingID = 0;
	m_iSmeltingTotal = 0;

	m_iResultID = 0;
	m_iResultTotal = 0;

	m_dBounceTime = 0;

	m_iCurrentSlot = SMELTING;

	m_dSmelt_Time = 0;
	m_dFuel_Time = 0;

	Smeltables.push_back(Item::ITEM_MEAT);

	Burnables.push_back(Item::ITEM_COAL);
	Burnables.push_back(Item::ITEM_WOOD);
}

Furnace::~Furnace()
{
	//Drop items in the furnace on the ground.

	for (int i = 0; i < m_iFuelID; ++i)
		std::cout << "drop m_iFuelID ";

	std::cout << std::endl;

	for (int i = 0; i < m_iSmeltingID; ++i)
		std::cout << "drop m_iSmeltingID ";

	std::cout << std::endl;
}

int Furnace::GetSmeltingID()
{
	return m_iSmeltingID;
}
int Furnace::GetSmeltingTotal()
{
	return m_iSmeltingTotal;
}

int Furnace::GetFuelID()
{
	return m_iFuelID;
}

int Furnace::GetFuelTotal()
{
	return m_iFuelTotal;
}

bool Furnace::GetStatus()
{
	return m_bAccessFurnace;
}


int Furnace::ReturnResult(int ID)
{
	if (ID == Item::ITEM_MEAT)
		return Item::ITEM_COOKED_MEAT;

	else return 0;
}

void Furnace::SetStatus(bool condition)
{
	m_bAccessFurnace = condition;
}

int Furnace::GetSlot()
{
	return m_iCurrentSlot;
}

int Furnace::GetResultID()
{
	return m_iResultID;
}

int Furnace::GetResultTotal()
{
	return m_iResultTotal;
}


void Furnace::SmeltingProccess(double dt)
{
	if (m_dFuel_Time > 0)
		m_dFuel_Time -= 0.01;

	std::cout << m_dSmelt_Time << " " << m_dFuel_Time << std::endl;

	/*	If there is no conflict between what is in the smelting slot
	*	Or if the smelting id is nothing
	*/
	if (m_iSmeltingID == ReturnResult(m_iSmeltingID) || m_iResultID == 0)
	{
		/*	If there is fuel and there is a smeltable item
	*	This should always be first to allow continuous combustion.
	*	Adding time to fuel only when needed.
	*/
		if (m_iFuelTotal > 0 && m_iSmeltingTotal > 0)
		{
			//Check if need to deduct fuel to add smelting time.
			if (m_dFuel_Time <= 0)
			{
				m_iFuelTotal -= 1;
				m_dFuel_Time = SMELT_TIME * 2;
			}
		}

		/*  If there is fuel , check if there is something required to smelt.
		*	If there is no fuel , set the smelting timer to 0.
		*	If there is soemthing to smelt , incement the smelting timer. else , set the smelting time to 0.
		*/
		if (m_dFuel_Time > 0)
		{
			// If there are things to smelt, add the smelting time.
			if (m_iSmeltingTotal > 0)
				m_dSmelt_Time += 1 * dt;
			else
				m_dSmelt_Time = 0;
		}
		else
			m_dSmelt_Time = 0;

		/*	If the smelting timer meets the requirement, add to the resultant.
		*/
		if (m_dSmelt_Time >= SMELT_TIME)
		{
			//Check if there is a need to set the id of the resultant.
			if (m_iResultID == 0)
				m_iResultID = ReturnResult(m_iSmeltingID);
			
			m_dSmelt_Time = 0;
			m_iSmeltingTotal -= 1;
			m_iResultTotal += 1;
		}
	}

	if (m_iSmeltingTotal == 0)
		m_iSmeltingID = 0;
	if (m_iFuelTotal == 0)
		m_iFuelID = 0;
	if (m_iResultTotal == 0)
		m_iResultID = 0;

}

void Furnace::AccessFurnace(double dt , PlayerInformation * player)
{
	player->SetFurnaceStatus(true);
	//Navigation for slots in the furnace.
	if (Application::IsKeyPressed(VK_DOWN) && m_dBounceTime <= 0)
	{
		m_iCurrentSlot -= 1;
		if (m_iCurrentSlot == START)
			m_iCurrentSlot = RESULT;

		m_dBounceTime = 0.2;
	}
	else
	if (Application::IsKeyPressed(VK_UP) && m_dBounceTime <= 0)
	{
		m_iCurrentSlot += 1;
		if (m_iCurrentSlot == END)
			m_iCurrentSlot = SMELTING;

		m_dBounceTime = 0.2;
	}

	//If player press enter , trying to add to current slot
	if (Application::IsKeyPressed(VK_RETURN) && m_dBounceTime <= 0)
	{
		if (m_iCurrentSlot == FUEL)
		{
			//Check if there is already fuel. if dont have,
			if (m_iFuelTotal > 0)
			{
				for (int i = 0; i < Burnables.size(); ++i)
				{
					if (player->getItem(player->getCurrentSlot())->getID() == m_iFuelID)
					{
						//Add 1 item from player slot
						m_iFuelTotal += 1;
						//Deduct 1 item from player slot
						player->getItem(player->getCurrentSlot())->addQuantity(-1);
						break;
					}
				}
			}
			else
			{
				int x = player->getItem(player->getCurrentSlot())->getID();

				for (int i = 0; i < Burnables.size(); ++i)
				{
					if (x == Burnables[i])
					{
						m_iFuelTotal += 1;
						m_iFuelID = player->getItem(player->getCurrentSlot())->getID();
						//Deduct 1 item from player slot
						player->getItem(player->getCurrentSlot())->addQuantity(-1);
						break;
					}
				}
			}
		}
		else if (m_iCurrentSlot == SMELTING)
		{
			//Check if there is already fuel. if dont have,
			if (m_iSmeltingTotal > 0)
			{
				for (int i = 0; i < Smeltables.size(); ++i)
				{
					if (player->getItem(player->getCurrentSlot())->getID() == m_iSmeltingID)
					{
						//Add 1 item from player slot
						m_iSmeltingTotal += 1;
						//Deduct 1 item from player slot
						player->getItem(player->getCurrentSlot())->addQuantity(-1);
						break;
					}
				}
			}
			else
			{
				int x = player->getItem(player->getCurrentSlot())->getID();

				for (int i = 0; i < Smeltables.size(); ++i)
				{
					if (x == Smeltables[i])
					{
						m_iSmeltingTotal += 1;
						m_iSmeltingID = player->getItem(player->getCurrentSlot())->getID();
						//Deduct 1 item from player slot
						player->getItem(player->getCurrentSlot())->addQuantity(-1);
						break;
					}
				}
			}
		}
		else
		{
			if (m_iResultTotal > 0)
			{
				player->addItem(new Item(m_iResultID, m_iResultTotal));
				m_iResultTotal = 0;
				m_iResultID = 0;
			}
		}

	
		m_dBounceTime = 0.2;
	}
}
void Furnace::update(double dt , PlayerInformation * player)
{
	m_dBounceTime -= 1 * dt;
	
	SmeltingProccess(dt);

	if (m_bAccessFurnace == true)
		AccessFurnace(dt, player);
}


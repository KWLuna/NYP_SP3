#include "PlayerInformation.h"
#include <fstream>
#include <string>
#include <iostream>
PlayerInformation::PlayerInformation()
{
	m_dHP = 100;
	m_dMaxHP = 100;
	m_dHunger = 100;
	m_dThirst = 100.f;
	m_fSpeed = 100.f;
	m_dBounceTime = 0;
	ThirstyOrHungry = 5;
	m_bCrafting = false;

	m_iCurrentStance = STAND;

	m_iInventorySlot = 0;
	m_dConstrainY = 40;
	m_iCraftingSlotOne = -1;
	m_iCraftingSlotTwo = -1;
	m_iSwitchInventorySlot = -1;

	m_bFurnaceStatus = false;
	m_bJump = false;
	m_bFall = true;
	m_bSwitchStance = false;
	//Init inventory as empty , 9 slots in total.
	for (int i = 0; i < 15; ++i)
	{
		ItemList.push_back(new Item(0, 0));
	}

	addItem(new Item(Item::ITEM_WOODEN_SWORD, 1));
	if (getItem(getCurrentSlot())->getID() == Item::ITEM_WOODEN_SWORD)
	{
		curtool = new Sword();
		curtool->Init();
	}
	action = STANDING;

	LoadData();
}

PlayerInformation::~PlayerInformation()
{
	std::cout << "Calling destructor" << std::endl;
	SaveData();
}

void PlayerInformation::SaveData()
{
	std::ofstream saveFile("PlayerSaveFile.txt");
	if (saveFile.is_open())
	{
		//15
		for (int i = 0; i < ItemList.size(); ++i)
			saveFile << ItemList[i]->getID() << std::endl;
		//15
		for (int i = 0; i < ItemList.size(); ++i)
			saveFile << ItemList[i]->getQuantity() << std::endl;

		//4
		saveFile << m_dHP << std::endl;
		saveFile << m_dHunger << std::endl;
		saveFile << m_iCurrentStance << std::endl;
		saveFile << m_dConstrainY << std::endl;

		saveFile.close();
	}
	else
	{
		std::cout << " cant save !" << std::endl;
	}
}

void PlayerInformation::LoadData()
{
	std::ifstream saveFile("PlayerSaveFile.txt"); //Open text file to read
	std::string row;
	if (saveFile.is_open())
	{
		for (int i = 0; i < ItemList.size(); ++i)
		{
			saveFile >> row;
			ItemList[i]->setID(std::stoi(row));
		}

		for (int i = 0; i < ItemList.size(); ++i)
		{
			saveFile >> row;
			ItemList[i]->setQuantity(std::stoi(row));
		}

		saveFile >> m_dHP;
		saveFile >> m_dHunger;
		saveFile >> m_iCurrentStance;
		saveFile >> m_dConstrainY;
		
		saveFile.close();
	}
	else
		std::cout << "Impossible to open save file!" << std::endl;
}

int PlayerInformation::getCurrentSlot()
{
	return m_iInventorySlot;
}
void PlayerInformation::AttachCamera(Camera3* _cameraPtr)
{
	attachedCamera = _cameraPtr;
}

void PlayerInformation::Constrain()
{
	//Anchor player to the ground
	Vector3 viewVector = attachedCamera->target - attachedCamera->position;

	attachedCamera->position.y = m_dConstrainY;

	attachedCamera->target = attachedCamera->position + viewVector;
	//
	/*if ()
	{

	}*/
}

Item * PlayerInformation::getItem(int id)
{
	return ItemList[id];
}

int PlayerInformation::getCraftingSlotOne()
{
	return m_iCraftingSlotOne;
}

int PlayerInformation::getCraftingSlotTwo()
{
	return m_iCraftingSlotTwo;
}

bool PlayerInformation::addItem(Item * object)
{
	bool condition = false;

	//Check for existing item
	for (int i = 0; i < ItemList.size(); ++i)
	{
		//Existing item , add item to that stack of item.
		if (ItemList[i]->getID() == object->getID())
		{
			ItemList[i]->addQuantity(object->getQuantity());
			return true;
		}
	}

	//Item dosent exist in inventory
	for (int i = 0; i < ItemList.size(); ++i)
	{
		//Empty slot , assign accordingly
		if (ItemList[i]->getID() == 0)
		{
			ItemList[i]->setID(object->getID());
			ItemList[i]->setQuantity(object->getQuantity());
			return true;
		} 
	}

	return false; //tell prog that adding item was unsuccesful
}

void PlayerInformation::SetFurnaceStatus(bool condition)
{
	m_bFurnaceStatus = condition;
}

int PlayerInformation::getTotalItems()
{
	return ItemList.size();
}

bool PlayerInformation::getIsCrafting()
{
	return m_bCrafting;
}

Item * PlayerInformation::craft(int firstItem, int secondItem)
{
	//Convert into ids.
	firstItem = ItemList[firstItem]->getID();
	secondItem = ItemList[secondItem]->getID();

	if (firstItem == Item::ITEM_STICK)
	{
		if (secondItem == Item::ITEM_COAL)
			return new Item(Item::ITEM_TORCH, 4);
	}

	if (firstItem == Item::ITEM_COAL)
	{
		if (secondItem == Item::ITEM_STICK)
			return new Item(Item::ITEM_TORCH, 4);
	}

	if (firstItem == Item::ITEM_WOODEN_SWORD)
	{
		if (secondItem == Item::ITEM_STONE)
			return new Item(Item::ITEM_STONE_SWORD, 1);
	}

	if (firstItem == Item::ITEM_STONE_SWORD)
	{
		if (secondItem == Item::ITEM_GOLD_NUGGET)
			return new Item(Item::ITEM_GOLD_SWORD, 1);
	}

	if (firstItem == Item::ITEM_WOODEN_PICKAXE)
	{
		if (secondItem == Item::ITEM_STONE)
			return new Item(Item::ITEM_STONE_PICKAXE, 1);
	}

	if (firstItem == Item::ITEM_STONE_PICKAXE)
	{
		if (secondItem == Item::ITEM_GOLD_NUGGET)
			return new Item(Item::ITEM_GOLD_PICKAXE, 1);
	}

	if (firstItem == Item::ITEM_WOODEN_AXE)
	{
		if (secondItem == Item::ITEM_STONE)
			return new Item(Item::ITEM_STONE_AXE, 1);
	}

	if (firstItem == Item::ITEM_STONE_AXE)
	{
		if (secondItem == Item::ITEM_GOLD_NUGGET)
			return new Item(Item::ITEM_GOLD_AXE, 1);
	}

	if (firstItem == Item::Item::ITEM_WHEAT)
	{
		if (secondItem == Item::Item::ITEM_WHEAT)
			return new Item(Item::ITEM_BREAD, 1);
	}

	if (firstItem == Item::Item::ITEM_STONE)
	{
		if (secondItem == Item::Item::ITEM_STONE)
			return new Item(Item::ITEM_FURNACE, 1);
	}

	if (firstItem == Item::Item::ITEM_WOOD)
	{
		if (secondItem == Item::Item::ITEM_WOOD)
			return new Item(Item::ITEM_STICK, 2);
	}

	return new Item(-1, 0);
}

void PlayerInformation::update(double dt, std::vector<CAnimal*> animalist, char tilearray[])
{
	// Update bounce time.
	m_dBounceTime -= 1 * dt;

	for (int i = 0; i < ItemList.size(); ++i)
	{
		if (ItemList[i]->getQuantity() <= 0)
		{
			ItemList[i]->setID(0);
			ItemList[i]->setQuantity(0);
		}
	}

	if (Application::IsKeyPressed('C') && m_bSwitchStance == false && m_dBounceTime <= 0)
	{
		m_iCurrentStance += 1;

		if (m_iCurrentStance > PRONE)
			m_iCurrentStance = 0;

		m_dBounceTime = 0.5;
		m_bSwitchStance = true;
	}

	if (m_bSwitchStance == true)
	{
		switch (m_iCurrentStance)
		{
		case STAND:
			if (m_dConstrainY < 40)
				m_dConstrainY += 50 * dt;
			else
				m_bSwitchStance = false;
			break;
		case CROUCH:
			if (m_dConstrainY > 20)
				m_dConstrainY -= 50 * dt;
			else
				m_bSwitchStance = false;
			break;
		case PRONE:
			if (m_dConstrainY > 5)
				m_dConstrainY -= 50 * dt;
			else
				m_bSwitchStance = false;
			break;
		}
	}

	if (m_bFurnaceStatus == false)
	{
		if (Application::IsKeyPressed('E') && m_dBounceTime <= 0)
		{
			if (m_bCrafting == true)
			{
				m_iCraftingSlotOne = -1;
				m_iCraftingSlotTwo = -1;
				m_bCrafting = false;
			}
			else
				m_bCrafting = true;

			m_dBounceTime = 0.2;
		}
	}

	if (Application::IsKeyPressed(VK_SPACE) && m_bJump == false && m_bFall == false)
		m_bJump = true;

	if (m_bJump == true && m_bSwitchStance == false)
	{
		if (m_iCurrentStance == STAND)
		{
			if (m_dConstrainY < 90)
				m_dConstrainY += 150 * dt;
			else
			{
				m_bFall = true;
				m_bJump = false;
			}
		}
		else if (m_iCurrentStance == CROUCH)
		{
			if (m_dConstrainY < 100)
				m_dConstrainY += 200 * dt;
			else
			{
				m_bFall = true;
				m_bJump = false;
			}
		}
		else
			m_bJump = false;
	}

	if (m_bFall == true)
	{
		if (m_iCurrentStance == STAND)
		{
			if (m_dConstrainY > 40)
				m_dConstrainY -= 150 * dt - 9.8 * dt;
			else
				m_bFall = false;
		}
		else if (m_iCurrentStance == CROUCH)
		{
			if (m_dConstrainY > 20)
				m_dConstrainY -= 200 * dt - 9.8 * dt;
			else
				m_bFall = false;
		}
	}

	if (Application::IsKeyPressed(VK_LEFT) && m_dBounceTime <= 0)
	{
		m_iInventorySlot -= 1;
		m_dBounceTime = 0.2;

		if (m_iInventorySlot < 0)
			m_iInventorySlot = ItemList.size() - 1;
	}

	if (Application::IsKeyPressed(VK_RIGHT) && m_dBounceTime <= 0)
	{
		m_iInventorySlot += 1;
		m_dBounceTime = 0.2;

		if (m_iInventorySlot > ItemList.size() - 1)
			m_iInventorySlot = 0;
	}
	if (action != PlayerInformation::STANDING)
	{
		walkingtime += dt;
	}
	if (m_dHunger < 30 || m_dThirst < 30)
	{
		m_fSpeed = 80;
		if (m_dHunger < 0)
		{
			m_dHunger = 0;
		}
		if (m_dThirst < 0)
		{
			m_dThirst = 0;
		}
		if (m_dHunger == 0)
		{
			ThirstyOrHungry -= dt;
			if (ThirstyOrHungry < 0)
			{
				ThirstyOrHungry = 5;
				m_dHP -= 2;
			}
		}
		if (m_dThirst == 0)
		{
			ThirstyOrHungry -= dt;
			if (ThirstyOrHungry < 0)
			{
				ThirstyOrHungry = 5;
				m_dHP -= 1;
			}
		}
	}
	else
	{
		m_fSpeed = 100;
		if (m_dHunger > 80 && m_dThirst > 80)
		{
			if (m_dHP < m_dMaxHP)
			{
				m_dHP += 0.5f;
				m_dHunger -= 0.5f;
				m_dThirst -= 0.5f;
			}
		}
	}

	switch (action)
	{
	case PlayerInformation::STANDING:
		break;
	case PlayerInformation::SPRINTING:
		if (walkingtime > 5)
		{
			m_dHunger -= 0.1f;
			m_dThirst -= 0.1f;
			walkingtime = 0;
		}
		break;
	case PlayerInformation::WALKING:
		if (walkingtime > 5)
		{
			m_dHunger -= 0.05f;
			m_dThirst -= 0.05f;
			walkingtime = 0;
		}
		break;
	case PlayerInformation::EATING:
		if (getItem(getCurrentSlot())->getID() == Item::ITEM_MEAT)
		{
			m_dHunger += 0.2f;
			m_dThirst -= 0.2f;
		}
		else if (getItem(getCurrentSlot())->getID() == Item::ITEM_COOKED_MEAT)
			m_dHunger += 0.5f;

		walkingtime = 0;
		m_fSpeed *= 0.4f;
		break;
	default:
		break;
	}

	if (m_bCrafting == true)
	{
		if (Application::IsKeyPressed(VK_RETURN) && m_dBounceTime <= 0)
		{
			m_dBounceTime = 0.2;

			if (m_iCraftingSlotOne != -1 && m_iCraftingSlotTwo != -1)
			{
				Item * result = craft(m_iCraftingSlotOne, m_iCraftingSlotTwo);

				if (result->getID() != -1)
				{
					if (addItem(result) == false)
					{
						// drop item , waiting for raycast
					}
					else
					{
						ItemList[m_iCraftingSlotTwo]->addQuantity(-1);
						ItemList[m_iCraftingSlotOne]->addQuantity(-1);
					}
				}

				m_iCraftingSlotOne = -1;
				m_iCraftingSlotTwo = -1;
			}
			else if (m_iCraftingSlotOne == -1)
			{
				m_iCraftingSlotOne = m_iInventorySlot;

				if (ItemList[m_iCraftingSlotOne]->getID() == 0)
					m_iCraftingSlotOne = -1;
			}
			else if (m_iCraftingSlotTwo == -1)
			{
				m_iCraftingSlotTwo = m_iInventorySlot;

				if (ItemList[m_iCraftingSlotTwo]->getID() == 0)
					m_iCraftingSlotTwo = -1;
			}
		}

		if (Application::IsKeyPressed(VK_BACK) && m_dBounceTime <= 0)
		{
			m_iCraftingSlotOne = -1;
			m_iCraftingSlotTwo = -1;
		}
	}

	if (m_bCrafting == false && m_bFurnaceStatus == false)
	{
		Vector3 viewVector = attachedCamera->target - attachedCamera->position;
		Vector3 rightUV;

		if (Application::IsKeyPressed(VK_RETURN) && m_dBounceTime <= 0)
		{
			if (m_iSwitchInventorySlot == -1)
				m_iSwitchInventorySlot = m_iInventorySlot;
			else
			{
				Item * temp = ItemList[m_iInventorySlot];

				ItemList[m_iInventorySlot] = ItemList[m_iSwitchInventorySlot];
				ItemList[m_iSwitchInventorySlot] = temp;

				m_iSwitchInventorySlot = -1;
			}

			m_dBounceTime = 0.2;
		}

		//Movement
		if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
		{
			Vector3 viewVector = attachedCamera->target - attachedCamera->position;
			Vector3 rightUV;
			if (Application::IsKeyPressed('W'))
			{
				if ((Application::IsKeyPressed('W')) && (Application::IsKeyPressed(VK_SHIFT) && m_iCurrentStance == STAND))
				{
					attachedCamera->position += viewVector.Normalized() * m_fSpeed * 3.0f * (float)dt;
					action = SPRINTING;
				}
				else
				{
					action = WALKING;
					attachedCamera->position = attachedCamera->position + viewVector.Normalized() * m_fSpeed * (float)dt;
				}
			}
			else if (Application::IsKeyPressed('S'))
			{
				attachedCamera->position -= viewVector.Normalized() * m_fSpeed * (float)dt;
				action = WALKING;
			}
			if (Application::IsKeyPressed('A'))
			{
				rightUV = (viewVector.Normalized()).Cross(attachedCamera->up);
				rightUV.y = 0;
				rightUV.Normalize();
				attachedCamera->position -= rightUV * m_fSpeed * (float)dt;
				action = WALKING;
			}
			else if (Application::IsKeyPressed('D'))
			{
				rightUV = (viewVector.Normalized()).Cross(attachedCamera->up);
				rightUV.y = 0;
				rightUV.Normalize();
				attachedCamera->position += rightUV * m_fSpeed * (float)dt;
				action = WALKING;
			}

			// Constrain the position
			// Update the target
			attachedCamera->target = attachedCamera->position + viewVector;
		}
		else
		{
			action = STANDING;
		}

		Constrain();

		Vector3 dir = attachedCamera->target - attachedCamera->position;
		curtool->UpdateAnimal(dt, dir, attachedCamera->position, animalist);
		curtool->SetStandinOn(false);
		curtool->SetTileType('N');
		curtool->UpdateTile(dt, dir, attachedCamera->position, tilearray);

		static bool bLButtonState = false;

		if (!bLButtonState &&Application::IsMousePressed(0))
		{
			bLButtonState = true;

			if (playerphysics.RayTraceDist(viewVector, attachedCamera->position, Vector3(12000, -500, 12000), Vector3(13000, 500, 13000)))
			{
				std::cout << "left " << playerphysics.GetDist();
			}
			else
				std::cout << "lnotcollide";
			if (getItem(getCurrentSlot())->getID() == Item::ITEM_WOODEN_SWORD)
			{
				curtool->SetCurSwing();
			}
		}
		else if (bLButtonState && !Application::IsMousePressed(0))
		{
			bLButtonState = false;

		}

		static bool bRButtonState = false;
		if (!bRButtonState && Application::IsMousePressed(1))
		{
			bRButtonState = true;

			if (playerphysics.RayTraceDist(viewVector, attachedCamera->position, Vector3(12000, -500, 12000), Vector3(13000, 500, 13000)))
			{
				std::cout << "right " << playerphysics.GetDist();
			}
			else
				std::cout << "rnotcollide";
			if (getItem(getCurrentSlot())->getID() == Item::ITEM_FURNACE)
			{
				curtool->SetCurSwing();
				PlaceBlock();
			}
		}
		else if (bRButtonState && !Application::IsMousePressed(1))
		{
			bRButtonState = false;

			if (m_dHunger < 100)
			{
				if (getItem(getCurrentSlot())->getID() == Item::ITEM_MEAT || getItem(getCurrentSlot())->getID() == Item::ITEM_COOKED_MEAT || getItem(getCurrentSlot())->getID() == Item::ITEM_CARROT)
				{
					action = PlayerInformation::EATING;
					getItem(getCurrentSlot())->addQuantity(-1);
				}
			}
		}
	}
}

void PlayerInformation::PlaceBlock()
{
	placedown = false;
	if (!curtool->GetStandinOn())
	{
		if (curtool->GetTileType() == 'G')
		{
			placedown = true;
		}
	}
}

bool PlayerInformation::GetPlaceDown()
{
	return placedown;
}

double PlayerInformation::getHunger()
{
	return m_dHunger;
}
double PlayerInformation::getHP()
{
	return m_dHP;
}
void PlayerInformation::SetHP(float m_dHP)
{
	this->m_dHP = m_dHP;
}
double PlayerInformation::getMaxHP()
{
	return m_dMaxHP;
}
void PlayerInformation::SetMaxHP(float m_dMaxHP)
{
	this->m_dMaxHP = m_dMaxHP;
}
Weapons * PlayerInformation::getcurtool()
{
	return curtool;
}
void PlayerInformation::SetThirst(double m_dThirst)
{
	this->m_dThirst = m_dThirst;
}
double PlayerInformation::getThirst()
{
	return m_dThirst;
}
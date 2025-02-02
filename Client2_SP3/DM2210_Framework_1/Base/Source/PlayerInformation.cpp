#include "PlayerInformation.h"
#include <fstream>
#include <string>
#include <iostream>
PlayerInformation::PlayerInformation()
{
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("Walking_Fast", "Image//Walking_Fast.mp3");
	CSoundEngine::GetInstance()->AddSound("Walking_Slow", "Image//Walking_Slow.mp3");
	CSoundEngine::GetInstance()->AddSound("Swing_Action", "Image///Swing.mp3");

	CSoundEngine::GetInstance()->AddSound("Open_Crafting", "Image///Open_Crafting.mp3");
	CSoundEngine::GetInstance()->AddSound("Close_Crafting", "Image///Close_Crafting.mp3");

	CSoundEngine::GetInstance()->AddSound("Pick_up", "Image///Pop.mp3");

	CSoundEngine::GetInstance()->AddSound("Drop_Item", "Image///Drop_Item.mp3");

	m_dHP = 100;
	m_dMaxHP = 100;
	m_dHunger = 100;
	m_dThirst = 100.f;
	m_fSpeed = 100.f;
	m_dBounceTime = 0;
	ThirstyOrHungry = 5;
	m_bCrafting = false;
	m_iCurrentStance = STAND;
	m_dDropTime = 0;
	m_iInventorySlot = 0;
	m_dConstrainY = 40;
	m_iCraftingSlotOne = -1;
	m_iCraftingSlotTwo = -1;
	m_iSwitchInventorySlot = -1;
	m_fPlayersDamage = 2.f;
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
		curtool = new WoodenSword();
		curtool->Init();
	}

	addItem(new Item(Item::ITEM_WOODEN_AXE, 1));
	addItem(new Item(Item::ITEM_WOODEN_HOE, 1));
	addItem(new Item(Item::ITEM_WOODEN_PICKAXE, 1));

	action = STANDING;
	placedown = false;
	breakblock = false;
}

PlayerInformation::~PlayerInformation()
{
	std::cout << "Calling destructor" << std::endl;
	SaveData();

	for (int i = 0; i < ItemList.size(); ++i)
	{
		if (ItemList[i] != nullptr)
			delete ItemList[i];
	}

	for (int i = 0; i < DroppedItemList.size(); ++i)
	{
		if (DroppedItemList[i] != nullptr)
			delete DroppedItemList[i];
	}

	delete curtool;
}

void PlayerInformation::SaveData()
{
	std::ofstream saveFile("PlayerSaveFile.txt");
	if (saveFile.is_open())
	{
		//15
		for (unsigned int i = 0; i < ItemList.size(); ++i)
			saveFile << ItemList[i]->getID() << std::endl;
		//15
		for (unsigned int i = 0; i < ItemList.size(); ++i)
			saveFile << ItemList[i]->getQuantity() << std::endl;

		//4
		saveFile << m_dHP << std::endl;
		saveFile << m_dHunger << std::endl;
		saveFile << m_iCurrentStance << std::endl;
		saveFile << m_dConstrainY << std::endl;
		saveFile << m_iInventorySlot << std::endl;
		
		saveFile << attachedCamera->position.x << std::endl;
		saveFile << attachedCamera->position.z << std::endl;
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
		for (unsigned int i = 0; i < ItemList.size(); ++i)
		{
			saveFile >> row;
			ItemList[i]->setID(std::stoi(row));
		}

		for (unsigned int i = 0; i < ItemList.size(); ++i)
		{
			saveFile >> row;
			ItemList[i]->setQuantity(std::stoi(row));
		}

		saveFile >> m_dHP;
		saveFile >> m_dHunger;
		saveFile >> m_iCurrentStance;
		saveFile >> m_dConstrainY;
		saveFile >> m_iInventorySlot;
		saveFile >> attachedCamera->position.x;
		saveFile >> attachedCamera->position.z;
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

void PlayerInformation::Constrain(char PlayerTile[], Vector3 prevpos)
{
	Vector3 viewVector = attachedCamera->target - attachedCamera->position;
	if (attachedCamera->position.x < 0)
	{
		attachedCamera->position.x = 0;
		attachedCamera->target = attachedCamera->position + viewVector;
	}
	else if (attachedCamera->position.x > 25000)
	{
		attachedCamera->position.x = 25000;
		attachedCamera->target = attachedCamera->position + viewVector;
	}

	if (attachedCamera->position.z < 0)
	{
		attachedCamera->position.z = 0;
		attachedCamera->target = attachedCamera->position + viewVector;
	}
	else if (attachedCamera->position.z > 25000)
	{
		attachedCamera->position.z = 25000;
		attachedCamera->target = attachedCamera->position + viewVector;
	}

	viewVector = attachedCamera->target - attachedCamera->position;
	attachedCamera->position.y = static_cast<float>(m_dConstrainY);
	attachedCamera->target = attachedCamera->position + viewVector;
	
	int checksides[4];
	//Anchor player to the ground
	if (PlayerTile[2] == 'F' || PlayerTile[2] == 'T' || PlayerTile[2] == 'O' || PlayerTile[2] == 'W' || PlayerTile[2] == 'C' || PlayerTile[2] == 'B' || PlayerTile[2] == 'b')
	{
		checksides[0] = ((int)((prevpos.x + 50) / 100) * 100 - 50);
	}
	else
	{
		checksides[0] = -100;
	}
	if (PlayerTile[7] == 'F' || PlayerTile[7] == 'T' || PlayerTile[7] == 'O' || PlayerTile[7] == 'W' || PlayerTile[7] == 'C' || PlayerTile[7] == 'B' || PlayerTile[7] == 'b')
	{
		checksides[1] = ((int)((prevpos.x + 150) / 100) * 100 - 50);
	}
	else
	{
		checksides[1] = -100;
	}
	if (PlayerTile[4] == 'F' || PlayerTile[4] == 'T' || PlayerTile[4] == 'O' || PlayerTile[4] == 'W' || PlayerTile[4] == 'C' || PlayerTile[4] == 'B' || PlayerTile[4] == 'b')
	{
		checksides[2] = ((int)((prevpos.z + 50) / 100) * 100 - 50);
	}
	else
	{
		checksides[2] = -100;
	}
	if (PlayerTile[5] == 'F' || PlayerTile[5] == 'T' || PlayerTile[5] == 'O' || PlayerTile[5] == 'W' || PlayerTile[5] == 'C' || PlayerTile[5] == 'B' || PlayerTile[5] == 'b')
	{
		checksides[3] = ((int)((prevpos.z + 150) / 100) * 100 - 50);
	}
	else
	{
		checksides[3] = -100;
	}
	if (checksides[0] != -100)
	{
		if (attachedCamera->position.x < checksides[0])
		{
			attachedCamera->position.x = static_cast<float> (checksides[0]);
		}
	}
	if (checksides[1] != -100)
	{
		if (attachedCamera->position.x > checksides[1])
		{
			attachedCamera->position.x = static_cast<float>(checksides[1]);
		}
	}
	if (checksides[2] != -100)
	{
		if (attachedCamera->position.z < checksides[2])
		{
			attachedCamera->position.z = static_cast<float>(checksides[2]);
		}
	}
	if (checksides[3] != -100)
	{
		if (attachedCamera->position.z > checksides[3])
		{
			attachedCamera->position.z = static_cast<float>(checksides[3]);
		}
	}
	attachedCamera->target = attachedCamera->position + viewVector;
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
	for (unsigned int i = 0; i < ItemList.size(); ++i)
	{
		//Existing item , add item to that stack of item.
		if (ItemList[i]->getID() == object->getID())
		{
			ItemList[i]->addQuantity(object->getQuantity());
			return true;
		}
	}

	//Item dosent exist in inventory
	for (unsigned int i = 0; i < ItemList.size(); ++i)
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

int PlayerInformation::getTotalDropItems()
{
	return DroppedItemList.size();
}

bool PlayerInformation::getIsCrafting()
{
	return m_bCrafting;
}

DroppedItem * PlayerInformation::getDroppedItem(int ID)
{
	return DroppedItemList[ID];
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

	if (firstItem == Item::Item::ITEM_ICE_CUBE)
	{
		if (secondItem == Item::Item::ITEM_TORCH)
			return new Item(Item::ITEM_WATER_BOTTLE, 1);
	}
	else if(firstItem == Item::Item::ITEM_TORCH)
	{
		if (secondItem == Item::Item::ITEM_ICE_CUBE)
			return new Item(Item::ITEM_WATER_BOTTLE, 1);
	}
	

	return new Item(-1, 0);
}

bool PlayerInformation::GetFurnaceStatus()
{
	return m_bFurnaceStatus;
}

void PlayerInformation::update(double dt, std::vector<CAnimal*> animalist, std::vector<CEnemy*> enemylist, char tilearray[], std::vector<char> FurnaceX, std::vector<char> FurnaceZ)
{
	// Update bounce time.
	m_dBounceTime -= 1 * dt;
	m_dDropTime -= 1 * dt;

	if (m_dHP <= 0)
	{
		if (Application::IsKeyPressed(VK_RETURN))
		{
			m_dHP = 100;
			attachedCamera->position.x = 12500;
			attachedCamera->position.z = 12500;
		}
	}
	else
	{

		for (unsigned int i = 0; i < ItemList.size(); ++i)
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

					CSoundEngine::GetInstance()->PlayASound2D("Close_Crafting");
				}
				else
				{
					CSoundEngine::GetInstance()->PlayASound2D("Open_Crafting");
					m_bCrafting = true;
				}

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
					m_dConstrainY -= 200 * dt - 9.8 * dt;
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
			if (getItem(getCurrentSlot())->getID() == Item::ITEM_WOODEN_SWORD)
			{
				delete curtool;
				curtool = new WoodenSword;
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_STONE_SWORD)
			{
				delete curtool;
				curtool = new StoneSword();
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_GOLD_SWORD)
			{
				delete curtool;
				curtool = new GoldSword;
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_WOODEN_PICKAXE)
			{
				delete curtool;
				curtool = new WoodenPickaxe;
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_STONE_PICKAXE)
			{
				delete curtool;
				curtool = new StonePickaxe();
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_GOLD_PICKAXE)
			{
				delete curtool;
				curtool = new GoldPickaxe;
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_WOODEN_AXE)
			{
				delete curtool;
				curtool = new WoodenAxe;
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_STONE_AXE)
			{
				delete curtool;
				curtool = new StoneAxe();
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_GOLD_AXE)
			{
				delete curtool;
				curtool = new GoldAxe;
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_WOODEN_HOE)
			{
				delete curtool;
				curtool = new WoodenHoe;
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_STONE_HOE)
			{
				delete curtool;
				curtool = new StoneHoe();
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_GOLD_HOE)
			{
				delete curtool;
				curtool = new GoldHoe;
				curtool->Init();
			}
		}

		if (Application::IsKeyPressed(VK_RIGHT) && m_dBounceTime <= 0)
		{
			m_iInventorySlot += 1;
			m_dBounceTime = 0.2;

			if (m_iInventorySlot > static_cast<int>(ItemList.size() - 1))
				m_iInventorySlot = 0;
			if (getItem(getCurrentSlot())->getID() == Item::ITEM_WOODEN_SWORD)
			{
				delete curtool;
				curtool = new WoodenSword;
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_STONE_SWORD)
			{
				delete curtool;
				curtool = new StoneSword();
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_GOLD_SWORD)
			{
				delete curtool;
				curtool = new GoldSword;
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_WOODEN_PICKAXE)
			{
				delete curtool;
				curtool = new WoodenPickaxe;
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_STONE_PICKAXE)
			{
				delete curtool;
				curtool = new StonePickaxe();
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_GOLD_PICKAXE)
			{
				delete curtool;
				curtool = new GoldPickaxe;
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_WOODEN_AXE)
			{
				delete curtool;
				curtool = new WoodenAxe;
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_STONE_AXE)
			{
				delete curtool;
				curtool = new StoneAxe();
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_GOLD_AXE)
			{
				delete curtool;
				curtool = new GoldAxe;
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_WOODEN_HOE)
			{
				delete curtool;
				curtool = new WoodenHoe;
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_STONE_HOE)
			{
				delete curtool;
				curtool = new StoneHoe();
				curtool->Init();
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_GOLD_HOE)
			{
				delete curtool;
				curtool = new GoldHoe;
				curtool->Init();
			}
		}
		if (action != PlayerInformation::STANDING)
		{
			walkingtime += static_cast<float>(dt);
		}
		if (m_dHunger < 30 || m_dThirst < 30)
		{
			m_fSpeed = 80;
			if (m_dHunger < 0.001)
			{
				m_dHunger = 0;
				ThirstyOrHungry -= static_cast<float>(dt);
				if (ThirstyOrHungry < 0)
				{
					ThirstyOrHungry = 5;
					m_dHP -= 2;
				}
			}
			if (m_dThirst < 0.001)
			{
				m_dThirst = 0;
				ThirstyOrHungry -= static_cast<float>(dt);
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
		if (m_dHP < 0.001)
		{
			m_dHP = 0;
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
			switch (getItem(getCurrentSlot())->getID())
			{
			case Item::ITEM_MEAT:
				m_dHunger += 1.f;
				m_dThirst -= 0.5f;
				break;
			case Item::ITEM_COOKED_MEAT:
				m_dHunger += 3.f;
				break;
			case Item::ITEM_CARROT:
				m_dHunger += 0.5f;
				m_dThirst += 0.5f;
				break;
			case Item::ITEM_BREAD:
				m_dHunger += 2.f;
				m_dThirst += 0.2f;
				break;
			case Item::ITEM_WATER_BOTTLE:
				m_dThirst += 2.0f;
				break;
			case Item::ITEM_BERRY:
				m_dHunger += 0.5f;
				m_dThirst += 1.0f;
				break;
			default:
				break;
			}
			walkingtime = 0;
			m_fSpeed *= 0.4f;
			break;
		default:
			break;
		}

		//If crafting is true
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
							//if inventory full , drop item from crafting section
							DroppedItemList.push_back(new DroppedItem(result->getID(), result->getQuantity(),
														attachedCamera->position.x, attachedCamera->position.z));
						
							m_dDropTime = 1;
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

		if (m_bFurnaceStatus == true)
		{
			curtool->ResetFurnaceID();
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

			// Prevent player from picking up the item if player just chucked the item.
			
			

			if (Application::IsKeyPressed('Q') && m_dBounceTime <= 0)
			{
				if (ItemList[m_iInventorySlot]->getQuantity() > 0)
				{
					CSoundEngine::GetInstance()->PlayASound2D("Drop_Item");

					DroppedItemList.push_back(new DroppedItem(ItemList[m_iInventorySlot]->getID(), ItemList[m_iInventorySlot]->getQuantity(),
						attachedCamera->position.x, attachedCamera->position.z));
					ItemList[m_iInventorySlot]->addQuantity(-1);
				}
				m_dBounceTime = 0.1;
				m_dDropTime = 1;
			}

			if (m_dDropTime <= 0 && DroppedItemList.size() > 0)
			{
				for (unsigned int i = 0; i < DroppedItemList.size(); ++i)
				{
					float xPosSum = (attachedCamera->position.x - DroppedItemList[i]->getXPos());
					float zPosSum = (attachedCamera->position.z - DroppedItemList[i]->getZPos());

					if (sqrt((xPosSum * xPosSum) + (zPosSum * zPosSum)) <= 25)
					{
						CSoundEngine::GetInstance()->PlayASound2D("Pick_up");
						addItem(new Item(DroppedItemList[i]->getID(), 1));
						DroppedItemList.erase(DroppedItemList.begin() + i);
					}
				}
			}

			//Movement
			Vector3 prevpos = attachedCamera->position;
			if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
			{
				Vector3 viewVector = attachedCamera->target - attachedCamera->position;
				Vector3 rightUV;

				if (Application::IsKeyPressed('W'))
				{
					movedir = 'W';
					if ((Application::IsKeyPressed('W')) && (Application::IsKeyPressed(VK_SHIFT) && m_iCurrentStance == STAND))
					{
						CSoundEngine::GetInstance()->PlayASound2D("Walking_Fast");

						attachedCamera->position += viewVector.Normalized() * m_fSpeed * 3.0f * (float)dt;
						action = SPRINTING;

						int randVal = Math::RandIntMinMax(0, 200);
						if (randVal < 1)
							addItem(new Item(Item::ITEM_SEED, 1));
						else if (randVal < 2)
							addItem(new Item(Item::ITEM_CARROT, 1));
					}
					else
					{
						CSoundEngine::GetInstance()->PlayASound2D("Walking_Slow");

						int randVal = Math::RandIntMinMax(0, 200);
						if (randVal < 1)
							addItem(new Item(Item::ITEM_SEED, 1));
						else if (randVal < 2)
							addItem(new Item(Item::ITEM_CARROT, 1));

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

			Constrain(tilearray, prevpos);
			CSoundEngine::GetInstance()->setListenerPosition(attachedCamera->position, Vector3(-1, 0, 0), attachedCamera->up);

			

			UpdatePlayersStrength();

			Vector3 dir = attachedCamera->target - attachedCamera->position;
			curtool->SetFeedAnimalPig(false);
			curtool->SetFeedAnimalCow(false);
			curtool->SetFeedAnimalChicken(false);
			curtool->SetAnimalID(-1);
			if (getItem(getCurrentSlot())->getID() == Item::ITEM_CARROT)
			{
				curtool->SetFoodHeld(1);
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_WHEAT)
			{
				curtool->SetFoodHeld(2);
			}
			else if (getItem(getCurrentSlot())->getID() == Item::ITEM_SEED)
			{
				curtool->SetFoodHeld(3);
			}
			else
			{
				curtool->SetFoodHeld(0);
			}
			curtool->UpdateAnimal(dt, dir, attachedCamera->position, animalist);
			curtool->UpdateEnemy(dt, dir, attachedCamera->position, enemylist);
			curtool->SetStandinOn(false);
			curtool->SetTileType('N');
			curtool->ResetFurnaceID(); 
			curtool->SetFurnaceClick(false);
			curtool->SetBerryClick(false);
			curtool->SetWaterClick(false);
			curtool->SetDungeonRitual(false);
			if (!curtool->GetTileOnce())
			{
				curtool->UpdateTile(dt, dir, attachedCamera->position, tilearray);
			}
			if (curtool->GetFurnaceClick())
			{
				curtool->UpdateFurnace(FurnaceX, FurnaceZ);
			}
			else
			{
				PlaceBlock();
			}

			static bool bLButtonState = false;

			if (!bLButtonState &&Application::IsMousePressed(0))
			{
				CSoundEngine::GetInstance()->PlayASound2D("Swing_Action");
				bLButtonState = true;
				curtool->SetLClick();
				if (playerphysics.RayTraceDist(viewVector, attachedCamera->position, Vector3(12000, -500, 12000), Vector3(13000, 500, 13000)))
				{
					//std::cout << "left " << playerphysics.GetDist();
				}
				else
					//std::cout << "lnotcollide";
				curtool->SetCurSwing();
				/*if (getItem(getCurrentSlot())->getID() == Item::ITEM_WOODEN_SWORD)
				{
					curtool->SetCurSwing();
				}
				if (getItem(getCurrentSlot())->getID() == Item::ITEM_WOODEN_PICKAXE ||
					getItem(getCurrentSlot())->getID() == Item::ITEM_STONE_PICKAXE ||
					getItem(getCurrentSlot())->getID() == Item::ITEM_GOLD_PICKAXE)
				{
					
				}*/
			}
			else if (bLButtonState && !Application::IsMousePressed(0))
			{
				bLButtonState = false;
			}

			static bool bRButtonState = false;
			if (!bRButtonState && Application::IsMousePressed(1))
			{
				bRButtonState = true;
				curtool->SetRClick();
				curtool->SetCurSwing();
			}
			else if (bRButtonState && !Application::IsMousePressed(1))
			{
				bRButtonState = false;


				if (m_dHunger < 100)
				{
					if (getItem(getCurrentSlot())->getID() == Item::ITEM_MEAT || getItem(getCurrentSlot())->getID() == Item::ITEM_COOKED_MEAT || getItem(getCurrentSlot())->getID() == Item::ITEM_CARROT || getItem(getCurrentSlot())->getID() == Item::ITEM_BREAD || getItem(getCurrentSlot())->getID() == Item::ITEM_BERRY)
					{
						action = PlayerInformation::EATING;
						getItem(getCurrentSlot())->addQuantity(-1);
					}
				}
				if (m_dThirst < 100)
				{
					if (getItem(getCurrentSlot())->getID() == Item::ITEM_WATER_BOTTLE || getItem(getCurrentSlot())->getID() == Item::ITEM_BERRY)
					{
						action = PlayerInformation::EATING;
						getItem(getCurrentSlot())->addQuantity(-1);
					}
				}
			}
		}
	}
}

void PlayerInformation::PlaceBlock()
{
	placedown = false;
	breakblock = false;
	if (!curtool->GetStandinOn())
	{
		//Tiles that player can edit.
		if (curtool->GetClick())
		{
			if (curtool->GetTileType() == 'G' || curtool->GetTileType() == 't')
			{
				placedown = true;
			}
		}
		else if (!curtool->GetClick())
		{
			if (curtool->GetTileType() == 'F' || curtool->GetTileType() == 'O' || curtool->GetTileType() == 'C' || curtool->GetTileType() == 'T' || curtool->GetTileType() == 'c' || curtool->GetTileType() == 'w' || curtool->GetTileType() == 'B' || curtool->GetTileType() == 'W')
			{
				breakblock = true;
			}
		}
	}
}

bool PlayerInformation::GetPlaceDown()
{
	return placedown;
}

bool PlayerInformation::GetBreakBlock()
{
	return breakblock;
}

double PlayerInformation::getHunger()
{
	return m_dHunger;
}
double PlayerInformation::getHP()
{
	return m_dHP;
}
void PlayerInformation::SetHP(double m_dHP)
{
	this->m_dHP = m_dHP;
}
double PlayerInformation::getMaxHP()
{
	return m_dMaxHP;
}
void PlayerInformation::SetMaxHP(double m_dMaxHP)
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
void PlayerInformation::UpdatePlayersStrength()
{
	switch (getItem(getCurrentSlot())->getID())
	{
	//Sword
	case Item::ITEM_WOODEN_SWORD:
		m_fPlayersDamage = 10.f;
		break;
	case Item::ITEM_STONE_SWORD:
		m_fPlayersDamage = 10.f;
		break;
	case Item::ITEM_GOLD_SWORD:
		m_fPlayersDamage = 15.f;
		break;
	//Pickaxe
	case Item::ITEM_WOODEN_PICKAXE:
		m_fPlayersDamage = 6.f;
		break;
	case Item::ITEM_STONE_PICKAXE:
		m_fPlayersDamage = 8.f;
		break;
	case Item::ITEM_GOLD_PICKAXE:
		m_fPlayersDamage = 10.f;
		break;
	//Axe
	case Item::ITEM_WOODEN_AXE:
		m_fPlayersDamage = 6.f;
		break;
	case Item::ITEM_STONE_AXE:
		m_fPlayersDamage = 8.f;
		break;
	case Item::ITEM_GOLD_AXE:
		m_fPlayersDamage = 10.f;
		break;
	//Hoe
	case Item::ITEM_WOODEN_HOE:
		m_fPlayersDamage = 5.f;
		break;
	case Item::ITEM_STONE_HOE:
		m_fPlayersDamage = 6.f;
		break;
	case Item::ITEM_GOLD_HOE:
		m_fPlayersDamage = 7.f;
		break;
	default:
		m_fPlayersDamage = 2.f;
		break;
	}
	
}
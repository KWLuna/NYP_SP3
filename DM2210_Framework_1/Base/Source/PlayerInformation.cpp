#include "PlayerInformation.h"
#include "Application.h"
#include <string>
#include <iostream>
using namespace std;
PlayerInformation::PlayerInformation()
{
	m_dHP = 100;
	m_dHunger = 100;
	
	m_fSpeed = 100.f;
	m_dBounceTime = 0;

	m_bCrafting = false;

	m_iCurrentStance = STAND;

	m_iInventorySlot = 0;
	m_iConstrainY = 40;
	m_iCraftingSlotOne = -1;
	m_iCraftingSlotTwo = -1;


	m_bSwitchStance = false;
	//Init inventory as empty , 9 slots in total.
	for (int i = 0; i < 15; ++i)
		ItemList.push_back(new Item(0, 0));
}

PlayerInformation::~PlayerInformation()
{
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
	attachedCamera->position.y = m_iConstrainY;
	attachedCamera->target = attachedCamera->position + viewVector;
	//
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

	/*if (firstItem == Item::ITEM_STONE)
	{
		if (secondItem == Item::ITEM_WOOD)
			return new Item(Item::ITEM_TORCH , 4);
	}*/

	return new Item(-1, 0);
}

void PlayerInformation::update(double dt)
{
	// Update bounce time.
	m_dBounceTime -= 1 * dt;

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
			if (m_iConstrainY < 40)
				m_iConstrainY += 10 * dt;
			else
				m_bSwitchStance = false;
			break;
		case CROUCH:
			if (m_iConstrainY > 20)
				m_iConstrainY -= 10 * dt;
			else
				m_bSwitchStance = false;
			break;
		case PRONE:
			if (m_iConstrainY > 5)
				m_iConstrainY -= 10 * dt;
			else
				m_bSwitchStance = false;
			break;
		}
	}

	if (Application::IsKeyPressed('E') && m_dBounceTime <= 0)
	{
		if (m_bCrafting == true)
			m_bCrafting = false;
		else
			m_bCrafting = true;

		m_dBounceTime = 0.2;
	}

	if (Application::IsKeyPressed(VK_LEFT) && m_dBounceTime <= 0)
	{
		m_iInventorySlot -= 1;
		m_dBounceTime = 0.2;

		if (m_iInventorySlot < 0)
			m_iInventorySlot = 14;
	}

	if (Application::IsKeyPressed(VK_RIGHT) && m_dBounceTime <= 0)
	{
		m_iInventorySlot += 1;
		m_dBounceTime = 0.2;

		if (m_iInventorySlot > 14)
			m_iInventorySlot = 0;
	}

	if (m_dHunger < 30)
	{
		m_fSpeed = 80;
		if (m_dHunger < 0)
		{
			m_dHunger = 0;
		}
		if (m_dHunger == 0)
		{
			m_dHP -= 3 * dt;
		}
	}

	if (m_bCrafting == true)
	{
		if (Application::IsKeyPressed(VK_RETURN) && m_dBounceTime <= 0)
		{
			m_dBounceTime = 0.2;

			if (m_iCraftingSlotOne != -1 && m_iCraftingSlotTwo != -1)
			{
				Item * result = craft(m_iCraftingSlotOne, m_iCraftingSlotTwo);

				m_iCraftingSlotOne = -1;
				m_iCraftingSlotTwo = -1;

				if (result->getID() != -1)
				{
					if (addItem(result) == false)
					{
						// drop item , waiting for raycast
					}
				}
			}

			if (m_iCraftingSlotOne == -1)
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
	}

		m_fSpeed = 100;
		if (m_dHunger > 80)
		{
			if (m_dHP < 100)
			{
				m_dHP += 0.5 * dt;
				m_dHunger -= 0.1 * dt;
			}
			
		}

	if (m_bCrafting == false)
	{
		Vector3 viewVector = attachedCamera->target - attachedCamera->position;
		Vector3 rightUV;

		//Movement
		if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
		{
			float m_fSpeed = 100;

			Vector3 viewVector = attachedCamera->target - attachedCamera->position;
			Vector3 rightUV;
			if (Application::IsKeyPressed('W'))
			{
				if ((Application::IsKeyPressed('W')) && (Application::IsKeyPressed(VK_SHIFT)))
				{
					attachedCamera->position += viewVector.Normalized() * m_fSpeed * 2.0f * (float)dt;
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
			Constrain();
			// Update the target
			attachedCamera->target = attachedCamera->position + viewVector;
		}
		else
		{
			action = STANDING;
		}

		if (action == NUM_ACTION)
		{
			action = STANDING;
		}

		switch (action)
		{
		case PlayerInformation::STANDING:
			break;
		case PlayerInformation::SPRINTING:
			m_dHunger -= 0.1 * dt;
			break;
		case PlayerInformation::WALKING:
			m_dHunger -= 0.05 * dt;
			break;
		case PlayerInformation::EATING:
			m_dHunger += 0.5 * dt;
			break;
		case PlayerInformation::NUM_ACTION:
			break;
		default:
			break;
		}

		if (m_dHunger < 30)
		{
			m_fSpeed = 80;
			if (m_dHunger < 0)
			{
				m_dHunger = 0;
			}
			if (m_dHunger == 0)
			{
				m_dHP -= 3 * dt;
			}
		}
		else
		{
			m_fSpeed = 100;
			if (m_dHunger > 80)
			{
				if (m_dHP < 100)
				{
					m_dHP += 0.5 * dt;
					m_dHunger -= 0.1 * dt;
				}
			}
		}
	}
}

double PlayerInformation::getHunger()
{
	return m_dHunger;
}
double PlayerInformation::getHP()
{
	return m_dHP;
}
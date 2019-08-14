#include "PlayerInformation.h"
#include "Application.h"
#include <string>
#include <iostream>
using namespace std;
PlayerInformation::PlayerInformation()
{
	m_dBounceTime = 0;

	m_bCrafting = false;

	m_iInventorySlot = 0;

	m_iCraftingSlotOne = 0;
	m_iCraftingSlotTwo = 0;

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
	attachedCamera->position.y = 40;
	attachedCamera->target = attachedCamera->position + viewVector;
	//
}

Item * PlayerInformation::getItem(int id)
{
	for (int i = 0; i < ItemList.size(); ++i)
	{
		if (ItemList[i]->getID() == id)
		{
			return ItemList[i];
		}
	}
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

void PlayerInformation::update(double dt)
{
	m_dBounceTime -= 1 * dt;
	for (int i = 0; i < ItemList.size(); ++i)
	{
		if (ItemList[i]->getQuantity() == 0)
		{
			ItemList[i]->setID(0);
		}
	}

	if (Application::IsKeyPressed('E') && m_dBounceTime <= 0)
	{
		if (m_bCrafting == true)
			m_bCrafting = false;
		else
			m_bCrafting = true;

		m_dBounceTime = 1;
	}

	if (m_bCrafting == true)
	{
		for (int i = 0; i < 9; ++i)
		{
			string chara = to_string(i);

			// Test if a key is pressed;
			if (Application::IsKeyPressed(chara[0]))
			{
				if (m_iCraftingSlotTwo == 0)
					m_iCraftingSlotTwo = i;

				m_iCraftingSlotOne = i;
			}
		}
	}

	if (m_bCrafting == false)
	{
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

		//Movement
		if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
		{
			float m_fSpeed = 100;

			Vector3 viewVector = attachedCamera->target - attachedCamera->position;
			Vector3 rightUV;
			if (Application::IsKeyPressed('W'))
			{
				if ((Application::IsKeyPressed('W')) && (Application::IsKeyPressed(VK_SHIFT)))
					attachedCamera->position += viewVector.Normalized() * m_fSpeed * 2.0f * (float)dt;
				else
				{
					attachedCamera->position = attachedCamera->position + viewVector.Normalized() * m_fSpeed * (float)dt;
				}
			}
			else if (Application::IsKeyPressed('S'))
			{
				attachedCamera->position -= viewVector.Normalized() * m_fSpeed * (float)dt;
			}
			if (Application::IsKeyPressed('A'))
			{
				rightUV = (viewVector.Normalized()).Cross(attachedCamera->up);
				rightUV.y = 0;
				rightUV.Normalize();
				attachedCamera->position -= rightUV * m_fSpeed * (float)dt;
			}
			else if (Application::IsKeyPressed('D'))
			{
				rightUV = (viewVector.Normalized()).Cross(attachedCamera->up);
				rightUV.y = 0;
				rightUV.Normalize();
				attachedCamera->position += rightUV * m_fSpeed * (float)dt;
			}
			// Constrain the position
			Constrain();
			// Update the target
			attachedCamera->target = attachedCamera->position + viewVector;
		}
	}
}
#include "PlayerInformation.h"
#include "Application.h"
#include <string>
#include <iostream>
using namespace std;
PlayerInformation::PlayerInformation()
{
	bounceTime = 0;
	m_dHunger = 100;
	m_dHP = 100;
	m_fSpeed = 100.f;
	//Init inventory as empty , 9 slots in total.
	for (int i = 0; i < 9; ++i)
		ItemList.push_back(new Item(0, 0));
}

PlayerInformation::~PlayerInformation()
{
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

void PlayerInformation::update(double dt)
{
	for (int i = 0; i < ItemList.size(); ++i)
	{
		if (ItemList[i]->getQuantity() == 0)
		{
			ItemList[i]->setID(0);
		}
	}

	for (int i = 1; i < 10; ++i)
	{
		string chara = to_string(i);

		// Test if a key is pressed;
		if (Application::IsKeyPressed(chara[0]))
		{
			if (slot1 != i)
			{
				if (slot1 >= 1 && slot1 <= 9)
				{
					Item * temp = new Item(ItemList[slot1]->getID(), ItemList[slot1]->getQuantity());

					ItemList[slot1]->setID(ItemList[i]->getID());
					ItemList[slot1]->setQuantity(ItemList[i]->getQuantity());

					ItemList[i]->setID(temp->getID());
					ItemList[i]->setQuantity(temp->getQuantity());
				}
				slot1 = i;
			}
			// If key is pressed , break
			break;
		}
	}

	//Movement
	if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
	{
		Vector3 viewVector = attachedCamera->target - attachedCamera->position;
		Vector3 rightUV;
		if (Application::IsKeyPressed('W'))
		{
			
			if ((Application::IsKeyPressed('W')) && (Application::IsKeyPressed(VK_SHIFT)) && m_dHunger > 30)
			{

				attachedCamera->position += viewVector.Normalized() * m_fSpeed * 2.0f * (float)dt;
				action = SPRINTING;
			}
			else
			{
				attachedCamera->position = attachedCamera->position + viewVector.Normalized() * m_fSpeed * (float)dt;
				action = WALKING;

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
double PlayerInformation::getHunger()
{
	return m_dHunger;
}
double PlayerInformation::getHP()
{
	return m_dHP;
}
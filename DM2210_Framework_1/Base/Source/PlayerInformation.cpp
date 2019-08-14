#include "PlayerInformation.h"
#include "Application.h"
#include <string>
#include <iostream>
using namespace std;
PlayerInformation::PlayerInformation()
{
	bounceTime = 0;

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
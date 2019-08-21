#pragma once
#include "Camera3.h"
#include "item.h"
#include "Physics.h"
#include "Weapons.h"
#include "Sword.h"
#include "CAnimal.h"
#include <vector>
#include "Application.h"
#include <string>
#include <iostream>
class PlayerInformation
{
	enum POSITION
	{
		STAND,
		CROUCH,
		PRONE,
	};
public:
	enum TYPE_ACTION
	{
		STANDING = 0,
		SPRINTING,
		WALKING,
		EATING,
	};

	TYPE_ACTION action;
	PlayerInformation();
	~PlayerInformation();

	void Constrain(void);
	void AttachCamera(Camera3* _cameraPtr);
	void update(double dt, std::vector<CAnimal *> animlist);
	/*void CheckHeldCollision(double dt, std::vector<CAnimal *> animlist);*/

	bool addItem(Item * object);
	Item * getItem(int ID);

	int getTotalItems();
	bool getIsCrafting();

	int getCurrentSlot();

	int getCraftingSlotOne();
	int getCraftingSlotTwo();

	void SetFurnaceStatus(bool condition);

	Item * craft(int firstItem, int secondItem);

	double getHunger();
	double getThirst();
	void SetThirst(double m_dThirst);
	double getHP();
	void SaveData();
	void LoadData();
	void SetHP(float m_dHP);
	double getMaxHP();
	void SetMaxHP(float m_dMaxHP);
	Weapons * getcurtool();
private:

	Camera3 * attachedCamera; // Attach camera to player

	bool m_bCrafting; // is the player in crafting mode

	int m_iCraftingSlotOne; // which slot the crafting slot 1 is refering to
	int m_iCraftingSlotTwo; // Which slot the crafting slot 2 is refering to

	int m_iSwitchInventorySlot;
	int m_iInventorySlot; // Which slot is currently selected

	double m_dBounceTime; // Bounce time

	double m_dConstrainY; // Height which the player is limited to
	int m_iCurrentStance;

	bool m_bSwitchStance;
	bool m_bFurnaceStatus;
	bool m_bJump;
	bool m_bFall;

	std::vector<Item*> ItemList;
	float m_fSpeed;
	float walkingtime;
	double m_dHunger;
	double m_dHP;
	double m_dMaxHP;
	double m_dThirst;
	float ThirstyOrHungry;
	Physics playerphysics;
	Weapons * curtool;
};

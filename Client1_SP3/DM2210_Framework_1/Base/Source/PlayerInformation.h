#pragma once
#include "Camera3.h"
#include "Physics.h"
#include "Weapons.h"
#include "WoodenSword.h"
#include "StoneSword.h"
#include "GoldSword.h"
#include "WoodenPickaxe.h"
#include "StonePickaxe.h"
#include "GoldPickaxe.h"
#include "WoodenHoe.h"
#include "StoneHoe.h"
#include "GoldHoe.h"
#include "WoodenAxe.h"
#include "StoneAxe.h"
#include "GoldAxe.h"
#include "CAnimal.h"
#include <vector>
#include "Application.h"
#include <string>
#include <iostream>
#include "SoundEngine.h"
#include "DroppedItem.h"
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

	void Constrain(char PlayerTile[], Vector3 prevpos);
	void AttachCamera(Camera3* _cameraPtr);
	void update(double dt, std::vector<CAnimal*> animlist, std::vector<CEnemy*> enemylist, char tilearray[], std::vector<char> FurnaceX, std::vector<char> FurnaceZ);

	bool addItem(Item * object);
	Item * getItem(int ID);
	DroppedItem * getDroppedItem(int ID);

	int getTotalItems();
	int getTotalDropItems();

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
	void SetHP(double m_dHP);
	double getMaxHP();
	void SetMaxHP(double m_dMaxHP);
	Weapons * getcurtool();

	void PlaceBlock();
	bool GetPlaceDown();
	bool GetBreakBlock();
	void UpdatePlayersStrength();

	bool GetFurnaceStatus();
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

	double m_dDropTime;

	bool m_bSwitchStance;
	bool m_bFurnaceStatus;
	bool m_bJump;
	bool m_bFall;

	std::vector<Item*> ItemList;
	std::vector<DroppedItem*> DroppedItemList;

	float m_fSpeed;
	float walkingtime;
	double m_dHunger;
	double m_dHP;
	double m_dMaxHP;
	double m_dThirst;
	float ThirstyOrHungry;
	Physics playerphysics;
	Weapons * curtool;
	float m_fPlayersDamage;
	bool placedown;
	bool breakblock;

	char movedir;
};
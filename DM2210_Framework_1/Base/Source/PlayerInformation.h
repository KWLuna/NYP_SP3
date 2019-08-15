#pragma once
#include "Camera3.h"
#include "item.h"
#include <vector>
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
		NUM_ACTION,
	};

	TYPE_ACTION action;
	PlayerInformation();
	~PlayerInformation();

	void Constrain(void);
	void AttachCamera(Camera3* _cameraPtr);
	void update(double dt);

	bool addItem(Item * object);
	Item * getItem(int ID);

	int getTotalItems();
	bool getIsCrafting();
	
	int getCurrentSlot();

	int getCraftingSlotOne();
	int getCraftingSlotTwo();
	
	
	Item * craft(int firstItem, int secondItem);

	double getHunger();
	double getHP();
private:
	 
	Camera3 * attachedCamera; // Attach camera to player

	bool m_bCrafting; // is the player in crafting mode

	int m_iCraftingSlotOne; // which slot the crafting slot 1 is refering to
	int m_iCraftingSlotTwo; // Which slot the crafting slot 2 is refering to

	int m_iInventorySlot; // Which slot is currently selected

	double m_dBounceTime; // Bounce time

	int m_iConstrainY; // Height which the player is limited to
	int m_iCurrentStance;

	bool m_bSwitchStance;

	std::vector<Item*> ItemList;
	float m_fSpeed;

	double hunger;
	double HP;
};
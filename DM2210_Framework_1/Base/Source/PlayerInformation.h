#pragma once
#include "Camera3.h"
#include "item.h"
#include <vector>
class PlayerInformation
{
public:
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

private:
	Camera3 * attachedCamera;

	bool m_bCrafting;

	int m_iCraftingSlotOne;
	int m_iCraftingSlotTwo;

	int m_iInventorySlot;

	double m_dBounceTime;

	std::vector<Item*> ItemList;
};
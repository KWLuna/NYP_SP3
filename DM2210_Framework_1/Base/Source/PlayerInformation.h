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

private:
	Camera3 * attachedCamera;

	int slot1;
	int slot2;

	double bounceTime;

	std::vector<Item*> ItemList;
};


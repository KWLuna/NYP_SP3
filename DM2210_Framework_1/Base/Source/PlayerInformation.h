#pragma once
#include "Camera3.h"
#include "item.h"
#include <vector>
class PlayerInformation
{
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

	double getHunger();
	double getHP();
private:
	Camera3 * attachedCamera;

	int slot1;
	int slot2;

	double bounceTime;

	std::vector<Item*> ItemList;
	float m_fSpeed;

	double hunger;
	double HP;
};
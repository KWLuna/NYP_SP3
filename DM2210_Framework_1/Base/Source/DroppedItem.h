#include <vector>
#include <iostream>
#include "Item.h"

class DroppedItem : public Item
{
public:
	DroppedItem(int id, int quantity , float xPos , float zPos);
	~DroppedItem();

	float getXPos();
	float getZPos();

private:
	float m_fxPos;
	float m_fzPos;
};
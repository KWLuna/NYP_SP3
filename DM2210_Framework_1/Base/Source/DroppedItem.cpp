#include "DroppedItem.h"

DroppedItem::DroppedItem(int id, int quantity , float xPos , float zPos)
{
	m_iID = id;
	m_iQuantity = quantity;

	m_fxPos = xPos;
	m_fzPos = zPos;
}

DroppedItem::~DroppedItem()
{

}

float DroppedItem::getXPos()
{
	return m_fxPos;
}

float DroppedItem::getZPos()
{
	return m_fzPos;
}
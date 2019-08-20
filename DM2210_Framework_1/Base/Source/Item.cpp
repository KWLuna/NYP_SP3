#include "Item.h"

Item::Item()
{
	m_iID = 0;
	m_iQuantity = 0;
}

Item::Item(int id, int quantity)
{
	m_iID = id;
	m_iQuantity = quantity;
}

Item::~Item()
{

}

void Item::setQuantity(int quantity)
{
	m_iQuantity = quantity;
}

int Item::getID()
{
	return m_iID;
}

void Item::setID(int id)
{
	m_iID = id;
}

int Item::getQuantity()
{
	return m_iQuantity;
}

void Item::addQuantity(int quantity)
{
	m_iQuantity += quantity;
}
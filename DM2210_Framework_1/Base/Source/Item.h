#pragma once
class Item
{
public:
	enum ITEM_TYPE
	{
		ITEM_MEAT,
		ITEM_WOOD,
		ITEM_STONE,
		ITEM_GOLD,

		ITEM_END
	};
	ITEM_TYPE Items;

	Item();
	Item(char id, int quantity);
	~Item();

	char getID();
	void setID(char id);
	void addQuantity(int quantity);
	int getQuantity();

	void setQuantity(int quantity);

private:
	char m_iID;
	int m_iQuantity;
};


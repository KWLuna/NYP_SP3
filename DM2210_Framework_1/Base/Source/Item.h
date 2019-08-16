#include <vector>
#include <iostream>

using namespace std;

class Item
{
public:
	enum ITEM_TYPE
	{
		//DONE
		ITEM_EMPTY,
		ITEM_MEAT,
		ITEM_COOKED_MEAT,

		ITEM_WOOD,
		ITEM_STICK,
		ITEM_COAL,
		ITEM_STONE,
		ITEM_GOLD_NUGGET,
		ITEM_TORCH,

		ITEM_WOODEN_SWORD,
		ITEM_WOODEN_PICKAXE,

		ITEM_STONE_SWORD,
		ITEM_STONE_PICKAXE,

		ITEM_GOLD_SWORD,
		ITEM_GOLD_PICKAXE,
		
		//TO ADD

		ITEM_END
	};

	Item();
	Item(char id, int quantity);
	~Item();

	char getID();
	void setID(char id);
	void addQuantity(int quantity);
	int getQuantity();

	void setQuantity(int quantity);

private:
	vector <int * > smeltables;

	char m_iID;
	int m_iQuantity;
};
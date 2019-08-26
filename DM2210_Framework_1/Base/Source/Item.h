#include <vector>
#include <iostream>

class Item
{
public:
	enum ITEM_TYPE
	{
		//DONE
		ITEM_EMPTY,
		ITEM_MEAT,
		ITEM_COOKED_MEAT,

		ITEM_WHEAT,
		ITEM_BREAD,
		ITEM_CARROT,
		ITEM_SEED,
		ITEM_BERRY,

		ITEM_WOOD,
		ITEM_STICK,
		ITEM_COAL,
		ITEM_STONE,
		ITEM_GOLD_NUGGET,
		ITEM_TORCH,

		ITEM_WOODEN_SWORD,
		ITEM_WOODEN_PICKAXE,
		ITEM_WOODEN_AXE,
		ITEM_WOODEN_HOE,

		ITEM_STONE_SWORD,
		ITEM_STONE_PICKAXE,
		ITEM_STONE_AXE,
		ITEM_STONE_HOE,

		ITEM_GOLD_SWORD,
		ITEM_GOLD_PICKAXE,
		ITEM_GOLD_AXE,
		ITEM_GOLD_HOE,
		
		ITEM_FURNACE,
		//TO ADD

		ITEM_END
	};

	Item();
	Item(int id, int quantity);
	~Item();

	int getID();
	void setID(int id);
	void addQuantity(int quantity);
	int getQuantity();

	void setQuantity(int quantity);

protected:
	std::vector <int * > smeltables;

	int m_iID;
	int m_iQuantity;
};
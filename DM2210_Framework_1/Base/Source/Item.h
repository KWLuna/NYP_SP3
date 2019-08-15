class Item
{
public:
	enum ITEM_TYPE
	{
		ITEM_EMPTY,
		ITEM_MEAT,
		ITEM_WOOD,
		ITEM_STICK,
		ITEM_COAL,
		ITEM_STONE,
		ITEM_GOLD_NUGGET,
		ITEM_TORCH,

		ITEM_WOODEN_SWORD,
		ITEM_WOODEN_PICKAXE,

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
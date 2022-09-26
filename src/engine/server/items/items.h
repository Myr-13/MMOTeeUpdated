#ifndef ENGINE_SERVER_ITEMS_ITEMS
#define ENGINE_SERVER_ITEMS_ITEMS

enum
{
	NOPE = 0,

#define ITEM(EnumName, ItemType, ItemName, ItemDesc) EnumName,
#include "items_list.h"
#undef ITEM

	MAX_ITEM,
};

class CItems
{
	char m_aItemName[MAX_ITEM][64] = {};
	char m_aItemDesc[MAX_ITEM][128] = {};

public:
	CItems()
	{
#define ITEM(EnumName, ItemType, ItemName, ItemDesc) \
	str_copy(m_aItemName[EnumName], ItemName, 64); \
	str_copy(m_aItemDesc[EnumName], ItemDesc, 128);
#include "items_list.h"
#undef ITEM
	}

	const char* GetItemName(int ItemID)
	{
		return m_aItemName[ItemID];
	}

	const char* GetItemDesc(int ItemID)
	{
		return m_aItemDesc[ItemID];
	}
};

#endif // !ENGINE_SERVER_ITEMS_ITEMS

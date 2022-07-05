#ifndef GAME_SERVER_COMPONETNS_BOX_H
#define GAME_SERVER_COMPONETNS_BOX_H

#include <game/server/gamecontext.h>

struct BoxItem
{
	int m_Type;
	int m_Count;
	int m_Proc;
};

class CBox
{
	CGameContext* m_pGameContext;

	std::vector<BoxItem> m_aItems;
	std::vector<BoxItem> m_aRareItems;

	char m_aBoxName[64];

public:
	CBox(class CGameContext *pGameContext, const char *pName = "");

	void SetName(const char* pName);

	void Add(int Item, int Count = 1);
	void AddRare(int Item, int Proc, int Count = 1);
	void Open(int ClientID, int Iterations);
};

#endif

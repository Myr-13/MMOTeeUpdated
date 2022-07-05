#ifndef GAME_SERVER_COMPONETNS_CRAFT_H
#define GAME_SERVER_COMPONETNS_CRAFT_H

#include <game/server/gamecontext.h>

struct CraftIngridient {
	int m_ItemID;
	int m_Count;
};

class CCraft
{
	CGameContext* m_pGameContext;
	CGameContext* GameServer() { return m_pGameContext; }
	IServer* Server();

	int m_Result;
	std::vector<CraftIngridient> m_aCraftIngridiets;

public:
	CCraft(CGameContext* pGameContext, int Result);

	void AddIngridient(int ItemID, int Count = 1);
	void Craft(int ClientID, int Count);
	void Clear();
	void Build();
};

#endif

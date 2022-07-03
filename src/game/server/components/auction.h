#ifndef GAME_SERVER_COMPONETNS_AUCTION_H
#define GAME_SERVER_COMPONETNS_AUCTION_H

#include "game/server/gamecontext.h"
#include "engine/server.h"

enum
{
	AUCTIONMENU_NONE,
	AUCTIONMENU_BUY,
	AUCTIONMENU_SELL,
	AUCTIONMENU_ITEMS,
};

struct AuctionItem
{
	char m_aSellerName[MAX_NAME_LENGTH];

	int m_ItemType;
	char m_aItemName[128];
	int m_ItemCount;

	int m_Cost;

	int m_Seconds;
};

class CAuction
{
public:
	CAuction(CGameContext* pGameServer);

	void OnMenuAction(int ClientID, const char* pMsg, const char* pReason);
	void BuildVoteMenu(int ClientID);
	void OnTick();

private:
	CGameContext* m_pGameServer;

	CGameContext* GameServer() const { return m_pGameServer; }
	IServer* Server() const;

	int m_aClientMenu[MAX_PLAYERS];
	AuctionItem m_aClientItems[MAX_PLAYERS];

	std::vector<AuctionItem> m_aAuctionItems;

	void ResetItem(AuctionItem& item);
};

#endif

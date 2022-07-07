#include "auction.h"

CAuction::CAuction(CGameContext* pGameServer)
{
	m_pGameServer = pGameServer;

	for (int i = 0; i < MAX_PLAYERS; i++)
		m_aClientMenu[i] = AUCTIONMENU_NONE;
}

IServer* CAuction::Server() const { return m_pGameServer->Server(); }

void CAuction::OnMenuAction(int ClientID, const char* pMsg, const char* pReason)
{
	if (!str_comp(pMsg, "auc_show_buy_menu"))
	{
		m_aClientMenu[ClientID] = AUCTIONMENU_BUY;

		GameServer()->ResetVotes(ClientID, AUCTION);

		return;
	}
	if (!str_comp(pMsg, "auc_show_sell_menu"))
	{
		m_aClientMenu[ClientID] = AUCTIONMENU_SELL;

		ResetItem(m_aClientItems[ClientID]);

		GameServer()->ResetVotes(ClientID, AUCTION);

		return;
	}
	if (!str_comp(pMsg, "auc_show_items"))
	{
		m_aClientMenu[ClientID] = AUCTIONMENU_ITEMS;

		GameServer()->ResetVotes(ClientID, AUCTION);

		return;
	}

	if (!str_comp(pMsg, "auc_ch_count"))
	{
		if (pReason[0] && isdigit(pReason[0]))
			m_aClientItems[ClientID].m_ItemCount = atoi(pReason);

		GameServer()->ResetVotes(ClientID, AUCTION);

		return;
	}
	if (!str_comp(pMsg, "auc_ch_gold"))
	{
		if (pReason[0] && isdigit(pReason[0]))
			m_aClientItems[ClientID].m_Cost = atoi(pReason);

		GameServer()->ResetVotes(ClientID, AUCTION);

		return;
	}
	if (!str_comp(pMsg, "auc_ch_time"))
	{
		if (pReason[0] && isdigit(pReason[0]))
			m_aClientItems[ClientID].m_Seconds = atoi(pReason);

		GameServer()->ResetVotes(ClientID, AUCTION);

		return;
	}

	if (!str_comp(pMsg, "auc_place"))
	{
		if (m_aClientItems[ClientID].m_ItemType < 0)
			return;
		if (m_aClientItems[ClientID].m_Cost <= 0)
			return;
		if (m_aClientItems[ClientID].m_ItemCount <= 0)
			return;
		if (m_aClientItems[ClientID].m_Seconds <= 0)
			return;
		if (Server()->GetItemCount(ClientID, m_aClientItems[ClientID].m_ItemType) < m_aClientItems[ClientID].m_ItemCount)
		{
			GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, "You dont have enough item count");
			return;
		}

		int Pay = m_aClientItems[ClientID].m_Cost * 0.0625f + m_aClientItems[ClientID].m_Seconds / 1000;
		if (GameServer()->m_apPlayers[ClientID]->AccData.Gold < Pay)
			return;

		str_copy(m_aClientItems[ClientID].m_aItemName, Server()->GetItemName(ClientID, m_aClientItems[ClientID].m_ItemType, false), 128);
		str_copy(m_aClientItems[ClientID].m_aSellerName, Server()->ClientName(ClientID), 16);
		m_aClientMenu[ClientID] = AUCTIONMENU_NONE;

		m_aAuctionItems.push_back(m_aClientItems[ClientID]);

		GameServer()->m_apPlayers[ClientID]->AccData.Gold -= Pay;

		Server()->RemItem(ClientID, m_aClientItems[ClientID].m_ItemType, m_aClientItems[ClientID].m_ItemCount, 
			Server()->GetItemType(ClientID, m_aClientItems[ClientID].m_ItemType));

		ResetItem(m_aClientItems[ClientID]);
		GameServer()->ResetVotes(ClientID, AUCTION);

		return;
	}

	for (int i = 0; i < MAX_ITEM; i++)
	{
		char aBuf[256];
		str_format(aBuf, sizeof(aBuf), "auc_set_item_%d", i);
		if (!str_comp(pMsg, aBuf))
		{
			m_aClientItems[ClientID].m_ItemType = i;

			m_aClientMenu[ClientID] = AUCTIONMENU_SELL;

			GameServer()->ResetVotes(ClientID, AUCTION);

			return;
		}
	}

	for (int i = 0; i < m_aAuctionItems.size(); i++)
	{
		char aBuf[64];
		str_format(aBuf, sizeof(aBuf), "auc_buy_%d", i);
		if (!str_comp(pMsg, aBuf) && i < m_aAuctionItems.size())
		{
			if (GameServer()->m_apPlayers[ClientID]->AccData.Gold < m_aAuctionItems[i].m_Cost)
			{
				GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, "You don't have enough gold for this operation");

				return;
			}

			Server()->GiveItem(ClientID, m_aAuctionItems[i].m_ItemType, m_aAuctionItems[i].m_ItemCount);
			GameServer()->m_apPlayers[ClientID]->AccData.Gold -= m_aAuctionItems[i].m_Cost;
			Server()->AddGold(m_aAuctionItems[i].m_aSellerName, m_aAuctionItems[i].m_Cost);

			m_aAuctionItems.erase(m_aAuctionItems.begin() + i);

			GameServer()->ResetVotes(ClientID, AUCTION);

			return;
		}
	}
}

void CAuction::BuildVoteMenu(int ClientID)
{
	GameServer()->AddVote_Localization(ClientID, "null", "Auction menu");
	GameServer()->AddVote("", "null", ClientID);

	GameServer()->AddVote_Localization(ClientID, "auc_show_buy_menu", "Buy item");
	GameServer()->AddVote_Localization(ClientID, "auc_show_sell_menu", "Sell item");
	GameServer()->AddBack(ClientID);
	GameServer()->AddVote("", "null", ClientID);

	if (m_aClientMenu[ClientID] == AUCTIONMENU_BUY)
	{
		for (int i = 0; i < m_aAuctionItems.size(); i++)
		{
			GameServer()->AddVote_Localization(ClientID, "null", "{str:item} x{int:count} by {str:name}", 
				"item", m_aAuctionItems[i].m_aItemName, "count", &m_aAuctionItems[i].m_ItemCount, "name", m_aAuctionItems[i].m_aSellerName);
			GameServer()->AddVote_Localization(ClientID, "null", "Cost: {int:count}", "count", &m_aAuctionItems[i].m_Cost);
			char aBuf[64];
			str_format(aBuf, sizeof(aBuf), "auc_buy_%d", i);
			GameServer()->AddVote_Localization(ClientID, aBuf, "> Buy");
			GameServer()->AddVote("", "null", ClientID);
		}

		if (m_aAuctionItems.size() == 0)
			GameServer()->AddVote_Localization(ClientID, "null", "No avaible auctions :(");
	}

	if (m_aClientMenu[ClientID] == AUCTIONMENU_SELL)
	{
		int Pay = m_aClientItems[ClientID].m_Cost * 0.0625f + m_aClientItems[ClientID].m_Seconds / 1000;

		GameServer()->AddVote_Localization(ClientID, "auc_show_items", "Item: {str:name}", "name", Server()->GetItemName(ClientID, m_aClientItems[ClientID].m_ItemType), NULL);
		GameServer()->AddVote_Localization(ClientID, "auc_ch_count", "Count: {int:count}", "count", &m_aClientItems[ClientID].m_ItemCount);
		GameServer()->AddVote_Localization(ClientID, "auc_ch_gold", "Cost: {int:cost} gold", "cost", &m_aClientItems[ClientID].m_Cost);
		GameServer()->AddVote_Localization(ClientID, "auc_ch_time", "Time: {int:time} sec", "time", &m_aClientItems[ClientID].m_Seconds);
		GameServer()->AddVote("", "null", ClientID);
		GameServer()->AddVote_Localization(ClientID, "null", "You will pay: {int:cost} gold", "cost", &Pay);
		GameServer()->AddVote_Localization(ClientID, "auc_place", "> Place to auction");
	}

	if (m_aClientMenu[ClientID] == AUCTIONMENU_ITEMS)
	{
		for (int i = 0; i < MAX_ITEM; i++)
		{
			int count = Server()->GetItemCount(ClientID, i);
			if (count)
			{
				char aBuf[256];
				str_format(aBuf, sizeof(aBuf), "auc_set_item_%d", i);
				GameServer()->AddVote_Localization(ClientID, aBuf, "{str:name} x{int:count}", "name", Server()->GetItemName(ClientID, i), "count", &count, NULL);
			}
		}
	}
}

void CAuction::OnTick()
{
	if (Server()->Tick() % Server()->TickSpeed() == 0)
	{
		for (int i = 0; i < m_aAuctionItems.size(); i++)
		{
			m_aAuctionItems[i].m_Seconds -= 1;

			if (m_aAuctionItems[i].m_Seconds <= 0)
				m_aAuctionItems.erase(m_aAuctionItems.begin() + i);
		}
	}
}

void CAuction::ResetItem(AuctionItem& item)
{
	str_copy(item.m_aItemName, "", sizeof(item.m_aItemName));
	str_copy(item.m_aSellerName, "", 16);
	item.m_Cost = 100;
	item.m_ItemCount = 0;
	item.m_ItemType = -1;
	item.m_Seconds = 1;
}

#ifndef GAME_SERVER_COMPONETNS_ADMIN_H
#define GAME_SERVER_COMPONETNS_ADMIN_H

#include "game/server/gamecontext.h"
#include "engine/server.h"

enum {
	ACTION_SET_HEALTH,
	ACTION_SET_ARMOR,
	ACTION_RAINBOW,
	ACTION_BLOODY,
	ACTION_GIVEGOLD,
	ACTION_ENCHANT_ARMOR,
	ACTION_RENAME,
	ACTION_CHANGE_SKIN,
	ACTION_GIVEDONATE,
	ACTION_BRING,
	ACTION_RETURN,
	ACTION_KILL,
	ACTION_GIVEUPGRADE,
	ACTION_GIVESKILLPOINT,
	ACTION_ANTIHOOK,
	ACTION_ANTIDMG,
	ACTION_KICK,
	ACTION_MUTE,
	ACTION_SETTITLE,
	ACTION_GIVELEVEL,
	ACTION_GIVEEXP,
	ACTION_GOTO,
	ACTION_SETWALLLIMIT,
	ACTION_INFMANA,

	ACTION_COUNT
};

enum {
	ADM_MAIN,
	ADM_SERVER,
	ADM_PLAYERS,
	ADM_PL_MENU,
};

class CAdmin
{
public:
	CAdmin(CGameContext* pGameServer);

	void OnMenuAction(int ClientID, const char *pMsg, const char* pReason);
	void BuildVoteMenu(int ClientID);
	void AddBack(int ClientID);

	const char* GetActionName(int Action);
	//const char* GetActionCmd(int Action);

private:
	CGameContext* m_pGameServer;

	CGameContext* GameServer() const { return m_pGameServer; }
	IServer* Server() const;

	int m_aPages[MAX_PLAYERS];
	int m_aLastPages[MAX_PLAYERS];
	int m_aSelectedPlayer[MAX_PLAYERS];
};

#endif

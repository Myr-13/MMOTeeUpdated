#ifndef GAME_SERVER_ADMIN_H
#define GAME_SERVER_ADMIN_H

#include "game/server/gamecontext.h"
#include "engine/server.h"

enum {
	ACTION_SET_HEALTH,
	ACTION_SET_ARMOR,
	ACTION_RAINBOW,
	ACTION_BLOODY,
	ACTION_GIVEGOLD,
	ACTION_ENCHANTARMOR,
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
	ACTION_GIVEEXP
};

enum {
	ADM_PLAYER
};

class CAdmin
{
	CGameContext* m_pGameServer;

public:
	CAdmin(CGameContext* pGameServer);

	void OnMenuAction(int ClientID, const char *pMsg);
	void BuildVoteMenu(int ClientID);

	CGameContext* GameServer() const { return m_pGameServer; }
	IServer* Server() const { return m_pGameServer->Server(); }
};

#endif

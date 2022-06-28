#ifndef GAME_SERVER_ADMIN_H
#define GAME_SERVER_ADMIN_H

#include "game/server/gamecontext.h"
#include "engine/server.h"

class CAdmin
{
public:
	CAdmin(CGameContext* pGameServer);

private:
	CGameContext* m_pGameServer;
	CGameContext* GameServer() const { return m_pGameServer; }

	void LastChat();
};

#endif

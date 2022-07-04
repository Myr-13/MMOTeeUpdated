#ifndef GAME_SERVER_COMPONETNS_DISCORD_H
#define GAME_SERVER_COMPONETNS_DISCORD_H

#include <game/server/gamecontext.h>
#include <dpp/dpp.h>

enum
{
	LOGTYPE_ENTER,
	LOGTYPE_LEFT,
	LOGTYPE_KILL
};

class CDiscord
{
	CGameContext* m_pGameContext;
	CGameContext *GameServer() { return m_pGameContext; }
	IServer* Server();

	dpp::snowflake m_IRCChat;
	dpp::snowflake m_LogsChat;

	dpp::cluster *m_Bot;

	bool m_Logger;

public:
	CDiscord(CGameContext *pGameContext);
	~CDiscord();

	void OnChat(const char *pFrom, const char *pText);
	void OnMessage(const dpp::message_create_t& event);
	void OnLog(int LogType, int ClientID1 = -1, int ClientID2 = -1);
};

#endif

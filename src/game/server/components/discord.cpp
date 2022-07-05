#include "discord.h"

#include <fstream>

CDiscord::CDiscord(CGameContext *pGameContext)
{
	m_pGameContext = pGameContext;
	m_Logger = true;

	// Tests: OTkzOTA1ODgzNjkwMDQ1NTYy.GJLvYe.avKrwc_g-BufQzAtTTnR_-L3eoLXARSkSJyfgQ
	// Release: OTkzMTQwNTM3NjE4MDg3OTg2.G7-qPQ.tsLVHiYH3INJYAmNBIJBEmmyBiRNM78fd2Jo5g
	m_Bot = new dpp::cluster("OTkzOTA1ODgzNjkwMDQ1NTYy.GJLvYe.avKrwc_g-BufQzAtTTnR_-L3eoLXARSkSJyfgQ", dpp::i_default_intents | dpp::i_message_content);
	m_Bot->on_log(dpp::utility::cout_logger());

	m_Bot->on_message_create([this](const dpp::message_create_t& event) { OnMessage(event); });

	std::string data;
	std::ifstream file("botconfig.txt");

	file >> data;
	m_IRCChat = strtoll(data.c_str(), NULL, 10);
	file >> data;
	m_LogsChat = strtoll(data.c_str(), NULL, 10);

	m_Bot->start();
}
IServer *CDiscord::Server() { return m_pGameContext->Server(); }

CDiscord::~CDiscord()
{
	delete m_Bot;
}

void CDiscord::OnChat(const char* pFrom, const char* pText)
{
	dpp::webhook Webhook("https://discord.com/api/webhooks/993486598480080917/DLJ9CXbFhCWdVurWL7wmIBjApUzdyehPHuLFoJFbEXbPXgx7oP62RdWocjLqUAjfRKer");

	Webhook.name = pFrom;
	m_Bot->edit_webhook(Webhook);

	m_Bot->execute_webhook(Webhook, dpp::message(m_IRCChat, pText));
}

void CDiscord::OnLog(int LogType, int ClientID1, int ClientID2)
{
	if (!m_Logger)
		return;

	char aBuf[512];
	struct tm* ptr;
	time_t lt;
	lt = time(NULL);
	ptr = localtime(&lt);

	switch (LogType)
	{
	case LOGTYPE_ENTER:
		str_format(aBuf, sizeof(aBuf), "[%d.%d.%d %d:%d]: %s entered on server", 
			ptr->tm_mday, ptr->tm_mon + 1, ptr->tm_year, ptr->tm_hour, ptr->tm_min,
			Server()->ClientName(ClientID1)
		);
		break;
	case LOGTYPE_LEFT:
		str_format(aBuf, sizeof(aBuf), "[%d.%d.%d %d:%d]: %s left from server",
			ptr->tm_mday, ptr->tm_mon + 1, ptr->tm_year, ptr->tm_hour, ptr->tm_min,
			Server()->ClientName(ClientID1)
		);
		break;
	case LOGTYPE_KILL:
		str_format(aBuf, sizeof(aBuf), "[%d.%d.%d %d:%d]: %s killed %s",
			ptr->tm_mday, ptr->tm_mon + 1, ptr->tm_year, ptr->tm_hour, ptr->tm_min,
			Server()->ClientName(ClientID1), Server()->ClientName(ClientID2)
		);
		break;
	}
}

void CDiscord::OnMessage(const dpp::message_create_t& event)
{
	std::string text = event.msg.content;

	if (text == "!help")
	{
		m_Bot->message_create(dpp::message(
			event.msg.channel_id,
			"List of commands:\n"
			"!help - show all commands\n"
			"!list - show all online players\n"
		));
	}
	if (text == "!list")
	{
		std::string data = "List of online players:\n";
		for (int i = 0; i < MAX_PLAYERS; i++)
			if (Server()->ClientIngame(i))
				data += std::string(Server()->ClientName(i)) + "\n";

		m_Bot->message_create(dpp::message(
			event.msg.channel_id,
			data
		));
	}

	if (text == "!toggle_logger")
	{
		m_Logger ^= 1;
	}
}

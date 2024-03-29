#include "admin.h"

CAdmin::CAdmin(CGameContext* pGameServer)
{
	m_pGameServer = pGameServer;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		m_aPages[i] = ADM_MAIN;
		m_aLastPages[i] = ADM_MAIN;
		m_aSelectedPlayer[i] = 0;
	}
}

IServer* CAdmin::Server() const { return m_pGameServer->Server(); }

void CAdmin::OnMenuAction(int ClientID, const char *pMsg, const char* pReason)
{
	if (!str_comp(pMsg, "adm_back"))
	{
		m_aPages[ClientID] = m_aLastPages[ClientID];

		GameServer()->ResetVotes(ClientID, ADMMENU);

		return;
	}
	if (!str_comp(pMsg, "adm_server"))
	{
		m_aPages[ClientID] = ADM_SERVER;

		GameServer()->ResetVotes(ClientID, ADMMENU);

		return;
	}
	if (!str_comp(pMsg, "adm_players"))
	{
		m_aPages[ClientID] = ADM_PLAYERS;

		GameServer()->ResetVotes(ClientID, ADMMENU);

		return;
	}

	// Players menu
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		char aBuf[64];
		str_format(aBuf, sizeof(aBuf), "adm_ply_%d", i);
		if (!str_comp(pMsg, aBuf))
		{
			m_aPages[ClientID] = ADM_PL_MENU;
			m_aSelectedPlayer[ClientID] = i;

			GameServer()->ResetVotes(ClientID, ADMMENU);

			return;
		}
	}

	// Player actions
	for (int i = 0; i < ACTION_COUNT; i++)
	{
		char aBuf[64];
		str_format(aBuf, sizeof(aBuf), "adm_act_%d", i);
		if (!str_comp(pMsg, aBuf))
		{
			RunAction(ClientID, i, pReason);

			return;
		}
	}
}

void CAdmin::RunAction(int ClientID, int ActionID, const char *pReason)
{
	switch (ActionID)
	{
	case ACTION_SET_HEALTH:
	{
		if (pReason[0] && isdigit(pReason[0]))
			GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->GetCharacter()->m_Health = atoi(pReason);
	} break;
	case ACTION_SET_ARMOR:
	{
		if (pReason[0] && isdigit(pReason[0]))
			GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->GetCharacter()->m_Armor = atoi(pReason);
	} break;
	case ACTION_RAINBOW:
		GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->m_Rainbow ^= 1;
		break;
	case ACTION_BLOODY:
		GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->m_Bloody ^= 1;
		break;
	case ACTION_GIVEGOLD:
	{
		if (pReason[0] && isdigit(pReason[0]))
			GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->AccData.Gold += atoi(pReason);
	} break;
	case ACTION_ENCHANT_ARMOR: break;
	case ACTION_RENAME:
		Server()->SetClientName(m_aSelectedPlayer[ClientID], pReason);
		break;
	case ACTION_CHANGE_SKIN:
		str_copy(GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->m_TeeInfos.m_SkinName, pReason, 64);
		break;
	case ACTION_GIVEDONATE:
	{
		if (pReason[0] && isdigit(pReason[0]))
			GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->AccData.Donate += atoi(pReason);
	} break;
	case ACTION_BRING:
		GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->m_LastPos = GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->GetCharacter()->m_Core.m_Pos;
		GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->GetCharacter()->m_Core.m_Pos = GameServer()->m_apPlayers[ClientID]->GetCharacter()->m_Core.m_Pos;
		break;
	case ACTION_RETURN:
		GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->GetCharacter()->m_Core.m_Pos = GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->m_LastPos;
		break;
	case ACTION_KILL:
		GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->KillCharacter();
		break;
	case ACTION_GIVEUPGRADE:
		if (pReason[0] && isdigit(pReason[0]))
			GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->AccUpgrade.Upgrade = atoi(pReason);
		break;
	case ACTION_GIVESKILLPOINT:
		if (pReason[0] && isdigit(pReason[0]))
			GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->AccUpgrade.SkillPoint = atoi(pReason);
		break;
	case ACTION_ANTIHOOK:
		GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->m_AntiHook ^= 1;
		break;
	case ACTION_ANTIDMG:
		GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->m_AntiDmg ^= 1;
		break;
	case ACTION_KICK:
		Server()->Kick(m_aSelectedPlayer[ClientID], pReason[0] ? pReason : "The worst intruder");
		break;
	case ACTION_MUTE:
		if (pReason[0] && isdigit(pReason[0]))
		{
			int sec = atoi(pReason);
			GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->m_EndMuteTick = Server()->Tick() + (Server()->TickSpeed() * sec);
			GameServer()->SendChatTarget_Localization(m_aSelectedPlayer[ClientID], CHATCATEGORY_DEFAULT, "You has been muted for {int:sec} sec", "sec", &sec);
		}
		break;
	case ACTION_SETTITLE: break;
	case ACTION_GIVELEVEL:
		if (pReason[0] && isdigit(pReason[0]))
			GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->AccData.Level = atoi(pReason);
		break;
	case ACTION_GIVEEXP:
		if (pReason[0] && isdigit(pReason[0]))
			GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->AccData.Exp += atoi(pReason);
		break;
	case ACTION_GOTO:
		GameServer()->m_apPlayers[ClientID]->m_LastPos = GameServer()->m_apPlayers[ClientID]->GetCharacter()->m_Core.m_Pos;
		GameServer()->m_apPlayers[ClientID]->GetCharacter()->m_Core.m_Pos = GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->GetCharacter()->m_Core.m_Pos;
		break;
	case ACTION_SETWALLLIMIT:
		if (pReason[0] && isdigit(pReason[0]))
			GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->m_WallLimit = atoi(pReason);
		break;
	case ACTION_INFMANA:
		GameServer()->m_apPlayers[m_aSelectedPlayer[ClientID]]->m_InfMana ^= 1;
		break;
	}
}

void CAdmin::BuildVoteMenu(int ClientID)
{
	if (m_aPages[ClientID] == ADM_MAIN)
	{
		if (GameServer()->m_apPlayers[ClientID]->m_Authed == AUTHED_ADMIN) GameServer()->AddVote_Localization(ClientID, "adm_server", "> Server");
		GameServer()->AddVote_Localization(ClientID, "adm_players", "> Players");

		GameServer()->AddBack(ClientID);
		return;
	}
	if (m_aPages[ClientID] == ADM_SERVER)
	{
		m_aLastPages[ClientID] = ADM_MAIN;
		
		AddBack(ClientID);

		return;
	}
	if (m_aPages[ClientID] == ADM_PLAYERS)
	{
		m_aLastPages[ClientID] = ADM_MAIN;

		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			if (!GameServer()->m_apPlayers[i])
				continue;

			char aBuf[64];
			str_format(aBuf, sizeof(aBuf), "adm_ply_%d", i);
			GameServer()->AddVote_Localization(ClientID, aBuf, "> {str:name}", "name", Server()->ClientName(i));
		}

		AddBack(ClientID);

		return;
	}
	if (m_aPages[ClientID] == ADM_PL_MENU)
	{
		m_aLastPages[ClientID] = ADM_PLAYERS;

		char aBuf[128];
		str_format(aBuf, sizeof(aBuf), "%s's player menu", Server()->ClientName(m_aSelectedPlayer[ClientID]));
		GameServer()->AddVote_Localization(ClientID, "null", aBuf);

		for (int i = 0; i < ACTION_COUNT; i++)
		{
			char aBuf[64];
			str_format(aBuf, sizeof(aBuf), "adm_act_%d", i);
			GameServer()->AddVote_Localization(ClientID, aBuf, "> {str:name}", "name", GetActionName(i));
		}

		AddBack(ClientID);

		return;
	}
}

void CAdmin::AddBack(int ClientID)
{
	GameServer()->AddVote("", "null", ClientID);
	GameServer()->AddVote_Localization(ClientID, "adm_back", "- Backpage");
}

const char* CAdmin::GetActionName(int Action)
{
	switch (Action)
	{
	case ACTION_SET_HEALTH: return "Set health";
	case ACTION_SET_ARMOR: return "Set armor";
	case ACTION_RAINBOW: return "Give/Take rainbow";
	case ACTION_BLOODY: return "Give/Take bloody";
	case ACTION_GIVEGOLD: return "Give gold";
	case ACTION_ENCHANT_ARMOR: return "Enchant armor (NOT WORKING)";
	case ACTION_RENAME: return "Rename";
	case ACTION_CHANGE_SKIN: return "Change skin";
	case ACTION_GIVEDONATE: return "Give donate";
	case ACTION_BRING: return "Bring";
	case ACTION_RETURN: return "Return";
	case ACTION_KILL: return "Kill";
	case ACTION_GIVEUPGRADE: return "Give upgrade point";
	case ACTION_GIVESKILLPOINT: return "Give skill point";
	case ACTION_ANTIHOOK: return "Anti hook";
	case ACTION_ANTIDMG: return "Anti damage";
	case ACTION_KICK: return "Kick";
	case ACTION_MUTE: return "Mute";
	case ACTION_SETTITLE: return "Set title (NOT WORKING)";
	case ACTION_GIVELEVEL: return "Set level";
	case ACTION_GIVEEXP: return "Give exp";
	case ACTION_GOTO: return "Goto";
	case ACTION_SETWALLLIMIT: return "Set wall limit";
	case ACTION_INFMANA: return "Infinitie mana";
	default: return "FUCK";
	}
}

/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <new>
#include <iostream>
#include <engine/shared/config.h>
#include "player.h"
#include <engine/shared/network.h>
#include <engine/server.h>
#include <game/server/gamecontext.h>
#include <engine/server/multi_world.h>

#include <game/server/entities/bots/monster.h>
#include <game/server/entities/bots/npcs.h>
#include <game/server/entities/bots/npcsw.h>
#include <game/server/entities/bots/kwah.h>
#include <game/server/entities/bots/boomer.h>
#include <game/server/entities/bots/bossslime.h>

//MACRO_ALLOC_POOL_ID_IMPL(CPlayer, MAX_CLIENTS * NUM_WORLDS + MAX_CLIENTS)

IServer *CPlayer::Server() const { return m_pGameServer->Server(); }

CPlayer::CPlayer(CGameContext *pGameServer, int ClientID, int Team)
{
	m_pGameServer = pGameServer;
	m_RespawnTick = Server()->Tick();
	m_DieTick = Server()->Tick();
	m_ScoreStartTick = Server()->Tick();

	m_pCharacter = 0;
	m_ClientID = ClientID;
	m_Team = Team;
	m_SpectatorID = SPEC_FREEVIEW;
	m_LastActionTick = Server()->Tick();

	m_Bot = (ClientID >= MAX_PLAYERS);
	m_BotType = m_BotSubType = m_SelectItem = m_SelectArmor = -1;

	m_Authed = AUTHED_NO;
	int *pIdMap = Server()->GetIdMap(m_ClientID);
	for (int i = 1; i < VANILLA_MAX_CLIENTS; i++)
	{
		pIdMap[i] = -1;
	}
	pIdMap[0] = m_ClientID;

	m_MapMenu = 0;
	m_MapMenuItem = -1;
	m_MapMenuTick = -1;

	m_Rainbow = false;
	m_RainbowColor = 0;
	m_Bloody = false;

	m_LastPos = vec2(0, 0);
	m_EndMuteTick = 0;

	m_WallLimit = 5;

	m_InfMana = false;

	m_PrevTuningParams = *pGameServer->Tuning();
	m_NextTuningParams = m_PrevTuningParams;

	m_MoneyAddEndTick = m_ExperienceAddEndTick = m_InviteClanID = m_Mana = 0;
	m_Search = m_BigBot = m_InArea = m_IsInGame = m_InBossed = false;

	AccData.Level = AccUpgrade.SkillPoint = AccUpgrade.Upgrade = AccUpgrade.Speed = AccUpgrade.Health = AccUpgrade.Damage = -1;
	AccData.Exp = AccData.Money = AccData.Rel = AccData.Gold = -1;
	AccData.Class = PLAYERCLASS_NONE;

	m_AcceptedDailyQuestID = 0;

	m_pChatCmd = new CCmd(this, m_pGameServer);
	SetLanguage(Server()->GetClientLanguage(ClientID));
}

CPlayer::~CPlayer()
{
	delete m_pChatCmd;
	m_pChatCmd = 0;

	delete m_pCharacter;
	m_pCharacter = 0;
}

bool CPlayer::GetShop()
{
	if(m_pCharacter && m_pCharacter->InShop)
		return true;

	return false;
}

bool CPlayer::GetWork()
{
	if(m_pCharacter && m_pCharacter->InWork)
		return true;

	return false;
}

void CPlayer::BasicAuthedTick()
{
	if(tickstr)
	{
		tickstr--;
		if(tickstr == 1)
		{
			tickstr = 15;
			int sz = strlen(pTitle);
			if(sz-sz+10 <= sz-1)
			{
				memmove(pTitle, pTitle + 1, sz - 1);
				pTitle[sz - 1] = '\0';

				if(sz-sz+10 >= sz-1)
					tickstr = 90;
			}
			else
				SetMoveChar();
		}
	}

	if(Server()->GetItemCount(m_ClientID, PIGPORNO) >= 50 && !Server()->GetItemCount(m_ClientID, PIGPIG))
		GameServer()->GiveItem(m_ClientID, PIGPIG, 1);

	if(AccData.Money >= 10000)
	{
		AccData.Gold += AccData.Money/10000;
		int Got = (int)(AccData.Money/10000);

		AccData.Money -= Got*10000;
	}

	if(AccData.Exp >= AccData.Level*GetNeedForUp())
	{
		GameServer()->SendChatTarget_Localization(m_ClientID, CHATCATEGORY_DEFAULT, _("[Level UP] Added Skill and Upgrade points."), NULL);
		AccData.Exp -= AccData.Level*GetNeedForUp();
		AccData.Level++;
		AccUpgrade.SkillPoint += 1;
		AccUpgrade.Upgrade += 1;

		int GetBag = Server()->GetItemCount(m_ClientID, AMULETCLEEVER) ? 2 : 1;
		GameServer()->GiveItem(m_ClientID, MONEYBAG, GetBag);
		if(AccData.Level % 10 == 0)
			GameServer()->SendMail(m_ClientID, "You got bonus every 10 level!", RANDOMCRAFTITEM, 3);
		if(AccData.Level == 2)
			GameServer()->SendChatTarget_Localization(m_ClientID, CHATCATEGORY_DEFAULT, _("You have opened quest in vote quest."), NULL);
		if(m_pCharacter)
		{
			GameServer()->CreateLolText(m_pCharacter, false, vec2(0, -75), vec2(0, -1), 50, "Level ++");
			GameServer()->CreateDeath(m_pCharacter->m_Pos, m_ClientID);
		}
		GameServer()->CreateSoundGlobal(SOUND_CTF_CAPTURE, m_ClientID);

		if (m_AcceptedDailyQuestID)
		{
			if (GameServer()->m_CurrentDailyQuest1 == QUEST_UP7LEVELS)
				m_CompleteDailyStep1 += 1;
			if (GameServer()->m_CurrentDailyQuest2 == QUEST_UP7LEVELS)
				m_CompleteDailyStep2 += 1;
			if (GameServer()->m_CurrentDailyQuest3 == QUEST_UP7LEVELS)
				m_CompleteDailyStep3 += 1;
		}

		GameServer()->UpdateUpgrades(m_ClientID);
		GameServer()->UpdateStats(m_ClientID);
	}
}

void CPlayer::Tick()
{
#ifdef CONF_DEBUG
	if(!g_Config.m_DbgDummies || m_ClientID < MAX_CLIENTS-g_Config.m_DbgDummies)
#endif
	if(!GameServer()->IsClientValid(m_ClientID))
		return;

	if(Server()->IsClientLogged(m_ClientID) && AccData.Level == -1)
	{
		AccData.Level = 1;
		if(!Server()->GetSeccurity(m_ClientID))
			GameServer()->SendChatTarget_Localization(m_ClientID, CHATCATEGORY_DEFAULT, _("Your account is at risk, set security"), NULL);
	}
	
	// Мана сучка ебал вас геи ебанные в рт вы шлюхи
	if (m_InfMana)
		m_Mana = 100;

	if (m_Mana < GetNeedMana())
	{
		if (!m_ManaTick)
		{
			m_Mana++;
			m_ManaTick = 10;
			GameServer()->SendBroadcast_LStat(m_ClientID, 2, 50, -1);
		}
		else m_ManaTick--;
	}

	// Снимаем ангру
	if(m_AngryWroth && Server()->Tick() % (1 * Server()->TickSpeed() * 20) == 0)
	{
		if(m_AngryWroth < 20)
			m_AngryWroth = 0;
		else
			m_AngryWroth -= 20;
	}

	// ОПЫТ КНИГИ ДОБАВКА НУЖНО ОПТИМИЗИРОВАТЬ
	if(m_MoneyAddEndTick > Server()->Tick())
	{
		if(Server()->Tick() % (Server()->TickSpeed() * 120) == 0)
		{
			int Time = (m_MoneyAddEndTick - Server()->Tick()) / Server()->TickSpeed() / 60;
			GameServer()->SendChatTarget_Localization(m_ClientID, CHATCATEGORY_DEFAULT, _("Item's ending {str:name} after {int:ends} min."), "name", Server()->GetItemName(m_ClientID, BOOKMONEYMIN), "ends", &Time, NULL);
		}
		if(m_MoneyAddEndTick - Server()->Tick() == 1)
			GameServer()->SendChatTarget_Localization(m_ClientID, CHATCATEGORY_DEFAULT, _("Item's ended {str:name}"), "name", Server()->GetItemName(m_ClientID, BOOKMONEYMIN), NULL);
	}
	if (m_ExperienceAddEndTick > Server()->Tick())
	{
		if (Server()->Tick() % (Server()->TickSpeed() * 120) == 0)
		{
			int Time = (m_ExperienceAddEndTick - Server()->Tick()) / Server()->TickSpeed() / 60;
			GameServer()->SendChatTarget_Localization(m_ClientID, CHATCATEGORY_DEFAULT, _("Item's ending {str:name} after {int:ends} min."), "name", Server()->GetItemName(m_ClientID, BOOKMONEYMIN), "ends", &Time, NULL);
		}
		if (m_ExperienceAddEndTick - Server()->Tick() == 1)
			GameServer()->SendChatTarget_Localization(m_ClientID, CHATCATEGORY_DEFAULT, _("Item's ended {str:name}"), "name", Server()->GetItemName(m_ClientID, BOOKMONEYMIN), NULL);
	}

	// Уровни и все такое повышение
	if(Server()->IsClientLogged(m_ClientID) && GetTeam() != TEAM_SPECTATORS)
	{
		if(g_Config.m_SvEventSchool)
		{
			if(Server()->Tick() % (1 * Server()->TickSpeed() * 600) == 0)
			{
				int Type;
				switch(rand()%5)
				{
					case 1: Type = COOPERPIX; break;
					case 2: Type = WOOD; break;
					case 3: Type = BOOKEXPMIN; break;
					default: Type = EVENTCUSTOMSOUL;
				}

				GameServer()->SendMail(m_ClientID, "Event Back to School you got item!", Type, 1);
				GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_DEFAULT, _("[Back to School] {str:name} got {str:item}."), "name", Server()->ClientName(m_ClientID), "item", Server()->GetItemName(m_ClientID, Type), NULL);

				if(Server()->GetItemCount(m_ClientID, EVENTCUSTOMSOUL) >= 25)
				{
					Server()->RemItem(m_ClientID, EVENTCUSTOMSOUL, 25, -1);
					GameServer()->SendMail(m_ClientID, "Soul automatic changes Custom Skin!", CUSTOMSKIN, 1);
					GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_DEFAULT, _("[Back to School] {str:name} Collect 25 Soul & get Custom Skin."), "name", Server()->ClientName(m_ClientID), NULL);
				}
			}
		}
		BasicAuthedTick();
	}

	// Агресия и тюрьма
	if(!m_Search && AccData.Rel >= 1000)
	{
		//m_Search = true;
		//GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_HEALER, _("Player {str:name}, in the wanted list"), "name", Server()->ClientName(m_ClientID), NULL);
	}
	if(m_JailTick && AccData.Jail)
	{
		int Time = m_JailTick/Server()->TickSpeed();
		GameServer()->SendBroadcast_Localization(m_ClientID, 100, 100, _("You in jail, wait {sec:siska}."), "siska", &Time, NULL);

		m_JailTick--;
		if(!m_JailTick)
		{
			m_JailTick = 0;
			AccData.Jail = false;

			if(m_pCharacter)
				m_pCharacter->Die(m_ClientID, WEAPON_WORLD);

			GameServer()->UpdateStats(m_ClientID);
			GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_HEALER, _("The player {str:name}, leaves the prison"), "name", Server()->ClientName(m_ClientID), NULL);
		}
	}
	if(GetTeam() != TEAM_SPECTATORS && AccData.Rel > 0 && Server()->Tick() % (1 * Server()->TickSpeed() * 60) == 0)
	{
		AccData.Rel -= 100;
		if(AccData.Rel < 0)
			AccData.Rel = 0;

		if(AccData.Rel == 0 && m_Search)
		{
			m_Search = false;
			GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_HEALER, _("Player {str:name}, crosed out in wanted list"), "name", Server()->ClientName(m_ClientID), NULL);
		}
		GameServer()->SendBroadcast_Localization(m_ClientID, BROADCAST_PRIORITY_GAMEANNOUNCE, BROADCAST_DURATION_GAMEANNOUNCE, _("Relations angry -100. Your {int:rel}"), "rel", &AccData.Rel, NULL);
		GameServer()->UpdateStats(m_ClientID);
	}

	// вывод текста АРЕНА
	if(m_InArea)
	{
		if(GameServer()->m_AreaStartTick)
		{
			int Time = GameServer()->m_AreaStartTick/Server()->TickSpeed();
			GameServer()->SendBroadcast_Localization(m_ClientID, 101, 100, _("Wait start area {int:siska} sec."), "siska", &Time, NULL);

			if(GameServer()->m_AreaStartTick == 100)
				GameServer()->SendBroadcast_Localization(m_ClientID, 105, 100, _("Fight!!."), NULL);
		}
		else if(GameServer()->m_AreaEndGame)
		{
			int Time = GameServer()->m_AreaEndGame/Server()->TickSpeed();
			int couns = GameServer()->GetAreaCount();
			GameServer()->SendBroadcast_Localization(m_ClientID, 102, 100, _("End area {int:siska} sec. Life {int:num} player's"), "siska", &Time, "num", &couns, NULL);
		}
	}

	// вывод текста по поводу ожидания времени босса
	if(m_InBossed)
	{
		if(GameServer()->m_WinWaitBoss)
		{
			int Time = GameServer()->m_WinWaitBoss/Server()->TickSpeed();
			GameServer()->SendBroadcast_Localization(m_ClientID, 101, 100, _("Wait players for pick item's {int:siska} sec."), "siska", &Time, NULL);
		}
		else if(GameServer()->m_BossStartTick > 10*Server()->TickSpeed())
		{
			int Time = GameServer()->m_BossStartTick/Server()->TickSpeed();
			GameServer()->SendBroadcast_Localization(m_ClientID, 101, 100, _("Wait players for raid {sec:siska}. Boss: {str:name}"), "siska", &Time, "name", GameServer()->GetBossName(GameServer()->m_BossType), NULL);
		}
		else if(Server()->Tick() % (1 * Server()->TickSpeed()) == 0 && GameServer()->m_BossStartTick > 100)
			GameServer()->SendGuide(m_ClientID, GameServer()->m_BossType);
		else if(GameServer()->m_BossStart)
			GameServer()->SendBroadcast_LBossed(m_ClientID, 250, 100);
	}

	// таймер синхронизации
	if(m_LoginSync)
	{
		m_LoginSync--;
		if(!m_LoginSync)
		{
			if(Server()->IsClientLogged(m_ClientID))
			{
				if(Server()->GetClanID(m_ClientID) > 0)
					Server()->UpdClanCount(Server()->GetClanID(m_ClientID));

				GameServer()->ResetVotes(m_ClientID, AUTH);
			}
		}
	}
	if(!Server()->IsClientLogged(m_ClientID))
		m_Team = TEAM_SPECTATORS;

	if(m_MapMenu > 0)
		m_MapMenuTick++;

	// Rainbow
	if (m_Rainbow)
		m_RainbowColor = (m_RainbowColor + 1) % 256;

	if (m_Bloody && Server()->Tick() % 10 == 0 && m_pCharacter)
		GameServer()->CreateDeath(m_pCharacter->m_Pos, m_ClientID);

	// do latency stuff
	{
		IServer::CClientInfo Info;
		if(Server()->GetClientInfo(m_ClientID, &Info))
		{
			m_Latency.m_Accum += Info.m_Latency;
			m_Latency.m_AccumMax = max(m_Latency.m_AccumMax, Info.m_Latency);
			m_Latency.m_AccumMin = min(m_Latency.m_AccumMin, Info.m_Latency);
		}
		// each second
		if(Server()->Tick()%Server()->TickSpeed() == 0)
		{
			m_Latency.m_Avg = m_Latency.m_Accum/Server()->TickSpeed();
			m_Latency.m_Max = m_Latency.m_AccumMax;
			m_Latency.m_Min = m_Latency.m_AccumMin;
			m_Latency.m_Accum = 0;
			m_Latency.m_AccumMin = 1000;
			m_Latency.m_AccumMax = 0;
		}
	}

	if(!GameServer()->m_World.m_Paused)
	{
		if(!m_pCharacter && m_Team == TEAM_SPECTATORS && m_SpectatorID == SPEC_FREEVIEW)
			m_ViewPos -= vec2(clamp(m_ViewPos.x-m_LatestActivity.m_TargetX, -500.0f, 500.0f), clamp(m_ViewPos.y-m_LatestActivity.m_TargetY, -400.0f, 400.0f));

		if(!m_pCharacter && m_DieTick+Server()->TickSpeed()*3 <= Server()->Tick())
			m_Spawning = true;

		if(m_pCharacter)
		{
			if(m_pCharacter->IsAlive())
			{
				m_ViewPos = m_pCharacter->m_Pos;
			}
			else
			{
				m_pCharacter->Destroy();
				delete m_pCharacter;
				m_pCharacter = 0;
			}
		}
		else if(m_Spawning && m_RespawnTick <= Server()->Tick())
			TryRespawn();
	}
	else
	{
		++m_RespawnTick;
		++m_DieTick;
		++m_ScoreStartTick;
		++m_LastActionTick;
	}
	HandleTuningParams();
}

int CPlayer::GetNeedForUp()
{
	int exp = 250;

	if(AccData.Level > 100) exp = 700;
	if(AccData.Level > 200) exp = 1000;
	if(AccData.Level > 300) exp = 1500;
	if(AccData.Level > 400) exp = 2500;
	if(AccData.Level > 500) exp = 4000;
	if(AccData.Level > 600) exp = 6000;
	if(AccData.Level > 700) exp = 8000;
	if(AccData.Level > 1000) exp = 12000;
	if(AccData.Level > 1100) exp = 13000;
	if(AccData.Level > 1200) exp = 14000;
	
	return exp;
}
int CPlayer::GetNeedForUpClan()
{
	int Get = Server()->GetClan(DLEVEL, Server()->GetClanID(m_ClientID));
	return 1000000 + Get * 1000000;
}
int CPlayer::GetNeedForUpgClan(int Type)
{
	int Get = Server()->GetClan(Type, Server()->GetClanID(m_ClientID));
	return 100 + Get * 500;
}

void CPlayer::PostTick()
{
	// update latency value
	if(m_PlayerFlags&PLAYERFLAG_SCOREBOARD)
	{
		for(int i = 0; i < MAX_CLIENTS; ++i)
		{
			if(GameServer()->m_apPlayers[i] && GameServer()->m_apPlayers[i]->GetTeam() != TEAM_SPECTATORS)
				m_aActLatency[i] = GameServer()->m_apPlayers[i]->m_Latency.m_Min;
		}
	}

	// update view pos for spectators
	if(m_Team == TEAM_SPECTATORS && m_SpectatorID != SPEC_FREEVIEW && GameServer()->m_apPlayers[m_SpectatorID])
		m_ViewPos = GameServer()->m_apPlayers[m_SpectatorID]->m_ViewPos;
}

void CPlayer::HandleTuningParams()
{
	if(!(m_PrevTuningParams == m_NextTuningParams))
	{
		if(m_IsReady)
		{
			CMsgPacker Msg(NETMSGTYPE_SV_TUNEPARAMS);
			int *pParams = (int *)&m_NextTuningParams;
			for(unsigned i = 0; i < sizeof(m_NextTuningParams)/sizeof(int); i++)
				Msg.AddInt(pParams[i]);
			Server()->SendMsg(&Msg, MSGFLAG_VITAL, GetCID());
		}
		m_PrevTuningParams = m_NextTuningParams;
	}
	m_NextTuningParams = *GameServer()->Tuning();
}

void CPlayer::MoneyAdd(int Size, bool ClanBonus, bool MoneyDouble)
{
	int GetMoney = Size;
	if(ClanBonus && Server()->GetClanID(m_ClientID))
		GetMoney += Server()->GetClan(DADDMONEY, Server()->GetClanID(m_ClientID)) * 100;
	GetMoney += Server()->GetItemCount(m_ClientID, ACCESSORY_ADD_MONEY) * 100;

	if(MoneyDouble)
	{
		if(Server()->GetItemCount(m_ClientID, PREMIUM_GOVNO))
			GetMoney += GetMoney * 3;
		else if(Server()->GetItemCount(m_ClientID, X2MONEYEXPVIP))
			GetMoney += GetMoney * 2;

		if(m_MoneyAddEndTick > Server()->Tick())
			GetMoney += GetMoney * 2;
	}

	if (Size >= 10000)
	{
		int Got = Size / 10000;
		AccData.Gold += Got;

		AccData.Money -= Got * 10000;
	}

	GameServer()->SendBroadcast_LStat(m_ClientID, BROADCAST_PRIORITY_GAMEANNOUNCE, 100, INADDMONEY, GetMoney);
	AccData.Money += GetMoney;
	if(rand()%8 == 1)
		GameServer()->UpdateStats(m_ClientID);

	GameServer()->ResetVotes(m_ClientID, AUTH);
	return;
}

void CPlayer::ExpAdd(int Size, bool Bonus)
{
	int GetExp = Size*5, Get = 0;
	int gete = GetExp;
	if(Bonus && Server()->GetClanID(m_ClientID))
	{
		Get = Size*100;
		Server()->InitClanID(Server()->GetClanID(m_ClientID), PLUS, "Exp", Get, true);
		GetExp = gete+Server()->GetClan(DADDEXP, Server()->GetClanID(m_ClientID));

	}

	int ml = 0;
	if (Bonus) {
		if (m_ExperienceAddEndTick > Server()->Tick())
			ml += 2;
		if (Server()->GetItemCount(m_ClientID, PREMIUM_GOVNO))
			ml += 2;
		else if (Server()->GetItemCount(m_ClientID, X2MONEYEXPVIP))
			ml += 2;
	}

	ml = (ml == 0) ? 1 : ml;

	GetExp = gete * ml;

	if (Server()->GetClanID(m_ClientID))
		GetExp += Server()->GetClan(DADDEXP, Server()->GetClanID(m_ClientID));

	if(Server()->GetClanID(m_ClientID) &&
		Server()->GetClan(DEXP, Server()->GetClanID(m_ClientID)) >= Server()->GetClan(DLEVEL, Server()->GetClanID(m_ClientID))*GetNeedForUpClan())
	{
		GameServer()->SendChatClan(Server()->GetClanID(m_ClientID), "[Clan Level UP] Great!");

		int warpminus = Server()->GetClan(DLEVEL, Server()->GetClanID(m_ClientID))*GetNeedForUpClan();
		Server()->InitClanID(Server()->GetClanID(m_ClientID), MINUS, "Exp", warpminus, true);
		Server()->InitClanID(Server()->GetClanID(m_ClientID), PLUS, "Level", 1, true);
	}
	
	GameServer()->SendBroadcast_LStat(m_ClientID, BROADCAST_PRIORITY_GAMEANNOUNCE, 100, Server()->GetClanID(m_ClientID) > 0 ? INADDCEXP : INADDEXP, GetExp, Get);
	AccData.Exp += GetExp;
	if(rand() % 4 == 1)
		GameServer()->UpdateStats(m_ClientID);

	return;
}

void CPlayer::Snap(int SnappingClient)
{
#ifdef CONF_DEBUG
	if(!g_Config.m_DbgDummies || m_ClientID < MAX_CLIENTS - g_Config.m_DbgDummies)
#endif
	if(!GameServer()->IsClientValid(m_ClientID))
		return;

	int id = m_ClientID;
	if (SnappingClient > -1 && !Server()->Translate(id, SnappingClient)) return;
	if (m_pCharacter && IsBot())
		if (m_pCharacter->NetworkClipped(SnappingClient))
			if (id > 63)
				id = id % 64;

	CNetObj_ClientInfo *pClientInfo = static_cast<CNetObj_ClientInfo *>(Server()->SnapNewItem(NETOBJTYPE_CLIENTINFO, id, sizeof(CNetObj_ClientInfo)));
	if(!pClientInfo)
		return;

	if (Server()->IsClientLogged(m_ClientID) && GetTeam() != TEAM_SPECTATORS)
	{
		char pSendName[32];
		str_format(pSendName, sizeof(pSendName), "%s", Server()->ClientName(m_ClientID));
		StrToInts(&pClientInfo->m_Name0, 4, pSendName);

		if (m_pCharacter)
		{
			float getlv = ((m_Health * 100.0) / m_HealthStart) - 1;
			switch (GetBotType())
			{
			default: str_format(pSendName, sizeof(pSendName), "%d:%s[%d%%]", AccData.Level, Server()->ClientName(m_ClientID), (int)getlv); break;
			case BOT_NPC:
			case BOT_BOSSSLIME: str_format(pSendName, sizeof(pSendName), "%s[%d%%]", Server()->ClientName(m_ClientID), (int)getlv);	break;
			case BOT_NPCW: str_format(pSendName, sizeof(pSendName), "%s", Server()->ClientName(m_ClientID)); break;
			}
			StrToInts(&pClientInfo->m_Name0, 4, pSendName);
		}
	}
	else
		StrToInts(&pClientInfo->m_Name0, 4, Server()->ClientName(m_ClientID));

	if(Server()->IsClientLogged(m_ClientID) && GetTeam() != TEAM_SPECTATORS)
	{
		char pSendName[32];
		str_format(pSendName, sizeof(pSendName), "%s", Server()->ClientName(m_ClientID));
		StrToInts(&pClientInfo->m_Name0, 4, pSendName);
	}
	else
		StrToInts(&pClientInfo->m_Name0, 4, Server()->ClientName(m_ClientID));

	if(Server()->IsClientLogged(m_ClientID) && tickstr) StrToInts(&pClientInfo->m_Clan0, 3, pTitle);
	else StrToInts(&pClientInfo->m_Clan0, 3, Server()->ClientClan(m_ClientID));

	pClientInfo->m_Country = Server()->ClientCountry(m_ClientID);

	StrToInts(&pClientInfo->m_Skin0, 6, m_TeeInfos.m_SkinName);
	if (!m_Rainbow)
	{
		pClientInfo->m_UseCustomColor = m_TeeInfos.m_UseCustomColor;
		pClientInfo->m_ColorBody = m_TeeInfos.m_ColorBody;
		pClientInfo->m_ColorFeet = m_TeeInfos.m_ColorFeet;
	}
	else {
		pClientInfo->m_UseCustomColor = 1;
		pClientInfo->m_ColorBody = m_RainbowColor * 0x010000 + 0xff00;
		pClientInfo->m_ColorFeet = m_RainbowColor * 0x010000 + 0xff00;
	}

	CNetObj_PlayerInfo *pPlayerInfo = static_cast<CNetObj_PlayerInfo *>(Server()->SnapNewItem(NETOBJTYPE_PLAYERINFO, id, sizeof(CNetObj_PlayerInfo)));
	if(!pPlayerInfo)
		return;

	pPlayerInfo->m_Latency = SnappingClient == -1 ? m_Latency.m_Min : GameServer()->m_apPlayers[SnappingClient]->m_aActLatency[m_ClientID];
	pPlayerInfo->m_Local = 0;
	pPlayerInfo->m_ClientID = id;

	pPlayerInfo->m_Score = AccData.Level;
	pPlayerInfo->m_Team = m_Team;

	if(m_ClientID == SnappingClient)
		pPlayerInfo->m_Local = 1;

	if(m_ClientID == SnappingClient && m_Team == TEAM_SPECTATORS)
	{
		CNetObj_SpectatorInfo *pSpectatorInfo = static_cast<CNetObj_SpectatorInfo *>(Server()->SnapNewItem(NETOBJTYPE_SPECTATORINFO, m_ClientID, sizeof(CNetObj_SpectatorInfo)));
		if(!pSpectatorInfo)
			return;

		pSpectatorInfo->m_SpectatorID = m_SpectatorID;
		pSpectatorInfo->m_X = m_ViewPos.x;
		pSpectatorInfo->m_Y = m_ViewPos.y;
	}
}

void CPlayer::FakeSnap(int SnappingClient)
{
	IServer::CClientInfo info;
	Server()->GetClientInfo(SnappingClient, &info);
	if (info.m_CustClt)
		return;

	int id = VANILLA_MAX_CLIENTS - 1;

	CNetObj_ClientInfo *pClientInfo = static_cast<CNetObj_ClientInfo *>(Server()->SnapNewItem(NETOBJTYPE_CLIENTINFO, id, sizeof(CNetObj_ClientInfo)));

	if(!pClientInfo)
		return;

	StrToInts(&pClientInfo->m_Name0, 4, " ");
	StrToInts(&pClientInfo->m_Clan0, 3, "");
	StrToInts(&pClientInfo->m_Skin0, 6, m_TeeInfos.m_SkinName);
}

void CPlayer::OnDisconnect(const char *pReason)
{
	GameServer()->ClearVotes(m_ClientID);
	KillCharacter();

	if(GameServer()->IsClientValid(m_ClientID))
		GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_DEFAULT, _("{str:PlayerName} has left the game {str:Reason}"), "PlayerName", Server()->ClientName(m_ClientID), NULL);
}

void CPlayer::OnPredictedInput(CNetObj_PlayerInput *NewInput)
{
	// skip the input if chat is active
	if((m_PlayerFlags & PLAYERFLAG_CHATTING) && (NewInput->m_PlayerFlags & PLAYERFLAG_CHATTING))
		return;

	if(m_pCharacter)
		m_pCharacter->OnPredictedInput(NewInput);
}

void CPlayer::OnDirectInput(CNetObj_PlayerInput *NewInput)
{
	if(NewInput->m_PlayerFlags&PLAYERFLAG_CHATTING)
	{
		// skip the input if chat is active
		if(m_PlayerFlags&PLAYERFLAG_CHATTING)
			return;

		// reset input
		if(m_pCharacter)
			m_pCharacter->ResetInput();

		m_PlayerFlags = NewInput->m_PlayerFlags;
		return;
	}

	m_PlayerFlags = NewInput->m_PlayerFlags;

	if(m_pCharacter)
		m_pCharacter->OnDirectInput(NewInput);

	if(!m_pCharacter && m_Team != TEAM_SPECTATORS && (NewInput->m_Fire&1))
		m_Spawning = true;

	// check for activity
	if(NewInput->m_Direction || m_LatestActivity.m_TargetX != NewInput->m_TargetX ||
		m_LatestActivity.m_TargetY != NewInput->m_TargetY || NewInput->m_Jump ||
		NewInput->m_Fire&1 || NewInput->m_Hook)
	{
		m_LatestActivity.m_TargetX = NewInput->m_TargetX;
		m_LatestActivity.m_TargetY = NewInput->m_TargetY;
		m_LastActionTick = Server()->Tick();
	}
}

CCharacter *CPlayer::GetCharacter()
{
	if(m_pCharacter && m_pCharacter->IsAlive())
		return m_pCharacter;
	return 0;
}

void CPlayer::KillCharacter(int Weapon)
{
	if(m_pCharacter)
	{
		if(m_ClientID != 63)
			m_pCharacter->Die(m_ClientID, Weapon);

		delete m_pCharacter;
		m_pCharacter = 0;
	}
}

void CPlayer::Respawn()
{
	if(m_Team != TEAM_SPECTATORS)
		m_Spawning = true;
}

void CPlayer::SetTeam(int Team, bool DoChatMsg)
{
	// clamp the team
	Team = GameServer()->m_pController->ClampTeam(Team);
	if(DoChatMsg)
		GameServer()->SendChatTarget_Localization(-1, -1, _("{str:PlayerName} joined the RPG Azataz"), "PlayerName", Server()->ClientName(m_ClientID), NULL);

	KillCharacter();

	m_Team = Team;
	m_LastActionTick = Server()->Tick();
	m_SpectatorID = SPEC_FREEVIEW;

	// we got to wait 0.5 secs before respawning
	m_RespawnTick = Server()->Tick()+Server()->TickSpeed()/2;
	GameServer()->m_pController->OnPlayerInfoChange(GameServer()->m_apPlayers[m_ClientID]);

	if(Team == TEAM_SPECTATORS)
	{
		// update spectator modes
		for(int i = 0; i < MAX_CLIENTS; ++i)
		{
			if(GameServer()->m_apPlayers[i] && GameServer()->m_apPlayers[i]->m_SpectatorID == m_ClientID)
				GameServer()->m_apPlayers[i]->m_SpectatorID = SPEC_FREEVIEW;
		}
	}
}

void CPlayer::TryRespawn()
{
	vec2 SpawnPos;
	if(!GameServer()->m_pController->PreSpawn(this, &SpawnPos))
		return;

	m_Spawning = false;

	if (IsBot())
	{
		m_BigBot = false;
		if (rand() % 20 == 10) m_BigBot = true;

		GameServer()->UpdateBotInfo(m_ClientID);

		int Level = 0;
		int Health = 0;
		int Damage = 0;

		switch (GetBotType())
		{
		case BOT_L1MONSTER:
			m_pCharacter = new(m_ClientID) CMonster(&GameServer()->m_World);
			Level = m_BigBot ? 15 + rand() % 3 : 10;
			Health = 1;
			if (m_BigBot)
			{
				Server()->SetMaxAmmo(m_ClientID, INFWEAPON_GUN, 10);
				Server()->SetAmmoRegenTime(m_ClientID, INFWEAPON_GUN, 100);
				Server()->SetFireDelay(m_ClientID, INFWEAPON_GUN, 800);
			}
			break;
		case BOT_L2MONSTER:
			m_pCharacter = new(m_ClientID) CKwah(&GameServer()->m_World);
			Level = m_BigBot ? 100 : 85 + rand() % 3;
			Health = Level;
			Damage = Level / 2;
			break;
		case BOT_L3MONSTER:
			m_pCharacter = new(m_ClientID) CBoomer(&GameServer()->m_World);
			Level = m_BigBot ? 170 + rand() % 3 : 130 + rand() % 3;
			Health = 50 + Level;
			Damage = Level;
			break;
		case BOT_L4MONSTER:
			m_pCharacter = new(m_ClientID) CMonster(&GameServer()->m_World);
			Level = m_BigBot ? 230 + rand() % 3 : 170 + rand() % 3;
			Health = 60 + Level;
			Damage = Level;
			break;
		case BOT_L5MONSTER:
			m_pCharacter = new(m_ClientID) CKwah(&GameServer()->m_World);
			Level = m_BigBot ? 310 + rand() % 3 : 270 + rand() % 3;
			Health = 70 + Level;
			Damage = Level;
			break;
		case BOT_BOSSSLIME:
			m_pCharacter = new(m_ClientID) CBossSlime(&GameServer()->m_World);
			Level = 300 + rand() % 20;
			Health = Level;
			Damage = 10;

			m_BigBot = true;
			break;
		case BOT_NPC:
			m_pCharacter = new(m_ClientID) CNpcSold(&GameServer()->m_World);
			Level = 500 + rand() % 20;
			Health = Level * 500;
			Damage = Level * 10;

			m_BigBot = true;
			break;
		}

		AccData.Level = Level;
		AccUpgrade.Health = Health;
		AccUpgrade.Damage = Damage;
	} else
		m_pCharacter = new(m_ClientID) CCharacter(&GameServer()->m_World);

	m_pCharacter->Spawn(this, SpawnPos);
	if(GetClass() != PLAYERCLASS_NONE)
		GameServer()->CreatePlayerSpawn(SpawnPos);
}

int CPlayer::GetClass()
{
	return AccData.Class;
}

void CPlayer::SetClassSkin(int newClass, int State)
{
	switch(newClass)
	{
		case PLAYERCLASS_ASSASINS:
			m_TeeInfos.m_UseCustomColor = 0;
			str_copy(m_TeeInfos.m_SkinName, "bluekitty", sizeof(m_TeeInfos.m_SkinName));
			break;
		case PLAYERCLASS_BERSERK:
			m_TeeInfos.m_UseCustomColor = 0;
			str_copy(m_TeeInfos.m_SkinName, "coala", sizeof(m_TeeInfos.m_SkinName));
			break;
		case PLAYERCLASS_HEALER:
			m_TeeInfos.m_UseCustomColor = 0;
			str_copy(m_TeeInfos.m_SkinName, "redstripe", sizeof(m_TeeInfos.m_SkinName));
			break;
		default:
			m_TeeInfos.m_UseCustomColor = 0;
			str_copy(m_TeeInfos.m_SkinName, "default", sizeof(m_TeeInfos.m_SkinName));
	}
}

void CPlayer::SetClass(int newClass)
{
	if(AccData.Class == newClass)
		return;

	AccData.Class = newClass;
	SetClassSkin(newClass);

	if(m_pCharacter)
		m_pCharacter->SetClass(newClass);
}

bool CPlayer::IsKownClass(int c)
{
	return m_knownClass[c];
}

const char* CPlayer::GetLanguage()
{
	return m_aLanguage;
}

const char* CPlayer::GetClassName()
{
	if(AccData.Class == PLAYERCLASS_ASSASINS)
		return "Assasin";
	else if(AccData.Class == PLAYERCLASS_BERSERK)
		return "Berserk";
	else if(AccData.Class == PLAYERCLASS_HEALER)
		return "Healer";
	else
		return "You bitch";
}

void CPlayer::SetLanguage(const char* pLanguage)
{
	str_copy(m_aLanguage, pLanguage, sizeof(m_aLanguage));
}

void CPlayer::OpenMapMenu(int Menu)
{
	m_MapMenu = Menu;
	m_MapMenuTick = 0;
}

void CPlayer::CloseMapMenu()
{
	m_MapMenu = 0;
	m_MapMenuTick = -1;
}

bool CPlayer::MapMenuClickable()
{
	return (m_MapMenu > 0 && (m_MapMenuTick > Server()->TickSpeed()/2));
}

void CPlayer::ResetUpgrade(int ClientID)
{
	if(Server()->IsClientLogged(m_ClientID))
	{
		int Back = AccUpgrade.Speed + AccUpgrade.Health + AccUpgrade.HPRegen + AccUpgrade.Mana + AccUpgrade.AmmoRegen + (AccUpgrade.Damage*3 + AccUpgrade.Ammo*5 + AccUpgrade.Spray*10);
		AccUpgrade.Speed = AccUpgrade.Health = AccUpgrade.Damage = AccUpgrade.HPRegen = AccUpgrade.Mana = 0;
		AccUpgrade.AmmoRegen = AccUpgrade.Ammo = AccUpgrade.Spray = 0;

		AccUpgrade.Upgrade += Back;
		GameServer()->UpdateUpgrades(ClientID);
	}
}

void CPlayer::ResetSkill(int ClientID)
{
	if(Server()->IsClientLogged(m_ClientID))
	{
		int Back = AccUpgrade.HammerRange*15 + AccUpgrade.Pasive2*15;
		AccUpgrade.Pasive2 = AccUpgrade.HammerRange = 0;
		AccUpgrade.SkillPoint += Back;
		GameServer()->UpdateUpgrades(ClientID);
	}
}

const char* CPlayer::TitleGot()
{
	// Admins
	if (m_Authed == AUTHED_ADMIN)
		return "_[ADMIN]_";
	if (m_Authed == AUTHED_MOD)
		return "_[MODER]_";

	if (Server()->GetItemSettings(m_ClientID, PREMIUM_GOVNO))
		return "_[PREM]_";
	else if (Server()->GetItemSettings(m_ClientID, X2MONEYEXPVIP))
		return "_[VIP]_";
	else if (Server()->GetItemSettings(m_ClientID, TITLEQUESTS))
		return "1LVQuests";
	else if (Server()->GetItemSettings(m_ClientID, BOSSDIE))
		return "_BossKiller_";
	else if (Server()->GetItemSettings(m_ClientID, PIGPIG))
		return "_Piggie_";
	else if (Server()->GetItemSettings(m_ClientID, BIGCRAFT))
		return "_Crafter_";
	else if (Server()->GetItemSettings(m_ClientID, TITLESUMMER))
		return "_I<3Summer";
	else if (Server()->GetItemSettings(m_ClientID, TITLEENCHANT))
		return "Enchant+10";
	else
		return "_Newbie_";
}

void CPlayer::SetMoveChar()
{
	char aBuf[64];
	str_format(aBuf, sizeof(aBuf), "%s | %s", Server()->ClientClan(m_ClientID), TitleGot());
	str_copy(pTitle, aBuf, sizeof(pTitle));
	tickstr = 90;
}

int CPlayer::GetBonusExp(int ExpCount) {
	return ExpCount + (ExpCount / 100);
}

int CPlayer::GetBonusMoney(int MoneyCount) {
	return MoneyCount + (MoneyCount / 100);
}

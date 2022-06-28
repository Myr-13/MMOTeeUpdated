/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/generated/protocol.h>
#include <engine/shared/config.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>

#include "pickup.h"

CPickup::CPickup(CGameWorld *pGameWorld, int Type, vec2 Pos, int SubType)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_PICKUP)
{
	m_Type = Type;
	m_Pos = Pos;
	m_SubType = SubType;
	m_Drop = 0;

	if (m_SubType == 6) {
		m_MoveDirection = vec2(1.5f, 0);
	}

	Reset();

	GameWorld()->InsertEntity(this);
}

void CPickup::Reset()
{
	if (g_pData->m_aPickups[m_Type].m_Spawndelay > 0)
		m_SpawnTick = Server()->Tick() + Server()->TickSpeed() * g_pData->m_aPickups[m_Type].m_Spawndelay;
	else
		m_SpawnTick = -1;
}

void CPickup::Tick()
{
	// wait for respawn
	if(m_SpawnTick > 0)
	{
		if(Server()->Tick() > m_SpawnTick)
		{
			// respawn
			m_SpawnTick = -1;

			if(m_Type == WEAPON_GRENADE || m_Type == WEAPON_SHOTGUN || m_Type == WEAPON_RIFLE)
				GameServer()->CreateSound(m_Pos, SOUND_WEAPON_SPAWN);
		}
		else
			return;
	}

	// Find other players
	for(CCharacter *p = (CCharacter*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_CHARACTER); p; p = (CCharacter *)p->TypeNext())
	{
		if(!p->GetPlayer()->IsBot() && distance(p->m_Pos, m_Pos) < 20) 
		{
			switch (m_Type)
			{
				case 0:
					if(m_SubType != 3 && m_SubType != 4 && m_SubType != 5 && m_SubType != 6 && m_SubType != 7)
					{
						GameServer()->CreateSound(m_Pos, SOUND_PICKUP_HEALTH);
						Picking(0);
					}
					break;

				case 1:
					//Picked = true;
					break;

				case 3:
					Picking(0);
					break;

				case 2:
					Picking(0);
					break;

				case 4:
					Picking(0);
					break;

				case 5:
					Picking(0);
					break;

				case 6:
					Picking(0);
					break;

				case 7:
					Picking(0);

				default:
					break;
			};

		}
	}

	// Двигаем рыбок
	vec2 NextPos = m_Pos + m_MoveDirection;
	if (GameServer()->Collision()->CheckPoint(NextPos)) {
		m_MoveDirection = vec2(-m_MoveDirection.x, -m_MoveDirection.y);
	}

	m_Pos += m_MoveDirection;
}

void CPickup::Picking(int Time)
{
	int RespawnTime = g_pData->m_aPickups[m_Type].m_Respawntime+Time;
	if(RespawnTime >= 0)
		m_SpawnTick = Server()->Tick() + Server()->TickSpeed() * RespawnTime;
}

void CPickup::StartFarm(int ClientID)
{
	CPlayer *pFarm = GameServer()->m_apPlayers[ClientID];
	if(!pFarm || !pFarm->GetCharacter() || m_SpawnTick != -1 || pFarm->IsBot())
		return;

	if(!m_SubType) // ########################### FARMING
	{
		m_Drop += 20;
		GameServer()->CreateSound(m_Pos, 20); 

		int LevelItem = 1+Server()->GetItemCount(ClientID, FARMLEVEL)/g_Config.m_SvFarmExp;
		int NeedExp = LevelItem*g_Config.m_SvFarmExp;
		int Exp = Server()->GetItemCount(ClientID, FARMLEVEL);

		float getlv = (m_Drop*100.0)/100;
		const char *Pick = GameServer()->LevelString(100, (int)getlv, 10, ':', ' ');
		GameServer()->SendBroadcast_Localization(ClientID, 1000, 100, _("Job Farming: {int:lvl} Level : {int:exp}/{int:expneed}EXP\nBonus: 1+{int:bonus} Item : Pick: {str:got} / {int:gotp}%"), 
			"lvl", &LevelItem, "exp", &Exp, "expneed", &NeedExp, "bonus", &LevelItem, "got", Pick, "gotp", &m_Drop, NULL);
		delete Pick;

		if(m_Drop >= 100)
		{
			if(rand()%2 == 0) Server()->SetMaterials(2, Server()->GetMaterials(2)+1);

			switch(rand()%5)
			{
				case 0:	GameServer()->GiveItem(ClientID, TOMATE, LevelItem); break; 
				case 1: GameServer()->GiveItem(ClientID, POTATO, LevelItem); break;
				default: 
					GameServer()->GiveItem(ClientID, CARROT, LevelItem);
					if (GameServer()->m_apPlayers[ClientID]->m_AcceptedDailyQuestID)
					{
						if (GameServer()->m_CurrentDailyQuest1 == QUEST_COLLECT200CARROTS)
							GameServer()->m_apPlayers[ClientID]->m_CompleteDailyStep1 += LevelItem;
						if (GameServer()->m_CurrentDailyQuest2 == QUEST_COLLECT200CARROTS)
							GameServer()->m_apPlayers[ClientID]->m_CompleteDailyStep2 += LevelItem;
						if (GameServer()->m_CurrentDailyQuest3 == QUEST_COLLECT200CARROTS)
							GameServer()->m_apPlayers[ClientID]->m_CompleteDailyStep3 += LevelItem;
					} break;
			}
			if(Server()->GetItemCount(ClientID, FARMLEVEL) % g_Config.m_SvFarmExp == 0)
			{
				GameServer()->SendChatTarget_Localization(ClientID, -1, _("~ Farm Level UP +1 got Farm item"), NULL);
				GameServer()->GiveItem(ClientID, FARMBOX, 1);
			}
			if(rand()%120 == 0)
				GameServer()->GiveItem(ClientID, FARMBOX, 1);

			GameServer()->GiveItem(ClientID, FARMLEVEL, 1);
		}
	}
	else if(m_SubType == 2) // ########################### MINER
	{
		int Dropable = 0;
		int Broke = 0;
		int Count = 0;
		const char* ItemName = "Nope";
		/*if (Server()->GetItemCount(ClientID, MITHRILPIX))
		{
			Count = Server()->GetItemCount(ClientID, MITHRILPIX);
			Broke = 2000 * Server()->GetItemCount(ClientID, MITHRILPIX);
			Dropable = Server()->GetItemSettings(ClientID, MITHRILPIX);
			if (!Dropable)
			{
				Server()->RemItem(ClientID, MITHRILPIX, Server()->GetItemCount(ClientID, MITHRILPIX), -1);
				GameServer()->SendChatTarget_Localization(ClientID, -1, _("~ Miner: {str:name} broke"), "name", Server()->GetItemName(ClientID, MITHRILPIX), NULL);
			}
			Server()->SetItemSettingsCount(ClientID, MITHRILPIX, Dropable - 1);
			ItemName = Server()->GetItemName(ClientID, MITHRILPIX);
			m_Drop += 50;
		}
		if (Server()->GetItemCount(ClientID, DRAGONPIX))
		{
			Count = Server()->GetItemCount(ClientID, DRAGONPIX);
			Broke = 1000 * Server()->GetItemCount(ClientID, DRAGONPIX);
			Dropable = Server()->GetItemSettings(ClientID, DRAGONPIX);
			if (!Dropable)
			{
				Server()->RemItem(ClientID, DRAGONPIX, Server()->GetItemCount(ClientID, DRAGONPIX), -1);
				GameServer()->SendChatTarget_Localization(ClientID, -1, _("~ Miner: {str:name} broke"), "name", Server()->GetItemName(ClientID, DRAGONPIX), NULL);
			}
			Server()->SetItemSettingsCount(ClientID, DRAGONPIX, Dropable - 1);
			ItemName = Server()->GetItemName(ClientID, DRAGONPIX);
			m_Drop += 50;
		}*/
		if(Server()->GetItemCount(ClientID, DIAMONDPIX))
		{
			Count = Server()->GetItemCount(ClientID, DIAMONDPIX);
			Broke = 699*Server()->GetItemCount(ClientID, DIAMONDPIX);
			Dropable = Server()->GetItemSettings(ClientID, DIAMONDPIX);
			if(!Dropable)
			{
				Server()->RemItem(ClientID, DIAMONDPIX, Server()->GetItemCount(ClientID, DIAMONDPIX), -1);
				GameServer()->SendChatTarget_Localization(ClientID, -1, _("~ Miner: {str:name} broke"), "name", Server()->GetItemName(ClientID, DIAMONDPIX), NULL);
			}
			Server()->SetItemSettingsCount(ClientID, DIAMONDPIX, Dropable-1);
			ItemName = Server()->GetItemName(ClientID, DIAMONDPIX);
			m_Drop += 35;	
		}
		else if(Server()->GetItemCount(ClientID, GOLDPIX))
		{
			Count = Server()->GetItemCount(ClientID, GOLDPIX);
			Broke = 491*Server()->GetItemCount(ClientID, GOLDPIX);
			Dropable = Server()->GetItemSettings(ClientID, GOLDPIX);
			if(!Dropable)
			{
				Server()->RemItem(ClientID, GOLDPIX, Server()->GetItemCount(ClientID, GOLDPIX), -1);
				GameServer()->SendChatTarget_Localization(ClientID, -1, _("~ Miner: {str:name} broke"), "name", Server()->GetItemName(ClientID, GOLDPIX), NULL);
			}
			Server()->SetItemSettingsCount(ClientID, GOLDPIX, Dropable-1);
			ItemName = Server()->GetItemName(ClientID, GOLDPIX);
			m_Drop += 20;	
		}
		else if(Server()->GetItemCount(ClientID, IRONPIX))
		{
			Count = Server()->GetItemCount(ClientID, IRONPIX);
			Broke = 211*Server()->GetItemCount(ClientID, IRONPIX);
			Dropable = Server()->GetItemSettings(ClientID, IRONPIX);
			if(!Dropable)
			{
				Server()->RemItem(ClientID, IRONPIX, Server()->GetItemCount(ClientID, IRONPIX), -1);
				GameServer()->SendChatTarget_Localization(ClientID, -1, _("~ Miner: {str:name} broke"), "name", Server()->GetItemName(ClientID, IRONPIX), NULL);
			}
			Server()->SetItemSettingsCount(ClientID, IRONPIX, Dropable-1);
			ItemName = Server()->GetItemName(ClientID, IRONPIX);
			m_Drop += 15;	
		}
		else if(Server()->GetItemCount(ClientID, COOPERPIX))
		{
			Count = Server()->GetItemCount(ClientID, COOPERPIX);
			Broke = 180*Server()->GetItemCount(ClientID, COOPERPIX);
			Dropable = Server()->GetItemSettings(ClientID, COOPERPIX);
			if(!Dropable)
			{
				Server()->RemItem(ClientID, COOPERPIX, Server()->GetItemCount(ClientID, COOPERPIX), -1);
				GameServer()->SendChatTarget_Localization(ClientID, -1, _("~ Miner: {str:name} broke"), "name", Server()->GetItemName(ClientID, COOPERPIX), NULL);
			}
			Server()->SetItemSettingsCount(ClientID, COOPERPIX, Dropable-1);
			ItemName = Server()->GetItemName(ClientID, COOPERPIX);
			m_Drop += 10;	
		}
		else
		{
			m_Drop += 5;
		}
		GameServer()->CreateSound(m_Pos, 20); 

		int LevelItem = 1+Server()->GetItemCount(ClientID, MINEREXP)/g_Config.m_SvMinerExp;
		int ExpNeed = LevelItem*g_Config.m_SvMinerExp;
		int Exp = Server()->GetItemCount(ClientID, MINEREXP);
		
		float getlv = (m_Drop*100.0)/100;
		const char *Pick = GameServer()->LevelString(100, (int)getlv, 10, ':', ' ');
		GameServer()->SendBroadcast_Localization(ClientID, 1000, 100, _("Job Miner: {int:lvl} Level : {int:exp}/{int:expneed}EXP\nItem: {str:name}x{int:cout} ({int:brok}/{int:brok2})\nPick: {str:got} / {int:gotp}%"), 
			"lvl", &LevelItem, "exp", &Exp, "expneed", &ExpNeed, "brok", &Dropable, "brok2", &Broke, "name", ItemName, "cout", &Count, "got", Pick, "gotp", &m_Drop, NULL);
		delete Pick;

		if(m_Drop >= 100)
		{
			int a = 1;
			if (LevelItem >= 50)
				a = 2;
			if (LevelItem >= 100)
				a = 3;
			if (LevelItem >= 150)
				a = 4;
			if (LevelItem >= 250)
				a = 5;
			if (LevelItem >= 400)
				a = 6;
			if (LevelItem >= 500)
				if (Server()->GetItemEnquip(ClientID, 18) == PET_MITHRIL_GOLEM) a = 7;

			int ItemDrop = 3 + a;
			int Bonus = (Server()->GetItemEnquip(ClientID, 18) == PET_MITHRIL_GOLEM) ? 1 : 0;
			if (Bonus) Bonus += Server()->GetItemEnchant(ClientID, PET_MITHRIL_GOLEM);

			switch(rand()%ItemDrop)
			{
				case 4: GameServer()->GiveItem(ClientID, IRONORE, 1 + LevelItem / 15 + Bonus); break; 
				case 5: GameServer()->GiveItem(ClientID, GOLDORE, 1 + LevelItem / 15 + Bonus); break; 
				case 6: GameServer()->GiveItem(ClientID, DIAMONDORE, 1 + LevelItem / 15 + Bonus); break; 
				case 7: GameServer()->GiveItem(ClientID, DRAGONORE, 1 + Bonus); break;
				case 8: GameServer()->GiveItem(ClientID, MITHRILORE, 1 + Bonus); break;
				case 9: GameServer()->GiveItem(ClientID, ORIHALCIUMORE, 1 + Bonus); break;
				case 10: GameServer()->GiveItem(ClientID, TITANIUMORE, 1 + Bonus); break;
				case 11: GameServer()->GiveItem(ClientID, ASTRALIUMORE, 1 + Bonus); break; 
				default: 
					GameServer()->GiveItem(ClientID, COOPERORE, 1 + LevelItem / 15); 
					if (GameServer()->m_apPlayers[ClientID]->m_AcceptedDailyQuestID)
					{
						if (GameServer()->m_CurrentDailyQuest1 == QUEST_COLLECT100COPPER)
							GameServer()->m_apPlayers[ClientID]->m_CompleteDailyStep1 += LevelItem / 15;
						if (GameServer()->m_CurrentDailyQuest2 == QUEST_COLLECT100COPPER)
							GameServer()->m_apPlayers[ClientID]->m_CompleteDailyStep2 += LevelItem / 15;
						if (GameServer()->m_CurrentDailyQuest3 == QUEST_COLLECT100COPPER)
							GameServer()->m_apPlayers[ClientID]->m_CompleteDailyStep3 += LevelItem / 15;
					} break;
			}
			GameServer()->GiveItem(ClientID, MINEREXP, 1);

			// ОПЫТ ГНИДАМ
			//GameServer()->m_apPlayers[ClientID]->AccData.Exp += 10+LevelItem;
			//GameServer()->SendChatTarget_Localization(ClientID, -1, _("[Player] EXP +10+{int:bonus} Level Work"), "bonus", &LevelItem, NULL);
		}
	}
	else if(m_SubType == 3) // ########################### WOOD
	{
		int bonus = 0;
		if (Server()->GetItemEnquip(ClientID, 18) == PET_MONKEY) bonus = 25;
		if (bonus) bonus += Server()->GetItemEnchant(ClientID, PET_MONKEY);

		m_Drop += 10 + rand() % 25 + bonus;
		GameServer()->CreateSound(m_Pos, 20); 

		float getlv = (m_Drop*100.0)/100;
		const char *Pick = GameServer()->LevelString(100, (int)getlv, 10, ':', ' ');
		GameServer()->SendBroadcast_Localization(ClientID, 1000, 100, _("Job Woodcutter: Not Leveling\nPick: {str:got} / {int:gotp}%"), 
			"got", Pick, "gotp", &m_Drop, NULL);
		delete Pick;

		if(m_Drop >= 100)
		{
			GameServer()->GiveItem(ClientID, WOOD, 1 + bonus / 5);
			if (rand() % 500 == 0)
				GameServer()->GiveItem(ClientID, PET_MONKEY, 1);
		
			// ОПЫТ ГНИДАМ
			GameServer()->m_apPlayers[ClientID]->AccData.Exp += 10;
			GameServer()->SendChatTarget_Localization(ClientID, -1, _("[Player] EXP +10+Nope Level Work"), NULL);
		}
	}

	else if(m_SubType == 4) // ########################### LOADER
	{
		MaterFarm(ClientID, 1);
	}

	else if(m_SubType == 5) // ########################### LOADER FARM
	{
		MaterFarm(ClientID, 2);
	}

	else if (m_SubType == 6) // ########################### FISHING
	{
		m_Drop += 20;
		GameServer()->CreateSound(m_Pos, 20); 

		int LevelItem = 1+Server()->GetItemCount(ClientID, FISHINGLEVEL) / g_Config.m_SvFishingExp;
		int NeedExp = LevelItem*g_Config.m_SvFishingExp;
		int Exp = Server()->GetItemCount(ClientID, FISHINGLEVEL);
		int GetItemCount = LevelItem / 5;

		float getlv = (m_Drop*100.0)/100;
		const char *Pick = GameServer()->LevelString(100, (int)getlv, 10, ':', ' ');
		GameServer()->SendBroadcast_Localization(ClientID, 1000, 100, _("Job Fishing: {int:lvl} Level : {int:exp}/{int:expneed}EXP\nBonus: 1+{int:bonus} Item : Pick: {str:got} / {int:gotp}%"), 
			"lvl", &LevelItem, "exp", &Exp, "expneed", &NeedExp, "bonus", &GetItemCount, "got", Pick, "gotp", &m_Drop, NULL);
		delete Pick;

		if(m_Drop == 100)
		{
			// Даём рыбу
			int r = rand() % 100;
			int bonus = (Server()->GetItemEnquip(ClientID, 18) == PET_DOLPHIN) ? 5 : 0;
			if (bonus) bonus += Server()->GetItemEnchant(ClientID, PET_DOLPHIN);
			if (r > 10) bonus = 0;
			r = rand() % 101;

			if (r >= 0 && r <= 50)
				GameServer()->GiveItem(ClientID, COMMON_FISH, 1 + LevelItem / 5 + bonus);
			if (r >= 51 && r <= 70)
				GameServer()->GiveItem(ClientID, UNCOMMON_FISH, 1 + LevelItem / 5 + bonus);
			if (r >= 71 && r <= 85)
				GameServer()->GiveItem(ClientID, RARE_FISH, 1 + LevelItem / 5 + bonus);
			if (r >= 86 && r <= 95)
				GameServer()->GiveItem(ClientID, EPIC_FISH, 1 + LevelItem / 5 + bonus);
			if (r >= 96 && r <= 100)
				GameServer()->GiveItem(ClientID, LEGENDARY_FISH, 1 + LevelItem / 5 + bonus);

			if (Server()->GetItemCount(ClientID, LEGENDARY_FISH))
				if (rand() % 1000 == 0)
					GameServer()->GiveItem(ClientID, PET_DOLPHIN, 1);

			if ((GameServer()->m_Season == 11) || (GameServer()->m_Season <= 1))
				if (rand() % 3000 == 0)
					GameServer()->GiveItem(ClientID, PET_ICE_FISH, 1);
				else
					if (Server()->GetItemEnquip(ClientID, 18) == PET_ICE_FISH)
						if (rand() % 100 <= 20)
							GameServer()->GiveItem(ClientID, ICE_FISH, 1 + (rand() % 100 <= 10) ? bonus : 0);

			if(Server()->GetItemCount(ClientID, FISHINGLEVEL) % g_Config.m_SvFishingExp == 0)
			{
				GameServer()->SendChatTarget_Localization(ClientID, -1, _("~ Fishing Level UP +1"), NULL);
			}

			GameServer()->GiveItem(ClientID, FISHINGLEVEL, 1);

			if (GameServer()->m_apPlayers[ClientID]->m_AcceptedDailyQuestID)
			{
				if (GameServer()->m_CurrentDailyQuest1 == QUEST_CATCH200FISHES)
					GameServer()->m_apPlayers[ClientID]->m_CompleteDailyStep1 += 1;
				if (GameServer()->m_CurrentDailyQuest2 == QUEST_CATCH200FISHES)
					GameServer()->m_apPlayers[ClientID]->m_CompleteDailyStep2 += 1;
				if (GameServer()->m_CurrentDailyQuest3 == QUEST_CATCH200FISHES)
					GameServer()->m_apPlayers[ClientID]->m_CompleteDailyStep3 += 1;
			}
		}
	}

	if(m_Drop >= 100)
	{
		m_Drop = 0;
		GameServer()->CreateSoundGlobal(7, ClientID);
		if(m_SubType != 4)
			Picking(30);
	}
}

void CPickup::MaterFarm(int ClientID, int MaterialID)
{
	int MaxMaterial = 6000;

	if (Server()->GetItemCount(ClientID, MATERIAL) > MaxMaterial) {
		return GameServer()->SendBroadcast_Localization(ClientID, 1000, 100, _("Max in inventory {int:mat} material. Come to in Shop!"), "mat", &MaxMaterial, NULL);
	}

	m_Drop += 25;
	GameServer()->CreateSound(m_Pos, 20); 

	int LevelItem = 1+Server()->GetItemCount(ClientID, LOADEREXP)/g_Config.m_SvMaterExp;
	int NeedExp = LevelItem*g_Config.m_SvMaterExp;
	int Exp = Server()->GetItemCount(ClientID, LOADEREXP);
	int Bonus = LevelItem*3 ;

	float getlv = (m_Drop*100.0)/100;
	const char *Pick = GameServer()->LevelString(100, (int)getlv, 10, ':', ' ');
	GameServer()->SendBroadcast_Localization(ClientID, 1000, 100, _("Job Loader: {int:lvl} Level : {int:exp}/{int:expneed}EXP\nBonus: 25+{int:bonus} : Pick: {str:got} / {int:gotp}%"), 
		"lvl", &LevelItem, "exp", &Exp, "expneed", &NeedExp, "bonus", &Bonus, "got", Pick, "gotp", &m_Drop, NULL);
	delete Pick;
	
	if(m_Drop >= 100)
	{
		GameServer()->SendBroadcast_Localization(ClientID, 1000, 100, _("Come this in shop."), NULL);
		GameServer()->GiveItem(ClientID, MATERIAL, 25+LevelItem*3);
		GameServer()->GiveItem(ClientID, LOADEREXP, 10);

		// ОПЫТ ГНИДАМ
		GameServer()->m_apPlayers[ClientID]->AccData.Exp += 20+LevelItem;
		GameServer()->SendChatTarget_Localization(ClientID, -1, _("[Player] EXP +20+{int:bonus} Level Work"), "bonus", &LevelItem, NULL);

		Picking(5);
	}
}

void CPickup::TickPaused()
{
	if(m_SpawnTick != -1)
		++m_SpawnTick;
}

void CPickup::Snap(int SnappingClient)
{
	if(m_SpawnTick != -1 || NetworkClipped(SnappingClient))
		return;

	if(m_SubType != 1)
	{
		CNetObj_Pickup *pP = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
		if(!pP)
			return;

		pP->m_X = (int)m_Pos.x;
		pP->m_Y = (int)m_Pos.y;
		pP->m_Type = m_Type;
	}
	else
	{
		CNetObj_Projectile *pObj = static_cast<CNetObj_Projectile *>(Server()->SnapNewItem(NETOBJTYPE_PROJECTILE, m_ID, sizeof(CNetObj_Projectile)));
		if(pObj)
		{
			pObj->m_X = (int)m_Pos.x;
			pObj->m_Y = (int)m_Pos.y;
			pObj->m_VelX = 0;
			pObj->m_VelY = 0;
			pObj->m_StartTick = Server()->Tick();
			pObj->m_Type = WEAPON_HAMMER;
		}		
	}
}

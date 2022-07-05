#include "craft.h"

CCraft::CCraft(CGameContext* pGameContext, int Result)
{
	m_pGameContext = pGameContext;

	m_Result = Result;
}
IServer* CCraft::Server() { return m_pGameContext->Server(); }

void CCraft::AddIngridient(int ItemID, int Count)
{
	CraftIngridient ing;
	ing.m_ItemID = ItemID;
	ing.m_Count = Count;
	m_aCraftIngridiets.push_back(ing);
}

void CCraft::Clear()
{
	m_aCraftIngridiets.clear();
	m_Result = -1;
}

void CCraft::Craft(int ClientID, int Count)
{
	bool Have = true;
	for (CraftIngridient ing : m_aCraftIngridiets)
		if (Server()->GetItemCount(ClientID, ing.m_ItemID) < ing.m_Count * Count)
			Have = false;

	if (!Have)
	{
		std::string text;
		for (CraftIngridient ing : m_aCraftIngridiets)
		{
			char aBuf[256];
			str_format(aBuf, sizeof(aBuf), "%s x%d, ", Server()->GetItemName(ClientID, ing.m_ItemID, false), ing.m_Count * Count);
			text += aBuf;
		}
		GameServer()->SendChatTarget_Localization(ClientID, CHATCATEGORY_DEFAULT, _("For crafted need {str:need}"), "need", text.c_str(), NULL);

		return;
	}

	for (CraftIngridient ing : m_aCraftIngridiets)
		Server()->RemItem(ClientID, ing.m_ItemID, ing.m_Count * Count, -1);

	GameServer()->SendChatTarget_Localization(-1, CHATCATEGORY_DEFAULT, _("{str:name} crafted item {str:item} x{int:coun}"), "name", Server()->ClientName(ClientID), "item", Server()->GetItemName(ClientID, m_Result, false), "coun", &Count, NULL);
	GameServer()->SendMail(ClientID, "Hello, succesful craft item!", m_Result, Count);
	Server()->GiveItem(ClientID, CRAFT_XP, Count);

	// Achievment
	if ((Server()->GetItemCount(ClientID, CRAFT_XP) >= 7) && (Server()->GetItemCount(ClientID, CRAFTMASTER_1) < 1)) {
		Server()->GiveItem(ClientID, CRAFTMASTER_1, 1);
		Server()->GiveItem(ClientID, COOPERORE, 100);

		GameServer()->SendChatTarget_Localization(ClientID, 0, "Achievment unlocked!");
		GameServer()->SendChatTarget(ClientID, "Your reward:");
		GameServer()->SendChatTarget(ClientID, "Copper ore x100");
	}
	if ((Server()->GetItemCount(ClientID, CRAFT_XP) >= 15) && (Server()->GetItemCount(ClientID, CRAFTMASTER_2) < 1)) {
		Server()->GiveItem(ClientID, CRAFTMASTER_2, 1);
		Server()->GiveItem(ClientID, COOPERORE, 300);

		GameServer()->SendChatTarget_Localization(ClientID, 0, "Achievment unlocked!");
		GameServer()->SendChatTarget(ClientID, "Your reward:");
		GameServer()->SendChatTarget(ClientID, "Copper ore x300");
	}
	if ((Server()->GetItemCount(ClientID, CRAFT_XP) >= 23) && (Server()->GetItemCount(ClientID, CRAFTMASTER_3) < 1)) {
		Server()->GiveItem(ClientID, CRAFTMASTER_3, 1);
		Server()->GiveItem(ClientID, IRONORE, 200);

		GameServer()->SendChatTarget_Localization(ClientID, 0, "Achievment unlocked!");
		GameServer()->SendChatTarget(ClientID, "Your reward:");
		GameServer()->SendChatTarget(ClientID, "Iron ore x200");
	}
	if ((Server()->GetItemCount(ClientID, CRAFT_XP) >= 30) && (Server()->GetItemCount(ClientID, CRAFTMASTER_4) < 1)) {
		Server()->GiveItem(ClientID, CRAFTMASTER_4, 1);
		Server()->GiveItem(ClientID, IRONORE, 300);

		GameServer()->SendChatTarget_Localization(ClientID, 0, "Achievment unlocked!");
		GameServer()->SendChatTarget(ClientID, "Your reward:");
		GameServer()->SendChatTarget(ClientID, "Iron ore x300");
	}
	if ((Server()->GetItemCount(ClientID, CRAFT_XP) >= 40) && (Server()->GetItemCount(ClientID, CRAFTMASTER_5) < 1)) {
		Server()->GiveItem(ClientID, CRAFTMASTER_5, 1);
		Server()->GiveItem(ClientID, IRONORE, 500);

		GameServer()->SendChatTarget_Localization(ClientID, 0, "Achievment unlocked!");
		GameServer()->SendChatTarget(ClientID, "Your reward:");
		GameServer()->SendChatTarget(ClientID, "Iron ore x500");
	}
	if ((Server()->GetItemCount(ClientID, CRAFT_XP) >= 50) && (Server()->GetItemCount(ClientID, CRAFTMASTER_6) < 1)) {
		Server()->GiveItem(ClientID, CRAFTMASTER_6, 1);
		Server()->GiveItem(ClientID, GOLDORE, 400);

		GameServer()->SendChatTarget_Localization(ClientID, 0, "Achievment unlocked!");
		GameServer()->SendChatTarget(ClientID, "Your reward:");
		GameServer()->SendChatTarget(ClientID, "Gold ore x400");
	}
	if ((Server()->GetItemCount(ClientID, CRAFT_XP) >= 60) && (Server()->GetItemCount(ClientID, CRAFTMASTER_7) < 1)) {
		Server()->GiveItem(ClientID, CRAFTMASTER_7, 1);
		Server()->GiveItem(ClientID, GOLDORE, 500);

		GameServer()->SendChatTarget_Localization(ClientID, 0, "Achievment unlocked!");
		GameServer()->SendChatTarget(ClientID, "Your reward:");
		GameServer()->SendChatTarget(ClientID, "Gold ore x500");
	}
}

void CCraft::Build()
{
	switch (m_Result)
	{
		case RARERINGSLIME:
		{
			AddIngridient(RARESLIMEDIRT);
			AddIngridient(FORMULAFORRING);
		} break;
		case MODULEEMOTE:
		{
			AddIngridient(AHAPPY);
			AddIngridient(AEVIL);
			AddIngridient(ASUPRRISE);
			AddIngridient(ABLINK);
			AddIngridient(APAIN);
		} break;
		case WEAPONPRESSED:
		{
			AddIngridient(IGUN);
			AddIngridient(ISHOTGUN);
			AddIngridient(IGRENADE);
			AddIngridient(ILASER);
			AddIngridient(PRESSEDPIECE, 10);
		} break;
		case RINGBOOMER:
		{
			AddIngridient(HEADBOOMER, 100);
			AddIngridient(FORMULAFORRING, 1);
		} break;
		case MODULESHOTGUNSLIME:
		{
			AddIngridient(FORMULAWEAPON);
			AddIngridient(RINGBOOMER);
		} break;
		case EARRINGSKWAH:
		{
			AddIngridient(FORMULAEARRINGS);
			AddIngridient(FOOTKWAH, 100);
		} break;
		case ZOMIBEBIGEYE:
		{
			AddIngridient(ZOMIBEBIGEYE, 30);
		} break;
		case SKELETSSBONE:
		{
			AddIngridient(SKELETSBONE, 30);
		} break;
		case CUSTOMSKIN:
		{
			AddIngridient(SKELETSSBONE, 30);
			AddIngridient(ZOMIBEBIGEYE, 30);
		} break;
		case ENDEXPLOSION:
		{
			AddIngridient(FORMULAWEAPON, 25);
		} break;
		case SHEALSUMMER:
		{
			AddIngridient(ESUMMER, 20);
		} break;
		case JUMPIMPULS:
		{
			AddIngridient(TOMATE, 30);
			AddIngridient(POTATO, 30);
			AddIngridient(CARROT, 30);
		} break;
		case COOPERPIX:
		{
			AddIngridient(WOOD, 30);
			AddIngridient(COOPERORE, 60);
		} break;
		case IRONPIX:
		{
			AddIngridient(WOOD, 40);
			AddIngridient(IRONORE, 60);
		} break;
		case GOLDPIX:
		{
			AddIngridient(WOOD, 50);
			AddIngridient(GOLDORE, 80);
		} break;
		case DIAMONDPIX:
		{
			AddIngridient(WOOD, 50);
			AddIngridient(DIAMONDORE, 100);
		} break;
		case FORMULAEARRINGS:
		{
			AddIngridient(COOPERORE, 100);
			AddIngridient(IRONORE, 100);
		} break;
		case FORMULAFORRING:
		{
			AddIngridient(COOPERORE, 125);
			AddIngridient(IRONORE, 125);
		} break;
		case FORMULAWEAPON:
		{
			AddIngridient(COOPERORE, 150);
			AddIngridient(IRONORE, 150);
		} break;
		case LEATHERBODY:
		{
			AddIngridient(LEATHER, 50);
			AddIngridient(WOOD, 150);
		} break;
		case LEATHERFEET:
		{
			AddIngridient(LEATHER, 40);
			AddIngridient(WOOD, 120);
		} break;
		case COOPERBODY:
		{
			AddIngridient(COOPERORE, 500);
			AddIngridient(WOOD, 150);
		} break;
		case COOPERFEET:
		{
			AddIngridient(COOPERORE, 400);
			AddIngridient(WOOD, 120);
		} break;
		case IRONBODY:
		{
			AddIngridient(IRONORE, 500);
			AddIngridient(WOOD, 150);
		} break;
		case IRONFEET:
		{
			AddIngridient(IRONORE, 400);
			AddIngridient(WOOD, 120);
		} break;
		case GOLDBODY:
		{
			AddIngridient(GOLDORE, 500);
			AddIngridient(WOOD, 150);
		} break;
		case GOLDFEET:
		{
			AddIngridient(GOLDORE, 400);
			AddIngridient(WOOD, 120);
		} break;
		case DIAMONDBODY:
		{
			AddIngridient(DIAMONDORE, 500);
			AddIngridient(WOOD, 150);
		} break;
		case DIAMONDFEET:
		{
			AddIngridient(DIAMONDORE, 400);
			AddIngridient(WOOD, 150);
		} break;
		case DRAGONBODY:
		{
			AddIngridient(DRAGONORE, 500);
			AddIngridient(WOOD, 150);
		} break;
		case DRAGONFEET:
		{
			AddIngridient(DRAGONORE, 400);
			AddIngridient(WOOD, 150);
		} break;
		case STCLASIC:
		{
			AddIngridient(COOPERORE, 100);
			AddIngridient(IRONORE, 10);
		} break;
		case MITHRIL_BODY:
		{
			AddIngridient(MITHRILORE, 500);
			AddIngridient(WOOD, 150);
			AddIngridient(DRAGONBODY);
		} break;
		case MITHRIL_FEET:
		{
			AddIngridient(MITHRILORE, 400);
			AddIngridient(WOOD, 150);
			AddIngridient(DRAGONFEET);
		} break;
		case ORIHALCIUM_BODY:
		{
			AddIngridient(ORIHALCIUMORE, 500);
			AddIngridient(WOOD, 150);
			AddIngridient(MITHRIL_BODY);
		} break;
		case ORIHALCIUM_FEET:
		{
			AddIngridient(ORIHALCIUMORE, 400);
			AddIngridient(WOOD, 150);
			AddIngridient(MITHRIL_FEET);
		} break;
		case TITANIUM_BODY:
		{
			AddIngridient(TITANIUMORE, 500);
			AddIngridient(WOOD, 150);
			AddIngridient(ORIHALCIUM_BODY);
		} break;
		case TITANIUM_FEET:
		{
			AddIngridient(TITANIUMORE, 400);
			AddIngridient(WOOD, 150);
			AddIngridient(ORIHALCIUM_FEET);
		} break;
		case ASTRALIUM_BODY:
		{
			AddIngridient(ASTRALIUMORE, 500);
			AddIngridient(WOOD, 150);
			AddIngridient(TITANIUM_BODY);
		} break;
		case ASTRALIUM_FEET:
		{
			AddIngridient(ASTRALIUMORE, 400);
			AddIngridient(WOOD, 150);
			AddIngridient(TITANIUM_FEET);
		} break;
		case PET_MITHRIL_GOLEM:
		{
			AddIngridient(MITHRILORE, 900);
			AddIngridient(COOPERORE, 10000);
		} break;
		case PET_CLEVER:
		{
			AddIngridient(GOLDORE, 100);
			AddIngridient(DIAMONDORE, 100);
			AddIngridient(COOPERORE, 500);
		} break;
		case PEX_WINGS:
		{
			AddIngridient(PIXI_DUST, 800);
			AddIngridient(GOLDORE, 400);
			AddIngridient(DIAMONDORE, 300);
		}
	}
}

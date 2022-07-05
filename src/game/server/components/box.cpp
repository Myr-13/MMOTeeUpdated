#include "box.h"

CBox::CBox(class CGameContext* pGameContext, const char* pName)
{
	m_pGameContext = pGameContext;

	str_copy(m_aBoxName, pName, sizeof(m_aBoxName));
}

void CBox::SetName(const char* pName)
{
	str_copy(m_aBoxName, pName, sizeof(m_aBoxName));
}

void CBox::Add(int Item, int Count)
{
	BoxItem item;
	item.m_Type = Item;
	item.m_Count = Count;
	item.m_Proc = -1;

	m_aItems.push_back(item);
}

void CBox::AddRare(int Item, int Proc, int Count)
{
	BoxItem item;
	item.m_Type = Item;
	item.m_Count = Count;
	item.m_Proc = Proc;

	m_aRareItems.push_back(item);
}

void CBox::Open(int ClientID, int Iterations) // Не читайте этот пиздец, лучше просто закройте xdd
{
	std::vector<BoxItem> GetItems;

	for (int i = 0; i < Iterations; i++)
	{
		BoxItem item = m_aItems[rand() % m_aItems.size()];
		for (BoxItem rare : m_aRareItems)
			if (rand() % 100 < rare.m_Proc)
				item = rare;

		GetItems.push_back(item);

		m_pGameContext->Server()->GiveItem(ClientID, item.m_Type, item.m_Count);
	}

	std::vector<BoxItem> ForText;

	for (BoxItem item : GetItems)
	{
		bool Found = false;
		for (int i = 0; i < ForText.size(); i++)
		{
			if (ForText[i].m_Type == item.m_Type)
			{
				Found = true;
				ForText[i].m_Count += item.m_Count;
			}
		}

		if (!Found)
			ForText.push_back(item);
	}

	std::string text;
	for (BoxItem item : ForText)
	{
		char aBuf[256];
		str_format(aBuf, sizeof(aBuf), "%s x%d, ", m_pGameContext->Server()->GetItemName(ClientID, item.m_Type, false), item.m_Count);
		text += aBuf;
	}

	m_pGameContext->SendChatTarget_Localization(-1, CHATCATEGORY_DEFAULT, "{str:name} used {str:box} x{int:count} and get {str:items}",
		"name", m_pGameContext->Server()->ClientName(ClientID), "box", m_aBoxName, "count", &Iterations, "items", text.c_str());
}

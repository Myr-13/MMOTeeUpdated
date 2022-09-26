#include "moon_door.h"

#include <engine/server.h>

CMoonDoor::CMoonDoor(CGameWorld* pGameWorld, vec2 Pos) :
	CEntity(pGameWorld, CGameWorld::ENTTYPE_MOONDOOR)
{
	m_Length = 4 * 32 + 16; // Closed
	
	GameWorld()->InsertEntity(this);
}

void CMoonDoor::Tick()
{
	CCharacter *pChr = GameWorld()->ClosestCharacter(m_Pos + vec2(0, m_Length / 2), 320, 0);

	if (pChr)
	{
		if (m_Length <= 1)
			m_Length -= 1;
	} 
	else
	{
		if (m_Length >= 4 * 32 + 15)
			m_Length += 1;
	}
}

void CMoonDoor::Snap(int SnappingClient)
{
	if (NetworkClipped(SnappingClient))
		return;

	CNetObj_Laser* pObj = static_cast<CNetObj_Laser*>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID, sizeof(CNetObj_Laser)));
	if (!pObj)
		return;

	pObj->m_X = (int)m_Pos.x;
	pObj->m_Y = (int)(m_Pos.y + m_Length);
	pObj->m_FromX = (int)m_Pos.x;
	pObj->m_FromY = (int)m_Pos.x;
	pObj->m_StartTick = Server()->Tick();
}

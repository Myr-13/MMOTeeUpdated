#include "moon_door.h"

#include <engine/server.h>
#include <game/server/gamecontext.h>

CMoonDoor::CMoonDoor(CGameWorld* pGameWorld, vec2 Pos) :
	CEntity(pGameWorld, CGameWorld::ENTTYPE_MOONDOOR)
{
	m_Length = 5 * 32; // Closed
	m_Pos = Pos;
	
	GameWorld()->InsertEntity(this);
}

void CMoonDoor::Tick()
{
	CCharacter *pChr = GameWorld()->ClosestCharacter(m_Pos + vec2(0, m_Length / 2), 160, 0);

	if (pChr)
	{
		m_Length -= (m_Length > 0) ? 1 : 0;

		vec2 Pos = m_Pos + vec2(0, m_Length);
		GameServer()->Collision()->SetCollisionAt(Pos.x, Pos.y, 0);
	} 
	else
	{
		m_Length += (m_Length < 5 * 32) ? 1 : 0;

		vec2 Pos = m_Pos + vec2(0, m_Length);
		GameServer()->Collision()->SetCollisionAt(Pos.x, Pos.y, 3);
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
	pObj->m_FromY = (int)m_Pos.y;
	pObj->m_StartTick = Server()->Tick();
}

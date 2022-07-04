#include "wings.h"

CWings::CWings(CGameWorld* pGameWorld, vec2 Pos, int ClientID)
	: CEntity(pGameWorld, CGameWorld::ENTTYPE_WINGS)
{
	m_Pos = Pos;
	m_ClientID = ClientID;
	
	m_Poss[0] = vec2(11.f, -17.f);
	m_Poss[1] = vec2(19.f, -24.f);
	m_Poss[2] = vec2(26.f, -30.f);
	m_Poss[3] = vec2(37.f, -34.f);
	m_Poss[4] = vec2(48.f, -34.f);
	m_Poss[5] = vec2(64.f, -33.f);
	m_Poss[6] = vec2(76.f, -37.f);
	m_Poss[7] = vec2(84.f, -41.f);
	m_Poss[8] = vec2(94.f, -49.f);
	m_Poss[9] = vec2(104.f, -60.f);
	m_Poss[10] = vec2(112.f, -70.f);
	m_Poss[11] = vec2(115.f, -59.f);
	m_Poss[12] = vec2(111.f, -47.f);
	m_Poss[13] = vec2(106.f, -37.f);
	m_Poss[14] = vec2(99.f, -27.f);
	m_Poss[15] = vec2(91.f, -17.f);
	m_Poss[16] = vec2(79.f, -9.f);
	m_Poss[17] = vec2(62.f, -8.f);
	m_Poss[18] = vec2(44.f, -8.f);
	m_Poss[19] = vec2(33.f, -10.f);
	m_Poss[20] = vec2(21.f, -11.f);
	m_Poss[21] = vec2(33.f, -22.f);
	m_Poss[22] = vec2(39.f, -21.f);
	m_Poss[23] = vec2(47.f, -21.f);
	m_Poss[24] = vec2(56.f, -21.f);
	m_Poss[25] = vec2(66.f, -24.f);
	m_Poss[26] = vec2(77.f, -28.f);
	m_Poss[27] = vec2(89.f, -35.f);
	m_Poss[28] = vec2(98.f, -45.f);
	m_Poss[29] = vec2(105.f, -54.f);
	m_Poss[30] = vec2(94.f, -33.f);
	m_Poss[31] = vec2(86.f, -26.f);
	m_Poss[32] = vec2(76.f, -17.f);
	m_Poss[33] = vec2(66.f, -17.f);
	m_Poss[34] = vec2(53.f, -17.f);
	m_Poss[35] = vec2(47.f, -27.f);
	m_Poss[36] = vec2(40.f, -28.f);
	m_Poss[37] = vec2(54.f, -29.f);
	m_Poss[38] = vec2(40.f, -18.f);
	m_Poss[39] = vec2(23.f, -19.f);
	m_Poss[40] = vec2(-11.f, -17.f);
	m_Poss[41] = vec2(-19.f, -24.f);
	m_Poss[42] = vec2(-26.f, -30.f);
	m_Poss[43] = vec2(-37.f, -34.f);
	m_Poss[44] = vec2(-48.f, -34.f);
	m_Poss[45] = vec2(-64.f, -33.f);
	m_Poss[46] = vec2(-76.f, -37.f);
	m_Poss[47] = vec2(-84.f, -41.f);
	m_Poss[48] = vec2(-94.f, -49.f);
	m_Poss[49] = vec2(-104.f, -60.f);
	m_Poss[50] = vec2(-112.f, -70.f);
	m_Poss[51] = vec2(-115.f, -59.f);
	m_Poss[52] = vec2(-111.f, -47.f);
	m_Poss[53] = vec2(-106.f, -37.f);
	m_Poss[54] = vec2(-99.f, -27.f);
	m_Poss[55] = vec2(-91.f, -17.f);
	m_Poss[56] = vec2(-79.f, -9.f);
	m_Poss[57] = vec2(-62.f, -8.f);
	m_Poss[58] = vec2(-44.f, -8.f);
	m_Poss[59] = vec2(-33.f, -10.f);
	m_Poss[60] = vec2(-21.f, -11.f);
	m_Poss[61] = vec2(-33.f, -22.f);
	m_Poss[62] = vec2(-39.f, -21.f);
	m_Poss[63] = vec2(-47.f, -21.f);
	m_Poss[64] = vec2(-56.f, -21.f);
	m_Poss[65] = vec2(-66.f, -24.f);
	m_Poss[66] = vec2(-77.f, -28.f);
	m_Poss[67] = vec2(-89.f, -35.f);
	m_Poss[68] = vec2(-98.f, -45.f);
	m_Poss[69] = vec2(-105.f, -54.f);
	m_Poss[70] = vec2(-94.f, -33.f);
	m_Poss[71] = vec2(-86.f, -26.f);
	m_Poss[72] = vec2(-76.f, -17.f);
	m_Poss[73] = vec2(-66.f, -17.f);
	m_Poss[74] = vec2(-53.f, -17.f);
	m_Poss[75] = vec2(-47.f, -27.f);
	m_Poss[76] = vec2(-40.f, -28.f);
	m_Poss[77] = vec2(-54.f, -29.f);
	m_Poss[78] = vec2(-40.f, -18.f);
	m_Poss[79] = vec2(-23.f, -19.f);

	for (int i = 0; i < 80; i++) {
		m_IDs[i] = Server()->SnapNewID();
	}

	GameWorld()->InsertEntity(this);
}

CWings::~CWings() {
	for (int i = 0; i < 80; i++) {
		Server()->SnapFreeID(m_IDs[i]);
	}
}

void CWings::Tick()
{
	if (GameServer()->m_apPlayers[m_ClientID] && GameServer()->m_apPlayers[m_ClientID]->GetCharacter())
		m_Pos = GameServer()->m_apPlayers[m_ClientID]->GetCharacter()->m_Pos;
	else
		Destroy();
}

void CWings::Snap(int SnappingClient) {
	if (NetworkClipped(SnappingClient))
		return;
	if (GameServer()->m_apPlayers[m_ClientID]->GetCharacter()->m_InvisibleEndTick > Server()->Tick() && SnappingClient != m_ClientID)
		return;

	for (int i = 0; i < 80; i++) {
		CNetObj_Projectile* pObj = static_cast<CNetObj_Projectile*>(Server()->SnapNewItem(NETOBJTYPE_PROJECTILE, m_IDs[i], sizeof(CNetObj_Projectile)));
		if (!pObj)
			return;

		pObj->m_Type = WEAPON_HAMMER;
		pObj->m_X = (int)m_Pos.x + m_Poss[i].x;
		pObj->m_Y = (int)m_Pos.y + m_Poss[i].y;
		pObj->m_VelX = 0;
		pObj->m_VelY = 0;
		pObj->m_StartTick = Server()->Tick();
	}
}

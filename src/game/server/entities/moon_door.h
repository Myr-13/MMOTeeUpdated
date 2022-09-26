#ifndef GAME_SERVER_ENTITIES_MOON_DOOR_H
#define GAME_SERVER_ENTITIES_MOON_DOOR_H

#include <game/server/entity.h>

class CMoonDoor : public CEntity
{
	int m_Length;

public:
	CMoonDoor(CGameWorld* pGameWorld, vec2 Pos);

	virtual void Tick();
	virtual void Snap(int SnappingClient);
};

#endif

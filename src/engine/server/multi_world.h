#ifndef ENGINE_SERVER_MULTI_WORLD_H
#define ENGINE_SERVER_MULTI_WORLD_H

#include <engine/shared/config.h>
#include <base/system.h>
#include <string>

enum {
	WORLD_MAIN,
	WORLD_MOON,
	NUM_WORLDS
};

struct CWorldData {
	char *m_pName;
	char *m_pPath;
};

class CMultiWorlds
{
public:
	CWorldData m_aWorlds[NUM_WORLDS];

	CMultiWorlds();
	~CMultiWorlds();

	int LoadWorlds();

	const char *GetWorldName(int WorldID);
	const char *GetWorldPath(int WorldID);
};

#endif // !ENGINE_SERVER_MULTI_WORLD_H

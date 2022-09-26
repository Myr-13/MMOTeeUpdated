#include "multi_world.h"

#include <engine/external/json-parser/json.h>
#include <fstream>
#include <streambuf>

CMultiWorlds::CMultiWorlds()
{
	for (int i = 0; i < NUM_WORLDS; i++)
	{
		m_aWorlds[i].m_pName = new char[128];
		m_aWorlds[i].m_pPath = new char[128];
	}
}

CMultiWorlds::~CMultiWorlds()
{
	for (int i = 0; i < NUM_WORLDS; i++)
	{
		delete[] m_aWorlds[i].m_pName;
		delete[] m_aWorlds[i].m_pPath;
	}
}

int CMultiWorlds::LoadWorlds()
{
	std::ifstream JsonFile("worlds.json");
	if (!JsonFile)
	{
		dbg_msg("server", "failed to load worlds.json");
		return -1;
	}
	else
	{
		dbg_msg("server", "loading worlds.json");
	}
	json_value* pJson = json_parse(std::string((std::istreambuf_iterator<char>(JsonFile)), std::istreambuf_iterator<char>()).c_str());

	const json_value& Start = (*pJson)["worlds"];
	if (Start.type == json_array)
	{
		for (int i = 0; i < Start.u.array.length; i++)
		{
			const char* pWorldName = Start[i]["name"];
			const char* pWorldPath = Start[i]["path"];

			str_copy(m_aWorlds[i].m_pName, pWorldName, sizeof(m_aWorlds[i].m_pName));
			str_copy(m_aWorlds[i].m_pPath, pWorldPath, sizeof(m_aWorlds[i].m_pPath) + 1);
		}
	}

	return 0;
}

const char* CMultiWorlds::GetWorldName(int WorldID)
{
	return m_aWorlds[WorldID].m_pName;
}

const char* CMultiWorlds::GetWorldPath(int WorldID)
{
	return m_aWorlds[WorldID].m_pPath;
}

/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_MMO_VARIABLES_H
#define GAME_MMO_VARIABLES_H
#undef GAME_MMO_VARIABLES_H // this file will be included several times

// Lol text
MACRO_CONFIG_INT(SvLoltextHspace, sv_loltext_hspace, 14, 10, 25, CFGFLAG_SERVER, "horizontal offset between loltext 'pixels'")
MACRO_CONFIG_INT(SvLoltextVspace, sv_loltext_vspace, 14, 10, 25, CFGFLAG_SERVER, "vertical offset between loltext 'pixels'")

// Spec
MACRO_CONFIG_INT(SvSpectatorSlots, sv_spectator_slots, 0, 0, MAX_CLIENTS, CFGFLAG_SERVER, "Number of slots to reserve for spectators")
MACRO_CONFIG_INT(SvTeambalanceTime, sv_teambalance_time, 1, 0, 1000, CFGFLAG_SERVER, "How many minutes to wait before autobalancing teams")
MACRO_CONFIG_INT(SvInactiveKickTime, sv_inactivekick_time, 3, 0, 1000, CFGFLAG_SERVER, "How many minutes to wait before taking care of inactive players")
MACRO_CONFIG_INT(SvInactiveKick, sv_inactivekick, 1, 0, 2, CFGFLAG_SERVER, "How to deal with inactive players (0=move to spectator, 1=move to free spectator slot/kick, 2=kick)")

// Drop
MACRO_CONFIG_INT(SvDropDance, sv_dropdance, 1, 0, 100, CFGFLAG_SERVER, "How to deal with inactive players (0=move to spectator, 1=move to free spectator slot/kick, 2=kick)")
MACRO_CONFIG_INT(SvEventSummer, sv_event_summer, 1, 0, 1, CFGFLAG_SERVER, "Event Summer")
MACRO_CONFIG_INT(SvEventHammer, sv_event_hammer, 0, 0, 1, CFGFLAG_SERVER, "Event hammer actived")
MACRO_CONFIG_INT(SvEventSchool, sv_event_school, 1, 0, 1, CFGFLAG_SERVER, "Event back to school")

// Works
MACRO_CONFIG_INT(SvMaterExp, sv_materexp_work, 1000, 0, 10000, CFGFLAG_SERVER, "Mater exp need for up")
MACRO_CONFIG_INT(SvFarmExp, sv_farmexp_work, 500, 0, 10000, CFGFLAG_SERVER, "Farming exp need for up")
MACRO_CONFIG_INT(SvMinerExp, sv_minerexp_work, 50, 0, 10000, CFGFLAG_SERVER, "Miner exp need for up")
MACRO_CONFIG_INT(SvFishingExp, sv_fishingexp_work, 100, 0, 10000, CFGFLAG_SERVER, "Fishing exp need for up")
MACRO_CONFIG_INT(SvMushroomerExp, sv_mushroomerexp_work, 100, 0, 10000, CFGFLAG_SERVER, "Mushroomer exp need for up")

MACRO_CONFIG_STR(SvMoonMap, sv_moon_map, 128, "", CFGFLAG_SERVER, "Moon map to use on the server")

// debug
#ifdef CONF_DEBUG // this one can crash the server if not used correctly
MACRO_CONFIG_INT(DbgDummies, dbg_dummies, 0, 0, 15, CFGFLAG_SERVER, "")
#endif
#endif

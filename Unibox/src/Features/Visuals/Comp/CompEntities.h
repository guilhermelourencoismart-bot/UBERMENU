#pragma once
#include "../../../SDK/SDK.h"
enum class CompGroup { PLAYERS_ALL, BUILDINGS_ALL, PICKUPS_HEALTH, PICKUPS_AMMO, PICKUPS_MONEY, WORLD_BOMBS, WORLD_GARGOYLE, PICKUPS_SPELLBOOK, PICKUPS_POWERUP };
class CCompEntities
{
    std::vector<CBaseEntity*> m_gargoyles,m_books,m_powerups;
public:
    void Refresh();
    const std::vector<CBaseEntity*>& GetGroup(CompGroup group);
};
ADD_FEATURE(CCompEntities, CompEntities);

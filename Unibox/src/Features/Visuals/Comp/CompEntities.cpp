#include "CompEntities.h"
void CCompEntities::Refresh()
{
    m_gargoyles.clear();m_books.clear();m_powerups.clear();
    if (!Vars::Comp::Radar::Main::Enabled.Value || !Vars::Comp::Radar::World::Enabled.Value) return;
    for(int i=1;i<=I::ClientEntityList->GetHighestEntityIndex();++i)
    {
        auto client=I::ClientEntityList->GetClientEntity(i);if(!client) continue;
        auto entity=client->As<CBaseEntity>();if(entity->IsDormant())continue;
        if(entity->GetClassID()==ETFClassID::CHalloweenGiftPickup){m_gargoyles.push_back(entity);continue;}
        auto model=entity->GetModel();if(!model)continue;
        auto name=I::ModelInfoClient->GetModelName(model);if(!name)continue;
        auto hash=FNV1A::Hash32(name);
        if(H::Entities.IsSpellbook(hash)) m_books.push_back(entity);
        if(H::Entities.IsPowerup(hash)) m_powerups.push_back(entity);
    }
}
const std::vector<CBaseEntity*>& CCompEntities::GetGroup(CompGroup group)
{
    switch(group)
    {
    case CompGroup::PLAYERS_ALL:return H::Entities.GetGroup(EntityEnum::PlayerAll);
    case CompGroup::BUILDINGS_ALL:return H::Entities.GetGroup(EntityEnum::BuildingAll);
    case CompGroup::PICKUPS_HEALTH:return H::Entities.GetGroup(EntityEnum::PickupHealth);
    case CompGroup::PICKUPS_AMMO:return H::Entities.GetGroup(EntityEnum::PickupAmmo);
    case CompGroup::PICKUPS_MONEY:return H::Entities.GetGroup(EntityEnum::PickupMoney);
    case CompGroup::WORLD_BOMBS:return H::Entities.GetGroup(EntityEnum::WorldBomb);
    case CompGroup::WORLD_GARGOYLE:return m_gargoyles;
    case CompGroup::PICKUPS_SPELLBOOK:return m_books;
    case CompGroup::PICKUPS_POWERUP:return m_powerups;
    }
    static const std::vector<CBaseEntity*> empty;return empty;
}

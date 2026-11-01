#include "AmmoTracker.h"
#include <cstring>
#ifndef TEXTMODE
#include <ImGui/imgui.h>
#endif

// Comp reference: associate item_pickup with the nearest known stationary supply.
// Unknown positions are skipped instead of inventing a timer at a player's feet.
void CAmmoTracker::Event(IGameEvent* event, uint32_t hash)
{
    if (hash == FNV1A::Hash32Const("client_disconnect") || hash == FNV1A::Hash32Const("game_newmap") || hash == FNV1A::Hash32Const("teamplay_round_start"))
    {
        m_supplies.clear(); m_timers.clear(); m_cache.Set({}); return;
    }
    if (!event || !Vars::AmmoTracker::Enabled.Value || hash != FNV1A::Hash32Const("item_pickup")) return;
    const char* item = event->GetString("item");
    if (!item) return;
    const bool health = std::strstr(item, "medkit") || std::strstr(item, "healthkit") || std::strstr(item, "health");
    if (!health && !std::strstr(item, "ammopack") && !std::strstr(item, "ammo")) return;
    auto clientEntity = I::ClientEntityList->GetClientEntity(I::EngineClient->GetPlayerForUserID(event->GetInt("userid")));
    if (!clientEntity) return;
    auto entity = clientEntity->As<CBaseEntity>();
    if (!entity->IsPlayer()) return;
    const SupplyPosition_t* nearest = nullptr;
    float distance = 128.f;
    for (const auto& supply : m_supplies)
    {
        if (supply.health != health) continue;
        const float candidate = (supply.position - entity->GetAbsOrigin()).Length();
        if (candidate < distance) { distance = candidate; nearest = &supply; }
    }
    if (!nearest) return;
    const float expires = I::GlobalVars->curtime + std::clamp(Vars::AmmoTracker::RespawnSeconds.Value, 1.f, 60.f);
    for (auto& timer : m_timers)
        if (timer.supply.health == health && (timer.supply.position - nearest->position).Length() < 8.f)
        { timer.expires = expires; return; }
    if (m_timers.size() < 512) m_timers.push_back({*nearest, expires});
}

void CAmmoTracker::CacheDrawInfo(CTFPlayer* local)
{
    std::vector<SupplyDraw_t> frame;
    if (!local || !Vars::AmmoTracker::Enabled.Value)
    {
        m_supplies.clear(); m_timers.clear(); m_cache.Set(std::move(frame)); return;
    }
    const auto map = std::string(I::EngineClient->GetLevelName());
    const float now = I::GlobalVars->curtime;
    if (map != m_map || now < m_lastTime) { m_timers.clear(); m_supplies.clear(); }
    m_map = map; m_lastTime = now;
    m_supplies.clear();
    for (const auto group : {EntityEnum::PickupHealth, EntityEnum::PickupAmmo})
        for (auto entity : H::Entities.GetGroup(group))
        {
            // Dropped ammo packs are not respawning map pickups.
            if (!entity || entity->IsDormant() || entity->GetClassID() == ETFClassID::CTFAmmoPack) continue;
            m_supplies.push_back({entity->GetAbsOrigin(), group == EntityEnum::PickupHealth});
        }
    for (auto it = m_timers.begin(); it != m_timers.end();)
    {
        const float remaining = it->expires - now;
        if (remaining <= 0) { it = m_timers.erase(it); continue; }
        Vec3 screen;
        if (!SDK::TakingScreenshot() && (it->supply.position - local->GetAbsOrigin()).Length() <= Vars::AmmoTracker::MaxDistance.Value && SDK::W2S(it->supply.position, screen))
            frame.push_back({{screen.x, screen.y}, remaining, std::clamp(remaining / std::clamp(Vars::AmmoTracker::RespawnSeconds.Value, 1.f, 60.f), 0.f, 1.f), it->supply.health});
        ++it;
    }
    m_cache.Set(std::move(frame));
}
void CAmmoTracker::Draw(ImDrawList* draw)
{
#ifndef TEXTMODE
    if (!draw) return;
    for (const auto& timer : m_cache.Get())
    {
        const ImVec2 center(timer.position.x, timer.position.y);
        const auto color = timer.health ? IM_COL32(100, 255, 140, 220) : IM_COL32(240, 220, 100, 220);
        draw->AddCircle(center, 14.f, IM_COL32(0, 0, 0, 180), 24, 3.f);
        draw->PathArcTo(center, 14.f, -1.5707963f, -1.5707963f + 6.2831853f * timer.fraction, 24);
        draw->PathStroke(color, 0, 2.f);
        const auto text = std::format("{:.1f}s", timer.remaining);
        draw->AddText({center.x + 18, center.y - 7}, color, text.c_str());
    }
#endif
}

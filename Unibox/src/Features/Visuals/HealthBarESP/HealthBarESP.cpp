#include "HealthBarESP.h"
#include "../Groups/Groups.h"
#ifndef TEXTMODE
#include <ImGui/imgui.h>
#endif

// Adapted from Amalgam-Comp HealthBarESP (WTFPL v2): medic selection,
// health gradient/overheal and ground polygon, using Unibox snapshot rendering.
void CHealthBarESP::CacheDrawInfo(CTFPlayer* local)
{
    std::vector<HealthOverlay_t> frame;
    using namespace Vars::HealthOverlay;
    if (!local || !Enabled.Value || SDK::TakingScreenshot() || I::EngineVGui->IsGameUIVisible())
    {
        m_cache.Set(std::move(frame));
        return;
    }
    const bool teammates = MedicMode.Value && local->m_iClass() == TF_CLASS_MEDIC;
    const auto origin = local->GetAbsOrigin();
    const auto eye = local->GetEyePosition();
    for (auto entity : H::Entities.GetGroup(teammates ? EntityEnum::PlayerTeam : EntityEnum::PlayerEnemy))
    {
        if (!entity || entity == local || entity->IsDormant())
            continue;
        auto player = entity->As<CTFPlayer>();
        if (!player->IsAlive() || player->InCond(TF_COND_STEALTHED) || player->InCond(TF_COND_DISGUISED))
            continue;
        const auto position = player->GetAbsOrigin();
        if ((position - origin).Length() > std::clamp(MaxDistance.Value, 50.f, 10000.f))
            continue;
        Vec3 chest = position; chest.z += 40.f;
        if (!ThroughWalls.Value && !SDK::VisPos(local, player, eye, chest, MASK_VISIBLE))
            continue;
        const int health = player->m_iHealth(), maximum = player->GetMaxHealth();
        if (maximum <= 0 || health <= 0)
            continue;
        HealthOverlay_t overlay;
        overlay.ratio = std::clamp(float(health) / maximum, 0.f, 1.f);
        overlay.color = health > maximum ? Color_t(71, 166, 255, 180)
            : Color_t(byte(255 * (1 - overlay.ratio)), byte(255 * overlay.ratio), 0, 180);
        Group_t* group = nullptr;
        const bool existingBar = F::Groups.GetGroup(player, group, false) && group && (group->m_tESP.Draw & ESPEnum::HealthBar);
        Vec3 screen;
        overlay.bar = Bars.Value && !existingBar && SDK::W2S(chest, screen);
        if (overlay.bar)
        {
            overlay.x = screen.x;
            overlay.y = screen.y + 30.f;
        }
        if (Polygons.Value)
        {
            // All vertices must project, otherwise discard this polygon at the near plane.
            for (int i = 0; i < 24; ++i)
            {
                const float angle = i * (6.28318530718f / 24);
                Vec3 vertex = position; vertex.x += std::cos(angle) * 30.f; vertex.y += std::sin(angle) * 30.f;
                Vec3 projected;
                if (!SDK::W2S(vertex, projected)) { overlay.polygon.clear(); break; }
                overlay.polygon.emplace_back(projected.x, projected.y);
            }
        }
        if (overlay.bar || !overlay.polygon.empty())
            frame.push_back(std::move(overlay));
    }
    m_cache.Set(std::move(frame));
}

void CHealthBarESP::Draw(ImDrawList* draw)
{
#ifndef TEXTMODE
    if (!draw) return;
    for (const auto& overlay : m_cache.Get())
    {
        const auto color = IM_COL32(overlay.color.r, overlay.color.g, overlay.color.b, overlay.color.a);
        if (overlay.bar)
        {
            draw->AddRectFilled({overlay.x - 45, overlay.y}, {overlay.x + 45, overlay.y + 10}, IM_COL32(0, 0, 0, 180));
            draw->AddRectFilled({overlay.x - 45, overlay.y}, {overlay.x - 45 + 90 * overlay.ratio, overlay.y + 10}, color);
        }
        // Outline is neutral; filled edge segments represent remaining health.
        for (size_t i = 0; i < overlay.polygon.size(); ++i)
        {
            const auto& a = overlay.polygon[i];
            const auto& b = overlay.polygon[(i + 1) % overlay.polygon.size()];
            draw->AddLine({a.x, a.y}, {b.x, b.y}, float(i) / overlay.polygon.size() < overlay.ratio ? color : IM_COL32(0, 0, 0, 120), 2.f);
        }
    }
#endif
}

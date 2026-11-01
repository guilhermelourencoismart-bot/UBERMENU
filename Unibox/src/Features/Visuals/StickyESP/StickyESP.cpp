#include "StickyESP.h"
#include "../Groups/Groups.h"
#ifndef TEXTMODE
#include <ImGui/imgui.h>
#endif
// Comp reference: distinguish stickies from pipes, filter by team/visibility,
// draw projected bounds. Chams remain owned by the native Unibox group system.
void CStickyESP::CacheDrawInfo(CTFPlayer* local)
{
    std::vector<StickyOverlay_t> frame;
    using namespace Vars::StickyESP;
    if (!local || !Enabled.Value || SDK::TakingScreenshot()) { m_cache.Set(std::move(frame)); return; }
    for (auto entity : H::Entities.GetGroup(EntityEnum::WorldProjectile))
    {
        if (!entity || entity->IsDormant() || entity->GetClassID() != ETFClassID::CTFGrenadePipebombProjectile) continue;
        auto sticky = entity->As<CTFGrenadePipebombProjectile>();
        if (sticky->m_iType() != 1) continue;
        const bool friendly = sticky->m_iTeamNum() == local->m_iTeamNum();
        if (EnemyOnly.Value && friendly) continue;
        const auto position = sticky->GetAbsOrigin();
        if ((position - local->GetAbsOrigin()).Length() > MaxDistance.Value) continue;
        const bool visible = SDK::VisPos(local, entity, local->GetEyePosition(), position);
        if (!ThroughWalls.Value && !visible) continue;
        Group_t* group = nullptr;
        if (F::Groups.GetGroup(entity, group, false) && group && (group->m_tESP.Draw & ESPEnum::Box)) continue;
        Vec3 screen;
        if (!SDK::W2S(position, screen)) continue;
        StickyOverlay_t overlay;
        overlay.center = {screen.x, screen.y};
        overlay.color = friendly ? Color_t(100, 180, 255, visible ? 255 : 120) : Color_t(255, 100, 100, visible ? 255 : 120);
        overlay.box2d = !Box3D.Value;
        if (Box3D.Value)
        {
            const auto minimum = sticky->m_vecMins(), maximum = sticky->m_vecMaxs();
            Vec3 projected[8]; bool allProjected = true;
            for (int i = 0; i < 8; ++i)
            {
                const Vec3 corner = position + Vec3((i & 1) ? maximum.x : minimum.x, (i & 2) ? maximum.y : minimum.y, (i & 4) ? maximum.z : minimum.z);
                if (!SDK::W2S(corner, projected[i])) { allProjected = false; break; }
            }
            if (allProjected)
                for (int i = 0; i < 8; ++i)
                    for (int bit : {1, 2, 4})
                        if (!(i & bit)) overlay.edges.push_back({{projected[i].x, projected[i].y}, {projected[i | bit].x, projected[i | bit].y}});
        }
        frame.push_back(std::move(overlay));
    }
    m_cache.Set(std::move(frame));
}
void CStickyESP::Draw(ImDrawList* draw)
{
#ifndef TEXTMODE
    if (!draw) return;
    for (const auto& sticky : m_cache.Get())
    {
        const auto color = IM_COL32(sticky.color.r, sticky.color.g, sticky.color.b, sticky.color.a);
        if (sticky.box2d) draw->AddRect({sticky.center.x - 6, sticky.center.y - 6}, {sticky.center.x + 6, sticky.center.y + 6}, color);
        for (const auto& [a, b] : sticky.edges) draw->AddLine({a.x, a.y}, {b.x, b.y}, color);
    }
#endif
}

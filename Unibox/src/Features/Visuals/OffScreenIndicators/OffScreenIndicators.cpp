#include "OffScreenIndicators.h"
#include "../Radar/RadarMath.h"
#include "../Groups/Groups.h"
#ifndef TEXTMODE
#include <ImGui/imgui.h>
#endif

// Comp's on-screen test and rotated triangle, adapted to existing Unibox groups.
// This alternative replaces the base arrows while enabled; it adds no hook.
void COffScreenIndicators::CacheDrawInfo(CTFPlayer* local)
{
    std::vector<OffscreenArrow_t> arrows;
    if (!local || !Vars::OffScreenIndicators::Enabled.Value || SDK::TakingScreenshot())
    {
        m_cache.Set(std::move(arrows)); return;
    }
    const float width = float(H::Draw.m_nScreenW), height = float(H::Draw.m_nScreenH);
    const float radius = std::min(std::clamp(Vars::OffScreenIndicators::Radius.Value, 50.f, 600.f), std::min(width, height) * .4f);
    const auto origin = local->GetAbsOrigin();
    const float yaw = I::EngineClient->GetViewAngles().y;
    for (auto& [entity, group] : F::Groups.GetGroup(false))
    {
        if (!entity || !group || !group->m_bOffscreenArrows || !entity->IsPlayer() || entity == local || entity->IsDormant()) continue;
        if (!entity->As<CTFPlayer>()->IsAlive()) continue;
        const auto delta = origin - entity->GetAbsOrigin();
        if (delta.Length() > group->m_flOffscreenArrowsMaxDistance) continue;
        Vec3 screen;
        if (SDK::W2S(entity->GetAbsOrigin(), screen) && screen.x >= 0 && screen.x <= width && screen.y >= 0 && screen.y <= height) continue;
        RadarMath::Point direction;
        if (!RadarMath::Project(delta.x, delta.y, yaw, 1.f, true, true, direction)) continue;
        const float length = std::hypot(direction.x, direction.y);
        if (length < .0001f) continue;
        const float x = direction.x / length, y = direction.y / length;
        const float cx = width * .5f + x * radius, cy = height * .5f + y * radius;
        arrows.push_back({{cx + x * 10, cy + y * 10}, {cx - x * 8 - y * 6, cy - y * 8 + x * 6}, {cx - x * 8 + y * 6, cy - y * 8 - x * 6}, group->m_tColor});
    }
    m_cache.Set(std::move(arrows));
}
void COffScreenIndicators::Draw(ImDrawList* draw)
{
#ifndef TEXTMODE
    if (!draw) return;
    for (const auto& arrow : m_cache.Get())
        draw->AddTriangleFilled({arrow.tip.x, arrow.tip.y}, {arrow.left.x, arrow.left.y}, {arrow.right.x, arrow.right.y}, IM_COL32(arrow.color.r, arrow.color.g, arrow.color.b, arrow.color.a));
#endif
}

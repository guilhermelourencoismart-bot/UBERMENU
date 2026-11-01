#include "Radar.h"
#include "RadarMath.h"
#ifndef TEXTMODE
#include <ImGui/imgui.h>
#endif

void CRadar::CacheDrawInfo(CTFPlayer* local)
{
    RadarFrame_t frame;
    using namespace Vars::Radar;
    if (!local || !Enabled.Value || !I::EngineClient->IsInGame() || SDK::TakingScreenshot())
    {
        m_cache.Set(std::move(frame));
        return;
    }
    frame.active = true;
    frame.circular = Circular.Value;
    frame.radius = std::clamp(Size.Value, 100.f, 500.f) * 0.5f;
    // Coordinates are the top-left of the panel in pixels, bounded to the viewport.
    frame.x = std::clamp(float(PositionX.Value), 0.f, std::max(0.f, float(H::Draw.m_nScreenW) - frame.radius * 2));
    frame.y = std::clamp(float(PositionY.Value), 0.f, std::max(0.f, float(H::Draw.m_nScreenH) - frame.radius * 2));
    const auto origin = local->GetAbsOrigin();
    const auto yaw = I::EngineClient->GetViewAngles().y;
    const auto addGroup = [&](int group, Color_t color, bool building, bool players)
    {
        for (auto entity : H::Entities.GetGroup(group))
        {
            if (!entity || entity == local || entity->IsDormant())
                continue;
            if (players && !entity->As<CTFPlayer>()->IsAlive())
                continue;
            const auto delta = origin - entity->GetAbsOrigin();
            RadarMath::Point point;
            if (RadarMath::Project(delta.x, delta.y, yaw, Range.Value, frame.circular, ClampToEdge.Value, point))
                frame.points.push_back({point.x, point.y, color, building});
        }
    };
    if (Players.Value)
    {
        addGroup(EntityEnum::PlayerEnemy, {255, 100, 100, 255}, false, true);
        if (Teammates.Value)
            addGroup(EntityEnum::PlayerTeam, {100, 170, 255, 255}, false, true);
    }
    if (Buildings.Value)
    {
        addGroup(EntityEnum::BuildingEnemy, {255, 100, 100, 255}, true, false);
        if (Teammates.Value)
            addGroup(EntityEnum::BuildingTeam, {100, 170, 255, 255}, true, false);
    }
    if (Pickups.Value)
    {
        addGroup(EntityEnum::PickupHealth, {100, 255, 140, 255}, false, false);
        addGroup(EntityEnum::PickupAmmo, {240, 220, 100, 255}, false, false);
        addGroup(EntityEnum::PickupMoney, {100, 220, 100, 255}, false, false);
    }
    m_cache.Set(std::move(frame));
}

void CRadar::Draw(ImDrawList* draw)
{
#ifndef TEXTMODE
    const auto frame = m_cache.Get();
    if (!draw || !frame.active)
        return;
    const ImVec2 center(frame.x + frame.radius, frame.y + frame.radius);
    const ImVec2 minimum(frame.x, frame.y), maximum(frame.x + frame.radius * 2, frame.y + frame.radius * 2);
    const auto background = IM_COL32(20, 23, 28, 180), border = IM_COL32(180, 190, 210, 210);
    if (frame.circular)
    {
        draw->AddCircleFilled(center, frame.radius, background, 64);
        draw->AddCircle(center, frame.radius, border, 64);
    }
    else
    {
        draw->AddRectFilled(minimum, maximum, background);
        draw->AddRect(minimum, maximum, border);
    }
    draw->AddLine({minimum.x, center.y}, {maximum.x, center.y}, IM_COL32(180, 190, 210, 70));
    draw->AddLine({center.x, minimum.y}, {center.x, maximum.y}, IM_COL32(180, 190, 210, 70));
    for (const auto& point : frame.points)
    {
        const ImVec2 position(center.x + point.x * (frame.radius - 4), center.y + point.y * (frame.radius - 4));
        const auto color = IM_COL32(point.color.r, point.color.g, point.color.b, point.color.a);
        if (point.building)
            draw->AddRectFilled({position.x - 3, position.y - 3}, {position.x + 3, position.y + 3}, color);
        else
            draw->AddCircleFilled(position, 3.f, color, 12);
    }
    draw->AddTriangleFilled({center.x, center.y - 5}, {center.x - 4, center.y + 4}, {center.x + 4, center.y + 4}, IM_COL32_WHITE);
#endif
}

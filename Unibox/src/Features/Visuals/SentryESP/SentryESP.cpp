#include "SentryESP.h"
#include "../Groups/Groups.h"
#ifndef TEXTMODE
#include <ImGui/imgui.h>
#endif
// Comp reference: sentry filtering, target warning and screen-space bounds.
// Aim lines point at the networked target, without depending on model bone indices.
void CSentryESP::CacheDrawInfo(CTFPlayer* local)
{
    std::vector<SentryOverlay_t> frame;
    if (!local || !Vars::SentryESP::Enabled.Value || SDK::TakingScreenshot()) { m_cache.Set(std::move(frame)); return; }
    for (auto entity : H::Entities.GetGroup(EntityEnum::BuildingEnemy))
    {
        if (!entity || entity->IsDormant() || entity->GetClassID() != ETFClassID::CObjectSentrygun) continue;
        auto sentry = entity->As<CObjectSentrygun>();
        if (sentry->m_bBuilding() || sentry->m_iHealth() <= 0 || (entity->GetAbsOrigin() - local->GetAbsOrigin()).Length() > Vars::SentryESP::MaxDistance.Value) continue;
        Vec3 bottom = entity->GetAbsOrigin(), top = bottom; top.z += entity->m_vecMaxs().z;
        if (!Vars::SentryESP::ThroughWalls.Value && !SDK::VisPos(local, entity, local->GetEyePosition(), top)) continue;
        SentryOverlay_t overlay;
        Vec3 a, b;
        Group_t* group = nullptr;
        const bool existingBox = F::Groups.GetGroup(entity, group, false) && group && (group->m_tESP.Draw & ESPEnum::Box);
        if (!existingBox && SDK::W2S(bottom, a) && SDK::W2S(top, b) && a.y > b.y)
        {
            const float halfWidth = (a.y - b.y) * .375f;
            overlay.minimum = {a.x - halfWidth, b.y}; overlay.maximum = {a.x + halfWidth, a.y}; overlay.box = true;
        }
        if (auto target = sentry->m_hEnemy().Get())
        {
            if (!target->IsDormant())
            {
                Vec3 targetPosition = target->GetAbsOrigin(); targetPosition.z += target->m_vecMaxs().z * .5f;
                if (SDK::W2S(top, a) && SDK::W2S(targetPosition, b))
                {
                    overlay.start = {a.x, a.y}; overlay.end = {b.x, b.y}; overlay.line = true;
                    overlay.targeting = target == local;
                }
            }
        }
        if (overlay.box || overlay.line) frame.push_back(overlay);
    }
    m_cache.Set(std::move(frame));
}
void CSentryESP::Draw(ImDrawList* draw)
{
#ifndef TEXTMODE
    if (!draw) return;
    for (const auto& sentry : m_cache.Get())
    {
        const auto color = sentry.targeting ? IM_COL32(255, 50, 50, 255) : IM_COL32(255, 190, 90, 220);
        if (sentry.box) draw->AddRect({sentry.minimum.x, sentry.minimum.y}, {sentry.maximum.x, sentry.maximum.y}, color);
        if (sentry.line) draw->AddLine({sentry.start.x, sentry.start.y}, {sentry.end.x, sentry.end.y}, color, sentry.targeting ? 2.f : 1.f);
        if (sentry.targeting) draw->AddText({sentry.start.x, sentry.start.y - 16}, color, "Sentry targeting you");
    }
#endif
}

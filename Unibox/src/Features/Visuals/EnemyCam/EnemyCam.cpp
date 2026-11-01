#include "EnemyCam.h"

// Comp reference: select an enemy and render from its viewpoint. Unibox owns
// the render target, material, hook and window; no secondary resource lifetime.
void CEnemyCam::CacheDrawInfo(CTFPlayer* local)
{
    EnemyView_t frame;
    if (!local || !Vars::EnemyCam::Enabled.Value || SDK::TakingScreenshot() || I::EngineVGui->IsGameUIVisible())
    { m_cache.Set(frame); return; }
    float bestDistance = std::clamp(Vars::EnemyCam::MaxDistance.Value, 50.f, 10000.f);
    for (auto entity : H::Entities.GetGroup(EntityEnum::PlayerEnemy))
    {
        if (!entity || entity->IsDormant()) continue;
        auto player = entity->As<CTFPlayer>();
        if (!player->IsAlive() || player->InCond(TF_COND_STEALTHED)) continue;
        const float distance = (player->GetAbsOrigin() - local->GetAbsOrigin()).Length();
        if (distance >= bestDistance) continue;
        if (Vars::EnemyCam::VisibleOnly.Value && !SDK::VisPos(local, player, local->GetEyePosition(), player->GetEyePosition())) continue;
        bestDistance = distance;
        frame.active = true;
        frame.origin = player->GetEyePosition();
        frame.angles = player->GetEyeAngles();
    }
    m_cache.Set(frame);
}

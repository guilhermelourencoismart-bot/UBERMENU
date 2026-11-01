#include "PlayerTrails.h"
#include <unordered_set>
#ifndef TEXTMODE
#include <ImGui/imgui.h>
#endif

// Comp's timed position history adapted to Unibox. Serial numbers distinguish
// reused entity slots. Rendering receives projected values, never entities.
void CPlayerTrails::CacheDrawInfo(CTFPlayer* local)
{
    std::vector<TrailLine_t> lines;
    using namespace Vars::PlayerTrails;
    if (!local || !Enabled.Value)
    {
        m_history.clear(); m_map.clear(); m_alive = false; m_lastTime = -1.f;
        m_cache.Set(std::move(lines));
        return;
    }
    const float now = I::GlobalVars->realtime;
    const std::string map = I::EngineClient->GetLevelName();
    if (map != m_map || now < m_lastTime || (!m_alive && local->IsAlive()))
        m_history.clear();
    m_map = map; m_lastTime = now; m_alive = local->IsAlive();
    const float duration = std::clamp(Duration.Value, .1f, 10.f);
    std::unordered_set<int> active;
    for (auto entity : H::Entities.GetGroup(EntityEnum::PlayerEnemy))
    {
        if (!entity || entity->IsDormant()) continue;
        auto player = entity->As<CTFPlayer>();
        if (!player->IsAlive() || player->InCond(TF_COND_STEALTHED)) continue;
        auto position = player->GetAbsOrigin();
        if ((position - local->GetAbsOrigin()).Length() > MaxDistance.Value) continue;
        const int index = player->entindex();
        active.insert(index);
        auto& history = m_history[index];
        const int serial = player->GetRefEHandle().GetSerialNumber();
        if (history.serial != serial || (!history.samples.empty() && (history.samples.back().position - position).Length() > 256.f))
            history.samples.clear();
        history.serial = serial;
        position.z += 5.f;
        while (!history.samples.empty() && now - history.samples.front().time > duration)
            history.samples.pop_front();
        if (history.samples.empty() || now - history.samples.back().time >= .05f)
            history.samples.push_back({position, now});
        while (history.samples.size() > 256) history.samples.pop_front();
        if (!ThroughWalls.Value && !SDK::VisPos(local, player, local->GetEyePosition(), player->GetEyePosition()))
            continue;
        for (size_t i = 1; i < history.samples.size(); ++i)
        {
            Vec3 a, b;
            if (SDK::W2S(history.samples[i - 1].position, a) && SDK::W2S(history.samples[i].position, b))
                lines.push_back({{a.x, a.y}, {b.x, b.y}, std::clamp(1.f - (now - history.samples[i].time) / duration, 0.f, 1.f)});
        }
    }
    for (auto it = m_history.begin(); it != m_history.end();)
        if (!active.contains(it->first)) it = m_history.erase(it); else ++it;
    // Keep sampling during screenshots but publish an empty frame.
    if (SDK::TakingScreenshot() || I::EngineVGui->IsGameUIVisible()) lines.clear();
    m_cache.Set(std::move(lines));
}

void CPlayerTrails::Draw(ImDrawList* draw)
{
#ifndef TEXTMODE
    if (!draw) return;
    for (const auto& line : m_cache.Get())
        draw->AddLine({line.from.x, line.from.y}, {line.to.x, line.to.y}, IM_COL32(255, 150, 80, int(220 * line.alpha)), 2.f);
#endif
}

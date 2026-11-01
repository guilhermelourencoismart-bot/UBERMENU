#pragma once
#include "../../../SDK/SDK.h"
#include "../../ImGui/IndicatorCache.h"
struct ImDrawList;
struct SupplyPosition_t { Vec3 position; bool health; };
struct SupplyTimer_t { SupplyPosition_t supply; float expires; };
struct SupplyDraw_t { Vec2 position; float remaining, fraction; bool health; };
class CAmmoTracker
{
    std::vector<SupplyPosition_t> m_supplies;
    std::vector<SupplyTimer_t> m_timers;
    std::string m_map;
    float m_lastTime = -1.f;
    CIndicatorCache<std::vector<SupplyDraw_t>> m_cache;
public:
    void Event(IGameEvent* event, uint32_t hash);
    void CacheDrawInfo(CTFPlayer* local);
    void Draw(ImDrawList* draw);
};
ADD_FEATURE(CAmmoTracker, AmmoTracker);

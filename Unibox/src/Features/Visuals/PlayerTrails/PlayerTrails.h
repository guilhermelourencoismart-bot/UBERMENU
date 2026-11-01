#pragma once
#include "../../../SDK/SDK.h"
#include "../../ImGui/IndicatorCache.h"
#include <deque>
struct ImDrawList;
struct TrailSample_t { Vec3 position; float time; };
struct TrailHistory_t { int serial = -1; std::deque<TrailSample_t> samples; };
struct TrailLine_t { Vec2 from, to; float alpha; };
class CPlayerTrails
{
    std::unordered_map<int, TrailHistory_t> m_history;
    std::string m_map;
    float m_lastTime = -1.f;
    bool m_alive = false;
    CIndicatorCache<std::vector<TrailLine_t>> m_cache;
public:
    void CacheDrawInfo(CTFPlayer* local);
    void Draw(ImDrawList* draw);
};
ADD_FEATURE(CPlayerTrails, PlayerTrails);

#pragma once
#include "../../../SDK/SDK.h"
#include "../../ImGui/IndicatorCache.h"
struct ImDrawList;
struct HealthOverlay_t
{
    float x = 0.f, y = 0.f, ratio = 0.f;
    bool bar = false;
    Color_t color = {};
    std::vector<Vec2> polygon;
};
class CHealthBarESP
{
    CIndicatorCache<std::vector<HealthOverlay_t>> m_cache;
public:
    void CacheDrawInfo(CTFPlayer* local);
    void Draw(ImDrawList* draw);
};
ADD_FEATURE(CHealthBarESP, HealthBarESP);

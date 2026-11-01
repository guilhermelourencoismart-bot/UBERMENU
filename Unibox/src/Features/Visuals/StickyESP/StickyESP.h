#pragma once
#include "../../../SDK/SDK.h"
#include "../../ImGui/IndicatorCache.h"
struct ImDrawList;
struct StickyOverlay_t { Vec2 center; Color_t color; std::vector<std::pair<Vec2, Vec2>> edges; bool box2d = false; };
class CStickyESP
{
    CIndicatorCache<std::vector<StickyOverlay_t>> m_cache;
public:
    void CacheDrawInfo(CTFPlayer* local);
    void Draw(ImDrawList* draw);
};
ADD_FEATURE(CStickyESP, StickyESP);

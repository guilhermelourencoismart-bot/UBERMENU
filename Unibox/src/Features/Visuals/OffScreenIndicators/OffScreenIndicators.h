#pragma once
#include "../../../SDK/SDK.h"
#include "../../ImGui/IndicatorCache.h"
struct ImDrawList;
struct OffscreenArrow_t { Vec2 tip, left, right; Color_t color; };
class COffScreenIndicators
{
    CIndicatorCache<std::vector<OffscreenArrow_t>> m_cache;
public:
    void CacheDrawInfo(CTFPlayer* local);
    void Draw(ImDrawList* draw);
};
ADD_FEATURE(COffScreenIndicators, OffScreenIndicators);

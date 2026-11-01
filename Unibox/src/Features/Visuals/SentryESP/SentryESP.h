#pragma once
#include "../../../SDK/SDK.h"
#include "../../ImGui/IndicatorCache.h"
struct ImDrawList;
struct SentryOverlay_t { Vec2 start, end, minimum, maximum; bool line = false, box = false, targeting = false; };
class CSentryESP
{
    CIndicatorCache<std::vector<SentryOverlay_t>> m_cache;
public:
    void CacheDrawInfo(CTFPlayer* local);
    void Draw(ImDrawList* draw);
};
ADD_FEATURE(CSentryESP, SentryESP);

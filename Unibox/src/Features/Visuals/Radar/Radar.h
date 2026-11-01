#pragma once
#include "../../../SDK/SDK.h"
#include "../../ImGui/IndicatorCache.h"

struct ImDrawList;

struct RadarPoint_t
{
    float x = 0.f, y = 0.f;
    Color_t color = {};
    bool building = false;
};

struct RadarFrame_t
{
    bool active = false, circular = true;
    float x = 0.f, y = 0.f, radius = 0.f;
    std::vector<RadarPoint_t> points;
};

class CRadar
{
    CIndicatorCache<RadarFrame_t> m_cache;
public:
    void CacheDrawInfo(CTFPlayer* local);
    void Draw(ImDrawList* draw);
};

ADD_FEATURE(CRadar, Radar);

#pragma once
#include "CompDraw.h"
class CCompVisuals
{
    std::string m_map;
    float m_time = -1.f;
    std::unordered_map<int, int> m_serials;
public:
    void Capture(CTFPlayer* local);
    void Event(IGameEvent* event, uint32_t hash);
    void Reset();
};
ADD_FEATURE(CCompVisuals, CompVisuals);

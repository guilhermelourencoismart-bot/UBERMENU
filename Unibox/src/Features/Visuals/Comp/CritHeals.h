// Full Comp port, source 366fa0dc; see docs/THIRD-PARTY-NOTICES.md.
#pragma once
#include "../../../SDK/SDK.h"
#include <unordered_map>

class CFullCritHeals
{
private:
    // Parameters are registered in Vars::Comp::CritHeals, without duplicate constants.
    // Tracking data
    std::unordered_map<int, float> m_LastDamageTimes;
    std::unordered_map<int, float> m_LastBuffTimes;

    // Helper functions
    void DrawTriangle(int x, int y, bool isEnemy);
    void DrawUberBuildWarning();
    bool IsPlayerVisible(CTFPlayer* pPlayer, CTFPlayer* pLocal);

public:
    void Draw();
    void OnPlayerHurt(int victimIndex);
};

ADD_FEATURE(CFullCritHeals, FullCritHeals);

#pragma once
#include "../../../SDK/SDK.h"
#include "../AimbotUtils/SolverPolicy.h"
#include "../../ImGui/IndicatorCache.h"
#include <chrono>
class CV2Router
{
    SolverPolicy::ContextGuard m_context;
    int m_targetIndex=-1;
    uint64_t m_targetToken=0;
    int m_backend=-1;
    struct DisplayStatus { int route=0; std::chrono::steady_clock::time_point captured{}; };
    CIndicatorCache<DisplayStatus> m_display;
public:
    // Menu reads a scalar snapshot, never entity pointers. Expire when commands stop.
    int DisplayRoute() {
        auto status=m_display.Get();
        return std::chrono::steady_clock::now()-status.captured < std::chrono::seconds(1) ? status.route : 0;
    }
    void Update(CTFPlayer* local,CTFWeaponBase* weapon);
    void Reset();
    bool Use(CTFWeaponBase* weapon) const;
    void Run(int type,CTFPlayer* local,CTFWeaponBase* weapon,CUserCmd* command);
};
ADD_FEATURE(CV2Router,V2Router);

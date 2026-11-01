#pragma once
#include "../../../SDK/SDK.h"
#include "../AimbotUtils/SolverPolicy.h"
class CV2Router
{
    SolverPolicy::ContextGuard m_context;
    int m_targetIndex=-1;
    uint64_t m_targetToken=0;
    int m_backend=-1;
public:
    void Update(CTFPlayer* local,CTFWeaponBase* weapon);
    void Reset();
    bool Use(CTFWeaponBase* weapon) const;
    void Run(int type,CTFPlayer* local,CTFWeaponBase* weapon,CUserCmd* command);
};
ADD_FEATURE(CV2Router,V2Router);

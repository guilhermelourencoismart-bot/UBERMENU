#pragma once
#include "../../../SDK/SDK.h"
#include "../../ImGui/IndicatorCache.h"
struct EnemyView_t { bool active = false; Vec3 origin = {}, angles = {}; };
class CEnemyCam
{
    CIndicatorCache<EnemyView_t> m_cache;
public:
    void CacheDrawInfo(CTFPlayer* local);
    EnemyView_t GetView() { return m_cache.Get(); }
};
ADD_FEATURE(CEnemyCam, EnemyCam);

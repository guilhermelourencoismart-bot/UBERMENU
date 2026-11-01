#pragma once
#include "../../Backtrack/Backtrack.h"
// V2 cached centers are reconstructed from Unibox's authoritative bone records.
// Keep TickRecord's native layout and ownership unchanged.
namespace V2Backtrack
{
    inline std::vector<HitboxInfo_t> ForRecord(CBaseEntity* entity,const TickRecord& record)
    {
        std::vector<HitboxInfo_t> boxes;
        if(!entity)return boxes;
        auto set=entity->As<CBaseAnimating>()->GetHitboxSet();if(!set)return boxes;
        boxes.reserve(set->numhitboxes);
        for(int hitbox=0;hitbox<set->numhitboxes;++hitbox)
        {
            auto box=set->pHitbox(hitbox);if(!box || box->bone<0 || box->bone>=MAXSTUDIOBONES)continue;
            Vec3 center;Math::VectorTransform((box->bbmin+box->bbmax)/2,record.m_aBones[box->bone],center);
            boxes.push_back({box->bone,hitbox,center,box->bbmin,box->bbmax});
        }
        return boxes;
    }
    inline std::vector<HitboxInfo_t> Latest(CBaseEntity* entity)
    {
        std::vector<TickRecord*> records;
        if(F::Backtrack.GetRecords(entity,records) && !records.empty())return ForRecord(entity,*records.front());
        return {};
    }
}

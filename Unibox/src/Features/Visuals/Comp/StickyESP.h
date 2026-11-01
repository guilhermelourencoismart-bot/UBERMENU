// Complete Comp port, source 366fa0dc, SHA256 4dd7d5088e1cf4d25e90ae17e3b1ab9749ecbfd8287c8bbd7253b4288b9f8ea4.
#pragma once
#include "../../../SDK/SDK.h"
#include <unordered_map>

class CFullStickyESP
{
private:
    // Configuration values are now read from Vars::Comp::StickyESP::* variables

    // Helper functions (no caching, direct like HealthBarESP)
    bool IsVisible(CBaseEntity* pEntity, CBaseEntity* pLocal);
    void Draw3DBox(const std::vector<Vec3>& vertices, const Color_t& color);
    void Draw2DBox(int x, int y, int size, const Color_t& color);
    std::pair<Vec3, Vec3> GetHitbox(CBaseEntity* pEntity);

public:
    // Chams tracking for stickies
    std::unordered_map<int, bool> m_mEntities;

    void Draw();
    void UpdateChamsEntities(); // Separate function to populate chams entities
};

ADD_FEATURE(CFullStickyESP, FullStickyESP)

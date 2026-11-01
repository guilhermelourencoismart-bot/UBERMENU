// Full Comp port, source 366fa0dc SHA256 2e360a7b84fd479edc899990020e962684b384f64fb3d681c4b91d4c9eb999bb
#pragma once
#include "../../../SDK/SDK.h"
#include <unordered_map>
#include <string>

class CFullOffScreenIndicators
{
private:
    // Color cache for Steam IDs (same as MarkSpot)
    std::unordered_map<std::string, Color_t> m_ColorCache;
    
    // Embedded avatar for fallback
    
    // Helper functions
    Color_t GenerateColor(const std::string& steamID);
    std::string GetPlayerSteamID(CTFPlayer* pPlayer);
    std::string GetPlayerClassName(int classId);
    bool IsOnScreen(CBaseEntity* entity);
    void DrawArrow(int centerX, int centerY, float angle, const Color_t& color, int size = 10);
    
public:
    void Draw();
};

ADD_FEATURE(CFullOffScreenIndicators, FullOffScreenIndicators)
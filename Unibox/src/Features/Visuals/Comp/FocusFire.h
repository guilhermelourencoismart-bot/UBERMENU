// Ported from Amalgam-Comp 366fa0dc; original SHA256 16cba0d52a3e52c91d1a057d45d430c3be9e5d34ebee6f9776b49e1621a25b39.
#pragma once
#include "../../../SDK/SDK.h"
#include <unordered_map>
#include <vector>

struct CompTargetInfo
{
    std::unordered_map<int, float> Attackers;
    bool IsMultiTargeted = false;
    int AttackerCount = 0;
};

class CFullFocusFire
{
private:
    // Configuration values are now read from Vars::Comp::FocusFire::* variables
    static constexpr float CLEANUP_INTERVAL = 0.5f;
    
    // State tracking
    std::unordered_map<int, CompTargetInfo> m_TargetData;
    float m_NextCleanupTime = 0.0f;
    std::unordered_map<int, float> m_NextVisCheck;
    std::unordered_map<int, bool> m_VisibilityCache;
    
    // Helper functions
    bool IsPlayerVisible(CTFPlayer* pPlayer);
    void DrawCorners(int x1, int y1, int x2, int y2);
    void CleanExpiredAttackers(float currentTime, CompTargetInfo& targetInfo);
    void CleanInvalidTargets();
    bool ShouldVisualizePlayer(CTFPlayer* pPlayer);
    void DrawBox();
    
public:
    // Chams tracking for focused players
    std::unordered_map<int, bool> m_mEntities;
    
    void Draw();
    void OnPlayerHurt(int victimIndex, int attackerIndex);
    void Reset();
    void UpdateChamsEntities(); // Separate function to populate chams entities
};

ADD_FEATURE(CFullFocusFire, FullFocusFire)